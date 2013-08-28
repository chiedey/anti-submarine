#ifndef QANIMATIONSTATE_H
#define QANIMATIONSTATE_H

#ifndef QT_STATEMACHINE_SOLUTION
    #include <QtCore/qstate.h>
    #include <QtCore/qabstractanimation.h>
#else
    #include "./qstate.h"
    #include "./qabstractanimation.h"
#endif

#ifndef QT_NO_ANIMATION

class QAbstractAnimation;

class QAnimationState : public QState
{
    Q_OBJECT

public:
    QAnimationState(QState *parent = 0);
    ~QAnimationState();

    void setAnimation(QAbstractAnimation *animation);
    QAbstractAnimation* animation() const;

signals:
    void animationFinished();

protected:
    void onEntry(QEvent *);
    void onExit(QEvent *);
    bool event(QEvent *e);

private:
    Q_DISABLE_COPY(QAnimationState)
    QAbstractAnimation *m_animation;

};

#endif

#endif
