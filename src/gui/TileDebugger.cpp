#include "TileDebugger.hpp"

QGBEmulator* TileDebugger::getEmulator() const
{
    return _emulator;
}

void TileDebugger::setEmulator(QGBEmulator* emulator)
{
    _emulator = emulator;
    connect(_emulator, &QGBEmulator::frameIdChanged, this, &TileDebugger::onFrameIdChanged);
}

QImage& TileDebugger::getTileSetImage()
{
    return _image;
}

int TileDebugger::getTileSetId() const
{
    return _tileSetId;
}

void TileDebugger::setTileSetId(int value)
{
    if (_tileSetId == value)
    {
        return;
    }

    _tileSetId = value;
    emit tileSetIdChanged();
}

void TileDebugger::onFrameIdChanged()
{
    if (_emulator != nullptr)
    {
        for (int tileId = 0; tileId < NBR_TILE_IN_TILESET; tileId++)
        {
            int lineOffset = (tileId / NBR_TILE_DISPLAY_PER_LINE) * SingleTile::TILE_HEIGHT;
            Tile tile = _emulator->getEmulator().getPPU().getTileById(tileId, _tileSetId);
            auto& tileImage = tile.getImage();
            int xOffset = (tileId % NBR_TILE_DISPLAY_PER_LINE) * SingleTile::TILE_WIDTH;
            _tileSetImageBuffer.copyImage(xOffset, lineOffset, tileImage);
        }

        emit tileSetImageChanged();
    }
}

TileDebugger::TileDebugger(QQuickItem* parent) : QQuickItem(parent)
{
    _image = QImage(_tileSetImageBuffer.getData().data(), _tileSetImageBuffer.getWidth(),
                    _tileSetImageBuffer.getHeight(), QImage::Format::Format_RGB888);
}
