#pragma once

#include "Cesium3DTiles/Library.h"
#include "Cesium3DTiles/RasterOverlay.h"
#include "Cesium3DTiles/CreditSystem.h"
#include "CesiumAsync/IAssetRequest.h"
#include "CesiumGeospatial/Ellipsoid.h"
#include <functional>

namespace Cesium3DTiles {

    /**
     * @brief Styles of Bing Maps overlays.
     *
     * Constants that can be passed to a {@link BingMapsRasterOverlay} to
     * indicate the overlays that should be painted.
     */
    struct BingMapsStyle final {
        /**
         * @brief Aerial imagery.
         */
        static const std::string AERIAL;

        /**
         * @brief Aerial imagery with a road overlay.
         * @deprecated See https://github.com/CesiumGS/cesium/issues/7128.
         * Use `BingMapsStyle.AERIAL_WITH_LABELS_ON_DEMAND` instead
         */
        static const std::string AERIAL_WITH_LABELS;

        /**
         * @brief Aerial imagery with a road overlay.
         */
        static const std::string AERIAL_WITH_LABELS_ON_DEMAND;

        /**
         * @brief Roads without additional imagery.
         * @deprecated See https://github.com/CesiumGS/cesium/issues/7128.
         * Use `BingMapsStyle.ROAD_ON_DEMAND` instead
         */
        static const std::string ROAD;

        /**
         * @brief Roads without additional imagery.
         */
        static const std::string ROAD_ON_DEMAND;

        /**
         * @brief A dark version of the road maps.
         */
        static const std::string CANVAS_DARK;

        /**
         * @brief A lighter version of the road maps.
         */
        static const std::string CANVAS_LIGHT;

        /**
         * @brief A grayscale version of the road maps.
         */
        static const std::string CANVAS_GRAY;

        /**
         * @brief Ordnance Survey imagery. 
         * 
         * This imagery is visible only for the London, UK area.
         */
        static const std::string ORDNANCE_SURVEY;

        /**
         * @brief Collins Bart imagery.
         */
        static const std::string COLLINS_BART;
    };

    /**
     * @brief A {@link RasterOverlay} that uses Bing Maps as the source for the imagery data.
     */
    class CESIUM3DTILES_API BingMapsRasterOverlay final : public RasterOverlay {
    public:

        /**
         * @brief Creates a new instance.
         *
         * @param url The url of the Bing Maps server hosting the imagery.
         * @param key The Bing Maps key for your application, which can be created at https://www.bingmapsportal.com/.
         * @param mapStyle The type of Bing Maps imagery to load. A value from {@link BingMapsStyle}, with {@link BingMapsStyle::AERIAL} being the default.
         * @param culture The culture to use when requesting Bing Maps imagery. Not all cultures are supported. 
         * See http://msdn.microsoft.com/en-us/library/hh441729.aspx for information on the supported cultures.
         * @param ellipsoid The ellipsoid. Default value: {@link CesiumGeospatial::Ellipsoid::WGS84}.
         */
        BingMapsRasterOverlay(
            const std::string& url,
            const std::string& key,
            const std::shared_ptr<CreditSystem>& pCreditSystem,
            const std::string& mapStyle = BingMapsStyle::AERIAL,
            const std::string& culture = "",
            const CesiumGeospatial::Ellipsoid& ellipsoid = CesiumGeospatial::Ellipsoid::WGS84
        );
        virtual ~BingMapsRasterOverlay() override;

        virtual const std::optional<Credit> getCredit() const noexcept override { return _credit; };

        virtual CesiumAsync::Future<std::unique_ptr<RasterOverlayTileProvider>> createTileProvider(
            const CesiumAsync::AsyncSystem& asyncSystem,
            std::shared_ptr<IPrepareRendererResources> pPrepareRendererResources,
            std::shared_ptr<spdlog::logger> pLogger,
            RasterOverlay* pOwner
        ) override;

    private:
        Credit _credit;
        std::string _url;
        std::string _key;
        std::string _mapStyle;
        std::string _culture;
        CesiumGeospatial::Ellipsoid _ellipsoid;
    };

}
