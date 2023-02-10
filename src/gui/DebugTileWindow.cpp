#include "DebugTileWindow.hpp"

QGBEmulator* DebugTileWindow::getEmulator() const
{
    return _emulator;
}

void DebugTileWindow::setEmulator(QGBEmulator* emulator)
{
    _emulator = emulator;
    connect(_emulator, &QGBEmulator::frameIdChanged, this, &DebugTileWindow::onFrameIdChanged);
}

QImage& DebugTileWindow::getTileSetImage()
{
    return _image;
}

int DebugTileWindow::getTileSetId() const
{
    return _tileSetId;
}

void DebugTileWindow::setTileSetId(int value)
{
    if (_tileSetId == value)
    {
        return;
    }

    _tileSetId = value;
    emit tileSetIdChanged();
}

void DebugTileWindow::onFrameIdChanged()
{
    if (_emulator != nullptr)
    {
        for (int tileId = 0; tileId < NBR_TILE_IN_TILESET; tileId++)
        {
            int lineOffset = (tileId / NBR_TILE_DISPLAY_PER_LINE) * SingleTile::TILE_HEIGHT;
            Tile tile = _emulator->getEmulator().getPPU().getTileById(tileId, _tileSetId);
            auto& tileImage = tile.getImage();
            int xOffset = (tileId % NBR_TILE_DISPLAY_PER_LINE) * SingleTile::TILE_WIDTH;
            _tileSetImageBuffer.copyRegion(xOffset, lineOffset, tileImage, 0, 0, tileImage.getHeight(),
                                           tileImage.getWidth());
        }

        emit tileSetImageChanged();
    }
}

DebugTileWindow::DebugTileWindow(QWindow* parent) : QQuickWindow(parent)
{
    _image = QImage(_tileSetImageBuffer.getData().data(), _tileSetImageBuffer.getWidth(),
                    _tileSetImageBuffer.getHeight(), QImage::Format::Format_RGB888);
}
