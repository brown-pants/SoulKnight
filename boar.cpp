#include "boar.h"
#include "player.h"
#include <QVector2D>
#include <QtGlobal>

Boar::Images Boar::BoarImgs;
Boar::Images Boar::DireBoarImgs;
bool Boar::isLoadImgs = false;

Boar::Boar(Player *player, const QPointF &position)
    : Enemy(position, 0.3, 500.0, 15, player), player(player), randomDir(0)
{
    if(!isLoadImgs)
    {
        LoadImages();
        isLoadImgs = true;
    }
    if(rand() % 2)
    {
        setMoveSpeed(0.5);
        setMoveDistance(600.0);
        setHealth(10);
        imgSize = DireBoarImgs.die_right.size();
        Enemy::init(&DireBoarImgs);
    }
    else
    {
        imgSize = BoarImgs.die_right.size();
        Enemy::init(&BoarImgs);
    }
}

QVector2D Boar::getMoveDir()
{
    QPointF dir;
    if(randomDir == 2)
    {//随机方向
        int x = rand() % 20;
        int y = rand() % 20;
        if(x < 10) x *= -1;
        else x -= 10;
        if(y < 10) y *= -1;
        else y -= 10;
        dir.setX(x * 0.1);
        dir.setY(y * 0.1);
    }
    else
    {//角色方向
        dir = player->getCenterPos() - (position + QPointF(imgSize.width() / 2.0, imgSize.height() / 2.0));
    }
    if(++randomDir == 3) randomDir = 0;
    return QVector2D(dir.x(), dir.y()).normalized();
}

void Boar::updateAABB()
{
    AABB::setPos(position + QPointF(3.0, 18.0));
    AABB::setSize(imgSize - QSize(3.0, 18.0));
}

void Boar::LoadImages()
{
    //boar
    QImage idle_imgs(":/res/images/enemy/Boar/Idle_Sheet.png");
    QImage run_imgs(":/res/images/enemy/Boar/Run_Sheet.png");
    BoarImgs.die_right.load(":/res/images/enemy/Boar/Die.png");
    BoarImgs.die_left = BoarImgs.die_right.mirrored(true, false);
    for(int i = 0; i < 2; i ++)
    {
        BoarImgs.idle_right.push_back(idle_imgs.copy(i * 32, 0, 32, 32));
        BoarImgs.idle_left.push_back(BoarImgs.idle_right[i].mirrored(true, false));
    }
    for(int i = 0; i < 4; i ++)
    {
        BoarImgs.run_right.push_back(run_imgs.copy(i * 32, 0, 32, 32));
        BoarImgs.run_left.push_back(BoarImgs.run_right[i].mirrored(true, false));
    }

    //dire boar
    idle_imgs.load(":/res/images/enemy/DireBoar/Idle-Sheet.png");
    run_imgs.load(":/res/images/enemy/DireBoar/Run-Sheet.png");
    DireBoarImgs.die_right.load(":/res/images/enemy/DireBoar/Die.png");
    DireBoarImgs.die_left = DireBoarImgs.die_right.mirrored(true, false);
    for(int i = 0; i < 2; i ++)
    {
        DireBoarImgs.idle_right.push_back(idle_imgs.copy(i * 32, 0, 32, 32));
        DireBoarImgs.idle_left.push_back(DireBoarImgs.idle_right[i].mirrored(true, false));
    }
    for(int i = 0; i < 4; i ++)
    {
        DireBoarImgs.run_right.push_back(run_imgs.copy(i * 32, 0, 32, 32));
        DireBoarImgs.run_left.push_back(DireBoarImgs.run_right[i].mirrored(true, false));
    }
}
