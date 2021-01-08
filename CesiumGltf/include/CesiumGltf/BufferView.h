// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/NamedObject.h"
#include <cstdint>

namespace CesiumGltf {
    /**
     * @brief A view into a buffer generally representing a subset of the buffer.
     */
    struct BufferView : public NamedObject {
        enum class Target {
            ARRAY_BUFFER = 34962,

            ELEMENT_ARRAY_BUFFER = 34963
        };

        /**
         * @brief The index of the buffer.
         */
        int32_t buffer;

        /**
         * @brief The offset into the buffer in bytes.
         */
        int64_t byteOffset;

        /**
         * @brief The length of the bufferView in bytes.
         */
        int64_t byteLength;

        /**
         * @brief The stride, in bytes.
         *
         * The stride, in bytes, between vertex attributes.  When this is not defined, data is tightly packed. When two or more accessors use the same bufferView, this field must be defined.
         */
        int64_t byteStride;

        /**
         * @brief The target that the GPU buffer should be bound to.
         */
        Target target;

    };
}
