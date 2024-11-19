// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <CesiumGltf/EnumValue.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/IntegerJsonHandler.h>
#include <CesiumJsonReader/StringJsonHandler.h>

namespace CesiumJsonReader {
class JsonReaderOptions;
} // namespace CesiumJsonReader

namespace CesiumGltfReader {
class EnumValueJsonHandler
    : public CesiumJsonReader::ExtensibleObjectJsonHandler {
public:
  using ValueType = CesiumGltf::EnumValue;

  explicit EnumValueJsonHandler(
      const CesiumJsonReader::JsonReaderOptions& options) noexcept;
  void reset(IJsonHandler* pParentHandler, CesiumGltf::EnumValue* pObject);

  IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeyEnumValue(
      const std::string& objectType,
      const std::string_view& str,
      CesiumGltf::EnumValue& o);

private:
  CesiumGltf::EnumValue* _pObject = nullptr;
  CesiumJsonReader::StringJsonHandler _name;
  CesiumJsonReader::StringJsonHandler _description;
  CesiumJsonReader::IntegerJsonHandler<int64_t> _value;
};
} // namespace CesiumGltfReader
