#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include "scene.h"

namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(Player *player, const QString &path, QWidget *parent = nullptr);
    ~GameWidget();

protected:
    void paintEvent(QPaintEvent *e);

#if !defined(ANDROID)
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
#endif

    void resizeEvent(QResizeEvent *e);

private:
    Ui::GameWidget *ui;

    Scene scene;

    double fps;

    QTimer fpsUpdateTimer;
};

#endif // GAMEWIDGET_H
