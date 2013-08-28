#include <QMenu>
#include <QLayout>
#include <QMenuBar>
#include <QGraphicsView>
#include <QtOpenGL/QtOpenGL>
#include "./mainwindow.h"
#include "./graphicsscene.h"

MainWindow::MainWindow() : QMainWindow(0)
{
    //创建file菜单栏
    QMenu *file = menuBar()->addMenu(tr("&File"));
    //创建NewGame菜单
    QAction *newAction = file->addAction(tr("New Game"));
    //创建Quit菜单
    QAction *quitAction = file->addAction(tr("Quit"));
    //窗口布局设置
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    //创建游戏场景
    scene = new GraphicsScene( 0, 0, 880, 630 );
    //创建游戏视图，监视scene场景
    view = new QGraphicsView( scene, this );
    //设置游戏场景
    scene->setupScene( newAction, quitAction );
    //设置中央窗口部件
    setCentralWidget(view);

}
