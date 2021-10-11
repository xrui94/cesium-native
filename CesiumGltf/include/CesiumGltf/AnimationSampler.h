// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "Library.h"

#include <CesiumUtility/ExtensibleObject.h>

#include <cstdint>

using namespace CesiumUtility;

namespace CesiumGltf {
/**
 * @brief An animation sampler combines timestamps with a sequence of output
 * values and defines an interpolation algorithm.
 */
struct CESIUMGLTF_API AnimationSampler final : public ExtensibleObject {
  static inline constexpr const char* TypeName = "AnimationSampler";

  /**
   * @brief Known values for Interpolation algorithm.
   */
  struct Interpolation {
    inline static const std::string LINEAR = "LINEAR";

    inline static const std::string STEP = "STEP";

    inline static const std::string CUBICSPLINE = "CUBICSPLINE";
  };

  /**
   * @brief The index of an accessor containing keyframe timestamps.
   *
   * The accessor **MUST** be of scalar type with floating-point components. The
   * values represent time in seconds with `time[0] >= 0.0`, and strictly
   * increasing values, i.e., `time[n + 1] > time[n]`.
   */
  int32_t input = -1;

  /**
   * @brief Interpolation algorithm.
   *
   * Known values are defined in {@link Interpolation}.
   *
   */
  std::string interpolation = Interpolation::LINEAR;

  /**
   * @brief The index of an accessor, containing keyframe output values.
   */
  int32_t output = -1;
};
} // namespace CesiumGltf
