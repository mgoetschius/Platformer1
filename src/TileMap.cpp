#include "TileMap.h"
#include "Model.h"
#include "Game.h"

#include<iostream>
#include<fstream>
#include<algorithm>

TileMap::TileMap()
{
    //ctor
}

void TileMap::Setup(Shader &shader)
{
    texture.Setup("res/textures/level1.png");
    SetupTexCoords();
    LoadFromFile(std::string("res/maps/map.txt"));
    BuildVertices();
    mesh.Setup(vertices, indices, texture);

    transUniform = glGetUniformLocation(shader.program, "transMatrix");

    translation = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0, 0.0, 1.0);
    scale = glm::vec3(1.0,1.0,1.0);
    rotationAmount = 0;
    transMatrix = glm::translate(transMatrix, translation);
    transMatrix = glm::rotate(transMatrix, glm::radians(0.0f),  rotation);
    transMatrix = glm::scale(transMatrix, scale);

}

void TileMap::Update()
{
    mesh.update();
}

void TileMap::Render()
{
    glUniformMatrix4fv(transUniform, 1, GL_FALSE, glm::value_ptr(transMatrix));
    mesh.render();
}

bool TileMap::GetTileCollision(int x, int y)
{
    if(x < 0 || x >= tileIndices[0].size() ||
       y < 0 || y >= tileIndices.size())
    {
        return true;
    }
    if (tileIndices[y][x] == "0")
        return false;
    else
        return true;
}

void TileMap::LoadFromFile(std::string filename)
{
    std::ifstream in(filename, std::ios::in);

    if(!in)
        std::cerr << "Error: unable to open file" << std::endl;

    std::string line;
    while(std::getline(in, line))
    {
        tileIndices.push_back(Split(line, ',', false));
    }
}

bool isEmptyString(std::string s) {return s.length() == 0;}
std::vector<std::string> TileMap::Split(std::string& s, char c, bool removeEmpties)
{

    std::vector<std::string> vstr;
    std::string::size_type i = 0;
    std::string::size_type j = s.find(c);
    while(j != std::string::npos)
    {
        vstr.push_back(s.substr(i, j-i));
        i = ++j;
        j = s.find(c, j);

        if (j == std::string::npos)
         vstr.push_back(s.substr(i, s.length()));
    }
    if(removeEmpties)
        vstr.erase(std::remove_if(vstr.begin(), vstr.end(), isEmptyString), vstr.end());

    return vstr;
}

void TileMap::BuildVertices()
{
    int indicesPlacer = 0;
    for(unsigned int y = 0; y < tileIndices.size(); y++)
    {
        for(unsigned int x = 0; x < tileIndices[0].size(); x++)
        {
            if(tileIndices[y][x] != "0")
            {
                Vertex v1(glm::vec3(x * Game::tileSize, y * Game::tileSize, 0.0f), glm::vec2(texCoords[atoi(tileIndices[y][x].c_str())][0], texCoords[atoi(tileIndices[y][x].c_str())][1]));
                Vertex v2(glm::vec3(x * Game::tileSize + Game::tileSize, y * Game::tileSize, 0.0f), glm::vec2(texCoords[atoi(tileIndices[y][x].c_str())][2], texCoords[atoi(tileIndices[y][x].c_str())][3]));
                Vertex v3(glm::vec3(x * Game::tileSize + Game::tileSize, y * Game::tileSize + Game::tileSize, 0.0f), glm::vec2(texCoords[atoi(tileIndices[y][x].c_str())][4], texCoords[atoi(tileIndices[y][x].c_str())][5]));
                Vertex v4(glm::vec3(x * Game::tileSize, y * Game::tileSize + Game::tileSize, 0.0f), glm::vec2(texCoords[atoi(tileIndices[y][x].c_str())][6], texCoords[atoi(tileIndices[y][x].c_str())][7]));
                vertices.push_back(v1);
                vertices.push_back(v2);
                vertices.push_back(v3);
                vertices.push_back(v4);
                indices.push_back(indicesPlacer++);
                indices.push_back(indicesPlacer++);
                indices.push_back(indicesPlacer);
                indices.push_back(indicesPlacer-2);
                indices.push_back(indicesPlacer);
                indices.push_back(++indicesPlacer);
                indicesPlacer++;
            }
        }
    }
}

void TileMap::SetupTexCoords()
{
    /******************************************************
    /// this function builds a nested vector textCoords that holds the 8 texture Coords for each
    /// square in a grid on the loaded texture for this TileMap.  Coords are stored in order u,v,u,v,u,v,u,v
    /// starting with the top left vertex and moving around the square clockwise.
    /// access the coords using
    /// texCoords[y][x]
    /// where y is the texture number--top left square on the texture is 0, should correspond to tilemap text file
    /// x is the u/v index.  ie. top left u -- x = 0
    ********************************************************/

    std::vector<float> row;
    float coords[8];
    int totalAcross = texture.GetWidth() / Game::tileSize;
    int totalDown = texture.GetHeight() / Game::tileSize;

    for(int y = 0; y < totalDown; y++)
    {
        for(int x = 0; x < totalAcross; x++)
        {
            coords[0] = x * 1.0 / totalAcross;
            coords[1] = y * 1.0 / totalDown;
            coords[2] = coords[0] + 1.0 / totalAcross;
            coords[3] = coords[1];
            coords[4] = coords[2];
            coords[5] = coords[1] + 1.0 / totalDown;
            coords[6] = coords[0];
            coords[7] = coords[5];

            row.clear();
            for(int i = 0; i < 8; i++)
            {
                row.push_back(coords[i]);
            }
            texCoords.push_back(row);
        }
    }
}

TileMap::~TileMap()
{
    //dtor
}
