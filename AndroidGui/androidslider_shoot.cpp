#include "androidslider_shoot.h"
#include "global.h"
#include <QPainter>
#include <QVector2D>
#include <QEvent>
#include <QTouchEvent>

AndroidSlider_Shoot *AndroidSlider_Shoot::instance = nullptr;

AndroidSlider_Shoot::AndroidSlider_Shoot(QWidget *parent)
    : QWidget{parent}, isFirstDraw(true)
{
    sliderImg.load(":/res/images/ui/Slider.png");
    sliderHandleImg.load(":/res/images/ui/SliderHandleLight.png");
    instance = this;
    setMinimumSize(150, 150);
    setAttribute(Qt::WA_AcceptTouchEvents);
}

QVector2D AndroidSlider_Shoot::getDir()
{
    return QVector2D(instance->sliderCenterPos.x() - instance->width() / 2, instance->sliderCenterPos.y() - instance->height() / 2).normalized();
}

void AndroidSlider_Shoot::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(isFirstDraw)
    {
        sliderCenterPos.setX(width() / 2);
        sliderCenterPos.setY(height() / 2);
        isFirstDraw = false;
    }
    painter.drawPixmap(0, 0, QPixmap::fromImage(sliderHandleImg).scaled(size()));
    QSize sliderSize(size() / 3);
    painter.translate(sliderCenterPos);
    painter.translate(-QPointF(sliderSize.width() / 2, sliderSize.height() / 2));
    painter.drawPixmap(0, 0, QPixmap::fromImage(sliderImg).scaled(sliderSize));
    update();

}

bool AndroidSlider_Shoot::event(QEvent *e)
{
    if(e->type() == QEvent::TouchBegin || e->type() == QEvent::TouchUpdate)
    {
        sliderCenterPos = static_cast<QTouchEvent *>(e)->points().first().position();
        Global::Touched = true;
        return true;
    }
    else if(e->type() == QEvent::TouchEnd)
    {
        sliderCenterPos.setX(width() / 2);
        sliderCenterPos.setY(height() / 2);
        Global::Touched = false;
        return true;
    }
    return QWidget::event(e);
}