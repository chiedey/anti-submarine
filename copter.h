#ifndef COPTER_H
#define COPTWR_H

#include "./pixmapitem.h"

class QVariantAnimation;
class QAbstractAnimation;
class QStateMachine;
class Bomb;

class Copter : public PixmapItem
{
    Q_OBJECT

public:
    enum Movement { None = 0, Left, Right };
    enum { Type = UserType + 2 };
    Copter();
    void run();
    void stop();

    int bombsLaunched() const;
    void setBombsLaunched( int number );

    int currentSpeed() const;
    void setCurrentSpeed( int speed );

    enum Movement currentDirection() const;
    void setCurrentDirection( Movement direction );

    void updateCopterMovement();

    virtual int type() const;

signals:
    void copterExecutionFinished();

private:
    int speed;
    int bombsAlreadyLaunched;
    Movement direction;
    QVariantAnimation *movementAnimation;
    QStateMachine *machine;

};

#endif
