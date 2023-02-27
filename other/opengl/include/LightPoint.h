#ifndef LIGHTPOINT_H
#define LIGHTPOINT_H

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <LightDirectional.h>
class LightPoint : public LightDirectional
{
public:
    LightPoint(glm::vec3 _position,glm::vec3 _angles,glm::vec3 _color = glm::vec3(1.0f,1.0f,1.0f));

    float constant = 1.0f; 
    float linear = 0.09f; 
    float quadratic = 0.032f; 
};

#endif