// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/AnimationChannelTarget.h"
#include "CesiumGltf/ExtensibleObject.h"
#include <cstdint>

namespace CesiumGltf {
    /**
     * @brief Targets an animation's sampler at a node's property.
     */
    struct AnimationChannel : public ExtensibleObject {

        /**
         * @brief The index of a sampler in this animation used to compute the value for the target.
         *
         * The index of a sampler in this animation used to compute the value for the target, e.g., a node's translation, rotation, or scale (TRS).
         */
        int32_t sampler;

        /**
         * @brief The index of the node and TRS property to target.
         */
        AnimationChannelTarget target;

    };
}
