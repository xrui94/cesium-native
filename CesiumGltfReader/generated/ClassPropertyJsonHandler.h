// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <CesiumGltf/ClassProperty.h>
#include <CesiumJsonReader/BoolJsonHandler.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/IntegerJsonHandler.h>
#include <CesiumJsonReader/JsonObjectJsonHandler.h>
#include <CesiumJsonReader/StringJsonHandler.h>

namespace CesiumGltf {
class ExtensionReaderContext;

class ClassPropertyJsonHandler
    : public CesiumJsonReader::ExtensibleObjectJsonHandler {
public:
  using ValueType = ClassProperty;

  ClassPropertyJsonHandler(
      const CesiumJsonReader::ExtensionReaderContext& context) noexcept;
  void reset(IJsonHandler* pParentHandler, ClassProperty* pObject);

  virtual IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeyClassProperty(
      const std::string& objectType,
      const std::string_view& str,
      ClassProperty& o);

private:
  ClassProperty* _pObject = nullptr;
  CesiumJsonReader::StringJsonHandler _name;
  CesiumJsonReader::StringJsonHandler _description;
  CesiumJsonReader::StringJsonHandler _type;
  CesiumJsonReader::StringJsonHandler _enumType;
  CesiumJsonReader::JsonObjectJsonHandler _componentType;
  CesiumJsonReader::IntegerJsonHandler<int64_t> _componentCount;
  CesiumJsonReader::BoolJsonHandler _normalized;
  CesiumJsonReader::JsonObjectJsonHandler _max;
  CesiumJsonReader::JsonObjectJsonHandler _min;
  CesiumJsonReader::JsonObjectJsonHandler _defaultProperty;
  CesiumJsonReader::BoolJsonHandler _optional;
  CesiumJsonReader::StringJsonHandler _semantic;
};
} // namespace CesiumGltf
