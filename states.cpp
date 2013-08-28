#include <QtCore/QFinalState>
#include <QtCore/QStateMachine>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QKeyEventTransition>
#include "./diver.h"
#include "./copter.h"
#include "./states.h"
#include "./graphicsscene.h"
#include "./animationmanager.h"

PlayState::PlayState( GraphicsScene *scene, QState *parent )
    : QState(parent), scene(scene), machine(0)
{}

PlayState::~PlayState()
{

    delete machine;

}

void PlayState::onEntry( QEvent * )
{

    if (machine) {
        machine->stop();
        scene->clearScene();
        delete machine;
    }

    machine = new QStateMachine;

    CopterState *copterState = new CopterState( scene, this, machine );
    //初始化游戏状态机
    machine->setInitialState(copterState);
    machine->start();

}

void CopterState::onEntry( QEvent * )
{

    initCopter();

}

void CopterState::initCopter()
{

    scene->copter->setPos( scene->width()/2, 60 );
    scene->copter->setCurrentSpeed(0);
    scene->copter->setCurrentDirection(Copter::None);
    scene->copter->setBombsLaunched(0);
    scene->copter->show();
    scene->setFocusItem(scene->copter, Qt::OtherFocusReason);
    scene->copter->run();
    //随机生成3艘潜艇
    for ( int i = 0; i < NUM_OF_DIVER; i++ ) {
        Diver *diver = new Diver();
        scene->addItem(diver);
        int random = (qrand() % 15 + 1);
        qreal x = random == 13 || random == 5 ? 0 : scene->width() - diver->size().width();
        qreal y = scene->height() - (qrand() % 180 + 1) - diver->size().height();
        diver->setPos(x,y);
        diver->setCurrentDirection( x == 0 ? Diver::Right : Diver::Left );
    }

}
