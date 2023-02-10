#ifndef GBEMULATOR_DEBUGTILEWINDOW_HPP
#define GBEMULATOR_DEBUGTILEWINDOW_HPP

#include <QGBEmulator.hpp>
#include <QObject>
#include <QQuickWindow.h>
#include <qqml.h>

class DebugTileWindow : public QQuickWindow
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int tileSetId READ getTileSetId WRITE setTileSetId NOTIFY tileSetIdChanged)
    Q_PROPERTY(QGBEmulator* emulator READ getEmulator WRITE setEmulator)
    Q_PROPERTY(QImage tileSetImage READ getTileSetImage NOTIFY tileSetImageChanged)

  public:
    DebugTileWindow(QWindow* parent = nullptr);

    QGBEmulator* getEmulator() const;

    void setEmulator(QGBEmulator* emulator);

    QImage& getTileSetImage();

    int getTileSetId() const;

    void setTileSetId(int value);

  public slots:
    void onFrameIdChanged();

  signals:
    void tileSetImageChanged();
    void tileSetIdChanged();

  private:
    static const int NBR_TILE_IN_TILESET = 256;
    static const int NBR_TILE_DISPLAY_PER_LINE = 16;
    QGBEmulator* _emulator = nullptr;
    RGBImage _tileSetImageBuffer = RGBImage(128, 128);
    QImage _image;
    int _tileSetId = 0;
};

#endif // GBEMULATOR_DEBUGTILEWINDOW_HPP
