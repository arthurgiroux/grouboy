#include "QGBEmulator.hpp"

int QGBEmulator::getFrameId()
{
    return _frameId;
}

QImage QGBEmulator::getRenderedImage()
{
    return _renderedImage;
}

void QGBEmulator::renderNextFrame()
{
    while (_frameId == _emulator.getPPU().getFrameId())
    {
        _emulator.exec();
    }

    _frameId = _emulator.getPPU().getFrameId();
    emit frameIdChanged();

    const RGBImage& image = _emulator.getPPU().getLastRenderedFrame();

    _renderedImage = QImage(static_cast<const uchar*>(image.getData().data()), image.getWidth(), image.getHeight(),
                            QImage::Format::Format_RGB888);
    emit renderedImageChanged();
}

bool QGBEmulator::loadCartridge(const QString& filepath)
{
    return _emulator.getMMU().loadCartridge(QUrl(filepath).toLocalFile().toStdString());
}

void QGBEmulator::onKeyPressed(Qt::Key key)
{
    if (_buttonMapping.count(key))
    {
        _emulator.getInputController().setButtonPressed(_buttonMapping[key]);
    }
}

void QGBEmulator::onKeyReleased(Qt::Key key)
{
    if (_buttonMapping.count(key))
    {
        _emulator.getInputController().setButtonReleased(_buttonMapping[key]);
    }
}
