#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QObject>
#include "aabb.h"
#include "Entity.h"

class Obstacle : public QObject, public AABB, public Entity
{
    Q_OBJECT
public:
    explicit Obstacle(const QPointF &position, QObject *parent = nullptr);

    void draw(QPainter *painter);

private:
    QPointF position;

signals:
};

#endif // OBSTACLE_H
