#include "passageway.h"
#include "room.h"

Passageway::Passageway(Room *beginRoom, Room *endRoom, bool isHorizontal, QObject *parent)
    : QObject{parent}, AABB(), beginRoom(beginRoom), endRoom(endRoom), isHorizontal(isHorizontal), isDoorRised(false), width(6)
{
    beginRoom->passageways.push_back(this);
    endRoom->passageways.push_back(this);

    beginRoomPos =  beginRoom->position;
    beginRoomSize = beginRoom->size;
    endRoomPos = endRoom->position;
    endRoomSize = endRoom->size;
    if(isHorizontal)
    {//水平通道
        if(endRoomPos.x() < beginRoomPos.x())
        {//如果end在begin左侧, 反转
            QPoint t1 = beginRoomPos;
            beginRoomPos = endRoomPos;
            endRoomPos = t1;
            QSize t2 = beginRoomSize;
            beginRoomSize = endRoomSize;
            endRoomSize = t2;
        }
        length = endRoomPos.x() - beginRoomPos.x() - beginRoomSize.width();
        drawPos = QPoint(beginRoomPos.x() + beginRoomSize.width() - 16, beginRoomPos.y() + beginRoomSize.height() / 2 - (width / 2) * 16 - 16);
        //设置包围盒
        AABB::setPos(drawPos + QPoint(-32, 16 + 8));
        AABB::setSize(QSize(length + 64 + 32, width * 16 - 10));
    }
    else{//垂直通道
        Room *topRoom = beginRoom;
        if(endRoomPos.y() < beginRoomPos.y())
        {//如果end在begin上方, 反转
            QPoint t1 = beginRoomPos;
            beginRoomPos = endRoomPos;
            endRoomPos = t1;
            QSize t2 = beginRoomSize;
            beginRoomSize = endRoomSize;
            endRoomSize = t2;
            topRoom = endRoom;
        }
        length = endRoomPos.y() - beginRoomPos.y() - beginRoomSize.height();
        drawPos = QPoint(beginRoomPos.x() + beginRoomSize.width() / 2 - (width / 2) * 16 - 16, beginRoomPos.y() + beginRoomSize.height() - 16);
        //设置房间下侧忽略墙壁
        topRoom->setDownWallIgnore(drawPos.x() - 16, width * 16 + 16);
        //设置包围盒
        AABB::setPos(drawPos + QPoint(16, -16));
        AABB::setSize(QSize(width * 16 - 16, length + 64 + 32));
    }
    //初始状态门上升
    raiseDoors();
}

void Passageway::drawFirst(QPainter *painter)
{
    if(isHorizontal)
    {//水平通道
        //画地砖
        painter->drawTiledPixmap(QRect(drawPos + QPoint(0, 16), QSize(length + 32, width * 16)), QPixmap::fromImage(MapImages::floor));
        //画上围墙
        painter->drawTiledPixmap(QRect(drawPos + QPoint(0, 8), QSize(length + 32, 16)), QPixmap::fromImage(MapImages::wall_up));
        painter->drawTiledPixmap(QRect(drawPos + QPoint(0, 16 + 8), QSize(length + 32, 8)), QPixmap::fromImage(MapImages::wall_down));
        //画门
        if(isDoorRised)
        {
            for(int i = 1; i < width; i++)
            {
                painter->drawPixmap(drawPos + QPoint(0, i * 16 + 8), QPixmap::fromImage(MapImages::Door_up));
            }
            for(int i = 1; i < width; i++)
            {
                painter->drawPixmap(drawPos + QPoint(length + 16, i * 16 + 8), QPixmap::fromImage(MapImages::Door_up));
            }
        }
        else{
            painter->drawTiledPixmap(QRect(drawPos + QPoint(0, 16 + 8 + 8), QSize(16, width * 16 - 16)), QPixmap::fromImage(MapImages::Door_down));
            painter->drawTiledPixmap(QRect(drawPos + QPoint(length + 16, 16 + 8 + 8), QSize(16, width * 16 - 16)), QPixmap::fromImage(MapImages::Door_down));
        }
    }
    else{//垂直通道
        //画地砖
        painter->drawTiledPixmap(QRect(drawPos + QPoint(16, 0), QSize(width * 16 - 16, length + 32 + 16)), QPixmap::fromImage(MapImages::floor));
        //画两侧围墙
        painter->drawTiledPixmap(QRect(drawPos + QPoint(0, 8), QSize(16, length + 32)), QPixmap::fromImage(MapImages::wall_up));
        painter->drawTiledPixmap(QRect(drawPos + QPoint(width * 16, 8), QSize(16, length + 32)), QPixmap::fromImage(MapImages::wall_up));
        //画门
        if(isDoorRised)
        {//画下侧门
            for(int i = 1; i < width; i++)
            {
                painter->drawPixmap(drawPos + QPoint(i * 16, length + 16), QPixmap::fromImage(MapImages::Door_up));
            }
        }
        else{
            painter->drawTiledPixmap(QRect(drawPos + QPoint(16, 16), QSize(width * 16 - 16, 16)), QPixmap::fromImage(MapImages::Door_down));
            painter->drawTiledPixmap(QRect(drawPos + QPoint(16, length + 16 + 16), QSize(width * 16 - 16, 16)), QPixmap::fromImage(MapImages::Door_down));
        }
    }
}

void Passageway::drawLast(QPainter *painter)
{
    if(isHorizontal)
    {//水平通道
        //画下围墙
        painter->drawTiledPixmap(QRect(drawPos + QPoint(0, width * 16 + 8), QSize(length + 32, 16)), QPixmap::fromImage(MapImages::wall_up));
        painter->drawTiledPixmap(QRect(drawPos + QPoint(16, 16 + width * 16 + 8), QSize(length, 8)), QPixmap::fromImage(MapImages::wall_down));

    }
    else{//垂直通道
        if(isDoorRised)
        {//画上侧门
            for(int i = 1; i < width; i++)
            {
                painter->drawPixmap(drawPos + QPoint(i * 16, 0), QPixmap::fromImage(MapImages::Door_up));
            }
        }
    }
}

void Passageway::raiseDoors()
{
    AABB::disable();
    isDoorRised = true;
}

void Passageway::lowerDoors()
{
    AABB::enable();
    isDoorRised = false;
}
