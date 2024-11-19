// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "TextureInfoJsonHandler.h"

#include <CesiumGltf/MaterialNormalTextureInfo.h>
#include <CesiumJsonReader/DoubleJsonHandler.h>

namespace CesiumJsonReader {
class JsonReaderOptions;
} // namespace CesiumJsonReader

namespace CesiumGltfReader {
class MaterialNormalTextureInfoJsonHandler : public TextureInfoJsonHandler {
public:
  using ValueType = CesiumGltf::MaterialNormalTextureInfo;

  explicit MaterialNormalTextureInfoJsonHandler(
      const CesiumJsonReader::JsonReaderOptions& options) noexcept;
  void reset(
      IJsonHandler* pParentHandler,
      CesiumGltf::MaterialNormalTextureInfo* pObject);

  IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeyMaterialNormalTextureInfo(
      const std::string& objectType,
      const std::string_view& str,
      CesiumGltf::MaterialNormalTextureInfo& o);

private:
  CesiumGltf::MaterialNormalTextureInfo* _pObject = nullptr;
  CesiumJsonReader::DoubleJsonHandler _scale;
};
} // namespace CesiumGltfReader
