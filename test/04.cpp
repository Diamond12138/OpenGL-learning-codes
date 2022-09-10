#include <iostream>
#include <glm/glm.hpp>

float calc(glm::vec3 position);

int main()
{
    std::cout << calc(glm::vec3(-2,-2,-2)) << std::endl;
    std::cout << calc(glm::vec3(-1,-1,-1)) << std::endl;
    std::cout << calc(glm::vec3(0,-1,-1)) << std::endl;
    std::cout << calc(glm::vec3(1,-1,-1)) << std::endl;
    std::cout << calc(glm::vec3(0,0,0)) << std::endl;
    return 0;
}

float calc(glm::vec3 position)
{
    const float constant = 1.0;
    const float linear = 0.7;
    const float quadratic = 1.8;

    glm::vec3 v_position = position;
    glm::vec3 light_position = glm::vec3(0.0,0.0,0.0);
    glm::vec3 u_camera_position = glm::vec3(1.0,1.0,1.0);

    float distance = glm::length(v_position - light_position);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    
    float num = distance / 0.001 + 1;
    float count = 0.0;
    glm::vec3 ray = v_position - u_camera_position;
    for(float i = 0;i <= num;i++)
    {
        glm::vec3 position = u_camera_position + ray * (i / num);
        float d = glm::length(position - light_position);
        float a = 1.0 / (constant + linear * d + quadratic * (d * d));
        count += a / num;
    }

    return count;
}