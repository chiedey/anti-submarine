#ifndef DIVER_H
#define DIVER_H

#include <QtWidgets/QGraphicsTransform>
#include "./pixmapitem.h"

class Diver : public PixmapItem
{
    Q_OBJECT

public:
    enum Movement { None = 0, Left, Right };
    enum { Type = UserType + 1 };
    Diver();

    void setCurrentDirection( Movement direction );
    enum Movement currentDirection() const;

    void setCurrentSpeed( int speed );
    int currentSpeed() const;

    void destroy();

    virtual int type() const;

    QGraphicsRotation *rotation() const { return graphicsRotation; }

signals:
    void diverDestroyed();
    void diverExecutionFinished();
    void diverStateChanged();

private:
    qreal speed;
    int diverType;
    Movement direction;
    QGraphicsRotation *graphicsRotation;

};

#endif
