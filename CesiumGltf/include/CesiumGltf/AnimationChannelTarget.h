// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/ExtensibleObject.h"
#include <cstdint>

namespace CesiumGltf {
    /**
     * @brief The index of the node and TRS property that an animation channel targets.
     */
    struct AnimationChannelTarget : public ExtensibleObject {
        enum class Path {
            translation,

            rotation,

            scale,

            weights
        };

        /**
         * @brief The index of the node to target.
         */
        int32_t node;

        /**
         * @brief The name of the node's TRS property to modify, or the "weights" of the Morph Targets it instantiates. For the "translation" property, the values that are provided by the sampler are the translation along the x, y, and z axes. For the "rotation" property, the values are a quaternion in the order (x, y, z, w), where w is the scalar. For the "scale" property, the values are the scaling factors along the x, y, and z axes.
         */
        Path path;

    };
}
