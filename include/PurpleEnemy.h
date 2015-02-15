#ifndef PURPLEENEMY_H
#define PURPLEENEMY_H

#include <Enemy.h>

class TileMap;

class PurpleEnemy : public Enemy
{
    public:
        PurpleEnemy(Shader &shader, const char *filename, float x = 650, float y = 400);
        void Update(TileMap *tileMap, double dt);
        virtual ~PurpleEnemy();
    protected:
    private:
};

#endif // PURPLEENEMY_H
