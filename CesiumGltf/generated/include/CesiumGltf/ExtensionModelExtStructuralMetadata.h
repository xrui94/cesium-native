// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/Library.h"
#include "CesiumGltf/PropertyAttribute.h"
#include "CesiumGltf/PropertyTable.h"
#include "CesiumGltf/PropertyTexture.h"
#include "CesiumGltf/Schema.h"
#include "CesiumUtility/IntrusivePointer.h"

#include <CesiumUtility/ExtensibleObject.h>

#include <optional>
#include <string>
#include <vector>

namespace CesiumGltf {
/**
 * @brief glTF extension that provides structural metadata about vertices,
 * texels, and features in a glTF asset.
 */
struct CESIUMGLTF_API ExtensionModelExtStructuralMetadata final
    : public CesiumUtility::ExtensibleObject {
  static inline constexpr const char* TypeName =
      "ExtensionModelExtStructuralMetadata";
  static inline constexpr const char* ExtensionName = "EXT_structural_metadata";

  /**
   * @brief An object defining classes and enums.
   */
  CesiumUtility::IntrusivePointer<CesiumGltf::Schema> schema;

  /**
   * @brief The URI (or IRI) of the external schema file.
   */
  std::optional<std::string> schemaUri;

  /**
   * @brief An array of property table definitions, which may be referenced by
   * index.
   */
  std::vector<CesiumGltf::PropertyTable> propertyTables;

  /**
   * @brief An array of property texture definitions, which may be referenced by
   * index.
   */
  std::vector<CesiumGltf::PropertyTexture> propertyTextures;

  /**
   * @brief An array of property attribute definitions, which may be referenced
   * by index.
   */
  std::vector<CesiumGltf::PropertyAttribute> propertyAttributes;

  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += this->schema->getSizeBytes();
    accum += sizeof(this->schemaUri) +
             (this->schemaUri.has_value() ? this->schemaUri->size() : 0);
    for (const CesiumGltf::PropertyTable& value : this->propertyTables) {
      accum += value.getSizeBytes();
    }
    for (const CesiumGltf::PropertyTexture& value : this->propertyTextures) {
      accum += value.getSizeBytes();
    }
    for (const CesiumGltf::PropertyAttribute& value :
         this->propertyAttributes) {
      accum += value.getSizeBytes();
    }
    return accum;
  }
};
} // namespace CesiumGltf
