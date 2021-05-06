// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/Class.h"
#include "CesiumGltf/Enum.h"
#include "CesiumGltf/ExtensibleObject.h"
#include "CesiumGltf/Library.h"
#include <optional>
#include <string>
#include <unordered_map>

namespace CesiumGltf {
/**
 * @brief An object defining classes and enums.
 */
struct CESIUMGLTF_API Schema final : public ExtensibleObject {
  static inline constexpr const char* TypeName = "Schema";

  /**
   * @brief The name of the schema.
   */
  std::optional<std::string> name;

  /**
   * @brief The description of the schema.
   */
  std::optional<std::string> description;

  /**
   * @brief Application-specific version of the schema.
   */
  std::optional<std::string> version;

  /**
   * @brief A dictionary, where each key is a class ID and each value is an
   * object defining the class.
   */
  std::unordered_map<std::string, std::optional<Class>> classes;

  /**
   * @brief A dictionary, where each key is an enum ID and each value is an
   * object defining the values for the enum.
   */
  std::unordered_map<std::string, std::optional<Enum>> enums;
};
} // namespace CesiumGltf
