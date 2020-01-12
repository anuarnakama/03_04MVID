#include "engine/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
 : _position(position), _worldUp(up), _yaw(yaw), _pitch(pitch), _fov(k_FOV) {
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : _position(glm::vec3(posX, posY, posZ)), _worldUp(glm::vec3(upX, upY, upZ)), _yaw(yaw), _pitch(pitch), _fov(k_FOV) {
    updateCameraVectors();
}


glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(_position, _position + _front, _up);
}

//Nueva funcion implementando un lookAt
glm::mat4 Camera::getViewMatrix_() const {

    glm::mat4 m1 = glm::mat4(0.0f);
    glm::mat4 m2 = glm::mat4(0.0f);
    glm::mat4 myLookAt;
    
    glm::vec3 cameratarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(_position - cameratarget);

    //Matriz 1
    m1[0][0] = _right.x; //columna - fila
    m1[1][0] = _right.y;
    m1[2][0] = _right.z;

    m1[0][1] = _up.x;
    m1[1][1] = _up.y;
    m1[2][1] = _up.z;

    m1[0][2] = cameraDirection.x;
    m1[1][2] = cameraDirection.y;
    m1[2][2] = cameraDirection.z;

    m1[3][3] = 1.0f;

    //Matriz 2
    m2[3][0] = -_position.x;
    m2[3][1] = -_position.y;
    m2[3][2] = -_position.z;

    m2[0][0] = 1.0f;
    m2[1][1] = 1.0f;
    m2[2][2] = 1.0f;
    m2[3][3] = 1.0f;

    myLookAt = m1 * m2;
    //return glm::lookAt(_position, _position + _front, _up);
    return myLookAt;
}

float Camera::getFOV() const {
    return _fov;
}

glm::vec3 Camera::getPosition() const {
    return _position;
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);

    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}

void Camera::handleKeyboard(Movement direction, float dt) {
    const float velocity = k_Speed * dt;

    switch (direction) {
        //modificacion para no permitir volar, comportamiento de FPS
    case Movement::Forward: _position = glm::vec3(_position.x , 0.0f, _position.z) + _front * velocity; break;
        case Movement::Backward: _position = glm::vec3(_position.x, 0.0f, _position.z) - _front * velocity; break;
        case Movement::Left: _position = glm::vec3(_position.x, 0.0f, _position.z) - _right * velocity; break;
        case Movement::Right: _position = glm::vec3(_position.x, 0.0f, _position.z) + _right * velocity; break;
        default:;
    }
}

void Camera::handleMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    const float xoff = xoffset * k_Sensitivity;
    const float yoff = yoffset * k_Sensitivity;

    _yaw += xoff;
    _pitch += yoff;

    if (constrainPitch) {
        if (_pitch > 89.0f) _pitch = 89.0f;
        if (_pitch < -89.0f) _pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::handleMouseScroll(float yoffset) {
    if (_fov >= 1.0f && _fov <= 45.0f) _fov -= yoffset;
    if (_fov <= 1.0f) _fov = 1.0f;
    if (_fov >= 45.0f) _fov = 45.0f;
}

