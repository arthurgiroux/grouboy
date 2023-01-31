#include "QGBEmulator.hpp"

int QGBEmulator::getFrameId()
{
    return _frameId;
}

void QGBEmulator::renderNextFrame()
{
    while (_frameId == _emulator.getPPU().getFrameId())
    {
        _emulator.exec();
    }

    _frameId = _emulator.getPPU().getFrameId();
    emit frameIdChanged();
}

bool QGBEmulator::loadCartridge(const QString& filepath)
{
    return _emulator.getMMU().loadCartridge(filepath.toStdString());
}