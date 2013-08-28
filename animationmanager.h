#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <QtCore/QObject>

class QAbstractAnimation;

class AnimationManager : public QObject
{
    Q_OBJECT

public:
    AnimationManager();
    void registerAnimation(QAbstractAnimation *anim);
    void unregisterAnimation(QAbstractAnimation *anim);
    void unregisterAllAnimations();
    static AnimationManager *self();

public slots:
    void pauseAll();
    void resumeAll();

private slots:
    void unregisterAnimation_helper(QObject *obj);

private:
    static AnimationManager *instance;
    QList<QAbstractAnimation *> animations;

};

#endif
