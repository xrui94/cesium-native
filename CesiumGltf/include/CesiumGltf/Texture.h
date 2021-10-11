// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "Library.h"
#include "NamedObject.h"

#include <cstdint>

using namespace CesiumUtility;

namespace CesiumGltf {
/**
 * @brief A texture and its sampler.
 */
struct CESIUMGLTF_API Texture final : public NamedObject {
  static inline constexpr const char* TypeName = "Texture";

  /**
   * @brief The index of the sampler used by this texture. When undefined, a
   * sampler with repeat wrapping and auto filtering **SHOULD** be used.
   */
  int32_t sampler = -1;

  /**
   * @brief The index of the image used by this texture. When undefined, an
   * extension or other mechanism **SHOULD** supply an alternate texture source,
   * otherwise behavior is undefined.
   */
  int32_t source = -1;
};
} // namespace CesiumGltf
