#ifndef TILEMAP_H
#define TILEMAP_H
#include <string>
#include <vector>
#include "Shader.h"
#include "Model.h"
#include "Texture.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Enemy.h"
#include "OrangeEnemy.h"
#include "PurpleEnemy.h"
#include "Key.h"
#include "Door.h"


class TileMap
{
    public:
        TileMap();
        void Setup(Shader &shader, string levelNum);
        void Update(double dt);
        void Render();
        void CleanUp();
        virtual ~TileMap();
        int GetMapHeight() {return tileIndices.size();};
        int GetMapWidth() {return tileIndices[0].size();};
        void SetHasKey(bool value) {hasKey = value;};
        bool GetTileCollision(int x, int y);
        int GetTileInt(int x, int y);
        bool GetKeyCollision(int x, int y);
        bool GetDoorCollision(int x, int y);
        bool GetLadderCollision(int x, int y);
        void UpdateDoor() {door.update();};
        std::vector<Enemy*>* GetEnemies() {return &enemies;};
    protected:
    private:
        Texture texture;
        Mesh mesh;
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 transMatrix;
        float rotationAmount;
        GLuint transUniform;
        void LoadFromFile(std::string filename);
        void SetupTexCoords();
        void BuildVertices(Shader &shader);
        std::vector<std::string> Split(std::string& s, char c, bool removeEmpties);

        std::vector<std::vector<std::string>> tileIndices;
        std::vector<std::vector<int>>tileInts;
        std::vector<std::vector<Model>> tiles;
        std::vector<vector<float>> texCoords;
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Enemy *> enemies;

        Key key;
        bool hasKey = false;
        Door door;

};

#endif // TILEMAP_H
