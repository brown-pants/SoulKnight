#ifndef ENTITY_H
#define ENTITY_H
#include <QPainter>

//渲染时需要根据顺序来绘制(Player, Obstracle, Enemy都继承该类)
class Entity
{
public:
    Entity(double zValue = -100000.0) : zValue(zValue) {}
    virtual void draw(QPainter *painter) = 0;   //绘制虚函数

    inline double getZValue() const { return zValue; }
    inline void setZValue(double zValue) { this->zValue = zValue; }

private:
    double zValue;  //zValue越大越后绘制(遮挡比其小的实体)
};

#endif // ENTITY_H
