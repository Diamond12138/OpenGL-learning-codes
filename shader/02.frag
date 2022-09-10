#version 460 core

in vec2 v_texCoord;
out vec4 o_color;
uniform sampler2D u_texture;

void main()
{
    o_color = texture(u_texture,v_texCoord);
    //o_color = vec4(0.0,0.0,1.0,1.0);
}