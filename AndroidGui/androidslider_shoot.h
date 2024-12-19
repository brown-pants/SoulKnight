#ifndef ANDROIDSLIDER_SHOOT_H
#define ANDROIDSLIDER_SHOOT_H

#include <QWidget>
#include <QImage>

class AndroidSlider_Shoot : public QWidget
{
    Q_OBJECT
public:
    explicit AndroidSlider_Shoot(QWidget *parent = nullptr);
    static QVector2D getDir();

protected:
    void paintEvent(QPaintEvent *e);
    bool event(QEvent *e);

private:
    QImage sliderImg;
    QImage sliderHandleImg;
    QPointF sliderCenterPos;
    bool isFirstDraw;

    static AndroidSlider_Shoot *instance;

signals:
};

#endif // ANDROIDSLIDER_SHOOT_H
