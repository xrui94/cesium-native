#include "Cesium3DTiles/Tile.h"
#include "Cesium3DTiles/Tileset.h"
#include "CesiumGeometry/QuadtreeTileRectangularRange.h"
#include "CesiumGeospatial/GlobeRectangle.h"
#include "CesiumUtility/Math.h"
#include "QuantizedMeshContent.h"
#include "tiny_gltf.h"
#include "Uri.h"
#include <glm/vec3.hpp>
#include <stdexcept>

using namespace CesiumUtility;
using namespace CesiumGeospatial;
using namespace CesiumGeometry;

namespace Cesium3DTiles {
    /*static*/ std::string QuantizedMeshContent::CONTENT_TYPE = "application/vnd.quantized-mesh";

    struct QuantizedMeshHeader
    {
        // The center of the tile in Earth-centered Fixed coordinates.
        double CenterX;
        double CenterY;
        double CenterZ;

        // The minimum and maximum heights in the area covered by this tile.
        // The minimum may be lower and the maximum may be higher than
        // the height of any vertex in this tile in the case that the min/max vertex
        // was removed during mesh simplification, but these are the appropriate
        // values to use for analysis or visualization.
        float MinimumHeight;
        float MaximumHeight;

        // The tile’s bounding sphere.  The X,Y,Z coordinates are again expressed
        // in Earth-centered Fixed coordinates, and the radius is in meters.
        double BoundingSphereCenterX;
        double BoundingSphereCenterY;
        double BoundingSphereCenterZ;
        double BoundingSphereRadius;

        // The horizon occlusion point, expressed in the ellipsoid-scaled Earth-centered Fixed frame.
        // If this point is below the horizon, the entire tile is below the horizon.
        // See http://cesiumjs.org/2013/04/25/Horizon-culling/ for more information.
        double HorizonOcclusionPointX;
        double HorizonOcclusionPointY;
        double HorizonOcclusionPointZ;

        // The total number of vertices.
        uint32_t vertexCount;
    };

    struct ExtensionHeader
    {
        uint8_t extensionId;
        uint32_t extensionLength;
    };

    // We can't use sizeof(QuantizedMeshHeader) because it may be padded.
    const size_t headerLength = 92;
    const size_t extensionHeaderLength = 5;

    int32_t zigZagDecode(int32_t value) {
        return (value >> 1) ^ (-(value & 1));
    }

    template <class T>
    void decodeIndices(const gsl::span<const T>& encoded, gsl::span<T>& decoded) {
        if (decoded.size() < encoded.size()) {
            throw std::runtime_error("decoded buffer is too small.");
        }

        T highest = 0;
        for (size_t i = 0; i < encoded.size(); ++i) {
            T code = encoded[i];
            decoded[i] = static_cast<T>(highest - code);
            if (code == 0) {
                ++highest;
            }
        }
    }

    template <class T>
    static T readValue(const gsl::span<const uint8_t>& data, size_t offset, T defaultValue) {
        if (offset >= 0 && offset + sizeof(T) <= data.size()) {
            return *reinterpret_cast<const T*>(data.data() + offset);
        }
        return defaultValue;
    }

    static glm::dvec3 octDecode(uint8_t x, uint8_t y) {
        const uint8_t rangeMax = 255;

        glm::dvec3 result;

        result.x = CesiumUtility::Math::fromSNorm(x, rangeMax);
        result.y = CesiumUtility::Math::fromSNorm(y, rangeMax);
        result.z = 1.0 - (glm::abs(result.x) + glm::abs(result.y));

        if (result.z < 0.0) {
            double oldVX = result.x;
            result.x = (1.0 - glm::abs(result.y)) * CesiumUtility::Math::signNotZero(oldVX);
            result.y = (1.0 - glm::abs(oldVX)) * CesiumUtility::Math::signNotZero(result.y);
        }

        return glm::normalize(result);
    }

    static void processMetadata(const QuadtreeTileID& tileID, gsl::span<const char> json, TileContentLoadResult& result);

    /*static*/ std::unique_ptr<TileContentLoadResult> QuantizedMeshContent::load(
        const TileContext& /*context*/,
        const TileID& tileID,
        const BoundingVolume& tileBoundingVolume,
        double /*tileGeometricError*/,
        const glm::dmat4& /*tileTransform*/,
        const std::optional<BoundingVolume>& /*tileContentBoundingVolume*/,
        TileRefine /*tileRefine*/,
        const std::string& /*url*/,
        const gsl::span<const uint8_t>& data
    ) {
        // TODO: use context plus tileID to compute the tile's rectangle, rather than inferring it from the parent tile.
        const QuadtreeTileID& id = std::get<QuadtreeTileID>(tileID);

        std::unique_ptr<TileContentLoadResult> pResult = std::make_unique<TileContentLoadResult>();

        if (data.size() < headerLength) {
            return pResult;
        }

        size_t readIndex = 0;

        const QuantizedMeshHeader* pHeader = reinterpret_cast<const QuantizedMeshHeader*>(data.data());
        glm::dvec3 center(pHeader->BoundingSphereCenterX, pHeader->BoundingSphereCenterY, pHeader->BoundingSphereCenterZ);
        glm::dvec3 horizonOcclusionPoint(pHeader->HorizonOcclusionPointX, pHeader->HorizonOcclusionPointY, pHeader->HorizonOcclusionPointZ);
        double minimumHeight = pHeader->MinimumHeight;
        double maximumHeight = pHeader->MaximumHeight;

        uint32_t vertexCount = pHeader->vertexCount;

        readIndex += headerLength;

        const gsl::span<const uint16_t> uBuffer(reinterpret_cast<const uint16_t*>(data.data() + readIndex), vertexCount);
        readIndex += uBuffer.size_bytes();
        if (readIndex > data.size()) {
            return pResult;
        }

        const gsl::span<const uint16_t> vBuffer(reinterpret_cast<const uint16_t*>(data.data() + readIndex), vertexCount);
        readIndex += vBuffer.size_bytes();
        if (readIndex > data.size()) {
            return pResult;
        }

        const gsl::span<const uint16_t> heightBuffer(reinterpret_cast<const uint16_t*>(data.data() + readIndex), vertexCount);
        readIndex += heightBuffer.size_bytes();
        if (readIndex > data.size()) {
            return pResult;
        }

        int32_t u = 0;
        int32_t v = 0;
        int32_t height = 0;

        const BoundingRegion* pRegion = std::get_if<BoundingRegion>(&tileBoundingVolume);
        if (!pRegion) {
            const BoundingRegionWithLooseFittingHeights* pLooseRegion = std::get_if<BoundingRegionWithLooseFittingHeights>(&tileBoundingVolume);
            if (pLooseRegion) {
                pRegion = &pLooseRegion->getBoundingRegion();
            }
        }

        if (!pRegion) {
            return pResult;
        }

        const CesiumGeospatial::GlobeRectangle& rectangle = pRegion->getRectangle();
        double west = rectangle.getWest();
        double south = rectangle.getSouth();
        double east = rectangle.getEast();
        double north = rectangle.getNorth();

        pResult->model.emplace();
        tinygltf::Model& model = pResult->model.value();
        
        int positionBufferId = static_cast<int>(model.buffers.size());
        model.buffers.emplace_back();

        int positionBufferViewId = static_cast<int>(model.bufferViews.size());
        model.bufferViews.emplace_back();

        int positionAccessorId = static_cast<int>(model.accessors.size());
        model.accessors.emplace_back();

        tinygltf::Buffer& positionBuffer = model.buffers[positionBufferId];
        positionBuffer.data.resize(vertexCount * 3 * sizeof(float));

        tinygltf::BufferView& positionBufferView = model.bufferViews[positionBufferViewId];
        positionBufferView.buffer = positionBufferId;
        positionBufferView.byteOffset = 0;
        positionBufferView.byteStride = 3 * sizeof(float);
        positionBufferView.byteLength = positionBuffer.data.size();
        positionBufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;

        tinygltf::Accessor& positionAccessor = model.accessors[positionAccessorId];
        positionAccessor.bufferView = positionBufferViewId;
        positionAccessor.byteOffset = 0;
        positionAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        positionAccessor.count = vertexCount;
        positionAccessor.type = TINYGLTF_TYPE_VEC3;

        int meshId = static_cast<int>(model.meshes.size());
        model.meshes.emplace_back();
        tinygltf::Mesh& mesh = model.meshes[meshId];
        mesh.primitives.emplace_back();

        tinygltf::Primitive& primitive = mesh.primitives[0];
        primitive.mode = TINYGLTF_MODE_TRIANGLES;
        primitive.attributes.emplace("POSITION", positionAccessorId);
        primitive.material = 0;

        float* pPositions = reinterpret_cast<float*>(positionBuffer.data.data());
        size_t positionOutputIndex = 0;
        
        const Ellipsoid& ellipsoid = Ellipsoid::WGS84;

        double minX = std::numeric_limits<double>::max();
        double minY = std::numeric_limits<double>::max();
        double minZ = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::lowest();
        double maxY = std::numeric_limits<double>::lowest();
        double maxZ = std::numeric_limits<double>::lowest();

        for (size_t i = 0; i < vertexCount; ++i) {
            u += zigZagDecode(uBuffer[i]);
            v += zigZagDecode(vBuffer[i]);
            height += zigZagDecode(heightBuffer[i]);

            double uRatio = static_cast<double>(u) / 32767.0;
            double vRatio = static_cast<double>(v) / 32767.0;

            double longitude = Math::lerp(west, east, uRatio);
            double latitude = Math::lerp(south, north, vRatio);
            double heightMeters = Math::lerp(minimumHeight, maximumHeight, static_cast<double>(height) / 32767.0);

            glm::dvec3 position = ellipsoid.cartographicToCartesian(Cartographic(longitude, latitude, heightMeters));
            position -= center;
            pPositions[positionOutputIndex++] = static_cast<float>(position.x);
            pPositions[positionOutputIndex++] = static_cast<float>(position.y);
            pPositions[positionOutputIndex++] = static_cast<float>(position.z);

            minX = glm::min(minX, position.x);
            minY = glm::min(minY, position.y);
            minZ = glm::min(minZ, position.z);

            maxX = glm::max(maxX, position.x);
            maxY = glm::max(maxY, position.y);
            maxZ = glm::max(maxZ, position.z);
        }

        positionAccessor.minValues = { minX, minY, minZ };
        positionAccessor.maxValues = { maxX, maxY, maxZ };

        int indicesBufferId = static_cast<int>(model.buffers.size());
        model.buffers.emplace_back();
        tinygltf::Buffer& indicesBuffer = model.buffers[indicesBufferId];
        
        int indicesBufferViewId = static_cast<int>(model.bufferViews.size());
        model.bufferViews.emplace_back();
        tinygltf::BufferView& indicesBufferView = model.bufferViews[indicesBufferViewId];
        indicesBufferView.buffer = indicesBufferId;
        indicesBufferView.byteOffset = 0;
        indicesBufferView.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

        int indicesAccessorId = static_cast<int>(model.accessors.size());
        model.accessors.emplace_back();
        tinygltf::Accessor& indicesAccessor = model.accessors[indicesAccessorId];
        indicesAccessor.bufferView = indicesBufferViewId;
        indicesAccessor.byteOffset = 0;
        indicesAccessor.type = TINYGLTF_TYPE_SCALAR;

        primitive.indices = indicesBufferId;

        size_t indexSizeBytes;

        if (pHeader->vertexCount > 65536) {
            // 32-bit indices
            if ((readIndex % 4) != 0) {
                readIndex += 2;
                if (readIndex > data.size()) {
                    return pResult;
                }
            }

            uint32_t triangleCount = readValue<uint32_t>(data, readIndex, 0);
            readIndex += sizeof(uint32_t);
            if (readIndex > data.size()) {
                return pResult;
            }

            const gsl::span<const uint32_t> indices(reinterpret_cast<const uint32_t*>(data.data() + readIndex), triangleCount * 3);
            readIndex += indices.size_bytes();
            if (readIndex > data.size()) {
                return pResult;
            }

            indicesBuffer.data.resize(triangleCount * 3 * sizeof(uint32_t));
            indicesBufferView.byteLength = indicesBuffer.data.size();
            indicesBufferView.byteStride = sizeof(uint32_t);
            indicesAccessor.count = triangleCount * 3;
            indicesAccessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;

            gsl::span<uint32_t> outputIndices(reinterpret_cast<uint32_t*>(indicesBuffer.data.data()), indicesBuffer.data.size() / sizeof(uint32_t));
            decodeIndices(indices, outputIndices);

            indexSizeBytes = 4;
        } else {
            // 16-bit indices
            uint32_t triangleCount = readValue<uint32_t>(data, readIndex, 0);
            readIndex += sizeof(uint32_t);
            if (readIndex > data.size()) {
                return pResult;
            }

            const gsl::span<const uint16_t> indices(reinterpret_cast<const uint16_t*>(data.data() + readIndex), triangleCount * 3);
            readIndex += indices.size_bytes();
            if (readIndex > data.size()) {
                return pResult;
            }

            indicesBuffer.data.resize(triangleCount * 3 * sizeof(uint16_t));
            indicesBufferView.byteLength = indicesBuffer.data.size();
            indicesBufferView.byteStride = sizeof(uint16_t);
            indicesAccessor.count = triangleCount * 3;
            indicesAccessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;

            gsl::span<uint16_t> outputIndices(reinterpret_cast<uint16_t*>(indicesBuffer.data.data()), indicesBuffer.data.size() / sizeof(uint16_t));
            decodeIndices(indices, outputIndices);

            indexSizeBytes = 2;
        }

        // Skip edge indices (for now, TODO)
        uint32_t westVertexCount = readValue<uint32_t>(data, readIndex, 0);
        readIndex += sizeof(uint32_t);
        readIndex += westVertexCount * indexSizeBytes;

        uint32_t southVertexCount = readValue<uint32_t>(data, readIndex, 0);
        readIndex += sizeof(uint32_t);
        readIndex += southVertexCount * indexSizeBytes;

        uint32_t eastVertexCount = readValue<uint32_t>(data, readIndex, 0);
        readIndex += sizeof(uint32_t);
        readIndex += eastVertexCount * indexSizeBytes;

        uint32_t northVertexCount = readValue<uint32_t>(data, readIndex, 0);
        readIndex += sizeof(uint32_t);
        readIndex += northVertexCount * indexSizeBytes;

        if (readIndex > data.size()) {
            return pResult;
        }

        while (readIndex < data.size()) {
            if (readIndex + extensionHeaderLength > data.size()) {
                break;
            }

            uint8_t extensionID = *reinterpret_cast<const uint8_t*>(data.data() + readIndex);
            readIndex += sizeof(uint8_t);
            uint32_t extensionLength = *reinterpret_cast<const uint32_t*>(data.data() + readIndex);
            readIndex += sizeof(uint32_t);

            if (extensionID == 1) {
                // Oct-encoded per-vertex normals
                if (readIndex + vertexCount * 2 > data.size()) {
                    break;
                }

                const uint8_t* pNormalData = reinterpret_cast<const uint8_t*>(data.data() + readIndex);

                int normalBufferId = static_cast<int>(model.buffers.size());
                model.buffers.emplace_back();

                int normalBufferViewId = static_cast<int>(model.bufferViews.size());
                model.bufferViews.emplace_back();

                int normalAccessorId = static_cast<int>(model.accessors.size());
                model.accessors.emplace_back();

                tinygltf::Buffer& normalBuffer = model.buffers[normalBufferId];
                normalBuffer.data.resize(vertexCount * 3 * sizeof(float));

                tinygltf::BufferView& normalBufferView = model.bufferViews[normalBufferViewId];
                normalBufferView.buffer = normalBufferId;
                normalBufferView.byteOffset = 0;
                normalBufferView.byteStride = 3 * sizeof(float);
                normalBufferView.byteLength = normalBuffer.data.size();
                normalBufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;

                tinygltf::Accessor& normalAccessor = model.accessors[normalAccessorId];
                normalAccessor.bufferView = normalBufferViewId;
                normalAccessor.byteOffset = 0;
                normalAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
                normalAccessor.count = vertexCount;
                normalAccessor.type = TINYGLTF_TYPE_VEC3;
                
                primitive.attributes.emplace("NORMAL", normalAccessorId);

                float* pNormals = reinterpret_cast<float*>(normalBuffer.data.data());
                size_t normalOutputIndex = 0;

                for (size_t i = 0; i < vertexCount * 2; i += 2) {
                    glm::dvec3 normal = octDecode(pNormalData[i], pNormalData[i + 1]);
                    pNormals[normalOutputIndex++] = static_cast<float>(normal.x);
                    pNormals[normalOutputIndex++] = static_cast<float>(normal.y);
                    pNormals[normalOutputIndex++] = static_cast<float>(normal.z);
                }
            } else if (extensionID == 4) {
                // Metadata
                if (readIndex + sizeof(uint32_t) > data.size()) {
                    break;
                }

                uint32_t jsonLength = *reinterpret_cast<const uint32_t*>(data.data() + readIndex);

                if (readIndex + sizeof(uint32_t) + jsonLength > data.size()) {
                    break;
                }

                gsl::span<const char> json(reinterpret_cast<const char*>(data.data() + sizeof(uint32_t) + readIndex), jsonLength);
                processMetadata(id, json, *pResult);
            }

            readIndex += extensionLength;
        }

        model.nodes.emplace_back();
        tinygltf::Node& node = model.nodes[0];
        node.mesh = 0;
        node.matrix = {
            1.0, 0.0,  0.0, 0.0,
            0.0, 0.0, -1.0, 0.0,
            0.0, 1.0,  0.0, 0.0,
            center.x, center.z, -center.y, 1.0
        };

        pResult->updatedBoundingVolume = BoundingRegion(rectangle, minimumHeight, maximumHeight);

        return pResult;
    }

    struct TileRange {
        uint32_t minimumX;
        uint32_t minimumY;
        uint32_t maximumX;
        uint32_t maximumY;
    };

    static void from_json(const nlohmann::json& json, TileRange& range) {
        json.at("startX").get_to(range.minimumX);
        json.at("startY").get_to(range.minimumY);
        json.at("endX").get_to(range.maximumX);
        json.at("endY").get_to(range.maximumY);
    }

    static void processMetadata(const QuadtreeTileID& tileID, gsl::span<const char> metadataString, TileContentLoadResult& result) {
        using namespace nlohmann;
        json metadata = json::parse(metadataString.begin(), metadataString.end());

        json::iterator availableIt = metadata.find("available");
        if (availableIt == metadata.end()) {
            return;
        }

        json& available = *availableIt;
        if (available.size() == 0) {
            return;
        }

        uint32_t level = tileID.level + 1;
        for (size_t i = 0; i < available.size(); ++i) {
            std::vector<TileRange> rangesAtLevel = available[i].get<std::vector<TileRange>>();

            for (const TileRange& range : rangesAtLevel) {
                result.availableTileRectangles.push_back(CesiumGeometry::QuadtreeTileRectangularRange {
                    level,
                    range.minimumX,
                    range.minimumY,
                    range.maximumX,
                    range.maximumY
                });
            }

            ++level;
        }
    }

}