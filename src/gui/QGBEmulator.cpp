#include "QGBEmulator.hpp"
#include <QDir>
#include <QMediaDevices>
#include <QStandardPaths>
#include <algorithm>

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

QGBEmulator::QGBEmulator(QObject* parent) : QObject(parent), audioBuffer(this, this)
{
    // connect(this, SIGNAL(frameIdChanged()), this, SLOT(onNewFrame()));

    QAudioFormat format;
    // Set up the format, eg.
    format.setSampleRate(48000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::UInt8);

    QAudioDevice info(QMediaDevices::defaultAudioOutput());
    if (!info.isFormatSupported(format))
    {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }

    audio = new QAudioSink(format, this);
    connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));

    audioBuffer.open(QIODevice::ReadWrite);
    audio->start(&audioBuffer);
}

void QGBEmulator::handleStateChanged(QAudio::State newState)
{
    switch (newState)
    {
    case QAudio::IdleState:
        qDebug() << "idle state";
        // Finished playing (no more data)
        break;

    case QAudio::StoppedState:
        qDebug() << "stop state";
        break;

    default:
        // ... other cases as appropriate
        break;
    }
}

void QGBEmulator::setFrameId(int frameId)
{
    if (_frameId == frameId)
    {
        return;
    }

    _frameId = frameId;
    emit frameIdChanged();
}

std::vector<uint8_t> QGBEmulator::gatherAudioSamples(int nbrSamples)
{
    while (_emulator.getAPU().getAudioBuffer().size() < nbrSamples)
    {
        _emulator.exec();

        int frameId = _emulator.getPPU().getFrameId();
        if (_frameId != frameId)
        {
            setFrameId(frameId);

            const RGBImage& image = _emulator.getPPU().getLastRenderedFrame();

            _renderedImage = QImage(static_cast<const uchar*>(image.getData().data()), image.getWidth(),
                                    image.getHeight(), QImage::Format::Format_RGB888);
            emit renderedImageChanged();
        }
    }

    setFrameId(_emulator.getPPU().getFrameId());

    auto buffer = _emulator.getAPU().getAudioBuffer();
    _emulator.getAPU().resetAudioBuffer();
    return buffer;
}

void QGBEmulator::onNewFrame()
{
    //    qDebug() << "new frame " << _frameId;
    //    const RGBImage& image = _emulator.getPPU().getLastRenderedFrame();
    //
    //    _renderedImage = QImage(static_cast<const uchar*>(image.getData().data()), image.getWidth(),
    //    image.getHeight(),
    //                            QImage::Format::Format_RGB888);
    //    emit renderedImageChanged();
}

bool QGBEmulator::isCartridgeLoaded()
{
    return _emulator.getMMU().getCartridge() != nullptr;
}

bool AudioSyncedEmulator::seek(qint64 pos)
{
    return true;
}

qint64 AudioSyncedEmulator::readData(char* data, qint64 maxlen)
{
    if (!_emulator->isCartridgeLoaded())
    {
        return 0;
    }

    if (maxlen > 48)
    {
        maxlen = 48;
    }

    auto buffer = _emulator->gatherAudioSamples(maxlen);

    for (int i = 0; i < maxlen; ++i)
    {
        data[i] = buffer[i];
    }

    return maxlen;
}

AudioSyncedEmulator::AudioSyncedEmulator(QGBEmulator* emulator, QObject* parent)
    : _emulator(emulator), QIODevice(parent)
{
}

qint64 AudioSyncedEmulator::writeData(const char* data, qint64 len)
{
    return 0;
}
