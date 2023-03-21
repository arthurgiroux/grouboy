#ifndef GBEMULATOR_QGBEMULATOR_HPP
#define GBEMULATOR_QGBEMULATOR_HPP

#include "emulator.hpp"
#include <QAudioSink>
#include <QBuffer>
#include <QDir>
#include <QImage>
#include <QObject>
#include <QString>
#include <qqml.h>
#include <vector>

class QGBEmulator;

class AudioSyncedEmulator : public QIODevice
{
  public:
    explicit AudioSyncedEmulator(QGBEmulator* emulator, QObject* parent = 0);
    bool seek(qint64 pos) override;

  protected:
    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char* data, qint64 len) override;

  private:
    int _bufferPos = 0;
    int _minBufferSize = 10 * 48000;
    QGBEmulator* _emulator;
};

class QGBEmulator : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(int frameId READ getFrameId NOTIFY frameIdChanged)
    Q_PROPERTY(QImage renderedImage READ getRenderedImage NOTIFY renderedImageChanged)

  public:
    explicit QGBEmulator(QObject* parent = nullptr);
    int getFrameId();
    QImage getRenderedImage();
    Emulator& getEmulator();
    std::vector<uint8_t> gatherAudioSamples(int nbrSamples);
    bool isCartridgeLoaded();

  signals:
    void frameIdChanged();
    void renderedImageChanged();

  public slots:
    void renderNextFrame();
    bool loadCartridgeFromLocalFile(const QString& filepath);
    bool loadCartridgeFromUrl(const QString& url);
    void onKeyPressed(Qt::Key key);
    void onKeyReleased(Qt::Key key);
    void saveToFile();
    void loadSaveFromFile();
    void handleStateChanged(QAudio::State newState);
    void onNewFrame();

  private:
    void setFrameId(int frameId);
    QDir getSaveFolder(const Cartridge* cartridge) const;
    static const QString SAVE_FILENAME;

    Emulator _emulator;
    int _frameId = 0;
    QImage _renderedImage = QImage(PPU::SCREEN_WIDTH, PPU::SCREEN_HEIGHT, QImage::Format_RGB888);
    std::map<Qt::Key, InputController::Button> _buttonMapping = {
        {Qt::Key_Up, InputController::Button::UP},       {Qt::Key_Down, InputController::Button::DOWN},
        {Qt::Key_Left, InputController::Button::LEFT},   {Qt::Key_Right, InputController::Button::RIGHT},
        {Qt::Key_Enter, InputController::Button::START}, {Qt::Key_Space, InputController::Button::SELECT},
        {Qt::Key_A, InputController::Button::A},         {Qt::Key_B, InputController::Button::B},
        {Qt::Key_Return, InputController::Button::START}};

    QAudioSink* audio;
    AudioSyncedEmulator audioBuffer;
};

#endif // GBEMULATOR_QGBEMULATOR_HPP
