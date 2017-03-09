#pragma once
#include "shader.hpp"
#include <world/world.hpp>
#include <GLFW/glfw3.h>
#include <common/log.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace GAME
{
/*! Use OpenGL API to render the world
 */
class OpenGLRenderer
{
private:
    GLFWwindow *window;
    GLuint staticObjVAO;        //!< Vertex array object to render static objects
    GLuint staticObjVBO;        //!< Vertex buffer object to render static objects
    GLuint staticObjIBO;
    size_t staticObjNumElements_;
    World& world_;
    Shader basicShader_;

    Shader shader_;

    void constructStaticVBO_(); //!< Construct VBO for static objects
    void initGL_();             //!< Init OpenGL context
    // GLFW callbacks
    static void error_callback(int error, const char* description)
    {
        LOG::writeLogErr("GLFW error: %s\n", description);
    }

    static void key_callback(GLFWwindow* window, int key, int scancode,
                             int action, int mods)
    {
        OpenGLRenderer* renderer =
            static_cast<OpenGLRenderer*>(glfwGetWindowUserPointer(window));
        FPScamera& camera = renderer->world_.camera();
        if (action == GLFW_PRESS) switch (key) {
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window, GL_TRUE);
                    break;
                case GLFW_KEY_W:
                    camera.cameraMatrix() = glm::translate(camera.cameraMatrix(), glm::vec3(0.0, 0.0, -1.0));
                    break;
                case GLFW_KEY_S:
                    camera.cameraMatrix() = glm::translate(camera.cameraMatrix(), glm::vec3(0.0, 0.0, 1.0));
                    break;
                case GLFW_KEY_A:
                    camera.cameraMatrix() = glm::translate(camera.cameraMatrix(), glm::vec3(-1.0, 0.0, 0.0));
                    break;
                case GLFW_KEY_D:
                    camera.cameraMatrix() = glm::translate(camera.cameraMatrix(), glm::vec3(1.0, 0.0, 0.0));
                    break;
                case GLFW_KEY_I:
                    camera.pitch(0.1);
                    break;
            }
    }

    void handleJoystick(int joystick)
    {
        int axisCount = 0;
        const float *axis = ::glfwGetJoystickAxes(joystick, &axisCount);
        int btnCount = 0;
        //const char* name = glfwGetJoystickName(joystick);
        //std::cout<<"Joystick name: "<<name<<std::endl;
        const unsigned char *btns = ::glfwGetJoystickButtons(joystick, &btnCount);
        //std::cout<<"================\n";
        //for (int i = 0; i < axisCount; i++)
        //    if (std::abs(axis[i] - 0.0) > 0.1)
        //        std::cout<<"axis "<<i<<": "<<axis[i]<<std::endl;
        //for (int i = 0; i < btnCount; i++)
        //    if (btns[i])
        //        std::cout<<"btn "<<i<<": "<<btns[i]<<std::endl;
        world_.camera().pitch(axis[5]/50.0);
        world_.camera().yaw(axis[2]/50.0);
        world_.camera().forward(-axis[1]/10.0);
        world_.camera().strafe(axis[0]/10.0);



    }

public:
    OpenGLRenderer(World &world):world_(world), window(nullptr){};
    void init() 
    {
        initGL_();
        constructStaticVBO_();
        shader_.createProgrammeFromFiles("shaders/shader.vs", "shaders/shader.fs");
        shader_.getUniforms();
        shader_.getAttributes();
    }
    void glDraw()
    {
        glBindVertexArray(staticObjVAO);
        shader_.use();
        // TODO: Use UBO
        glUniformMatrix4fv( 
                glGetUniformLocation(shader_.programme(), "projection"),
                1, GL_FALSE,
                glm::value_ptr(world_.camera().projectionMatrix()));
        glUniformMatrix4fv( 
                glGetUniformLocation(shader_.programme(), "view"),
                1, GL_FALSE,
                glm::value_ptr(world_.camera().cameraMatrix()));
        glDrawElements(GL_TRIANGLES, staticObjNumElements_, GL_UNSIGNED_INT, nullptr);
        glUseProgram(0);
        glBindVertexArray(0);
    }
    void runLoop()
    {
        while (!glfwWindowShouldClose(window)) {
            if (::glfwJoystickPresent(GLFW_JOYSTICK_2))
                    handleJoystick(GLFW_JOYSTICK_2);
            world_.camera().update();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            int width, height; 
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);

            glDraw();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    ~OpenGLRenderer()
    {
        glDeleteBuffers(1, &staticObjVBO);
        glDeleteBuffers(1, &staticObjIBO);
        glDeleteVertexArrays(1, &staticObjVAO);
        if (window) {
            glfwDestroyWindow(window);
            glfwTerminate();
        }
    }
};
}
