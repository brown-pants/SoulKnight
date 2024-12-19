#ifndef AABB_H
#define AABB_H
#include <QRectF>
#include <QPainter>

class AABB
{
public:
    AABB(const QRectF &rect = QRectF());
    bool collision(const AABB &other) const; //判断是否有交集
    bool inside(const AABB &other) const; //判断当前包围盒是否在other包围盒内

    void setPos(const QPointF &pos);
    void setSize(const QSizeF &size);
    inline QRectF getRect() const { return rect; }
    void drawBoundingBox(QPainter *painter, Qt::GlobalColor color) const; //绘制包围盒

    inline void enable() { isEnable = true; }
    inline void disable() { isEnable = false; }

private:
    QRectF rect;
    bool isEnable;
};

#endif // AABB_H
