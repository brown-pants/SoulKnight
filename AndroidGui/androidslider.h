#ifndef ANDROIDSLIDER_H
#define ANDROIDSLIDER_H

#include <QWidget>
#include <QImage>

class AndroidSlider : public QWidget
{
    Q_OBJECT
public:
    explicit AndroidSlider(QWidget *parent = nullptr);
    static QVector2D getDir();

protected:
    void paintEvent(QPaintEvent *e);
    bool event(QEvent *e);

private:
    QImage sliderImg;
    QImage sliderHandleImg;
    QPointF sliderCenterPos;
    bool isFirstDraw;

    static AndroidSlider *instance;

signals:
};

#endif // ANDROIDSLIDER_H
