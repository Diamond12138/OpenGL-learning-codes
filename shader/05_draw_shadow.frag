#version 460 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texCoord;

out vec4 o_color;

layout(location=5)uniform bool u_enable_texture;
layout(location=6)uniform vec4 u_color;
layout(location=7)uniform sampler2D u_shadow;
layout(location=8)uniform sampler2D u_texture;
layout(location=9)uniform mat4 u_light_projection;
layout(location=10)uniform mat4 u_light_view;
layout(location=2)uniform mat4 u_model;

void main()
{
    const vec3 light_direction = normalize(vec3(-2.0,-4.0,-6.0));
    const float ambient = 0.1;

    vec4 color;
    if(u_enable_texture)
    {
        color = texture(u_texture,v_texCoord);
    }
    else
    {
        color = u_color;
    }

    vec4 light_space_position = u_light_projection * u_light_view * vec4(v_position,1.0);
    vec3 projCoords = light_space_position.xyz / light_space_position.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(u_shadow, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    if(currentDepth - 0.005 > closestDepth)
    {
        o_color = ambient * color;
    }
    else
    {
        float diffuse1 = max(dot(-light_direction,v_normal),0.0);
        float diffuse2 = max(dot(-light_direction,-v_normal),0.0);

        o_color = vec4((ambient + diffuse1 + diffuse2) * vec3(color),1.0);
    }
}