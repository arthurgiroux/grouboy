#ifndef GBEMULATOR_QGBEMULATOR_HPP
#define GBEMULATOR_QGBEMULATOR_HPP

#include "emulator.hpp"
#include <QObject>
#include <QString>
#include <qqml.h>

class QGBEmulator : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(int frameId READ getFrameId NOTIFY frameIdChanged)

  public:
    int getFrameId();

  signals:
    void frameIdChanged();

  public slots:
    void renderNextFrame();
    bool loadCartridge(const QString& filepath);

  private:
    Emulator _emulator;
    int _frameId = 0;
};

#endif // GBEMULATOR_QGBEMULATOR_HPP
