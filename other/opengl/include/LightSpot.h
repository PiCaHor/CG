#ifndef LIGHTSOPT_H
#define LIGHTSOPT_H

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <LightPoint.h>


class LightSpot : public LightPoint
{
public:
    LightSpot(glm::vec3 _position,glm::vec3 _angles,glm::vec3 _color = glm::vec3(1.0f,1.0f,1.0f));

    float cosPhyInner = 0.95f;
    float cosPhyOuter = 0.9f;   
};




#endif