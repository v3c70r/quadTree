#pragma once
#include <object.hpp>
namespace GAME
{
class Camera : public Object {
protected:
    glm::mat4 cameraMatrix_;
public:
    glm::mat4& cameraMatrix() {return cameraMatrix_;}
    const glm::mat4& cameraMatrix() const {return cameraMatrix_;}
};
}
