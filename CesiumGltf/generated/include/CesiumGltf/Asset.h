// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <CesiumGltf/Library.h>
#include <CesiumUtility/ExtensibleObject.h>

#include <optional>
#include <string>

namespace CesiumGltf {
/**
 * @brief Metadata about the glTF asset.
 */
struct CESIUMGLTF_API Asset final : public CesiumUtility::ExtensibleObject {
  /**
   * @brief The original name of this type.
   */
  static constexpr const char* TypeName = "Asset";

  /**
   * @brief A copyright message suitable for display to credit the content
   * creator.
   */
  std::optional<std::string> copyright;

  /**
   * @brief Tool that generated this glTF model.  Useful for debugging.
   */
  std::optional<std::string> generator;

  /**
   * @brief The glTF version in the form of `<major>.<minor>` that this asset
   * targets.
   */
  std::string version;

  /**
   * @brief The minimum glTF version in the form of `<major>.<minor>` that this
   * asset targets. This property **MUST NOT** be greater than the asset
   * version.
   */
  std::optional<std::string> minVersion;

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += int64_t(sizeof(Asset));
    accum += CesiumUtility::ExtensibleObject::getSizeBytes() -
             int64_t(sizeof(CesiumUtility::ExtensibleObject));
    if (this->copyright) {
      accum += int64_t(this->copyright->capacity() * sizeof(char));
    }
    if (this->generator) {
      accum += int64_t(this->generator->capacity() * sizeof(char));
    }
    accum += int64_t(this->version.capacity() * sizeof(char));
    if (this->minVersion) {
      accum += int64_t(this->minVersion->capacity() * sizeof(char));
    }
    return accum;
  }
};
} // namespace CesiumGltf
