#include "goblin.h"

#include <QDebug>

Goblin::Images Goblin::GoblinImgs;
Goblin::Images Goblin::EliteGoblinImgs;
bool Goblin::isLoadImgs = false;

Goblin::Goblin(Player *player, const QPointF &position)
    : Enemy(position, 0.3, 500.0, 10, player, true), player(player), isElite(false)
{
    if(!isLoadImgs)
    {
        LoadImages();
        isLoadImgs = true;
    }
    if(rand() % 2)
    {
        isElite = true;
        imgSize = EliteGoblinImgs.die_right.size();
        Enemy::init(&EliteGoblinImgs);
    }
    else
    {
        imgSize = GoblinImgs.die_right.size();
        Enemy::init(&GoblinImgs);
    }
}

QVector2D Goblin::getMoveDir()
{
    QPointF dir;
    int x = rand() % 20;
    int y = rand() % 20;
    if(x < 10) x *= -1;
    else x -= 10;
    if(y < 10) y *= -1;
    else y -= 10;
    dir.setX(x * 0.1);
    dir.setY(y * 0.1);
    return QVector2D(dir.x(), dir.y()).normalized();
}

void Goblin::updateAABB()
{
    AABB::setPos(position + QPointF(7.0, 18.0));
    AABB::setSize(imgSize - QSize(7.0, 18.0));
}

QVector<Bullet> Goblin::getBullet()
{
    QVector<Bullet> bullets;
    QPointF centerPos(position + QPointF(imgSize.width() / 2.0, imgSize.height() / 2.0));
    if(isElite)
    {
        int count = 16;
        for(float r = 0.0f; r < 360.0f; r += 360.0f / count)
        {
            QVector2D dir(cos(r), sin(r));
            bullets.push_back(Bullet(Bullet::EnemyBullet2_imgs, centerPos, dir.normalized().toPointF(), 0.8, 8));
        }
    }
    else
    {
        QPointF dir = player->getCenterPos() - centerPos;
        bullets.push_back(Bullet(Bullet::EnemyBullet1_imgs, centerPos, QVector2D(dir.x(), dir.y()).normalized().toPointF(), 1.0, 10));
    }
    return bullets;
}

void Goblin::LoadImages()
{
    //globlin
    QImage idle_imgs(":/res/images/enemy/Goblin/Idle_Sheet.png");
    QImage run_imgs(":/res/images/enemy/Goblin/Run_Sheet.png");
    GoblinImgs.die_right.load(":/res/images/enemy/Goblin/Die.png");
    GoblinImgs.die_left = GoblinImgs.die_right.mirrored(true, false);
    for(int i = 0; i < 4; i ++)
    {
        GoblinImgs.idle_right.push_back(idle_imgs.copy(i * 32, 0, 32, 32));
        GoblinImgs.idle_left.push_back(GoblinImgs.idle_right[i].mirrored(true, false));
    }
    for(int i = 0; i < 4; i ++)
    {
        GoblinImgs.run_right.push_back(run_imgs.copy(i * 32, 0, 32, 32));
        GoblinImgs.run_left.push_back(GoblinImgs.run_right[i].mirrored(true, false));
    }

    //elite globlin
    idle_imgs.load(":/res/images/enemy/EliteGoblin/Idle-Sheet.png");
    run_imgs.load(":/res/images/enemy/EliteGoblin/Run-Sheet.png");
    EliteGoblinImgs.die_right.load(":/res/images/enemy/EliteGoblin/Die.png");
    EliteGoblinImgs.die_left = EliteGoblinImgs.die_right.mirrored(true, false);
    for(int i = 0; i < 4; i ++)
    {
        EliteGoblinImgs.idle_right.push_back(idle_imgs.copy(i * 32, 0, 32, 32));
        EliteGoblinImgs.idle_left.push_back(EliteGoblinImgs.idle_right[i].mirrored(true, false));
    }
    for(int i = 0; i < 4; i ++)
    {
        EliteGoblinImgs.run_right.push_back(run_imgs.copy(i * 32, 0, 32, 32));
        EliteGoblinImgs.run_left.push_back(EliteGoblinImgs.run_right[i].mirrored(true, false));
    }
}
