// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <CesiumGltf/Library.h>
#include <CesiumUtility/ExtensibleObject.h>

#include <optional>

namespace CesiumGltf {
/**
 * @brief A perspective camera containing properties to create a perspective
 * projection matrix.
 */
struct CESIUMGLTF_API CameraPerspective final
    : public CesiumUtility::ExtensibleObject {
  /**
   * @brief The original name of this type.
   */
  static constexpr const char* TypeName = "CameraPerspective";

  /**
   * @brief The floating-point aspect ratio of the field of view.
   *
   * When undefined, the aspect ratio of the rendering viewport **MUST** be
   * used.
   */
  std::optional<double> aspectRatio;

  /**
   * @brief The floating-point vertical field of view in radians. This value
   * **SHOULD** be less than π.
   */
  double yfov = double();

  /**
   * @brief The floating-point distance to the far clipping plane.
   *
   * When defined, `zfar` **MUST** be greater than `znear`. If `zfar` is
   * undefined, client implementations **SHOULD** use infinite projection
   * matrix.
   */
  std::optional<double> zfar;

  /**
   * @brief The floating-point distance to the near clipping plane.
   */
  double znear = double();

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += int64_t(sizeof(CameraPerspective));
    accum += CesiumUtility::ExtensibleObject::getSizeBytes() -
             int64_t(sizeof(CesiumUtility::ExtensibleObject));

    return accum;
  }
};
} // namespace CesiumGltf
