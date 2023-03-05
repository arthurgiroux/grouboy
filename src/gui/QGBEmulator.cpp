#include "QGBEmulator.hpp"
#include <QDir>
#include <QStandardPaths>

const QString QGBEmulator::SAVE_FILENAME = "savestate.bin";

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

bool QGBEmulator::loadCartridgeFromLocalFile(const QString& filepath)
{
    return _emulator.getMMU().loadCartridge(filepath.toStdString());
}

bool QGBEmulator::loadCartridgeFromUrl(const QString& url)
{
    return loadCartridgeFromLocalFile(QUrl(url).toLocalFile());
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

Emulator& QGBEmulator::getEmulator()
{
    return _emulator;
}

void QGBEmulator::saveToFile()
{
    Cartridge* cartridge = _emulator.getMMU().getCartridge();
    if (cartridge != nullptr)
    {
        QDir saveDir = getSaveFolder(cartridge);
        if (saveDir.mkpath(saveDir.absolutePath()))
        {
            _emulator.saveToFile(saveDir.absoluteFilePath(SAVE_FILENAME).toStdString());
        }
    }
}
QDir QGBEmulator::getSaveFolder(const Cartridge* cartridge) const
{
    QString saveFolder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    saveFolder += QDir::separator() + QString::fromStdString(cartridge->getTitle());
    return QDir(saveFolder);
}

void QGBEmulator::loadSaveFromFile()
{
    Cartridge* cartridge = _emulator.getMMU().getCartridge();
    if (cartridge != nullptr)
    {
        QDir saveDir = getSaveFolder(cartridge);
        if (saveDir.exists(SAVE_FILENAME))
        {
            _emulator.loadFromFile(saveDir.absoluteFilePath(SAVE_FILENAME).toStdString());
        }
    }
}