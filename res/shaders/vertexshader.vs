#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 transMatrix;
uniform mat4 projMatrix;

out vec2 TexCoord;

void main()
{
    gl_Position = projMatrix * transMatrix * vec4(position.x, position.y, position.z, 1.0);
	TexCoord = texCoord;
}
