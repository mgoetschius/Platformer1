#include "Texture.h"
#include "soil.h"
#include <GL/glew.h>
#include <iostream>

Texture::Texture()
{
    //ctor
}

Texture::Texture(const char * filepath)
{
    texID = loadTexture(filepath);
}

void Texture::Setup(const char * filepath)
{
    texID = loadTexture(filepath);
}

GLuint Texture::loadTexture(const char * filepath)
{
    GLuint ID;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    unsigned char* image = SOIL_load_image(filepath, &width, &height, 0, SOIL_LOAD_RGBA);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);

    return ID;
}

std::vector<std::vector<float>> Texture::SetupTexCoords(int tileSize)
{
    /******************************************************
    /// this function builds a nested vector textCoords that holds the 8 texture Coords for each
    /// square in a grid on the loaded texture for this TileMap.  Coords are stored in order u,v,u,v,u,v,u,v
    /// starting with the top left vertex and moving around the square clockwise in world coordinates.
    /// access the coords using
    /// texCoords[y][x]
    /// where y is the texture number--top left square on the texture is 0, should correspond to tilemap text file
    /// x is the u/v index.  ie. top left u -- x = 0
    ********************************************************/

    std::vector<std::vector<float>> texCoords;
    std::vector<float> row;
    float coords[8];
    int totalAcross = width / tileSize;
    int totalDown = height / tileSize;

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
    return texCoords;
}

Texture::~Texture()
{

}
