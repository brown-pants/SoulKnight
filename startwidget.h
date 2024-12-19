#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include "gamewidget.h"
#include "player.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class StartWidget;
}
QT_END_NAMESPACE

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    StartWidget(QWidget *parent = nullptr);
    ~StartWidget();

protected:
    void paintEvent(QPaintEvent *e);

private:
    Ui::StartWidget *ui;

    Player player;
    GameWidget *gameWidget;
};
#endif // STARTWIDGET_H
