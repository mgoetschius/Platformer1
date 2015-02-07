#ifndef TEXTURECHANGEMESH_H
#define TEXTURECHANGEMESH_H

#include <GL/glew.h>
#include "Texture.h"

class TextureChangeMesh
{
    public:
        TextureChangeMesh();
        void Setup(GLfloat verts[], GLfloat tex[], GLuint inds[], Texture &text);
        void render();
        virtual ~TextureChangeMesh();
    protected:
    private:
        GLuint vao, vbo, tbo, ibo;
        int indicesCount;
        int size;
        Texture texture;
};

#endif // TEXTURECHANGEMESH_H
