#ifndef ROOM_H
#define ROOM_H

#include <QObject>
#include <QPainter>
#include <QImage>
#include "aabb.h"
#include "enemy.h"
#include "obstacle.h"

class MapImages
{
public:
    static QImage floor; //地板
    static QImage wall_up; //通道上
    static QImage wall_down; //通道上
    static QImage obstracle_up; //障碍上
    static QImage obstracle_down; //障碍上
    static QImage Door_up;  //门升起状态
    static QImage Door_down;  //门下降状态
    static void LoadImages();

private:
    MapImages();
};

class Passageway;
class Room : public QObject, public AABB
{
    Q_OBJECT
    friend class Passageway;    //Passageway可访问该类所有成员
public:
    explicit Room(const QPoint &position, const QSize &size, QVector<QPoint> &obstaclesPos, const QVector<Enemy *> &enemies, QObject *parent = nullptr);
    void drawFirst(QPainter *painter);
    void drawLast(QPainter *painter);

    QVector<Obstacle *> &getObstacles() { return obstacles; }
    QVector<Enemy *> &getEnemies() { return enemies; }

    bool startTest(const AABB &playerAABB);
    bool stopTest();

private:
    QPoint position;
    QSize size;

    //处理下侧围墙忽略
    bool has_ignore;
    int ignore_x, ignore_w;
    void setDownWallIgnore(int x, int w);

    //障碍物
    QVector<Obstacle *> obstacles;

    //通道指针
    QVector<Passageway *> passageways;

    //敌人
    QVector<Enemy *> enemies;

    //角色第一次进入房间时启动
    bool started;

signals:
};

#endif // ROOM_H
