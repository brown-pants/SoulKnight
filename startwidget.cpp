#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 300
#include "startwidget.h"
#include "ui_startwidget.h"
#include <QPainter>
#include <QFileDialog>

StartWidget::StartWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartWidget), gameWidget(nullptr)
{
    ui->setupUi(this);
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    ui->lineEdit->setText(":/res/map/map1.json");
    connect(ui->pb_switchRole, &QPushButton::clicked, [this](){
        static int i = 1;
        if(++ i == 4) i = 1;
        player.changeImages(i);
    });
    connect(ui->pb_startGame, &QPushButton::clicked, [this](){
        QString path = ui->lineEdit->text();
        gameWidget = new GameWidget(&player, path);
        this->hide();
        gameWidget->show();
    });
    connect(ui->pb_choose, &QPushButton::clicked, [this](){
        QString path = QFileDialog::getOpenFileName(this, "选择地图", "", "Json Files(*.json)");
        if(!path.isEmpty()) ui->lineEdit->setText(path);
    });
}

StartWidget::~StartWidget()
{
    delete ui;
    delete gameWidget;
}

void StartWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setWindow(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    painter.setViewport(0, 0, size().width(), size().height());
#if !defined(ANDROID)
    player.draw(&painter, QPointF(WINDOW_WIDTH / 2 - 32, WINDOW_HEIGHT / 3), QSize(64, 64));
#endif
    update();
}
