#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class QGraphicsView;
class GraphicsScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
    GraphicsScene *scene;
    QGraphicsView *view;

};

#endif
