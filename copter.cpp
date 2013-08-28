#include <QtCore/QState>
#include <QtCore/QFinalState>
#include <QtCore/QStateMachine>
#include <QtCore/QHistoryState>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QSequentialAnimationGroup>
#include "./bomb.h"
#include "./copter.h"
#include "./copter_p.h"
#include "./pixmapitem.h"
#include "./graphicsscene.h"
#include "./qanimationstate.h"
#include "./animationmanager.h"

Copter::Copter() : PixmapItem(QString("copter")),
    speed(0), bombsAlreadyLaunched(0), direction(Copter::None), movementAnimation(0)
{
    //设置绘图顺序
    setZValue(4);
    setFlags(QGraphicsItem::ItemIsFocusable);
    //用于直升机移动的动画
    movementAnimation = new QPropertyAnimation( this, "pos" );
    //设置直升机的状态机
    machine = new QStateMachine(this);
    //直升机移动状态
    QState *moving = new QState(machine);
    //直升机停止状态
    StopState *stopState = new StopState( this, moving );
    //状态机初始状态
    machine->setInitialState(moving);
    //移动状态初始为禁止
    moving->setInitialState(stopState);
    //向右移动状态
    MoveStateRight *moveStateRight = new MoveStateRight( this, moving );
    //向左移动状态
    MoveStateLeft *moveStateLeft = new MoveStateLeft( this, moving );
    //投放炸弹状态
    LaunchState *launchState = new LaunchState( this, machine );

    KeyStopTransition *leftStopRight = new KeyStopTransition(
                this, QEvent::KeyPress, Qt::Key_Left );
    leftStopRight->setTargetState(stopState);
    KeyMoveTransition *leftMoveRight = new KeyMoveTransition(
                this, QEvent::KeyPress, Qt::Key_Left );
    leftMoveRight->setTargetState(moveStateRight);
    KeyMoveTransition *rightMoveRight = new KeyMoveTransition(
                this, QEvent::KeyPress, Qt::Key_Right );
    rightMoveRight->setTargetState(moveStateRight);
    KeyMoveTransition *rightMoveStop = new KeyMoveTransition(
                this, QEvent::KeyPress, Qt::Key_Right );
    rightMoveStop->setTargetState(moveStateRight);

    KeyStopTransition *rightStopLeft = new KeyStopTransition(
                this, QEvent::KeyPress, Qt::Key_Right );
    rightStopLeft->setTargetState(stopState);
    KeyMoveTransition *rightMoveLeft = new KeyMoveTransition(
                this, QEvent::KeyPress, Qt::Key_Right );
    rightMoveLeft->setTargetState(moveStateLeft);
    KeyMoveTransition *leftMoveLeft = new KeyMoveTransition(
                this, QEvent::KeyPress,Qt::Key_Left );
    leftMoveLeft->setTargetState(moveStateLeft);
    KeyMoveTransition *leftMoveStop = new KeyMoveTransition(
                this, QEvent::KeyPress,Qt::Key_Left );
    leftMoveStop->setTargetState(moveStateLeft);

    moveStateRight->addTransition(leftStopRight);
    moveStateRight->addTransition(leftMoveRight);
    moveStateRight->addTransition(rightMoveRight);
    stopState->addTransition(rightMoveStop);

    moveStateLeft->addTransition(rightStopLeft);
    moveStateLeft->addTransition(leftMoveLeft);
    moveStateLeft->addTransition(rightMoveLeft);
    stopState->addTransition(leftMoveStop);
    //动画结束，意味着到达屏幕边缘，触发转换到停止状态
    moveStateLeft->addTransition( movementAnimation, SIGNAL(finished()), stopState );
    moveStateRight->addTransition( movementAnimation, SIGNAL(finished()), stopState );
    //设置投放炸弹按键
    KeyLaunchTransition *downFireLeft = new KeyLaunchTransition( this, QEvent::KeyPress, Qt::Key_Space );
    downFireLeft->setTargetState(launchState);
    KeyLaunchTransition *downFireRight = new KeyLaunchTransition( this, QEvent::KeyPress, Qt::Key_Space );
    downFireRight->setTargetState(launchState);
    KeyLaunchTransition *downFireMove = new KeyLaunchTransition( this, QEvent::KeyPress, Qt::Key_Space );
    downFireMove->setTargetState(launchState);

    moveStateRight->addTransition(downFireRight);
    moveStateLeft->addTransition(downFireLeft);
    stopState->addTransition(downFireMove);

    QHistoryState *historyState = new QHistoryState(moving);
    launchState->addTransition(historyState);

}

void Copter::run()
{

    AnimationManager::self()->registerAnimation(movementAnimation);
    machine->start();

}

void Copter::stop()
{

    movementAnimation->stop();
    machine->stop();

}

void Copter::updateCopterMovement()
{

    if ( 0 == speed || direction == Copter::None ) {
        movementAnimation->stop();
        return;
    }

    movementAnimation->stop();

    if ( direction == Copter::Left ) {
        movementAnimation->setEndValue(QPointF(0,y()));
        movementAnimation->setDuration(x()/speed*15);
    }
    else {
        movementAnimation->setEndValue(
                    QPointF(scene()->width()-size().width(),y()) );
        movementAnimation->setDuration(
                    (scene()->width()-size().width()-x())/speed*15 );
    }
    movementAnimation->start();

}

int Copter::bombsLaunched() const
{

    return bombsAlreadyLaunched;

}

void Copter::setBombsLaunched( int number )
{

    bombsAlreadyLaunched = number;

}

int Copter::currentSpeed() const
{

    return speed;

}

void Copter::setCurrentSpeed( int speed )
{

    this->speed = speed;

}

enum Copter::Movement Copter::currentDirection() const
{

    return direction;

}

void Copter::setCurrentDirection( Movement direction )
{

    this->direction = direction;

}

int Copter::type() const
{

    return Type;

}
