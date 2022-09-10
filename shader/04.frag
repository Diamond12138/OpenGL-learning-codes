#version 460 core

in vec3 v_position;
in vec3 v_normal;
in vec3 v_texCoord;

out vec4 o_color;

uniform sampler3D u_texture;
uniform vec3 u_camera_position;

void main()
{
    const vec3 light_position = vec3(0.0,0.0,0.0);
    const float ambient = 0.1;
    const float constant = 1.0;
    const float linear = 0.2;
    const float quadratic = 0.2;

    vec3 light_direction = normalize(v_position - light_position);
    float diffuse1 = max(dot(-light_direction,v_normal),0.0);
    float diffuse2 = max(dot(-light_direction,-v_normal),0.0);

    float distance = length(v_position - light_position);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    
    float num = distance / 0.01 + 1;
    float count = 0.0;
    vec3 ray = v_position - u_camera_position;
    for(float i = 0;i <= num;i++)
    {
        vec3 position = u_camera_position + ray * (i / num);
        float d = length(position - light_position);
        float a = ambient + 1.0 / (1.0 / linear * d) + 1.0 / (1.0 / quadratic * d * d);
        count += clamp(a / num,0.0,1.0);
    }
    count = clamp(count,0.0,1.0);

    vec4 object_color = texture(u_texture,v_texCoord) * (ambient + diffuse1 + diffuse2) * attenuation;
    vec4 count_color1 = vec4(vec3(1.0,1.0,1.0) * count,1.0 / attenuation);
    vec4 count_color2 = vec4(vec3(1.0,1.0,1.0) * count,1.0);
    o_color = object_color /* count_color1*/ + count_color2;
}