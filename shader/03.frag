#version 460 core

in vec3 v_normal;
in vec2 v_texCoord;

out vec4 o_color;

uniform sampler2D u_texture;
uniform float u_angle;

void main()
{
    const vec3 light_direction = normalize(vec3(cos(u_angle),sin(u_angle),0.0));
    const float ambient = 0.1;
    float diffuse = max(dot(-light_direction,v_normal),0.0);
    o_color = texture(u_texture,v_texCoord) * (ambient + diffuse);
    //o_color = vec4(0.0,0.0,1.0,1.0);
}