// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <CesiumGltf/Library.h>
#include <CesiumGltf/TextureInfo.h>

namespace CesiumGltf {
/**
 * @brief Material Occlusion Texture Info
 */
struct CESIUMGLTF_API MaterialOcclusionTextureInfo final : public TextureInfo {
  static constexpr const char* TypeName = "MaterialOcclusionTextureInfo";

  /**
   * @brief A scalar multiplier controlling the amount of occlusion applied.
   *
   * A scalar parameter controlling the amount of occlusion applied. A value of
   * `0.0` means no occlusion. A value of `1.0` means full occlusion. This value
   * affects the final occlusion value as: `1.0 + strength * (<sampled occlusion
   * texture value> - 1.0)`.
   */
  double strength = 1;

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += int64_t(sizeof(MaterialOcclusionTextureInfo));
    accum += TextureInfo::getSizeBytes() - int64_t(sizeof(TextureInfo));

    return accum;
  }
};
} // namespace CesiumGltf
