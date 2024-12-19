#include "scene.h"
#include "bullet.h"
#include "boar.h"
#include "goblin.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Scene::Scene(Player *player, const QString &path, QObject *parent)
    : QObject{parent}, player(player), curRoom(nullptr)
{
    MapImages::LoadImages();
    if(!GenerateScene(path)) exit(-1);
    srand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

void Scene::drawScene(QPainter *painter)
{
    //画房间(先)
    for(auto room : rooms)
    {
        room->drawFirst(painter);
    }
    //画通道(先)
    for(auto passageway : passageways)
    {
        passageway->drawFirst(painter);
    }
    //画实体
    drawEntities(painter);
    //画敌人子弹
    Enemy::getBulletsManager().Draw(painter);
    //画房间(后)
    for(auto room : rooms)
    {
        room->drawLast(painter);
    }
    //画通道(后)
    for(auto passageway : passageways)
    {
        passageway->drawLast(painter);
    }
}

void Scene::drawEntities(QPainter *painter)
{
    QList<Entity *> curRoomEntities;
    //添加角色
    curRoomEntities.push_back(player);
    //添加敌人
    for(auto room : rooms)
    {
        for(auto enemy : room->getEnemies())
        {
            curRoomEntities.push_back(enemy);
        }
        for(auto obstacle : room->getObstacles())
        {
            curRoomEntities.push_back(obstacle);
        }
    }
    //排序
    std::sort(curRoomEntities.begin(), curRoomEntities.end(), [](Entity *e1, Entity *e2){
        return e1->getZValue() < e2->getZValue();
    });
    //绘制
    for(auto entity : curRoomEntities)
    {
        entity->draw(painter);
    }
}

void Scene::drawAABB(QPainter *painter)
{
    player->drawBoundingBox(painter, Qt::red);
    player->getBulletsManager().DrawBoundingBoxes(painter, Qt::red);
    Enemy::getBulletsManager().DrawBoundingBoxes(painter, Qt::red);
    for(auto room : rooms)
    {
        room->drawBoundingBox(painter, Qt::green);
        for(auto obstacle : room->getObstacles())
        {
            obstacle->drawBoundingBox(painter, Qt::red);
        }
    }
    for(auto passageway : passageways)
    {
        passageway->drawBoundingBox(painter, Qt::green);
    }
    if(curRoom)
    {
        for(auto enemy : curRoom->getEnemies())
        {
            enemy->drawBoundingBox(painter, Qt::red);
        }
    }
}

void Scene::update()
{
    //边界
    QVector<AABB *>rooms_aabb;
    for(auto room : rooms)
    {
        rooms_aabb.push_back(room);
    }
    for(auto passageway : passageways)
    {
        rooms_aabb.push_back(passageway);
    }
    //敌人
    QVector<AABB *> enemies_aabb;
    if(curRoom)
    {
        for(auto enemy : curRoom->getEnemies())
        {
            enemies_aabb.push_back(enemy);
        }
    }
    //障碍物
    QVector<AABB *> obstacles_aabb;
    for(auto room : rooms)
    {
        for(auto obstacle : room->getObstacles())
        {
            obstacles_aabb.push_back(obstacle);
        }
    }
    //角色更新
    player->update(rooms_aabb, enemies_aabb, obstacles_aabb, Enemy::getBulletsManager());
    //房间及敌人更新
    for(auto room : rooms)
    {
        if(room->startTest(*player))
        {
            curRoom = room;
        }
    }
    if(curRoom)
    {
        for(auto enemy : curRoom->getEnemies())
        {
            enemy->update(*curRoom, obstacles_aabb, player->getBulletsManager());
        }
        if(curRoom->stopTest())
        {
            curRoom = nullptr;
        }
    }
    Enemy::getBulletsManager().Update(rooms_aabb, obstacles_aabb);
}

#define JSON_ROOT_OBJECT(json_name, new_name) \
if(json_name.isObject()) { QJsonObject new_name = json_name.object();

#define JSON_OBJECT(json_name, new_name) \
if(json_name.isObject()) { QJsonObject new_name = json_name.toObject();

#define JSON_ARRAY(json_name, new_name) \
if(json_name.isArray()) { QJsonArray new_name = json_name.toArray();

#define JSON_STRING(json_name, new_name) \
if(json_name.isString()) { QString new_name = json_name.toString();

#define JSON_DOUBLE(json_name, new_name) \
if(json_name.isDouble()) { double new_name = json_name.toDouble();

#define JSON_BOOL(json_name, new_name) \
if(json_name.isBool()) { bool new_name = json_name.toBool();

#define JSON_ERROR(json_name, json_type) \
else { qDebug() << json_name << "is not a " << #json_type << Qt::endl; return false; }

#define JSON_CHILDRENS(json_name) \
for(auto child_iter = json_name.begin(); child_iter != json_name.end(); child_iter ++)

bool Scene::GenerateScene(const QString &path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "地图文件打开失败" << Qt::endl;
        return false;
    }

    //解析地图数据
    QJsonDocument json = QJsonDocument::fromJson(file.readAll());
    qDebug() << json << Qt::endl;

    JSON_ROOT_OBJECT(json, root)
    //根节点
    JSON_CHILDRENS(root)
    {//遍历第二层节点
        JSON_OBJECT(child_iter.value(), jsonObj)
        //房间或通道
            JSON_STRING(jsonObj["type"], type)
                if(type == "room")
                {//房间
                    QString room_name = child_iter.key();
                    QPoint room_pos;
                    QSize room_size;
                    QVector<QPoint> obstacles_pos;
                    QVector<Enemy *> enemies;
                    //必须
                    JSON_ARRAY(jsonObj["position"], json_var)
                    //房间位置
                        double x = json_var.begin()->toDouble() * 16;
                        double y = (json_var.begin() + 1)->toDouble() * 16;
                        room_pos = QPoint(x, y);
                    }JSON_ERROR(jsonObj["position"], "Array")
                    //必须
                    JSON_ARRAY(jsonObj["size"], json_var)
                    //房间尺寸
                        double w = json_var.begin()->toDouble() * 16;
                        double h = (json_var.begin() + 1)->toDouble() * 16;
                        room_size = QSize(w, h);
                    }JSON_ERROR(jsonObj["size"], "Array")
                    //可选
                    JSON_ARRAY(jsonObj["player"], json_var)
                    //人物位置
                        double x = json_var.begin()->toDouble() * 16;
                        double y = (json_var.begin() + 1)->toDouble() * 16;
                        player->setPosition(room_pos + QPointF(x, y));
                        qDebug() << "Player : " << room_pos + QPointF(x, y) << Qt::endl;
                    }
                    //可选
                    JSON_ARRAY(jsonObj["obstacles"], json_var)
                    //房间障碍物
                        JSON_CHILDRENS(json_var)
                        {//遍历所有障碍物坐标
                            JSON_ARRAY((*child_iter), obstaclePos)
                                int x = obstaclePos.begin()->toDouble() * 16;
                                int y = (obstaclePos.begin() + 1)->toDouble() * 16;
                                obstacles_pos.push_back(room_pos + QPoint(x, y));
                            }
                        }
                    }
                    //可选
                    JSON_ARRAY(jsonObj["boars"], json_var)
                    //野猪敌人
                        JSON_CHILDRENS(json_var)
                        {//遍历所有野猪坐标
                            JSON_ARRAY((*child_iter), boars)
                                int x = boars.begin()->toDouble() * 16;
                                int y = (boars.begin() + 1)->toDouble() * 16;
                                enemies.push_back(new Boar(player, room_pos + QPointF(x, y)));
                            }
                        }
                    }
                    //可选
                    JSON_ARRAY(jsonObj["goblins"], json_var)
                        //妖怪敌人
                        JSON_CHILDRENS(json_var)
                        {//遍历所有野猪坐标
                            JSON_ARRAY((*child_iter), goblins)
                                int x = goblins.begin()->toDouble() * 16;
                                int y = (goblins.begin() + 1)->toDouble() * 16;
                                enemies.push_back(new Goblin(player, room_pos + QPointF(x, y)));
                            }
                        }
                    }
                    rooms.insert(room_name, new Room(room_pos, room_size, obstacles_pos, enemies, this));
                    qDebug() << room_name << ": Room(" << room_pos << ", " << room_size << ", " << obstacles_pos << ", " << enemies << ")" << Qt::endl;
                }
                else if(type == "passageway")
                {//通道
                    Room *begin_room = nullptr, *end_room = nullptr;
                    bool isHorizontal;
                    //必须
                    JSON_ARRAY(jsonObj["connect"], json_var)
                    //通道连接
                        QString begin_room_name = json_var.begin()->toString();
                        QString end_room_name = (json_var.begin() + 1)->toString();
                        auto begin_room_iter = rooms.find(begin_room_name);
                        auto end_room_iter = rooms.find(end_room_name);
                        if(begin_room_iter != rooms.end())
                        {
                            begin_room = *begin_room_iter;
                        }
                        else {
                            qDebug() << begin_room_name << " is not exist" << Qt::endl;
                            return false;
                        }
                        if(end_room_iter != rooms.end())
                        {
                            end_room = *end_room_iter;
                        }
                        else {
                            qDebug() << end_room_name << "is not exist" << Qt::endl;
                            return false;
                        }
                    }JSON_ERROR(jsonObj["size"], "Array")
                    //必须
                    JSON_BOOL(jsonObj["horizontal"], horizontal)
                    //通道为横向还是纵向
                        isHorizontal = horizontal;
                    }JSON_ERROR(jsonObj["horizontal"], "Bool")
                    passageways.push_back(new Passageway(begin_room, end_room, isHorizontal, this));
                    qDebug() << "Passageway(" << begin_room << ", " << end_room << ", " << isHorizontal << ")" << Qt::endl;
                    }
            }JSON_ERROR(jsonObj["type"], "String")
        }JSON_ERROR(child_iter.value(), "Object")
    }
    }JSON_ERROR(json, "Object")

    return true;
}
