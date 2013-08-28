#include <QtCore/QFinalState>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QStateMachine>
#include <QtCore/QSequentialAnimationGroup>
#include "./diver.h"
#include "./diver_p.h"
#include "./pixmapitem.h"
#include "./graphicsscene.h"
#include "./qanimationstate.h"
#include "./animationmanager.h"

static QAbstractAnimation *setupDestroyAnimation( Diver *diver )
{

    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(diver);
    PixmapItem *explosion = new PixmapItem( QString("explosion"), diver );
    explosion->setZValue(6);
    explosion->setOpacity(0);
    QPropertyAnimation *anim = new QPropertyAnimation( explosion, "opacity", group );
    anim->setDuration(500);
    anim->setEndValue(1);
    AnimationManager::self()->registerAnimation(group);

    return group;
}

Diver::Diver() : PixmapItem(QString("diver")),
    diverType(qrand()%88),
    direction(Diver::None),
    speed(qrand()%3+1)
{

    setZValue(5);
    setTransformOriginPoint(boundingRect().center());

    graphicsRotation = new QGraphicsRotation(this);
    graphicsRotation->setAxis(Qt::YAxis);
    graphicsRotation->setOrigin(QVector3D(size().width()/2, size().height()/2, 0));
    QList<QGraphicsTransform *> r;
    r.append(graphicsRotation);
    setTransformations(r);
    //设置潜艇状态机
    QStateMachine *machine = new QStateMachine(this);
    //潜艇移动状态
    QState *moving = new QState(machine);
    //潜艇自左向右移动状态
    MovementState *movement = new MovementState(this, moving);
    //潜艇自左向右转弯状态
    ReturnState *rotation = new ReturnState(this, moving);

    moving->setInitialState(movement);
    //当潜艇方向改变时，切换打反方向状态
    movement->addTransition(this, SIGNAL(diverStateChanged()), moving);
    //潜艇状态机初始化为移动
    machine->setInitialState(moving);
    //结束状态
    QFinalState *final = new QFinalState(machine);
    //如果移动动画结束，变成转向状态
    movement->addTransition(movement, SIGNAL(animationFinished()), rotation);
    //如果转向动画结束，变成移动状态
    rotation->addTransition(rotation, SIGNAL(animationFinished()), movement);
    //播放潜艇摧毁动画状态
    QAnimationState *destroyedState = new QAnimationState(machine);
    //为“播放潜艇摧毁动画状态”安装动画
    destroyedState->setAnimation(setupDestroyAnimation(this));
    //在潜艇移动过程中，被摧毁时，切换到潜艇摧毁状态
    moving->addTransition(this, SIGNAL(diverDestroyed()), destroyedState);
    //潜艇炸毁动画结束后，切换到结束状态
    destroyedState->addTransition(destroyedState, SIGNAL(animationFinished()), final);
    //状态机结束时，潜艇销毁
    connect(machine,SIGNAL(finished()),this, SIGNAL(diverExecutionFinished()));
    //状态机开启
    machine->start();

}

void Diver::setCurrentDirection( Diver::Movement direction )
{

    if ( this->direction == direction )
        return;
    if ( direction == Diver::Right && this->direction == Diver::None ) {
          graphicsRotation->setAngle(180);
    }
    this->direction = direction;

}

enum Diver::Movement Diver::currentDirection() const
{

    return direction;

}

void Diver::setCurrentSpeed(int speed)
{

    this->speed = speed;
    emit diverStateChanged();

}

int Diver::currentSpeed() const
{

    return speed;

}

void Diver::destroy()
{

    emit diverDestroyed();

}

int Diver::type() const
{

    return Type;

}

