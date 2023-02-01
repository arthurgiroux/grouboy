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

  signals:
    void frameIdChanged();
    void renderedImageChanged();

  public slots:
    void renderNextFrame();
    bool loadCartridge(const QString& filepath);

  private:
    Emulator _emulator;
    int _frameId = 0;
    QImage _renderedImage = QImage(PPU::SCREEN_WIDTH, PPU::SCREEN_HEIGHT, QImage::Format_RGB888);
};

#endif // GBEMULATOR_QGBEMULATOR_HPP
