// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <CesiumGltf/Library.h>
#include <CesiumGltf/NamedObject.h>

#include <cstdint>
#include <optional>

namespace CesiumGltf {
/**
 * @brief A view into a buffer generally representing a subset of the buffer.
 */
struct CESIUMGLTF_API BufferView final : public CesiumGltf::NamedObject {
  /**
   * @brief The original name of this type.
   */
  static constexpr const char* TypeName = "BufferView";

  /**
   * @brief Known values for The hint representing the intended GPU buffer type
   * to use with this buffer view.
   */
  struct Target {
    static constexpr int32_t ARRAY_BUFFER = 34962;

    static constexpr int32_t ELEMENT_ARRAY_BUFFER = 34963;
  };

  /**
   * @brief The index of the buffer.
   */
  int32_t buffer = -1;

  /**
   * @brief The offset into the buffer in bytes.
   */
  int64_t byteOffset = 0;

  /**
   * @brief The length of the bufferView in bytes.
   */
  int64_t byteLength = int64_t();

  /**
   * @brief The stride, in bytes.
   *
   * The stride, in bytes, between vertex attributes.  When this is not defined,
   * data is tightly packed. When two or more accessors use the same buffer
   * view, this field **MUST** be defined.
   */
  std::optional<int64_t> byteStride;

  /**
   * @brief The hint representing the intended GPU buffer type to use with this
   * buffer view.
   *
   * Known values are defined in {@link Target}.
   *
   */
  std::optional<int32_t> target;

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += int64_t(sizeof(BufferView));
    accum += CesiumGltf::NamedObject::getSizeBytes() -
             int64_t(sizeof(CesiumGltf::NamedObject));

    return accum;
  }
};
} // namespace CesiumGltf
