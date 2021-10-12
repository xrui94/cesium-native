// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "Library.h"

#include <CesiumUtility/ExtensibleObject.h>

#include <cstdint>
#include <unordered_map>

namespace CesiumGltf {
/**
 * @brief undefined
 */
struct CESIUMGLTF_API KHR_draco_mesh_compression final
    : public CesiumUtility::ExtensibleObject {
  static inline constexpr const char* TypeName = "KHR_draco_mesh_compression";
  static inline constexpr const char* ExtensionName =
      "KHR_draco_mesh_compression";

  /**
   * @brief The index of the bufferView.
   */
  int32_t bufferView = -1;

  /**
   * @brief A dictionary object, where each key corresponds to an attribute and
   * its unique attribute id stored in the compressed geometry.
   */
  std::unordered_map<std::string, int32_t> attributes;
};
} // namespace CesiumGltf
