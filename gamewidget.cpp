#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "global.h"
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>

static bool isDrawBoundingBox = false;

GameWidget::GameWidget(Player *player, const QString &path, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWidget), scene(player, path)
{
    ui->setupUi(this);
    resize(Global::RenderSize);
    setMouseTracking(true);
    QCursor pointingHandCursor(QPixmap(":/res/images/ui/Point.png").scaled(50, 50));
    setCursor(pointingHandCursor);

    connect(&fpsUpdateTimer, &QTimer::timeout, [this](){
        fps = 1000.0 / Global::elapsed;
    });
    fpsUpdateTimer.start(1000);

#if !defined(ANDROID)
    ui->widget_androidSlider->hide();
    ui->widget_androidSlider_Shoot->hide();
#endif
}

GameWidget::~GameWidget()
{
    delete ui;
}

void GameWidget::paintEvent(QPaintEvent *)
{
    auto start = std::chrono::high_resolution_clock::now();
    QPainter painter(this);
    //画背景
    painter.save();
    painter.setBrush(Qt::black);
    painter.drawRect(QRect(QPoint(0, 0), size()));
    painter.restore();
    //观察者坐标
    painter.setWindow(QRect(QPoint(0, 0), Global::RenderSize));
    painter.setViewport(QRect(QPoint(0, 0), size()));
    painter.save();
    painter.translate(QPointF(Global::RenderSize.width() / 2, Global::RenderSize.height() / 2) - Global::CameraPos);
    //绘制场景
    scene.drawScene(&painter);
    if(isDrawBoundingBox) scene.drawAABB(&painter);
    //更新场景
    scene.update();
    //显示帧数
    painter.restore();
    painter.setPen(Qt::red);
    painter.drawText(QRect(0, 0, 100, 50), QString("FPS: %1").arg(fps));
    //计算渲染所需毫秒数
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    Global::elapsed = elapsed.count();
    //重绘
    update();
}

#if !defined(ANDROID)
void GameWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->isAutoRepeat()) return;
    //qDebug() << "GameWidget::keyPressEvent(" << e->key() << ")" << Qt::endl;
    switch(e->key())
    {
    case Qt::Key::Key_W: Global::KeyPressed.W = true;
        break;
    case Qt::Key::Key_S: Global::KeyPressed.S = true;
        break;
    case Qt::Key::Key_A: Global::KeyPressed.A = true;
        break;
    case Qt::Key::Key_D: Global::KeyPressed.D = true;
        break;
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent *e)
{
    if(e->isAutoRepeat()) return;
    //qDebug() << "GameWidget::keyReleaseEvent(" << e->key() << ")" << Qt::endl;
    switch(e->key())
    {
    case Qt::Key::Key_W: Global::KeyPressed.W = false;
        break;
    case Qt::Key::Key_S: Global::KeyPressed.S = false;
        break;
    case Qt::Key::Key_A: Global::KeyPressed.A = false;
        break;
    case Qt::Key::Key_D: Global::KeyPressed.D = false;
        break;
    }
}

void GameWidget::mousePressEvent(QMouseEvent *e)
{
    //qDebug() << "GameWidget::mousePressEvent(" << e->pos() << ")" << Qt::endl;
    if(e->buttons() == Qt::LeftButton)
    {
        Global::MouseState.leftMouseClickedCount ++;
    }
    else if(e->button() == Qt::RightButton)
    {
        isDrawBoundingBox = !isDrawBoundingBox;
    }
}

void GameWidget::mouseMoveEvent(QMouseEvent *e)
{
    //qDebug() << "GameWidget::mouseMoveEvent(" << e->pos() << ")" << Qt::endl;
    Global::MouseState.pos = e->pos();
}

#endif

void GameWidget::resizeEvent(QResizeEvent *e)
{
    Global::GameWindowSize = e->size();
}


