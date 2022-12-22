#version 460 core

in vec2 v_texCoord;
out vec4 o_color;

uniform sampler2D u_texture;
uniform vec4 u_color;

void main()
{
    o_color = texture(u_texture,v_texCoord).r * u_color;
}