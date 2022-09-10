#version 460 core

layout(location=0)in vec3 a_position;

layout(location=0)uniform mat4 u_projection;
layout(location=1)uniform mat4 u_view;
layout(location=2)uniform mat4 u_model;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_position,1.0);
}