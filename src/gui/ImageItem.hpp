#ifndef GBEMULATOR_IMAGEITEM_HPP
#define GBEMULATOR_IMAGEITEM_HPP

#include <QImage>
#include <QPainter>
#include <QQuickItem>
#include <QQuickPaintedItem>

class ImageItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
  public:
    ImageItem(QQuickItem* parent = nullptr);
    Q_INVOKABLE void setImage(const QImage& image);
    void paint(QPainter* painter);
    QImage image() const;
  signals:
    void imageChanged();

  private:
    QImage _image;
};

#endif // GBEMULATOR_IMAGEITEM_HPP
