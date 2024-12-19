#ifndef BOAR_H
#define BOAR_H
#include "enemy.h"

class Player;
class Boar : public Enemy
{
public:
    Boar(Player *player, const QPointF &position);

protected:
    QVector2D getMoveDir() override;
    void updateAABB() override;

private:
    static Images BoarImgs;
    static Images DireBoarImgs;
    static bool isLoadImgs;
    static void LoadImages();

    QSize imgSize;
    Player *player;

    int randomDir;
};

#endif // BOAR_H
