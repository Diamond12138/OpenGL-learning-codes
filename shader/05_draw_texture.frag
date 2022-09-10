#version 460 core

in vec2 v_texCoord;

out vec4 o_color;

uniform sampler2D u_texture;

void main()
{
    float d = texture(u_texture,v_texCoord).r ;
    o_color = vec4(d,d,d,1.0);
}