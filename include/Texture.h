#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

class Texture
{
    public:
        Texture();
        void Setup(const char * filepath);
        GLuint loadTexture(const char * filepath);
        GLuint GetTexture() { return texID; };
        int GetWidth() {return width;};
        int GetHeight() {return height;};
        virtual ~Texture();
    protected:
    private:
        int width, height;
        GLuint texID;
};

#endif // TEXTURE_H
