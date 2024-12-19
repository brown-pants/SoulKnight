#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QImage>
#include <QTimer>
#include <QVector2D>
#include "Entity.h"
#include "aabb.h"
#include "bullet.h"

class Enemy : public QObject, public AABB, public Entity
{
    Q_OBJECT
public:
    explicit Enemy(const QPointF &position, double moveSpeed, double maxMoveDistance, int health, QObject *parent = nullptr, bool shoot_able = false);

    virtual void draw(QPainter *painter);
    virtual void update(const AABB &room_aabb, const QVector<AABB *> obstacles_aabb, BulletsManager &playerBulletsManager);

    inline bool died() const { return isDied; }

    inline void setMoveSpeed(double speed) { this->moveSpeed = speed; }
    inline void setMoveDistance(double distance) { this->maxMoveDistance = distance; }
    inline void setHealth(int health) { this->health = health; this->curHealth = health; }

    static BulletsManager &getBulletsManager();

protected:
    struct Images
    {
        QVector<QImage> idle_left;
        QVector<QImage> idle_right;
        QVector<QImage> run_left;
        QVector<QImage> run_right;
        QImage die_left, die_right;
    };
    void init(Images *imgs);

    virtual void updateAABB() {}
    virtual QVector2D getMoveDir() { return QVector2D(); }
    virtual QVector<Bullet> getBullet() { return QVector<Bullet>(); };
    QPointF position;

private:
    Images *imgs;
    QImage *curImg;
    bool isLeftImage;
    int index_idleImgs;
    int index_runImgs;

    QTimer idle_anim_timer;
    QTimer run_anim_timer;
    bool isStart;

    QTimer moveInterval_timer;
    QVector2D curMoveDir;
    double moveSpeed;
    double maxMoveDistance;
    double curMoveDistance;

    bool isDied;
    int health;
    int curHealth;

    void enemyMoved(const AABB &room_aabb, const QVector<AABB *> obstacles_aabb);

    //敌人子弹
    static BulletsManager bulletsManager;

    QTimer shoot_timer;
    bool isShoot;
    bool shoot_able;

signals:
};

#endif // ENEMY_H
