// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "ExtensionExtFeatureMetadataFeatureIDAttributeJsonHandler.h"
#include "ExtensionExtFeatureMetadataFeatureIDTextureJsonHandler.h"

#include <CesiumGltf/ExtensionMeshPrimitiveExtFeatureMetadata.h>
#include <CesiumJsonReader/ArrayJsonHandler.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/StringJsonHandler.h>

namespace CesiumJsonReader {
class JsonReaderOptions;
}

namespace CesiumGltfReader {
class ExtensionMeshPrimitiveExtFeatureMetadataJsonHandler
    : public CesiumJsonReader::ExtensibleObjectJsonHandler,
      public CesiumJsonReader::IExtensionJsonHandler {
public:
  using ValueType = CesiumGltf::ExtensionMeshPrimitiveExtFeatureMetadata;

  static inline constexpr const char* ExtensionName = "EXT_feature_metadata";

  ExtensionMeshPrimitiveExtFeatureMetadataJsonHandler(
      const CesiumJsonReader::JsonReaderOptions& options) noexcept;
  void reset(
      IJsonHandler* pParentHandler,
      CesiumGltf::ExtensionMeshPrimitiveExtFeatureMetadata* pObject);

  virtual IJsonHandler* readObjectKey(const std::string_view& str) override;

  virtual void reset(
      IJsonHandler* pParentHandler,
      CesiumUtility::ExtensibleObject& o,
      const std::string_view& extensionName) override;

  virtual IJsonHandler& getHandler() override { return *this; }

protected:
  IJsonHandler* readObjectKeyExtensionMeshPrimitiveExtFeatureMetadata(
      const std::string& objectType,
      const std::string_view& str,
      CesiumGltf::ExtensionMeshPrimitiveExtFeatureMetadata& o);

private:
  CesiumGltf::ExtensionMeshPrimitiveExtFeatureMetadata* _pObject = nullptr;
  CesiumJsonReader::ArrayJsonHandler<
      CesiumGltf::ExtensionExtFeatureMetadataFeatureIDAttribute,
      ExtensionExtFeatureMetadataFeatureIDAttributeJsonHandler>
      _featureIdAttributes;
  CesiumJsonReader::ArrayJsonHandler<
      CesiumGltf::ExtensionExtFeatureMetadataFeatureIDTexture,
      ExtensionExtFeatureMetadataFeatureIDTextureJsonHandler>
      _featureIdTextures;
  CesiumJsonReader::
      ArrayJsonHandler<std::string, CesiumJsonReader::StringJsonHandler>
          _featureTextures;
};
} // namespace CesiumGltfReader
