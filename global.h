#ifndef GLOBAL_H
#define GLOBAL_H
#include <QSize>
#include <QPointF>

//全局静态成员类
class Global
{
public:
    static QSize RenderSize;//游戏投影尺寸
    static QSize GameWindowSize;//游戏窗口当前尺寸
    static double elapsed;  //渲染一帧需要的时间以毫秒为单位
    static struct Key{
        bool W = false, S = false, A = false, D = false;
    }KeyPressed;
    static struct Mouse{
        QPoint pos;
        unsigned int leftMouseClickedCount = 0; //左键被点击的次数
    }MouseState;
    static QPointF CameraPos;

    static bool Touched;

private:
    Global();
};

#endif // GLOBAL_H
