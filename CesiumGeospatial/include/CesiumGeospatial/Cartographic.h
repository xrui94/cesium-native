#pragma once

#include "CesiumGeospatial/Library.h"

namespace CesiumGeospatial {

    /**
     * @brief A position defined by longitude, latitude, and height.
     */
    class CESIUMGEOSPATIAL_API Cartographic {
    public:

        /**
         * @brief Creates a new instance.
         * 
         * @param longitudeRadians The longitude, in radians.
         * @param latitudeRadians The latitude, in radians.
         * @param heightMeters The height, in meters. Default value: 0.0.
         */
        Cartographic(double longitudeRadians, double latitudeRadians, double heightMeters = 0.0);

        /**
         * @brief Creates a new instance from a longitude and latitude specified in degrees, and a height given in meters.
         *
         * The values in the resulting object will be in radians.
         * 
         * @param longitudeDegrees The longitude, in degrees.
         * @param latitudeDegrees The latitude, in degrees.
         * @param heightMeters The height, in meters. Default value: 0.0.
         */
        static Cartographic fromDegrees(double longitudeDegrees, double latitudeDegrees, double heightMeters = 0.0);

        /**
         * @brief The longitude, in radians.
         */
        double longitude;

        /**
         * @brief The latitude, in radians.
         */
        double latitude;

        /**
         * @brief The height, in meters.
         */
        double height;
    };
}