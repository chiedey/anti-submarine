#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QtCore/QSet>
#include <QtCore/QState>
#include <QtWidgets/QGraphicsScene>

class QAction;
class Bomb;
class Diver;
class Copter;
class PixmapItem;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:

    GraphicsScene( int x, int y, int width, int height );
    void setupScene( QAction *newAction, QAction *quitAction );
    void addItem( Bomb *bomb );
    void addItem( Diver *diver );
    void addItem( QGraphicsItem *item );
    void clearScene();

signals:
    void diverDestroyed();
    void allDiverDestroyed();

private slots:
    void onBombExecutionFinished();
    void onDiverExecutionFinished();

private:
    Copter *copter;
    QSet<Diver *> divers;
    QSet<Bomb *> bombs;

    friend class PlayState;
    friend class CopterState;

};

#endif
