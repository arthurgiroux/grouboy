#ifndef GBEMULATOR_QTILEMAP_HPP
#define GBEMULATOR_QTILEMAP_HPP

#include <QGBEmulator.hpp>
#include <QObject>
#include <qqml.h>

class QTileMap : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QGBEmulator* emulator READ getEmulator WRITE setEmulator)
    Q_PROPERTY(QImage image READ getImage NOTIFY imageChanged)

  public:
    QImage getImage()
    {
        return _image;
    }

    QGBEmulator* getEmulator() const
    {
        return _emulator;
    }

    void setEmulator(QGBEmulator* emulator)
    {
        _emulator = emulator;
        connect(_emulator, &QGBEmulator::frameIdChanged, this, &QTileMap::onFrameIdChanged);
    }

  public slots:
    void onFrameIdChanged()
    {
        if (_emulator != nullptr)
        {
            auto tile = _emulator->getEmulator().getPPU().getTileById(2, 0);
            const RGBImage& image = tile.getImage();
            _image = QImage(static_cast<const uchar*>(image.getData().data()), image.getWidth(), image.getHeight(),
                            QImage::Format::Format_RGB888);
            emit imageChanged();
        }
    };

  signals:
    void imageChanged();

  private:
    QGBEmulator* _emulator = nullptr;
    QImage _image;
};

#endif // GBEMULATOR_QTILEMAP_HPP
