#include "Camera.h"

Camera::Camera(float x, float y, float z){
    SetPosition(x, y, z);
    cameraAngle = 70.f;
    calculateAngles();
}

void Camera::calculateAngles(){
    float b = 1.f / tanf(glm::radians(cameraAngle));
    cameraFront = glm::vec3(0.f, -1.f, -b);
    cameraUp = glm::vec3(0.f, 1.f, -b);

}

void Camera::SetPosition(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

void Camera::setAngle(float a){
    cameraAngle = a;
    calculateAngles();
}

glm::mat4 Camera::GetViewMatrix() {
    glm::vec3 camPosition(x, y, z);
    return glm::lookAt(camPosition, camPosition + cameraFront, cameraUp);
}

void Camera::Follow(float px, float py){
    x = px;
    y = 3.f;
    z = py + 1.f;
}

void Camera::Follow(Entity* e){
    x = e->GetX();
    y = 3.f;
    z = e->GetY() + 1.f;
}