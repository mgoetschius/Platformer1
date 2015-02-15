#ifndef ORANGEENEMY_H
#define ORANGEENEMY_H

#include <Enemy.h>

class TileMap;

class OrangeEnemy : public Enemy
{
    public:
        OrangeEnemy(Shader &shader, const char *filename, float x = 650, float y = 400);
        void Update(TileMap *tileMap, double dt);
        virtual ~OrangeEnemy();
    protected:
    private:
};

#endif // ORANGEENEMY_H
