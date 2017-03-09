#include "camera.hpp"
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <glm/gtx/string_cast.hpp>
namespace GAME
{
    class FPScamera: public Camera
    {
    protected:
        float roll_;
        float pitch_;
        float yaw_;
        float strafe_;
        float forward_;
        glm::vec3 eyeVector_;
    public:
        FPScamera() : 
             roll_(0.0),
             pitch_(0.0),
             yaw_(0.0),
             strafe_(0.0),
             forward_(0.0),
             eyeVector_(glm::vec3(0.0))
        {
        }
        void pitch(float angle)
        {
            pitch_ += angle;
        }
        void yaw(float angle)
        {
            yaw_ += angle;
        }
        void forward(float dist)
        {
            forward_ = dist;
        }
        void strafe(float dist)
        {
            strafe_ = dist;
        }
        void update()
        {
            glm::mat4 matRoll = glm::mat4(1.0f);   // identity matrix;
            glm::mat4 matPitch = glm::mat4(1.0f);  // identity matrix
            glm::mat4 matYaw = glm::mat4(1.0f);    // identity matrix
            // roll, pitch and yaw are used to store our angles in our class
            matRoll = glm::rotate(matRoll, roll_, glm::vec3(0.0f, 0.0f, 1.0f));
            matPitch =
                glm::rotate(matPitch, pitch_, glm::vec3(1.0f, 0.0f, 0.0f));
            matYaw = glm::rotate(matYaw, yaw_, glm::vec3(0.0f, 1.0f, 0.0f));
             //order matters
            glm::mat4 rotate = matRoll * matPitch * matYaw;
            glm::vec3 forward(cameraMatrix_[0][2], cameraMatrix_[1][2], cameraMatrix_[2][2]);
            glm::vec3 strafe(cameraMatrix_[0][0], cameraMatrix_[1][0], cameraMatrix_[2][0]);
            eyeVector_ +=  (-forward_ * forward + strafe_ * strafe);
            forward_ = 0; strafe_ = 0;
            glm::mat4 translate = glm::mat4(1.0f);
            translate = glm::translate(translate, -eyeVector_);
            cameraMatrix_ = rotate * translate;
        }
    };
}
