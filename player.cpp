#include "player.h"
#include "global.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QVector2D>

#if defined(ANDROID)
#include "AndroidGui/androidslider.h"
#endif

Player::Images Player::imgs_1, Player::imgs_2, Player::imgs_3;
bool Player::isLoadImgs = false;

Player::Player(QObject *parent)
    : QObject{parent}, isLeftImage(false), runSpeed(0.3), isDied(false), health(100), curHealth(health), beHurt(true)
{
    if(!isLoadImgs)
    {
        LoadImages();
        isLoadImgs = true;
    }
    changeImages(1);
    initAnimations();
    QPointF center((Global::RenderSize / 2).width() - imgSize.width() / 2, (Global::RenderSize / 2).height() - imgSize.height() / 2);
    position = center;
    updateCamera();
    updateAABB();
    //受伤间隔
    connect(&hurt_timer, &QTimer::timeout, [this](){
        beHurt = true;
    });
    hurt_timer.setSingleShot(true);
    hurt_timer.setInterval(200);

#if defined(ANDROID)
    canShoot = false;
    connect(&shoot_timer, &QTimer::timeout, [this]() {
        canShoot = true;
    });
    shoot_timer.setInterval(50);
    shoot_timer.start();
#endif
}

void Player::changeImages(int i)
{
    if(i == 1)
    {
        imgs = &imgs_1;
        imgSize = QSize(32, 32);
    }
    else if(i == 2)
    {
        imgs = &imgs_2;
        imgSize = QSize(32, 32);
    }
    else if(i == 3)
    {
        imgs = &imgs_3;
        imgSize = QSize(25, 32);
    }
    else qDebug() << "只有三个角色" << Qt::endl;
}

void Player::LoadImages()
{
    LoadImgs_1();
    LoadImgs_2();
    LoadImgs_3();
}

void Player::draw(QPainter *painter)
{
    painter->drawPixmap(position, QPixmap::fromImage(*curImg));
    bulletsManager.Draw(painter);
    if(!isDied)
    {
        weapon.draw(painter, getCenterPos());
        //画血条
        int healthWidth = 32;
        int xpos = position.x() - (healthWidth - curImg->size().width()) / 2;
        painter->save();
        painter->setBrush(QBrush(Qt::gray));
        painter->drawRect(xpos, position.y(), healthWidth, 3);
        painter->setBrush(QBrush(Qt::red));
        painter->drawRect(xpos, position.y(), healthWidth * (curHealth / (float)health), 3);
        painter->restore();
    }
}

void Player::draw(QPainter *painter, const QPointF &pos, const QSize &size)
{
    painter->drawPixmap(pos, QPixmap::fromImage(curImg->scaled(size)));
}

void Player::update(const QVector<AABB *> &rooms_aabb, const QVector<AABB *> &enemies_aabb, const QVector<AABB *> &obstacles_aabb, BulletsManager &playerBulletsManager)
{
    if(!isDied)
    {
        playerMoved(rooms_aabb, obstacles_aabb);
        hurtByEnemies(enemies_aabb);
        shoot();
        dieTest();
    }
    bulletsManager.Update(rooms_aabb, obstacles_aabb);
    //中弹
    int attack = playerBulletsManager.hit(*this);
    curHealth -= attack;
}

void Player::shoot()
{
#if defined(ANDROID)
    if(Global::Touched && canShoot)
    {
        QVector2D shootDir = weapon.getShootDir();
        QPointF offset = (shootDir * 8).toPointF();
        bulletsManager.AddBullet(Bullet(Bullet::ak47Bullet_imgs, getCenterPos() + offset, shootDir.toPointF(), 1.5, 2));
        canShoot = false;
        shoot_timer.start();
    }
#else
    if(Global::MouseState.leftMouseClickedCount)
    {
        QVector2D shootDir = weapon.getShootDir();
        QPointF offset = (shootDir * 8).toPointF();
        bulletsManager.AddBullet(Bullet(Bullet::ak47Bullet_imgs, getCenterPos() + offset, shootDir.toPointF(), 1.5, 2));
        Global::MouseState.leftMouseClickedCount--;
    }
#endif
}

void Player::hurtByEnemies(const QVector<AABB *> &enemies_aabb)
{
    bool isStart = false;
    if(beHurt)
    {
        for(auto aabb : enemies_aabb)
        {
            if(aabb->collision(*this))
            {
                curHealth -= 10;
                isStart = true;
            }
        }
    }
    if(isStart)
    {
        beHurt = false;
        hurt_timer.start();
    }
}

void Player::dieTest()
{
    if(curHealth < 0)
    {
        isDied = true;
        curAnim->stop();
        curImg = isLeftImage ? &imgs->die_left : &imgs->die_right;
        setZValue(position.y() + curImg->height());
    }
}

void Player::LoadImgs_1()
{
    QImage idle_images(":/res/images/player/1/Idle_Sheet.png");
    QImage run_images(":/res/images/player/1/Run_Sheet.png");
    QImage die_image(":/res/images/player/1/c01_8.png");
    for(int i = 0; i < 4; i++)
    {
        imgs_1.idle_right[i] = idle_images.copy(i * 32, 0, 32, 32);
        imgs_1.idle_left[i] = imgs_1.idle_right[i].mirrored(true, false);
    }
    run_images = run_images.copy(12, 0, 256 - 12, 32);
    for(int i = 0; i < 4; i++)
    {
        imgs_1.run_right[i] = run_images.copy(i * 64, 0, 32, 32);
        imgs_1.run_left[i] = imgs_1.run_right[i].mirrored(true, false);
    }
    imgs_1.die_right = die_image;
    imgs_1.die_left = imgs_1.die_right.mirrored(true, false);
}

void Player::LoadImgs_2()
{
    QImage idle_images(":/res/images/player/2/Idle-Sheet.png");
    QImage run_images(":/res/images/player/2/Run-Sheet.png");
    QImage die_image(":/res/images/player/2/Die.png");
    idle_images = idle_images.copy(12, 16, 256 - 12, 32);
    for(int i = 0; i < 4; i++)
    {
        imgs_2.idle_right[i] = idle_images.copy(i * 64, 0, 32, 32);
        imgs_2.idle_left[i] = imgs_2.idle_right[i].mirrored(true, false);
    }
    run_images = run_images.copy(12, 16, 256 - 12, 32);
    for(int i = 0; i < 4; i++)
    {
        imgs_2.run_right[i] = run_images.copy(i * 64, 0, 32, 32);
        imgs_2.run_left[i] = imgs_2.run_right[i].mirrored(true, false);
    }
    imgs_2.die_right = die_image;
    imgs_2.die_left = imgs_2.die_right.mirrored(true, false);
}

void Player::LoadImgs_3()
{
    QImage idle_image_1(":/res/images/player/3/c01_s3_4.png");
    QImage idle_image_2(":/res/images/player/3/c01_s3_5.png");
    QImage idle_image_3(":/res/images/player/3/c01_s3_6.png");
    QImage idle_image_4(":/res/images/player/3/c01_s3_7.png");
    QImage run_image_1(":/res/images/player/3/c01_s3_0.png");
    QImage run_image_2(":/res/images/player/3/c01_s3_1.png");
    QImage run_image_3(":/res/images/player/3/c01_s3_2.png");
    QImage run_image_4(":/res/images/player/3/c01_s3_3.png");
    QImage die_image(":/res/images/player/3/c01_s3_8.png");
    imgs_3.idle_right[0] = idle_image_1.copy(2, 0, 25, 32);
    imgs_3.idle_right[1] = idle_image_2.copy(1, 1, 25, 32);
    imgs_3.idle_right[2] = idle_image_3.copy(2, 1, 25, 32);
    imgs_3.idle_right[3] = idle_image_4.copy(2, 0, 25, 32);
    for(int i = 0; i < 4; i++)
    {
        imgs_3.idle_left[i] = imgs_3.idle_right[i].mirrored(true, false);
    }
    imgs_3.run_right[0] = run_image_1;
    imgs_3.run_right[1] = run_image_2;
    imgs_3.run_right[2] = run_image_3;
    imgs_3.run_right[3] = run_image_4;
    for(int i = 0; i < 4; i++)
    {
        imgs_3.run_left[i] = imgs_3.run_right[i].mirrored(true, false);
    }
    imgs_3.die_right = die_image;
    imgs_3.die_left = imgs_3.die_right.mirrored(true, false);
}

void Player::initAnimations()
{
    connect(&idle_anim_timer, &QTimer::timeout, [this](){
        static int index = 0;
        if(++index == 4) index = 0;
        if(isLeftImage) curImg = &imgs->idle_left[index];
        else curImg = &imgs->idle_right[index];
        idle_anim_timer.setInterval(120);//重新设置时延
    });
    connect(&run_anim_timer, &QTimer::timeout, [this](){
        static int index = 0;
        if(++index == 4) index = 0;
        if(isLeftImage) curImg = &imgs->run_left[index];
        else curImg = &imgs->run_right[index];
        run_anim_timer.setInterval(120);//重新设置时延
    });
    curAnim = nullptr;
    showIdleAnim();
}

void Player::showIdleAnim()
{
    if(curAnim != &idle_anim_timer)
    {
        if(curAnim != nullptr) curAnim->stop();
        idle_anim_timer.start(1);//立刻进入动画
        curAnim = &idle_anim_timer;
    }
}

void Player::showRunAnim()
{
    if(curAnim != &run_anim_timer)
    {
        if(curAnim != nullptr) curAnim->stop();
        run_anim_timer.start(1);//立刻进入动画
        curAnim = &run_anim_timer;
    }
}

void Player::playerMoved(const QVector<AABB *> &rooms_aabb, const QVector<AABB *> &obstacles_aabb)
{
#if defined(ANDROID)
    QPointF move_dir = AndroidSlider::getDir().toPointF();
#else
    int x = 0, y = 0;
    if(Global::KeyPressed.W)
    {
        y --;
    }
    if(Global::KeyPressed.S)
    {
        y ++;
    }
    if(Global::KeyPressed.A)
    {
        x --;
    }
    if(Global::KeyPressed.D)
    {
        x ++;
    }
    QPointF move_dir(x, y);
#endif
    //方向
    QVector2D shootDir = Weapon::getShootDir();
    if(shootDir.x() > 0) isLeftImage = false;
    else isLeftImage = true;

    //移动动画
    double move_speed = runSpeed * Global::elapsed;
    if(move_dir.x() != 0 || move_dir.y() != 0)
    {
        showRunAnim();
    }
    else{
        showIdleAnim();
    }
    QVector2D dir_vec2d(move_dir.x(), move_dir.y());
    dir_vec2d.normalize();

    //包围盒
    QPointF dirOff = dir_vec2d.toPointF() * move_speed;
    QPointF offset(0.0, 0.0);
    //x轴检测
    bool inside = false;
    AABB aabb_x;
    aabb_x.setPos(position + QPointF(5.0, 15.0) + QPointF(dirOff.x(), 0));
    aabb_x.setSize(AABB::getRect().size());
    for(auto aabb : rooms_aabb)
    {
        if(aabb_x.inside(*aabb))
        {
            inside = true;
            break;
        }
    }
    for(auto aabb : obstacles_aabb)
    {
        if(aabb_x.collision(*aabb))
        {
            inside = false;
            break;
        }
    }
    if(inside)
    {
        offset.setX(dirOff.x());
    }
    //y轴检测
    inside = false;
    AABB aabb_y;
    aabb_y.setPos(position + QPointF(5.0, 15.0) + QPointF(0, dirOff.y()));
    aabb_y.setSize(AABB::getRect().size());
    for(auto aabb : rooms_aabb)
    {
        if(aabb_y.inside(*aabb))
        {
            inside = true;
            break;
        }
    }
    for(auto aabb : obstacles_aabb)
    {
        if(aabb_y.collision(*aabb))
        {
            inside = false;
            break;
        }
    }
    if(inside)
    {
        offset.setY(dirOff.y());
    }

    position += offset;
    setZValue(position.y() + curImg->height());

    //更新摄像机与包围盒
    updateCamera();
    updateAABB();
}

void Player::updateCamera()
{
    Global::CameraPos = position + QPointF(imgSize.width() / 2, imgSize.height() / 2);
}

void Player::updateAABB()
{
    AABB::setPos(position + QPointF(5.0, 15.0));
    AABB::setSize(QSizeF(imgSize.width() - 10, imgSize.height() - 15));
}
