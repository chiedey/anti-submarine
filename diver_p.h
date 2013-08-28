#ifndef DIVER_P_H
#define DIVER_P_H

#include <QtCore/QPropertyAnimation>
#include <QtWidgets/QGraphicsScene>
#include "./animationmanager.h"
#include "./diver.h"
#include "./qanimationstate.h"

class MovementState : public QAnimationState
{
    Q_OBJECT

public:
    explicit MovementState( Diver *diver, QState *parent = 0 ) : QAnimationState(parent)
    {
        movementAnimation = new QPropertyAnimation( diver, "pos" );
        setAnimation(movementAnimation);
        AnimationManager::self()->registerAnimation(movementAnimation);
        this->diver = diver;
    }

protected:
    void onEntry(QEvent *e)
    {
        if ( diver->currentDirection() == Diver::Left ) {
            movementAnimation->setEndValue(QPointF(0,diver->y()));
            movementAnimation->setDuration(diver->x()/diver->currentSpeed()*12);
        }
        else {
            movementAnimation->setEndValue(
                        QPointF(
                            diver->scene()->width() -
                            diver->size().width(),
                            diver->y()) );
            movementAnimation->setDuration(
                        (diver->scene()->width()-diver->size().width()-diver->x()) /
                        diver->currentSpeed()*12 );
        }
        QAnimationState::onEntry(e);
    }

private:
    Diver *diver;
    QPropertyAnimation *movementAnimation;

};

class ReturnState : public QAnimationState
{

public:
    explicit ReturnState( Diver *diver, QState *parent = 0 ) : QAnimationState(parent)
    {
        returnAnimation = new QPropertyAnimation( diver->rotation(), "angle" );
        returnAnimation->setDuration(500);
        AnimationManager::self()->registerAnimation(returnAnimation);
        setAnimation(returnAnimation);
        this->diver = diver;
    }

protected:
    void onEntry( QEvent *e )
    {
        returnAnimation->stop();
        returnAnimation->setEndValue( diver->currentDirection() ==
                                      Diver::Right ? 360. : 180. );
        QAnimationState::onEntry(e);
    }

    void onExit(QEvent *e)
    {
        diver->currentDirection() == Diver::Right ?
                    diver->setCurrentDirection(Diver::Left) :
                    diver->setCurrentDirection(Diver::Right);
        QAnimationState::onExit(e);
    }

private:
    Diver *diver;
    QPropertyAnimation *returnAnimation;

};

#endif
