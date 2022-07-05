#include "SubtreeAvailability.h"
#include <CesiumGeometry/QuadtreeTileID.h>
#include <libmorton/morton.h>
#include <catch2/catch.hpp>
#include <vector>
#include <cstddef>

using namespace Cesium3DTilesSelection;

namespace {
void markAvailableForQuadtree(
    const CesiumGeometry::QuadtreeTileID& tileID,
    std::vector<std::byte>& available) {
  // This function assumes that subtree tile ID is (0, 0, 0).
  // TileID must be within the subtree
  uint64_t numOfTilesFromRootToParentLevel =
      ((1 << (2 * tileID.level)) - 1) / 3;
  uint64_t availabilityBitIndex =
      numOfTilesFromRootToParentLevel +
      libmorton::morton2D_64_encode(tileID.x, tileID.y);
  const uint64_t byteIndex = availabilityBitIndex / 8;
  const uint64_t bitIndex = availabilityBitIndex % 8;
  available[byteIndex] |= std::byte(1 << bitIndex);
}
} // namespace

TEST_CASE("Test SubtreeAvailability methods") {

  uint64_t maxSubtreeLevels = 5;
  uint64_t maxTiles = static_cast<uint64_t>(
      std::pow(4.0, static_cast<double>(maxSubtreeLevels - 1)));
  uint64_t bufferSize = static_cast<uint64_t>(std::ceil(maxTiles / 8));

  // create expected available tiles
  std::vector<CesiumGeometry::QuadtreeTileID> availableTileIDs;
  availableTileIDs.emplace_back(CesiumGeometry::QuadtreeTileID{0, 0, 0});
  availableTileIDs.emplace_back(CesiumGeometry::QuadtreeTileID{1, 1, 0});
  availableTileIDs.emplace_back(CesiumGeometry::QuadtreeTileID{2, 2, 2});
  availableTileIDs.emplace_back(CesiumGeometry::QuadtreeTileID{2, 3, 1});

  // create expected unavailable tiles
  std::vector<CesiumGeometry::QuadtreeTileID> unavailableTileIDs;
  unavailableTileIDs.emplace_back(CesiumGeometry::QuadtreeTileID{1, 1, 1});
  unavailableTileIDs.emplace_back(CesiumGeometry::QuadtreeTileID{1, 0, 0});
  unavailableTileIDs.emplace_back(CesiumGeometry::QuadtreeTileID{2, 0, 2});
  unavailableTileIDs.emplace_back(CesiumGeometry::QuadtreeTileID{2, 3, 0});
  unavailableTileIDs.emplace_back(CesiumGeometry::QuadtreeTileID{3, 0, 4});

  // setup tile availability buffer
  std::vector<std::byte> contentAvailabilityBuffer(bufferSize);
  std::vector<std::byte> tileAvailabilityBuffer(bufferSize);
  std::vector<std::byte> subtreeAvailabilityBuffer(
      uint64_t(1) << (2 * (maxSubtreeLevels)));
  for (const auto& tileID : availableTileIDs) {
    markAvailableForQuadtree(tileID, tileAvailabilityBuffer);
    markAvailableForQuadtree(tileID, contentAvailabilityBuffer);
  }

  std::vector<std::vector<std::byte>> buffers{
      std::move(tileAvailabilityBuffer),
      std::move(subtreeAvailabilityBuffer),
      std::move(contentAvailabilityBuffer)};

  SubtreeBufferViewAvailability tileAvailability{buffers[0]};
  SubtreeBufferViewAvailability subtreeAvailability{buffers[1]};
  std::vector<AvailabilityView> contentAvailability{
      SubtreeBufferViewAvailability{buffers[2]}};

  SubtreeAvailability quadtreeAvailability(
      2,
      tileAvailability,
      subtreeAvailability,
      std::move(contentAvailability),
      std::move(buffers));

  SECTION("isTileAvailable()") {
    for (const auto& tileID : availableTileIDs) {
      CHECK(quadtreeAvailability.isTileAvailable(
          tileID.level,
          libmorton::morton2D_64_encode(tileID.x, tileID.y)));
    }

    for (const auto& tileID : unavailableTileIDs) {
      CHECK(!quadtreeAvailability.isTileAvailable(
          tileID.level,
          libmorton::morton2D_64_encode(tileID.x, tileID.y)));
    }
  }

  SECTION("isContentAvailable()") {}

  SECTION("isSubtreeAvailable()") {}
}
