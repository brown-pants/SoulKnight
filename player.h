#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QImage>
#include <QTimer>
#include "aabb.h"
#include "weapon.h"
#include "Entity.h"
#include "bullet.h"

class Player : public QObject, public AABB, public Entity
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    //切换角色贴图
    void changeImages(int i);
    //绘制函数
    void draw(QPainter *painter) override;//根据人物坐标绘制    重写 virual Entity::void draw(QPainter *painter);
    void draw(QPainter *painter, const QPointF &pos, const QSize &size);//根据位置参数与尺寸参数绘制
    //更新角色
    void update(const QVector<AABB *> &rooms_aabb, const QVector<AABB *> &enemies_aabb, const QVector<AABB *> &obstacles_aabb, BulletsManager &playerBulletsManager);
    //设置位置
    inline void setPosition(const QPointF &pos) { this->position = pos; }
    //获取中心位置
    inline QPointF getCenterPos() const { return position + QPointF(imgSize.width() / 2.0, imgSize.height() / 2.0); }
    //获取子弹管理器
    inline BulletsManager &getBulletsManager() { return bulletsManager; }

private:
    struct Images{
        QImage idle_left[4];
        QImage idle_right[4];
        QImage run_left[4];
        QImage run_right[4];
        QImage die_left;
        QImage die_right;
    };
    Images *imgs;
    QImage *curImg;
    bool isLeftImage;   //为true: 人物面向左侧; 为false: 人物面向右侧
    QSize imgSize;  //图片尺寸

    //三个主角贴图
    static Images imgs_1, imgs_2, imgs_3;
    static void LoadImgs_1();
    static void LoadImgs_2();
    static void LoadImgs_3();
    static void LoadImages();
    static bool isLoadImgs;

    //动画设置
    QTimer idle_anim_timer; //静止动画
    QTimer run_anim_timer;  //跑步动画
    QTimer *curAnim;       //当前动画
    void initAnimations();
    void showIdleAnim();
    void showRunAnim();

    //绝对位置
    QPointF position;
    double runSpeed; //移动速度(/ms)
    void playerMoved(const QVector<AABB *> &rooms_aabb, const QVector<AABB *> &obstacles_aabb); //更新位置

    //更新摄像机与包围盒
    void updateCamera();
    void updateAABB();

    //武器
    Weapon weapon;
    BulletsManager bulletsManager;
    void shoot();

    //生命值
    bool isDied;
    int health;
    int curHealth;

    //受伤
    void hurtByEnemies(const QVector<AABB *> &enemies_aabb);
    QTimer hurt_timer;
    bool beHurt;
    void dieTest();

#if defined(ANDROID)
    QTimer shoot_timer;
    bool canShoot;
#endif
signals:
};

#endif // PLAYER_H
