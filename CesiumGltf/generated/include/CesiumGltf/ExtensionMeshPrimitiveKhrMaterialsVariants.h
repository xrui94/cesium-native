// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <CesiumGltf/ExtensionMeshPrimitiveKhrMaterialsVariantsMappingsValue.h>
#include <CesiumGltf/Library.h>
#include <CesiumUtility/ExtensibleObject.h>

#include <vector>

namespace CesiumGltf {
/**
 * @brief KHR_materials_variants glTF Mesh Primitive Extension
 */
struct CESIUMGLTF_API ExtensionMeshPrimitiveKhrMaterialsVariants final
    : public CesiumUtility::ExtensibleObject {
  static constexpr const char* TypeName =
      "ExtensionMeshPrimitiveKhrMaterialsVariants";
  static constexpr const char* ExtensionName = "KHR_materials_variants";

  /**
   * @brief A list of material to variant mappings
   *
   * An array of object values that associate an indexed material to a set of
   * variants.
   */
  std::vector<
      CesiumGltf::ExtensionMeshPrimitiveKhrMaterialsVariantsMappingsValue>
      mappings;

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += int64_t(sizeof(ExtensionMeshPrimitiveKhrMaterialsVariants));
    accum += CesiumUtility::ExtensibleObject::getSizeBytes() -
             int64_t(sizeof(CesiumUtility::ExtensibleObject));
    accum += int64_t(
        sizeof(CesiumGltf::
                   ExtensionMeshPrimitiveKhrMaterialsVariantsMappingsValue) *
        this->mappings.capacity());
    for (const CesiumGltf::
             ExtensionMeshPrimitiveKhrMaterialsVariantsMappingsValue& value :
         this->mappings) {
      accum +=
          value.getSizeBytes() -
          int64_t(sizeof(
              CesiumGltf::
                  ExtensionMeshPrimitiveKhrMaterialsVariantsMappingsValue));
    }
    return accum;
  }
};
} // namespace CesiumGltf
