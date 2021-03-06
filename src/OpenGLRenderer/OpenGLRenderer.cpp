#include "OpenGLRenderer.hpp"
namespace GAME
{
void OpenGLRenderer::initGL_()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) 
    {
        LOG::writeLogErr("Could not init GLFW\n");
        throw std::runtime_error("");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(640, 480, "Simple example", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
        throw std::runtime_error("Unable to init glew");
    glfwSetKeyCallback(window, key_callback);

    // Store current window address
    glfwSetWindowUserPointer(window, this);

    // Init joystick
    int present = glfwJoystickPresent(GLFW_JOYSTICK_1);

}

void OpenGLRenderer::initImGui_()
{
    if (!ImGui_ImplGlfwGL3_Init(window, true))
        throw std::runtime_error("Unable to init ImGui");
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
}

void OpenGLRenderer::constructStaticVBO_() {
    // Construct VBO for rendering, in this stage
    // assumes that all of the data contains only 
    // locations and normals in vec3s, no materials
    //
    // Since they are all static objects, world coordinates are pre-calculated
    
    std::vector<float> buffer; //!< X,Y,Z,W,Nx,Ny,Nz
    std::vector<GLuint> index; // Face indices for the who objects
    GLuint indexOffset = 0;
    for (auto &obj : world_.staticObjs())
    {
        glm::mat4 modelMatrix(obj.modelMatrix());
        glm::mat4 normalMat = glm::inverse(glm::transpose(modelMatrix));
        for (size_t i = 0; i < obj.data().size(); i += 6) {
            glm::vec4 pos(obj.data()[i], obj.data()[i + 1], obj.data()[i + 2],
                          1.0);
            glm::vec4 normal(obj.data()[i + 3], obj.data()[i + 4],
                             obj.data()[i + 5], 0.0);
            pos = modelMatrix * pos;
            normal = normalMat * normal;
            buffer.push_back(pos.x);
            buffer.push_back(pos.y);
            buffer.push_back(pos.z);
            buffer.push_back(pos.w);
            buffer.push_back(normal.x);
            buffer.push_back(normal.y);
            buffer.push_back(normal.z);
        }
        for (const auto& idx: obj.index())
            index.push_back(idx + indexOffset);
        indexOffset += obj.data().size() / 6;
    }
    staticObjNumElements_ = index.size();

    // Populate data to VBO;
    glGenVertexArrays(1, &staticObjVAO);
    glGenBuffers(1, &staticObjVBO);
    glGenBuffers(1, &staticObjIBO);

    glBindVertexArray(staticObjVAO);
    glBindBuffer(GL_ARRAY_BUFFER, staticObjVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buffer.size(),
                 buffer.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 7 * sizeof(GLfloat),
                          (void*)(4 * sizeof(GLfloat)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, staticObjIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*index.size(), index.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Unbind buffers after unbinding vertex array
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
}
