#include "ImageItem.hpp"

ImageItem::ImageItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    _image = QImage();
}

void ImageItem::paint(QPainter *painter)
{
    if (_image.isNull()) {
        return;
    }

    QRectF bounding_rect = boundingRect();
    QImage scaled = _image.scaledToHeight(bounding_rect.height());
    QPointF center = bounding_rect.center() - scaled.rect().center();

    if(center.x() < 0)
        center.setX(0);
    if(center.y() < 0)
        center.setY(0);
    painter->drawImage(center, scaled);
}

QImage ImageItem::image() const
{    return _image;
}

void ImageItem::setImage(const QImage &image)
{
    _image = image;
    update();
    emit imageChanged();
}