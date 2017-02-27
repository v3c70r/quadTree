#pragma once
#include <world.hpp>
#include <GL/glew.h>
namespace GAME
{
/*! Use OpenGL API to render the world
 */
class OpenGLRenderer
{
private:
    GLuint staticObjVAO;        //!< Vertex array object to render static objects
    GLuint staticObjVBO;        //!< Vertex buffer object to render static objects
    void constructStaticVBO_(); //!< Construct VBO for static objects
    const World& world_;
public:
    OpenGLRenderer(const World &world):world_(world){};
};
}
