#include "Cesium3DTiles/RasterMappedTo3DTile.h"
#include "Cesium3DTiles/RasterOverlayTileProvider.h"
#include "Cesium3DTiles/TilesetExternals.h"

namespace Cesium3DTiles {

    RasterMappedTo3DTile::RasterMappedTo3DTile(
        const std::shared_ptr<RasterOverlayTile>& pRasterTile,
        const CesiumGeometry::Rectangle& textureCoordinateRectangle
    ) :
        _pRasterTile(pRasterTile),
        _textureCoordinateRectangle(textureCoordinateRectangle),
        _state(AttachmentState::Unattached)
    {
    }

    void RasterMappedTo3DTile::attachToTile(Tile& tile) {
        if (this->getState() != AttachmentState::Unattached) {
            return;
        }

        RasterOverlayTile& rasterTile = this->getRasterTile();
        if (rasterTile.getState() != RasterOverlayTile::LoadState::Done) {
            return;
        }

        TilesetExternals& externals = this->getRasterTile().getTileProvider().getExternals();
        externals.pPrepareRendererResources->attachRasterInMainThread(tile, this->getRasterTile(), this->getTextureCoordinateRectangle());
        this->_state = AttachmentState::Attached;
    }

}
