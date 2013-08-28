#include <QtCore/QDebug>
#include <QtCore/QAbstractAnimation>
#include "animationmanager.h"

AnimationManager *AnimationManager::instance = 0;

AnimationManager::AnimationManager()
{
}

AnimationManager *AnimationManager::self()
{

    if (!instance) {
        instance = new AnimationManager;
    }
    return instance;
}

void AnimationManager::registerAnimation( QAbstractAnimation *anim )
{

    QObject::connect(anim, SIGNAL(destroyed(QObject*)),
                     this, SLOT(unregisterAnimation_helper(QObject*)));
    animations.append(anim);

}

void AnimationManager::unregisterAnimation_helper(QObject *obj)
{

    unregisterAnimation(static_cast<QAbstractAnimation*>(obj));

}

void AnimationManager::unregisterAnimation( QAbstractAnimation *anim )
{

    QObject::disconnect(anim, SIGNAL(destroyed(QObject*)),
                        this, SLOT(unregisterAnimation_helper(QObject*)));
    animations.removeAll(anim);

}

void AnimationManager::unregisterAllAnimations()
{

    animations.clear();

}

void AnimationManager::pauseAll()
{

    foreach ( QAbstractAnimation* animation, animations ) {
        if ( animation->state() == QAbstractAnimation::Running )
            animation->pause();
    }

}
void AnimationManager::resumeAll()
{
    foreach ( QAbstractAnimation* animation, animations ) {
        if ( animation->state() == QAbstractAnimation::Paused )
            animation->resume();
    }
}
