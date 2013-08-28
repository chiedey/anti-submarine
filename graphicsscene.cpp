#include <QtCore/QDir>
#include <QtWidgets/QAction>
#include <QtCore/QFinalState>
#include <QtCore/QStateMachine>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QParallelAnimationGroup>
#include <QtCore/QSequentialAnimationGroup>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include "./bomb.h"
#include "./diver.h"
#include "./states.h"
#include "./copter.h"
#include "./pixmapitem.h"
#include "./graphicsscene.h"
#include "./qanimationstate.h"
#include "./animationmanager.h"

GraphicsScene::GraphicsScene( int x, int y, int width, int height )
    : QGraphicsScene( x , y, width, height ), copter( new Copter )
{
    //设置背景图片
    PixmapItem *backgroundItem = new PixmapItem(QString("background"));
    //设置背景图片绘图顺序
    backgroundItem->setZValue(1);
    //加入背景图片
    addItem(backgroundItem);
    //添加直升机
    addItem(copter);

}

void GraphicsScene::setupScene( QAction *newAction, QAction *quitAction )
{
    //游戏场景状态机
    QStateMachine *machine = new QStateMachine(this);
    //游戏状态
    PlayState *gameState = new PlayState( this, machine );
    //游戏结束状态
    QFinalState *final = new QFinalState(machine);
    //当newAction发送消息时，切换到游戏状态
    gameState->addTransition( newAction, SIGNAL(triggered()), gameState );
    //当quitAction发送消息时，切换到游戏结束状态
    gameState->addTransition( quitAction, SIGNAL(triggered()), final );
    //游戏场景状态机初始化为游戏状态
    machine->setInitialState(gameState);
    //启动游戏场景状态机
    machine->start();
    //到达结束状态，退出程序
    connect( machine, SIGNAL(finished()), qApp, SLOT(quit()) );

}

void GraphicsScene::addItem( Bomb *bomb )
{

    bombs.insert(bomb);
    connect( bomb, SIGNAL(bombExecutionFinished()),
             this, SLOT(onBombExecutionFinished()) );
    QGraphicsScene::addItem(bomb);

}

void GraphicsScene::addItem( Diver *diver )
{

    divers.insert(diver);
    connect( diver, SIGNAL(diverExecutionFinished()),
             this,  SLOT(onDiverExecutionFinished()) );
    QGraphicsScene::addItem(diver);

}

void GraphicsScene::addItem( QGraphicsItem *item )
{

    QGraphicsScene::addItem(item);

}

void GraphicsScene::onBombExecutionFinished()
{
    Bomb *bomb = qobject_cast<Bomb *>(sender());
    bombs.remove(bomb);
    bomb->deleteLater();
    if (copter) {
        copter->setBombsLaunched( copter->bombsLaunched() - 1 );
    }

}

void GraphicsScene::onDiverExecutionFinished()
{
    Diver *diver = qobject_cast<Diver *>(sender());
    divers.remove(diver);
    if (divers.count() == 0)
        emit allDiverDestroyed();
    else {
        emit diverDestroyed();
        Diver *diver = new Diver();
        addItem(diver);
        int random = (qrand() % 15 + 1);
        qreal x = random==13 || random==5 ? 0 : width()-diver->size().width();
        qreal y = height() - (qrand() % 150 + 1) - diver->size().height();
        diver->setPos( x, y );
        diver->setCurrentDirection( x == 0 ? Diver::Right : Diver::Left );
    }
    diver->deleteLater();

}

void GraphicsScene::clearScene()
{

    foreach ( Diver *diver, divers ) {
        diver->destroy();
        diver->deleteLater();
    }

    foreach ( Bomb *bomb, bombs ) {
        bomb->destroy();
        bomb->deleteLater();
    }

    divers.clear();
    bombs.clear();

    AnimationManager::self()->unregisterAllAnimations();

    copter->stop();
    copter->hide();
    copter->setEnabled(true);

}
