#include <QtCore/qmath.h>
#include <QtCore/QFinalState>
#include <QtCore/QStateMachine>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QSequentialAnimationGroup>
#include "bomb.h"
#include "diver.h"
#include "pixmapitem.h"
#include "qanimationstate.h"
#include "animationmanager.h"

Bomb::Bomb( int speed, int direction ) : PixmapItem( QString("bomb") ),
    speed(speed), direction(direction)
{

    setZValue(2);

}

void Bomb::launch()
{
/*
    QSequentialAnimationGroup *launchAnimation = new QSequentialAnimationGroup;
    AnimationManager::self()->registerAnimation(launchAnimation);
    QPropertyAnimation *anim = new QPropertyAnimation(this, "pos");
    anim->setEndValue(QPointF(x(),scene()->height()));
    anim->setDuration(2500);
    anim->setEasingCurve(QEasingCurve::InQuad);
    launchAnimation->addAnimation(anim);
    connect(anim,SIGNAL(valueChanged(QVariant)),this,SLOT(onAnimationLaunchValueChanged(QVariant)));
    connect(this, SIGNAL(bombExploded()), launchAnimation, SLOT(stop()));
    //炸弹状态机
    QStateMachine *machine = new QStateMachine(this);
    //炸弹投放状态
    QAnimationState *launched = new QAnimationState(machine);
    launched->setAnimation(launchAnimation);
    //炸弹结束状态
    QFinalState *final = new QFinalState(machine);
    //炸弹状态机初始化为投放状态
    machine->setInitialState(launched);
    //当炸弹爆炸后，切换为结束状态
    launched->addTransition(this, SIGNAL(bombExploded()),final);
    //当炸弹下落动画结束后，切换为结束状态
    launched->addTransition(launched, SIGNAL(animationFinished()), final);

    connect(machine,SIGNAL(finished()),this, SIGNAL(bombExecutionFinished()));
    //启动炸弹状态机
    machine->start();
*/

    QSequentialAnimationGroup *launchAnimation = new QSequentialAnimationGroup;
    AnimationManager::self()->registerAnimation(launchAnimation);
    QPropertyAnimation *anim = new QPropertyAnimation(this, "pos");
    anim->setDuration(2500);
//    speed = 5;
    qreal step = 1.0/2500.0;
    qreal _x = direction > 1 ? speed*180/2500.0 : -1*speed*180/2500.0;
    for ( int i = 1; step <= 1; step += 0.0005, i++ ) {
        qreal _y = pow(step,2)*1000 ;
        anim->setKeyValueAt(step,QPointF(x()+_x*i,y()+_y));
    }
    anim->setEndValue(QPointF(x()+speed*180,scene()->height()));
    launchAnimation->addAnimation(anim);
    connect(anim,SIGNAL(valueChanged(QVariant)),this,SLOT(onAnimationLaunchValueChanged(QVariant)));
    connect(this, SIGNAL(bombExploded()), launchAnimation, SLOT(stop()));
    //炸弹状态机
    QStateMachine *machine = new QStateMachine(this);
    //炸弹投放状态
    QAnimationState *launched = new QAnimationState(machine);
    launched->setAnimation(launchAnimation);
    //炸弹结束状态
    QFinalState *final = new QFinalState(machine);
    //炸弹状态机初始化为投放状态
    machine->setInitialState(launched);
    //当炸弹爆炸后，切换为结束状态
    launched->addTransition(this, SIGNAL(bombExploded()),final);
    //当炸弹下落动画结束后，切换为结束状态
    launched->addTransition(launched, SIGNAL(animationFinished()), final);

    connect(machine,SIGNAL(finished()),this, SIGNAL(bombExecutionFinished()));
    //启动炸弹状态机
    machine->start();
}

void Bomb::onAnimationLaunchValueChanged(const QVariant &)
{

    foreach ( QGraphicsItem * item ,
              collidingItems(Qt::IntersectsItemBoundingRect) ) {
        if ( item->type() == Diver::Type ) {
            Diver *s = static_cast<Diver *>(item);
            destroy();
            s->destroy();
        }
    }

}

void Bomb::destroy()
{
    emit bombExploded();
}
