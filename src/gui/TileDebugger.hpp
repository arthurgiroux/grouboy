#ifndef GBEMULATOR_TILEDEBUGGER_HPP
#define GBEMULATOR_TILEDEBUGGER_HPP

#include <QGBEmulator.hpp>
#include <QGuiApplication.h>
#include <QQuickItem>
#include <qqml.h>

class TileDebugger : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int tileSetId READ getTileSetId WRITE setTileSetId NOTIFY tileSetIdChanged)
    Q_PROPERTY(int selectedTileId READ getSelectedTileId WRITE setSelectedTileId NOTIFY selectedTileIdChanged)
    Q_PROPERTY(QGBEmulator* emulator READ getEmulator WRITE setEmulator)
    Q_PROPERTY(QImage tileSetImage READ getTileSetImage NOTIFY tileSetImageChanged)
    Q_PROPERTY(QImage selectedTileImage READ getSelectedTileImage NOTIFY selectedTileImageChanged)

  public:
    TileDebugger(QQuickItem* parent = nullptr);
    QGBEmulator* getEmulator() const;
    void setEmulator(QGBEmulator* emulator);
    QImage& getTileSetImage();
    int getTileSetId() const;
    void setTileSetId(int value);
    QImage& getSelectedTileImage();
    int getSelectedTileId() const;
    void setSelectedTileId(int value);

  public slots:
    void onFrameIdChanged();

  signals:
    void tileSetImageChanged();
    void tileSetIdChanged();
    void selectedTileIdChanged();
    void selectedTileImageChanged();

  private:
    static const int NBR_TILE_IN_TILESET = 256;
    static const int NBR_TILE_DISPLAY_PER_LINE = 16;
    QGBEmulator* _emulator = nullptr;
    RGBImage _tileSetImageBuffer = RGBImage(128, 128);
    RGBImage _selectedTileImageBuffer = RGBImage(SingleTile::TILE_HEIGHT, SingleTile::TILE_WIDTH);
    QImage _tileSetImage;
    QImage _selectedTileImage;
    int _tileSetId = 0;
    int _selectedTileId = 3;
};

#endif // GBEMULATOR_TILEDEBUGGER_HPP
