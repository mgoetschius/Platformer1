#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <vector>


class Texture
{
    public:
        Texture();
        Texture(const char * filepath);
        void Setup(const char * filepath);
        GLuint loadTexture(const char * filepath);
        GLuint GetTexture() { return texID; };
        std::vector<std::vector<float>> SetupTexCoords(int tileSize);
        int GetWidth() {return width;};
        int GetHeight() {return height;};
        virtual ~Texture();
    protected:
    private:
        int width, height;
        GLuint texID;
};

#endif // TEXTURE_H
