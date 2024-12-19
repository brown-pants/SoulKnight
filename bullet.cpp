#include "bullet.h"
#include "global.h"

Bullet::Images Bullet::ak47Bullet_imgs;
Bullet::Images Bullet::EnemyBullet1_imgs;
Bullet::Images Bullet::EnemyBullet2_imgs;
bool Bullet::isLoadImages = false;

void Bullet::LoadImages()
{
    ak47Bullet_imgs = Bullet::Images(QImage(":/res/images/weapon/ak47_bullet.png"));
    EnemyBullet1_imgs = Bullet::Images(QImage(":/res/images/enemy/Goblin/bullet_18.png"));
    EnemyBullet2_imgs = Bullet::Images(QImage(":/res/images/enemy/Goblin/bullet_21.png"));
}

Bullet::Bullet(Images &images, const QPointF centerPos, const QPointF moveDir, const double &moveSpeed, int attack)
    : centerPos(centerPos), moveDir(moveDir), moveSpeed(moveSpeed), attack(attack)
{
    setUpRotate(images, QVector2D(moveDir.x(), moveDir.y()));
    updateAABB();
    if(!isLoadImages)
    {
        LoadImages();
        isLoadImages = true;
    }
}

void Bullet::setUpRotate(Bullet::Images &imgs, const QVector2D &dir)
{
    //计算角度
    QVector2D x_axis(1.0f, 0.0f);
    if(dir.x() < 0)
    {
        x_axis.setX(-1.0f);
        img = &imgs.left;
    }
    else{
        img = &imgs.right;
    }

    float dot = QVector2D::dotProduct(dir, x_axis);
    float cos_angle = dot;
    float angle = acos(cos_angle) / 3.1415927 * 180.0;

    if(dir.x() > 0 && dir.y() < 0)
    {
        angle *= -1.0;
    }
    if(dir.x() < 0 && dir.y() > 0)
    {
        angle *= -1.0;
    }
    this->rotateAngle = angle;
}

bool Bullet::update(const QVector<AABB *> &rooms_aabb, const QVector<AABB *> &obstacles_aabb)
{
    double speed = moveSpeed * Global::elapsed;
    centerPos += moveDir * speed;
    updateAABB();
    bool inside = false;
    for(auto aabb : rooms_aabb)
    {
        if(this->inside(*aabb))
        {
            inside = true;
            break;
        }
    }
    for(auto aabb : obstacles_aabb)
    {
        if(this->collision(*aabb))
        {
            inside = false;
            break;
        }
    }
    return inside;
}

void Bullet::updateAABB()
{
    AABB::setPos(centerPos - QPoint(img->size().width() / 2.0 - 6.0, img->size().height() / 2.0 - 10));
    AABB::setSize(QSize(6, 10));
}

void Bullet::draw(QPainter *painter)
{
    painter->save();
    painter->translate(centerPos + QPointF(0, 10));
    painter->rotate(rotateAngle);
    painter->drawPixmap(-img->size().width() / 2.0, -img->size().height() / 2.0, QPixmap::fromImage(*img));
    painter->restore();
}

BulletsManager::BulletsManager()
{

}

void BulletsManager::AddBullet(const Bullet &bullet)
{
    bullets.push_back(bullet);
}

void BulletsManager::Update(const QVector<AABB *> &rooms_aabb, const QVector<AABB *> &obstacles_aabb)
{
    for(auto bullet_iter = bullets.begin(); bullet_iter != bullets.end(); )
    {
        if(!bullet_iter->update(rooms_aabb, obstacles_aabb))
        {
            bullet_iter = bullets.erase(bullet_iter);
        }
        else
        {
            bullet_iter++;
        }
    }
}

void BulletsManager::Draw(QPainter *painter)
{
    for(auto bullet : bullets)
    {
        bullet.draw(painter);
    }
}

void BulletsManager::DrawBoundingBoxes(QPainter *painter, Qt::GlobalColor color)
{
    for(auto bullet : bullets)
    {
        bullet.drawBoundingBox(painter, color);
    }
}

int BulletsManager::hit(const AABB &aabb)
{
    int attack = 0;
    for(auto bullet_iter = bullets.begin(); bullet_iter != bullets.end(); )
    {
        if(bullet_iter->collision(aabb))
        {
            attack += bullet_iter->attack;
            bullet_iter = bullets.erase(bullet_iter);
        }
        else
        {
            bullet_iter++;
        }
    }
    return attack;
}
