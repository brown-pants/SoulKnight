#include "room.h"
#include "passageway.h"

QImage MapImages::floor;
QImage MapImages::wall_up;
QImage MapImages::wall_down;
QImage MapImages::obstracle_up;
QImage MapImages::obstracle_down;
QImage MapImages::Door_up;
QImage MapImages::Door_down;

void MapImages::LoadImages()
{
    QImage images(":/res/images/room/floor1.png");
    obstracle_up = images.copy(0, 0, 16, 16);
    obstracle_down = images.copy(0, 16, 16, 8);
    wall_up = images.copy(5 * 16, 0, 16, 16);
    wall_down = images.copy(5 * 16, 16, 16, 8);
    Door_up = QImage(":/res/images/room/door.png");
    Door_down = images.copy(2 * 16, 0, 16, 16);
    floor = images.copy(16, 0, 16, 16);
}

Room::Room(const QPoint &position, const QSize &size, QVector<QPoint> &obstaclesPos, const QVector<Enemy *> &enemies, QObject *parent)
    : QObject{parent}, AABB(), position(position), size(size), has_ignore(false), enemies(enemies), started(false)
{
    AABB::setPos(position + QPoint(16, 16 + 8));
    AABB::setSize(size - QSize(32, 26));
    //对障碍物根据位置排序
    std::sort(obstaclesPos.begin(), obstaclesPos.end(), [](const QPoint &p1, const QPoint &p2){
        return p1.y() < p2.y();
    });
    //添加障碍物
    for(auto obstaclePos : obstaclesPos)
    {
        this->obstacles.push_back(new Obstacle(obstaclePos, this));
    }
}

void Room::drawFirst(QPainter *painter)
{
    //画地板
    painter->drawTiledPixmap(QRect(position + QPoint(16, 16), size - QSize(32 - 8, 16)), QPixmap::fromImage(MapImages::floor));
    //画上侧围墙
    painter->drawTiledPixmap(QRect(position + QPoint(0, 8), QSize(size.width(), 16)), QPixmap::fromImage(MapImages::wall_up));
    painter->drawTiledPixmap(QRect(position + QPoint(16, 16 + 8), QSize(size.width() - 32, 8)), QPixmap::fromImage(MapImages::wall_down));
    //画侧边围墙
    painter->drawTiledPixmap(QRect(position + QPoint(0, 16 + 8), QSize(16, size.height() - 32)), QPixmap::fromImage(MapImages::wall_up));
    painter->drawTiledPixmap(QRect(position + QPoint(size.width() - 16, 16 + 8), QSize(16, size.height() - 32)), QPixmap::fromImage(MapImages::wall_up));
}

void Room::drawLast(QPainter *painter)
{
    //画下侧围墙
    if(has_ignore)
    {
        painter->drawTiledPixmap(QRect(position + QPoint(0, size.height() - 16 + 8), QSize(ignore_x - position.x() + 16 + 16, 16)), QPixmap::fromImage(MapImages::wall_up));
        painter->drawTiledPixmap(QRect(position + QPoint(0, size.height() + 8), QSize(ignore_x - position.x() + 16, 8)), QPixmap::fromImage(MapImages::wall_down));

        painter->drawTiledPixmap(QRect(position + QPoint(ignore_x + ignore_w - position.x(), size.height() - 16 + 8), QSize(size.width() - ignore_x + position.x() - ignore_w, 16)), QPixmap::fromImage(MapImages::wall_up));
        painter->drawTiledPixmap(QRect(position + QPoint(ignore_x + ignore_w - position.x() + 16, size.height() + 8), QSize(size.width() - ignore_x + position.x() - ignore_w - 16, 8)), QPixmap::fromImage(MapImages::wall_down));
    }
    else{
        painter->drawTiledPixmap(QRect(position + QPoint(0, size.height() - 16 + 8), QSize(size.width(), 16)), QPixmap::fromImage(MapImages::wall_up));
        painter->drawTiledPixmap(QRect(position + QPoint(0, size.height() + 8), QSize(size.width(), 8)), QPixmap::fromImage(MapImages::wall_down));
    }
}

bool Room::startTest(const AABB &playerAABB)
{
    if(started) return false;
    if(playerAABB.inside(*this))
    {
        //启动房间
        for(auto passageway : passageways)
        {//关门
            passageway->raiseDoors();
        }
        started = true;
    }
    return started;
}

bool Room::stopTest()
{
    for(auto enemy : enemies)
    {
        if(!enemy->died()) return false;
    }
    for(auto passageway : passageways)
    {//开门
        passageway->lowerDoors();
    }
    return true;
}

void Room::setDownWallIgnore(int x, int w)
{
    has_ignore = true;
    ignore_x = x;
    ignore_w = w;
}
