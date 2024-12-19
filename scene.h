#ifndef SCENE_H
#define SCENE_H

#include "player.h"
#include "room.h"
#include "passageway.h"
#include <QMap>

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(Player *player, const QString &path, QObject *parent = nullptr);

    void drawScene(QPainter *painter);
    void drawAABB(QPainter *painter);
    void update();

private:
    Player *player;
    QMap<QString, Room *> rooms;
    QVector<Passageway *> passageways;
    Room *curRoom;

    //生成场景
    bool GenerateScene(const QString &path);
    //绘制实体
    void drawEntities(QPainter *painter);

signals:
};

#endif // SCENE_H
