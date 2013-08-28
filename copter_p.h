#ifndef COPTER_P_H
#define COPTER_P_H

#include <QtWidgets/QKeyEventTransition>
#include "./bomb.h"
#include "./graphicsscene.h"
//为了维持游戏平衡，限制最大弹量为5枚
static const int MAX_BOMB = 1000;

class KeyStopTransition : public QKeyEventTransition
{

public:
    KeyStopTransition( Copter *c, QEvent::Type t, int k )
        : QKeyEventTransition( c, t, k ), copter(c){}

protected:
    virtual bool eventTest(QEvent *event)
    {
        if (!QKeyEventTransition::eventTest(event)) {
            return false;
        }
        return ( copter->currentSpeed() == 1 );
    }

private:
    Copter *copter;

};

class KeyMoveTransition : public QKeyEventTransition
{

public:
    KeyMoveTransition( Copter *c, QEvent::Type t, int k )
        : QKeyEventTransition( c, t, k ), copter(c), key(k){}

protected:
    virtual bool eventTest(QEvent *event)
    {
        if (!QKeyEventTransition::eventTest(event)) {
            return false;
        }
        return ( copter->currentSpeed() >= 0 );
    }

    void onTransition(QEvent *)
    {
        if ( key==Qt::Key_Left && copter->currentDirection()==Copter::Right ) {
            copter->setCurrentSpeed( copter->currentSpeed() - 1 );
        }
        else if ( key==Qt::Key_Right && copter->currentDirection()==Copter::Left ) {
            copter->setCurrentSpeed( copter->currentSpeed() - 1 );
        }
        else if ( copter->currentSpeed() < 3 ) {
            copter->setCurrentSpeed( copter->currentSpeed() + 1 );
        }
        copter->updateCopterMovement();
    }

private:
    Copter * copter;
    int key;

};

class KeyLaunchTransition : public QKeyEventTransition
{

public:
    KeyLaunchTransition( Copter *copter, QEvent::Type type, int key )
        : QKeyEventTransition( copter, type, key ), copter(copter), key(key){}

protected:
    virtual bool eventTest( QEvent *event )
    {
        if (!QKeyEventTransition::eventTest(event)) {
            return false;
        }
        return ( copter->bombsLaunched() < MAX_BOMB );
    }

private:
    Copter *copter;
    int key;

};

class MoveStateRight : public QState
{

public:
    explicit MoveStateRight( Copter *copter, QState *parent = 0 )
        : QState(parent), copter(copter){}

protected:
    void onEntry(QEvent *)
    {
        copter->setCurrentDirection(Copter::Right);
        copter->updateCopterMovement();
    }

private:
    Copter *copter;

};

class MoveStateLeft : public QState
{

public:
    explicit MoveStateLeft( Copter *copter, QState *parent = 0 )
        : QState(parent), copter(copter){}

protected:
    void onEntry(QEvent *)
    {
        copter->setCurrentDirection(Copter::Left);
        copter->updateCopterMovement();
    }

private:
    Copter * copter;

};

class StopState : public QState
{

public:
    explicit StopState( Copter *copter, QState *parent = 0)
        : QState(parent),   copter(copter){}

protected:
    void onEntry(QEvent *)
    {
        copter->setCurrentSpeed(0);
        copter->setCurrentDirection(Copter::None);
        copter->updateCopterMovement();
    }

private:
    Copter *copter;

};

class LaunchState : public QState
{

public:
    explicit LaunchState( Copter *copter, QState *parent = 0 )
        : QState(parent), copter(copter) {}

protected:
    void onEntry(QEvent *)
    {
        Bomb *b = new Bomb( copter->currentSpeed(), copter->currentDirection() );
        b->setPos( copter->x()+(b->size().width())*2, copter->y()+b->size().height()  );
        GraphicsScene *scene = static_cast<GraphicsScene *>(copter->scene());
        scene->addItem(b);
        b->launch();
        copter->setBombsLaunched(copter->bombsLaunched() + 1);
    }

private:
    Copter *copter;

};

#endif
