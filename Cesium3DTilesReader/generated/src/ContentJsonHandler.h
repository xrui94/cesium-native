// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "BoundingVolumeJsonHandler.h"
#include "MetadataEntityJsonHandler.h"

#include <Cesium3DTiles/Content.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/IntegerJsonHandler.h>
#include <CesiumJsonReader/StringJsonHandler.h>

namespace CesiumJsonReader {
class JsonReaderOptions;
} // namespace CesiumJsonReader

namespace Cesium3DTilesReader {
class ContentJsonHandler
    : public CesiumJsonReader::ExtensibleObjectJsonHandler {
public:
  using ValueType = Cesium3DTiles::Content;

  explicit ContentJsonHandler(
      const CesiumJsonReader::JsonReaderOptions& options) noexcept;
  void reset(IJsonHandler* pParentHandler, Cesium3DTiles::Content* pObject);

  IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeyContent(
      const std::string& objectType,
      const std::string_view& str,
      Cesium3DTiles::Content& o);

private:
  Cesium3DTiles::Content* _pObject = nullptr;
  BoundingVolumeJsonHandler _boundingVolume;
  CesiumJsonReader::StringJsonHandler _uri;
  MetadataEntityJsonHandler _metadata;
  CesiumJsonReader::IntegerJsonHandler<int64_t> _group;
};
} // namespace Cesium3DTilesReader
