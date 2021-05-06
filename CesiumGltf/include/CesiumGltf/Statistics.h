// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/ClassStatistics.h"
#include "CesiumGltf/ExtensibleObject.h"
#include "CesiumGltf/Library.h"
#include <optional>
#include <unordered_map>

namespace CesiumGltf {
/**
 * @brief Statistics about features.
 */
struct CESIUMGLTF_API Statistics final : public ExtensibleObject {
  static inline constexpr const char* TypeName = "Statistics";

  /**
   * @brief A dictionary, where each key is a class ID declared in the `classes`
   * dictionary and each value is an object containing statistics about features
   * that conform to the class.
   */
  std::unordered_map<std::string, std::optional<ClassStatistics>> classes;
};
} // namespace CesiumGltf
