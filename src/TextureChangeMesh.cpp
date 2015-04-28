#include "TextureChangeMesh.h"
#include <iostream>

TextureChangeMesh::TextureChangeMesh()
{
    //ctor
}

void TextureChangeMesh::Setup(GLfloat verts[], GLfloat tex[], GLuint inds[], Texture *text)
{
    //vertCoords = verts;
    //texCoords = tex;
    //indices = inds;
    texture = text;
    size = sizeof(inds)/sizeof(inds[0]);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glBindVertexArray(0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void TextureChangeMesh::render()
{
    glBindVertexArray(vao);
    //make sure soil is loading RGBA image
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTexture());
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, 2*3);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

TextureChangeMesh::~TextureChangeMesh()
{
    //dtor
}
