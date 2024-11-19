// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <CesiumGltf/Library.h>
#include <CesiumGltf/NamedObject.h>

#include <cstdint>
#include <vector>

namespace CesiumGltf {
/**
 * @brief The root nodes of a scene.
 */
struct CESIUMGLTF_API Scene final : public CesiumGltf::NamedObject {
  static constexpr const char* TypeName = "Scene";

  /**
   * @brief The indices of each root node.
   */
  std::vector<int32_t> nodes;

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += int64_t(sizeof(Scene));
    accum += CesiumGltf::NamedObject::getSizeBytes() -
             int64_t(sizeof(CesiumGltf::NamedObject));
    accum += int64_t(sizeof(int32_t) * this->nodes.capacity());
    return accum;
  }
};
} // namespace CesiumGltf
