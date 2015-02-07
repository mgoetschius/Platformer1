#ifndef TILEMAP_H
#define TILEMAP_H
#include <string>
#include <vector>
#include "Shader.h"
#include "Model.h"



class TileMap
{
    public:
        TileMap();
        void Setup(Shader &shader);
        void Update();
        void Render();
        virtual ~TileMap();
        int GetMapHeight() {return tiles.size();};
        int GetMapWidth() {return tiles[0].size();};
        bool GetTileCollision(int x, int y);
    protected:
    private:
        void LoadFromFile(std::string filename);
        std::vector<std::string> Split(std::string& s, char c, bool removeEmpties);

        std::vector<std::vector<std::string>> tileIndices;
        std::vector<std::vector<Model>> tiles;
};

#endif // TILEMAP_H
