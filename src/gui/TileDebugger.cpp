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
    return _tileSetImage;
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
            if (tileId == _selectedTileId)
            {
                _selectedTileImageBuffer.copyImage(0, 0, tileImage);
            }
        }

        emit tileSetImageChanged();
        emit selectedTileImageChanged();
    }
}

TileDebugger::TileDebugger(QQuickItem* parent) : QQuickItem(parent)
{
    _tileSetImage = QImage(_tileSetImageBuffer.getData().data(), _tileSetImageBuffer.getWidth(),
                           _tileSetImageBuffer.getHeight(), QImage::Format::Format_RGB888);
    _selectedTileImage = QImage(_selectedTileImageBuffer.getData().data(), _selectedTileImageBuffer.getWidth(),
                                _selectedTileImageBuffer.getHeight(), QImage::Format::Format_RGB888);
}

QImage& TileDebugger::getSelectedTileImage()
{
    return _selectedTileImage;
}

int TileDebugger::getSelectedTileId() const
{
    return _selectedTileId;
}

void TileDebugger::setSelectedTileId(int value)
{
    if (_selectedTileId == value)
    {
        return;
    }

    _selectedTileId = value;
    emit selectedTileIdChanged();
}
