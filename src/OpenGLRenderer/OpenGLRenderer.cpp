#include "OpenGLRenderer.hpp"
namespace GAME
{
void OpenGLRenderer::constructStaticVBO_() {
    // Construct VBO for rendering, in this stage
    // assumes that all of the data contains only 
    // locations and normals in vec3s, no materials
    //
    // Since they are all static objects, world coordinates are pre-calculated
    
    std::vector<glm::vec3>
}
}
