#include "aabb.h"

AABB::AABB(const QRectF &rect) : rect(rect), isEnable(true) {}

bool AABB::collision(const AABB &other) const
{
    if(!isEnable || !other.isEnable) return false;
    if(rect.left() < other.rect.right() && rect.right() > other.rect.left() && rect.top() < other.rect.bottom() && rect.bottom() > other.rect.top())
    {
        return true;
    }
    else return false;
}

bool AABB::inside(const AABB &other) const
{
    if(!isEnable || !other.isEnable) return false;
    if(rect.left() > other.rect.left() && rect.right() < other.rect.right() && rect.top() > other.rect.top() && rect.bottom() < other.rect.bottom())
    {
        return true;
    }
    else return false;
}

void AABB::drawBoundingBox(QPainter *painter, Qt::GlobalColor color) const
{
    if(!isEnable) return;
    painter->save();
    painter->setPen(QPen(color));
    painter->drawRect(rect);
    painter->restore();
}

void AABB::setPos(const QPointF &pos)
{
    rect.setX(pos.x());
    rect.setY(pos.y());
}

void AABB::setSize(const QSizeF &size)
{
    rect.setSize(size);
}
