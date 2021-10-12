// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "ClassJsonHandler.h"
#include "EnumJsonHandler.h"

#include <CesiumGltf/Schema.h>
#include <CesiumJsonReader/DictionaryJsonHandler.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/StringJsonHandler.h>

namespace CesiumGltf {
class ExtensionReaderContext;

class SchemaJsonHandler : public CesiumJsonReader::ExtensibleObjectJsonHandler {
public:
  using ValueType = Schema;

  SchemaJsonHandler(
      const CesiumJsonReader::ExtensionReaderContext& context) noexcept;
  void reset(IJsonHandler* pParentHandler, Schema* pObject);

  virtual IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeySchema(
      const std::string& objectType,
      const std::string_view& str,
      Schema& o);

private:
  Schema* _pObject = nullptr;
  CesiumJsonReader::StringJsonHandler _name;
  CesiumJsonReader::StringJsonHandler _description;
  CesiumJsonReader::StringJsonHandler _version;
  CesiumJsonReader::DictionaryJsonHandler<Class, ClassJsonHandler> _classes;
  CesiumJsonReader::DictionaryJsonHandler<Enum, EnumJsonHandler> _enums;
};
} // namespace CesiumGltf
