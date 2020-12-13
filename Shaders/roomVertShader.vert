#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 mvp;

out vec2 TexCoord;
out vec3 Position;
out vec3 Normal;
out mat3 TBN;

void main()
{
    gl_Position = mvp * vec4(position, 1.0f);
    TexCoord = texCoord;
    Position = position;
    Normal = normal;
    TBN = mat3(tangent, bitangent, normal);
}