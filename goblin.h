#ifndef GOBLIN_H
#define GOBLIN_H

#include "player.h"
#include "enemy.h"
#include "bullet.h"

class Goblin : public Enemy
{
public:
    Goblin(Player *player, const QPointF &position);

protected:
    QVector2D getMoveDir() override;
    void updateAABB() override;
    QVector<Bullet> getBullet() override;

private:
    static Images GoblinImgs;
    static Images EliteGoblinImgs;
    QSize imgSize;
    static bool isLoadImgs;
    static void LoadImages();

    Player *player;
    bool isElite;

signals:
};

#endif // GOBLIN_H
