#ifndef GBEMULATOR_QGBEMULATOR_HPP
#define GBEMULATOR_QGBEMULATOR_HPP

#include "emulator.hpp"
#include <QImage>
#include <QObject>
#include <QString>
#include <qqml.h>

class QGBEmulator : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(int frameId READ getFrameId NOTIFY frameIdChanged)
    Q_PROPERTY(QImage renderedImage READ getRenderedImage NOTIFY renderedImageChanged)

  public:
    int getFrameId();
    QImage getRenderedImage();
    Emulator& getEmulator();

  signals:
    void frameIdChanged();
    void renderedImageChanged();

  public slots:
    void renderNextFrame();
    bool loadCartridgeFromLocalFile(const QString& filepath);
    bool loadCartridgeFromUrl(const QString& url);
    void onKeyPressed(Qt::Key key);
    void onKeyReleased(Qt::Key key);

  private:
    Emulator _emulator;
    int _frameId = 0;
    QImage _renderedImage = QImage(PPU::SCREEN_WIDTH, PPU::SCREEN_HEIGHT, QImage::Format_RGB888);
    std::map<Qt::Key, InputController::Button> _buttonMapping = {
        {Qt::Key_Up, InputController::Button::UP},       {Qt::Key_Down, InputController::Button::DOWN},
        {Qt::Key_Left, InputController::Button::LEFT},   {Qt::Key_Right, InputController::Button::RIGHT},
        {Qt::Key_Enter, InputController::Button::START}, {Qt::Key_Space, InputController::Button::SELECT},
        {Qt::Key_A, InputController::Button::A},         {Qt::Key_B, InputController::Button::B},
        {Qt::Key_Return, InputController::Button::START}};
};

#endif // GBEMULATOR_QGBEMULATOR_HPP
