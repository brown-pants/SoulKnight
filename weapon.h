#ifndef WEAPON_H
#define WEAPON_H

#include <QObject>
#include <QImage>

class Weapon : public QObject
{
    Q_OBJECT
public:
    explicit Weapon(QObject *parent = nullptr);
    void draw(QPainter *painter, const QPointF &player_center);
    static QVector2D getShootDir();

private:
    QImage weapon_right_img;
    QImage weapon_left_img;
    QSize weapon_imgSize;


signals:
};

#endif // WEAPON_H
