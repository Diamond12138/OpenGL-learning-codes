#version 460 core

out vec4 o_color;

layout(location=4)uniform vec4 u_color;

void main()
{
    o_color = u_color;
}