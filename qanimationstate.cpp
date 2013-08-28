#include "qanimationstate.h"

#include <QtCore/qstate.h>

QT_BEGIN_NAMESPACE

/*!
\class QAnimationState

\brief The QAnimationState class provides state that handle an animation and emit
a signal when this animation is finished.

\ingroup statemachine

QAnimationState provides a state that handle an animation. It will start this animation
when the state is entered and stop it when it is leaved. When the animation has finished the
state emit animationFinished signal.
QAnimationState is part of \l{The State Machine Framework}.

\code
QStateMachine machine;
QAnimationState *s = new QAnimationState(machine->rootState());
QPropertyAnimation *animation = new QPropertyAnimation(obj, "pos");
s->setAnimation(animation);
QState *s2 = new QState(machine->rootState());
s->addTransition(s, SIGNAL(animationFinished()), s2);
machine.start();
\endcode

\sa QState, {The Animation Framework}
*/


#ifndef QT_NO_ANIMATION

/*!
  Constructs a new state with the given \a parent state.
*/
QAnimationState::QAnimationState(QState *parent)
    : QState(parent), m_animation(0)
{
}

/*!
  Destroys the animation state.
*/
QAnimationState::~QAnimationState()
{
}

/*!
  Set an \a animation for this QAnimationState. If an animation was previously handle by this
  state then it won't emit animationFinished for the old animation. The QAnimationState doesn't
  take the ownership of the animation.
*/
void QAnimationState::setAnimation(QAbstractAnimation *animation)
{
    if (animation == m_animation)
        return;

    //Disconnect from the previous animation if exist
    if(m_animation)
        disconnect(m_animation, SIGNAL(finished()), this, SIGNAL(animationFinished()));

    m_animation = animation;

    if (m_animation) {
        //connect the new animation
        connect(m_animation, SIGNAL(finished()), this, SIGNAL(animationFinished()));
    }
}

/*!
  Returns the animation handle by this animation state, or 0 if there is no animation.
*/
QAbstractAnimation* QAnimationState::animation() const
{
    return m_animation;
}

/*!
  \reimp
*/
void QAnimationState::onEntry(QEvent *)
{
    if (m_animation)
        m_animation->start();
}

/*!
  \reimp
*/
void QAnimationState::onExit(QEvent *)
{
    if (m_animation)
        m_animation->stop();
}

/*!
  \reimp
*/
bool QAnimationState::event(QEvent *e)
{
    return QState::event(e);
}

QT_END_NAMESPACE

#endif

