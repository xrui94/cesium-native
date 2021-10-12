// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "ClassProperty.h"
#include "Library.h"

#include <CesiumUtility/ExtensibleObject.h>

#include <optional>
#include <string>
#include <unordered_map>

namespace CesiumGltf {
/**
 * @brief A class containing a set of properties.
 */
struct CESIUMGLTF_API Class final : public CesiumUtility::ExtensibleObject {
  static inline constexpr const char* TypeName = "Class";

  /**
   * @brief The name of the class, e.g. for display purposes.
   */
  std::optional<std::string> name;

  /**
   * @brief The description of the class.
   */
  std::optional<std::string> description;

  /**
   * @brief A dictionary, where each key is a property ID and each value is an
   * object defining the property.
   */
  std::unordered_map<std::string, ClassProperty> properties;
};
} // namespace CesiumGltf
