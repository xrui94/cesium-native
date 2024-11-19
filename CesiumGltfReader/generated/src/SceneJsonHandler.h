// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "NamedObjectJsonHandler.h"

#include <CesiumGltf/Scene.h>
#include <CesiumJsonReader/ArrayJsonHandler.h>
#include <CesiumJsonReader/IntegerJsonHandler.h>

namespace CesiumJsonReader {
class JsonReaderOptions;
} // namespace CesiumJsonReader

namespace CesiumGltfReader {
class SceneJsonHandler : public CesiumGltfReader::NamedObjectJsonHandler {
public:
  using ValueType = CesiumGltf::Scene;

  explicit SceneJsonHandler(
      const CesiumJsonReader::JsonReaderOptions& options) noexcept;
  void reset(IJsonHandler* pParentHandler, CesiumGltf::Scene* pObject);

  IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeyScene(
      const std::string& objectType,
      const std::string_view& str,
      CesiumGltf::Scene& o);

private:
  CesiumGltf::Scene* _pObject = nullptr;
  CesiumJsonReader::
      ArrayJsonHandler<int32_t, CesiumJsonReader::IntegerJsonHandler<int32_t>>
          _nodes;
};
} // namespace CesiumGltfReader
