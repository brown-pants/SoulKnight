#ifndef PASSAGEWAY_H
#define PASSAGEWAY_H

#include <QObject>
#include "aabb.h"

class Room;
class Passageway : public QObject, public AABB
{
    Q_OBJECT
public:
    explicit Passageway(Room *beginRoom, Room *endRoom, bool isHorizontal, QObject *parent = nullptr);

    void drawFirst(QPainter *painter);
    void drawLast(QPainter *painter);

    void raiseDoors();
    void lowerDoors();

private:
    Room *beginRoom, *endRoom;  //连接的房间, 起始与结束 (从一侧的平行延伸至另一侧)
    bool isHorizontal;
    bool isDoorRised;
    int width;  //通道宽度
    int length;
    QPoint drawPos;

    QPoint beginRoomPos, endRoomPos;
    QSize beginRoomSize, endRoomSize;

signals:
};

#endif // PASSAGEWAY_H
