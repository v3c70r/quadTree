#pragma once
#include "shader.hpp"
#include <world/world.hpp>
#include <GLFW/glfw3.h>
#include <common/log.hpp>
namespace GAME
{
/*! Use OpenGL API to render the world
 */
class OpenGLRenderer
{
private:
    GLuint staticObjVAO;        //!< Vertex array object to render static objects
    GLuint staticObjVBO;        //!< Vertex buffer object to render static objects
    GLuint staticObjIndexVBO;
    const World& world_;
    Shader basicShader_;
    GLFWwindow *window;
    Shader shader_;

    void constructStaticVBO_(); //!< Construct VBO for static objects
    void initGL_();             //!< Init OpenGL context
    // GLFW callbacks
    static void error_callback(int error, const char* description)
    {
        LOG::writeLogErr("GLFW error: %s\n", description);
    }
public:
    OpenGLRenderer(const World &world):world_(world), window(nullptr){};
    void init() 
    {
        initGL_();
        constructStaticVBO_();
        shader_.createProgrammeFromFiles("shader.vs", "shader.fs");
        shader_.getUniforms();
        shader_.getAttributes();
    }
    void glDraw()
    {
    }
    ~OpenGLRenderer()
    {
        glDeleteBuffers(1, &staticObjVBO);
        glDeleteBuffers(1, &staticObjIndexVBO);
        glDeleteVertexArrays(1, &staticObjVAO);
        if (window) {
            glfwDestroyWindow(window);
            glfwTerminate();
        }
    }
};
}
