#include "obstacle.h"
#include "room.h"
#include <QPainter>

Obstacle::Obstacle(const QPointF &position, QObject *parent)
    : QObject{parent}, position(position)
{
    AABB::setPos(position + QPointF(0.0, 16));
    AABB::setSize(QSizeF(16, 10));
    Entity::setZValue(position.y() + 16 + 16 + MapImages::obstracle_down.size().height());
}

void Obstacle::draw(QPainter *painter)
{
    painter->drawPixmap(position + QPoint(0, 8), QPixmap::fromImage(MapImages::obstracle_up));
    painter->drawPixmap(position + QPoint(0, 16 + 8), QPixmap::fromImage(MapImages::obstracle_down));
}
