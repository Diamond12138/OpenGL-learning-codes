#version 460 core

layout(location=0)out float o_color;

void main()
{
    //gl_FragDepth = gl_FragCoord.z;
    o_color = gl_FragCoord.z;
}