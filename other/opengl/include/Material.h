#ifndef MATERIAL_H
#define MATERIAL_H

#include  "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material
{
public:
    Shader shader; 
    unsigned int diffuse; 
    glm::vec3 ambient; 
    unsigned int specular; 
    unsigned int emission; 
    float shininess; 

    Material(Shader _shader,unsigned int _diffuse, unsigned int _specular,glm::vec3 _ambient, unsigned int _emission, float _shininess); 

    Material(Shader _shader,unsigned int _diffuse, unsigned int _specular,glm::vec3 _ambient, float _shininess); 
}; 


#endif