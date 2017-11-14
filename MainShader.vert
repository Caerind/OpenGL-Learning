#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vUV;
layout (location = 2) in vec3 vNormal;

uniform mat4 MV;
uniform mat3 N;
uniform mat4 MVP;

out vec3 Position;
out vec2 UV;
out vec3 Normal;

void main()
{
    Position = (MV * vec4(vPos, 1.0)).xyz;
    UV = vUV;
    Normal = normalize(N * vNormal);

    gl_Position = MVP * vec4(vPos, 1.0);
}