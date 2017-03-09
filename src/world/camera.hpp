#pragma once
#include "object.hpp"
namespace GAME
{
class Camera {
protected:
    glm::mat4 cameraMatrix_;
    glm::mat4 projectionMatrix_;
public:
    Camera (){}
    glm::mat4& cameraMatrix() {return cameraMatrix_;}
    const glm::mat4& cameraMatrix() const {return cameraMatrix_;}

    glm::mat4& projectionMatrix() {return projectionMatrix_;}
    const glm::mat4& projectionMatrix() const {return projectionMatrix_;}
};
}
