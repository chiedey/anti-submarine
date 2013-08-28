#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include <QtWidgets/QGraphicsObject>
#include "./graphicsscene.h"

class PixmapItem : public QGraphicsObject
{

public:
    PixmapItem( const QString &fileName, QGraphicsItem * parent = 0 );
    PixmapItem( const QString &fileName, QGraphicsScene *scene );
    QSizeF size() const;
    QRectF boundingRect() const;
    void paint( QPainter *, const QStyleOptionGraphicsItem *, QWidget * );

private:
    QPixmap pix;

};

#endif
