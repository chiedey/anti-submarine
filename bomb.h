#ifndef BOMB_H
#define BOMB_H

#include "pixmapitem.h"

class Bomb : public PixmapItem
{
    Q_OBJECT

public:
    Bomb( int speed, int direction );
    void launch();
    void destroy();

signals:
    void bombExploded();
    void bombExecutionFinished();

private:
    double speed;
    int direction;

private slots:
    void onAnimationLaunchValueChanged( const QVariant & );

};

#endif

