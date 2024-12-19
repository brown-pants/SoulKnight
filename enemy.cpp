#include "enemy.h"
#include "global.h"
#include <qdebug.h>

BulletsManager Enemy::bulletsManager;

Enemy::Enemy(const QPointF &position, double moveSpeed, double maxMoveDistance, int health, QObject *parent, bool shoot_able)
    : QObject{parent}, position(position), isLeftImage(false), index_idleImgs(0), index_runImgs(0), isStart(false),
    moveSpeed(moveSpeed), maxMoveDistance(maxMoveDistance), curMoveDistance(0.0), isDied(false), health(health), curHealth(health),
    isShoot(false), shoot_able(shoot_able)
{
}

void Enemy::init(Enemy::Images *imgs)
{
    this->imgs = imgs;
    this->curImg = &imgs->idle_right[0];
    connect(&idle_anim_timer, &QTimer::timeout,[this](){
        if(index_idleImgs == this->imgs->idle_right.size()) index_idleImgs = 0;
        curImg = isLeftImage ? &this->imgs->idle_left[index_idleImgs] : &this->imgs->idle_right[index_idleImgs];
        index_idleImgs++;
    });
    connect(&run_anim_timer, &QTimer::timeout,[this](){
        if(index_runImgs == this->imgs->run_right.size()) index_runImgs = 0;
        curImg = isLeftImage ? &this->imgs->run_left[index_runImgs] : &this->imgs->run_right[index_runImgs];
        index_runImgs++;
    });
    idle_anim_timer.setInterval(120);
    run_anim_timer.setInterval(120);
    idle_anim_timer.start();

    connect(&moveInterval_timer, &QTimer::timeout,[this](){
        curMoveDir = getMoveDir();
        if(curMoveDir.x() == 0.0f && curMoveDir.y() == 0.0)
        {
            curMoveDir = QVector2D(1.0f, 0.0f);
        }
        if(curMoveDir.x() > 0)
        {
            isLeftImage = false;
        }
        else {
            isLeftImage = true;
        }
        idle_anim_timer.stop();
        run_anim_timer.start();
        moveInterval_timer.stop();
    });
    moveInterval_timer.setInterval(1000);

    if(shoot_able)
    {
        connect(&shoot_timer, &QTimer::timeout, [this](){
            isShoot = true;
        });
        shoot_timer.setInterval(1000);
    }

    updateAABB();
}

BulletsManager &Enemy::getBulletsManager()
{
    return bulletsManager;
}

void Enemy::draw(QPainter *painter)
{
    painter->drawPixmap(position, QPixmap::fromImage(*curImg));
    //画血条
    if(!isDied)
    {
        int healthWidth = 32;
        int xpos = position.x() - (healthWidth - curImg->size().width()) / 2;
        painter->save();
        painter->setBrush(QBrush(Qt::gray));
        painter->drawRect(xpos, position.y() + 5, healthWidth, 2);
        painter->setBrush(QBrush(Qt::red));
        painter->drawRect(xpos, position.y() + 5, healthWidth * (curHealth / (float)health), 2);
        painter->restore();
    }
}

void Enemy::update(const AABB &room_aabb, const QVector<AABB *> obstacles_aabb, BulletsManager &playerBulletsManager)
{
    if(isDied) return;
    if(!isStart)
    {
        if(shoot_able) shoot_timer.start();
        moveInterval_timer.start();
        isStart = true;
    }
    if(isShoot)
    {
        for(auto bullet : getBullet())
        {
            bulletsManager.AddBullet(bullet);
        }
        isShoot = false;
    }
    enemyMoved(room_aabb, obstacles_aabb);
    int attack = playerBulletsManager.hit(*this);
    curHealth -= attack;
    if(curHealth < 0)
    {
        isDied = true;
        AABB::disable();
        idle_anim_timer.stop();
        run_anim_timer.stop();
        moveInterval_timer.stop();
        if(isLeftImage)
        {
            curImg = &imgs->die_left;
        }
        else curImg = &imgs->die_right;
    }
}

void Enemy::enemyMoved(const AABB &room_aabb, const QVector<AABB *> obstacles_aabb)
{
    double runSpeed = moveSpeed * Global::elapsed;
    curMoveDistance += runSpeed;
    //运动距离检测
    if(curMoveDistance >= maxMoveDistance)
    {
        curMoveDir = QVector2D(0.0, 0.0);
        curMoveDistance = 0.0;
        run_anim_timer.stop();
        idle_anim_timer.start();
        moveInterval_timer.start();
    }
    //边界判断
    //横向
    bool stop = false;
    position += QPointF(curMoveDir.x(), 0.0) * runSpeed;
    updateAABB();
    if(!AABB::inside(room_aabb)) {
        stop = true;
    }
    for(auto aabb : obstacles_aabb)
    {
        if(AABB::collision(*aabb) && stop == false)
        {
            stop = true;
            break;
        }
    }
    if(stop) {
        position -= QPointF(curMoveDir.x(), 0.0) * runSpeed;
        updateAABB();
    }
    //纵向
    stop = false;
    position += QPointF(0.0, curMoveDir.y()) * runSpeed;
    updateAABB();
    if(!AABB::inside(room_aabb)) {
        stop = true;
    }
    for(auto aabb : obstacles_aabb)
    {
        if(AABB::collision(*aabb) && stop == false)
        {
            stop = true;
            break;
        }
    }
    if(stop) {
        position -= QPointF(0.0, curMoveDir.y()) * runSpeed;
        updateAABB();
    }
    setZValue(position.y() + curImg->height());
}
