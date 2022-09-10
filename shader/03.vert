#version 460 core

layout(location=0)in vec3 a_position;
layout(location=2)in vec3 a_normal;
layout(location=3)in vec2 a_texCoord;

out vec3 v_normal;
out vec2 v_texCoord;

void main()
{
    gl_Position = vec4(a_position,1.0);
    v_normal = normalize(a_normal);
    v_texCoord = a_texCoord;
}