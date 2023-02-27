#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(glm::vec3 position,glm::vec3 target,glm::vec3 worldup); 
    Camera(glm::vec3 position,float pitch, float yaw ,glm::vec3 worldup); 
    ~Camera(); 

    glm::vec3 Position; 
    glm::vec3 Forward; 
    glm::vec3 Right; 
    glm::vec3 Up; 
    glm::vec3 WorldUp; 
    float Pitch, Yaw; 
    float SenseX = 0.001f,SenseY = 0.001f; 
    float speedX = 0; 
    float speedY = 0; 
    float speedZ = 0; 

    glm:: mat4 GetViewMatrix();  
    void ProcessMouseMovement(float deltaX,float deltaY); 
    void UpdateCameraPos(); 

private:
    void UpdataCameraVectors(); 
}; 

#endif