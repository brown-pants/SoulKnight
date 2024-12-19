#include "weapon.h"
#include "global.h"
#include "qvectornd.h"
#include <QPainter>
#include <QDebug>
#if defined(ANDROID)
#include "AndroidGui/androidslider_shoot.h"
#endif

Weapon::Weapon(QObject *parent)
    : QObject{parent}, weapon_right_img(":/res/images/weapon/Ak47.png"), weapon_left_img(weapon_right_img.mirrored(true, false)), weapon_imgSize(weapon_right_img.size())
{
}

void Weapon::draw(QPainter *painter, const QPointF &player_center)
{
    QPointF weapon_center(player_center + QPointF(0, 10));

    QImage *img = nullptr;
    QVector2D shootDir = getShootDir();
    //计算角度
    QVector2D x_axis(1.0f, 0.0f);
    if(shootDir.x() < 0)
    {
        x_axis.setX(-1.0f);
        img = &weapon_left_img;
    }
    else{
        img = &weapon_right_img;
    }

    float dot = QVector2D::dotProduct(shootDir, x_axis);
    float cos_angle = dot;
    float angle = acos(cos_angle) / 3.1415927 * 180.0;

    if(shootDir.x() > 0 && shootDir.y() < 0)
    {
        angle *= -1.0;
    }
    if(shootDir.x() < 0 && shootDir.y() > 0)
    {
        angle *= -1.0;
    }
    painter->save();
    painter->translate(weapon_center);
    painter->translate((shootDir * 8).toPointF());
    painter->rotate(angle);

    painter->drawPixmap(-weapon_imgSize.width() / 2.0, -weapon_imgSize.height() / 2.0, QPixmap::fromImage(*img));
    painter->restore();
}

QVector2D Weapon::getShootDir()
{
#if defined(ANDROID)
    static QPointF lastVec;
    QPointF vec = AndroidSlider_Shoot::getDir().toPointF();
    if(vec == QPointF(0.0, 0.0))
    {
        vec = lastVec;
    }
    lastVec = vec;
#else
    QPointF vec = Global::MouseState.pos - QPointF(Global::GameWindowSize.width() / 2.0, Global::GameWindowSize.height() / 2.0);
#endif
    QVector2D shootDir(vec.x(), vec.y());
    return shootDir.normalized();
}
