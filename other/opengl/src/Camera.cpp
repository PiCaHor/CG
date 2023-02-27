#include "Camera.h"

Camera::Camera(glm::vec3 position,glm::vec3 target,glm::vec3 worldup)
{
    Position = position; 
    WorldUp =  worldup; 
    Forward = glm::normalize(target- position); 
    Right = glm::normalize(glm::cross(Forward,WorldUp)); 
    Up = glm::normalize(glm::cross(Right,Forward)); 
    
}

Camera::Camera(glm::vec3 position,float pitch, float yaw ,glm::vec3 worldup)
{
    Position = position; 
    WorldUp =  worldup; 
    Pitch = pitch; 
    Yaw = yaw; 
    Forward.x = glm::cos(Pitch) * glm:: sin(Yaw); 
    Forward.y = glm::sin(Pitch); 
    Forward.z = glm::cos(Pitch) * glm:: cos(Yaw); 
    Right = glm::normalize(glm::cross(Forward,WorldUp)); 
    Up = glm::normalize(glm::cross(Right,Forward)); 
}

Camera::~Camera()
{

}

glm:: mat4 Camera::GetViewMatrix()
{   
    return glm::lookAt(Position,Position + Forward, WorldUp); 
}

void Camera::UpdataCameraVectors()
{
    Forward.x = glm::cos(Pitch) * glm:: sin(Yaw); 
    Forward.y = glm::sin(Pitch); 
    Forward.z = glm::cos(Pitch) * glm:: cos(Yaw); 
    Right = glm::normalize(glm::cross(Forward,WorldUp)); 
    Up = glm::normalize(glm::cross(Right,Forward)); 
}

void Camera::ProcessMouseMovement(float deltaX,float deltaY)
{
    Pitch -= deltaY * SenseY; 
    Yaw -= deltaX * SenseX; 
    UpdataCameraVectors(); 
}

void Camera::UpdateCameraPos()
{
    Position += (Forward * speedZ + Right * speedX + Up * speedY)* 0.1f; 
}
