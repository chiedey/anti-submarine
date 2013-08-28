#ifndef STATES_H
#define STATES_H

#include <QtCore/QSet>
#include <QtCore/QState>
#include <QtCore/QSignalTransition>
#include <QtCore/QPropertyAnimation>
#include <QtWidgets/QKeyEventTransition>

class QStateMachine;
class GraphicsScene;
class Copter;
class Diver;
//随机生成潜艇数量
static const int NUM_OF_DIVER = 5;

class PlayState : public QState
{

public:
    explicit PlayState(GraphicsScene *scene, QState *parent = 0 );
    ~PlayState();

protected:
    void onEntry(QEvent *);

private :
    GraphicsScene *scene;
    QStateMachine *machine;

    friend class CopterState;

};

class CopterState : public QState
{

public:
    CopterState( GraphicsScene *scene, PlayState *game, QState *parent )
        : QState(parent), scene(scene), game(game) {}

protected:
    void onEntry( QEvent * );

private :
    void initCopter();
    GraphicsScene *scene;
    PlayState *game;

};

#endif
