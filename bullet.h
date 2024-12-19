#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QVector2D>
#include "aabb.h"

struct Bullet : AABB
{
    struct Images
    {
        Images(const QImage &right = QImage())
            : right(right)
        {
            left = right.mirrored(true, false);
        }
        QImage left, right;
    };
    static Images ak47Bullet_imgs;
    static Images EnemyBullet1_imgs;
    static Images EnemyBullet2_imgs;
    static bool isLoadImages;
    static void LoadImages();

    Bullet(Images &images = ak47Bullet_imgs, const QPointF centerPos = QPointF(), const QPointF moveDir = QPointF(), const double &moveSpeed = 0.0, int attack = 1);

    QPointF centerPos;
    QPointF moveDir;
    double moveSpeed;
    double rotateAngle;
    QImage *img;
    int attack;

    void setUpRotate(Images &imgs, const QVector2D &dir);
    bool update(const QVector<AABB *> &rooms_aabb, const QVector<AABB *> &obstacles_aabb);
    void updateAABB();
    void draw(QPainter *painter);
};

class BulletsManager
{
public:
    BulletsManager();

    void AddBullet(const Bullet &bullet);
    void Update(const QVector<AABB *> &rooms_aabb, const QVector<AABB *> &obstacles_aabb);
    void Draw(QPainter *painter);
    void DrawBoundingBoxes(QPainter *painter, Qt::GlobalColor color);

    int hit(const AABB &aabb);

private:
    QVector<Bullet> bullets;
};

#endif // BULLET_H
