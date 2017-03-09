#include <GL/glew.h>
#include <OpenGLRenderer/OpenGLRenderer.hpp>
#include <world/world.hpp>
#include <quadTree/rectangle.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
using namespace std;
class splitstring : public string {
    vector<string> flds;
    vector<float> floatArray;
public:
    splitstring(char *s) : string(s) { };
    splitstring(string s) : string(s) { };
    vector<string>& split(char delim, int rep=0)
    {
           if (!flds.empty()) flds.clear();  // empty vector if necessary
    string work = data();
    string buf = "";
    unsigned i = 0;
    while (i < work.length()) {
        if (work[i] != delim)
            buf += work[i];
        else if (rep == 1) {
            flds.push_back(buf);
            buf = "";
        } else if (buf.length() > 0) {
            flds.push_back(buf);
            buf = "";
        }
        i++;
    }
    if (!buf.empty())
        flds.push_back(buf);
return flds;
    }
};

void loadSimpleOBJ(std::string objFile, std::vector<float> &buf, std::vector<unsigned> &index)
{
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLuint> indices;
    GLuint nV = 0;
    GLuint nF = 0;
    std::ifstream inFile(objFile);
    std::string str;
    std::cout << "Loading " << objFile << "..." << std::endl;
    while (std::getline(inFile, str))  // read  position
    {
        splitstring split(str);
        std::vector<std::string> splited = split.split(' ');
        if (splited.size() > 0) {
            if (splited[0] == "v") {
                nV++;
                for (size_t i=1; i<splited.size(); i++)
                {
                    vertices.push_back(std::stof(splited[i]));
                }
            }
            else if (splited[0] == "f") {
                if (splited.size() == 4)  // triangle
                {
                    nF++;
                    for (size_t i=1; i<splited.size(); i++)
                        indices.push_back((GLuint)std::stoi(splited[i]) - 1);
                }
                else if (splited.size() == 5)  // quad faces
                {
                    nF += 2;
                    indices.push_back((GLuint)std::stoi(splited[1]) - 1);
                    indices.push_back((GLuint)std::stoi(splited[2]) - 1);
                    indices.push_back((GLuint)std::stoi(splited[4]) - 1);
                    indices.push_back((GLuint)std::stoi(splited[4]) - 1);
                    indices.push_back((GLuint)std::stoi(splited[2]) - 1);
                    indices.push_back((GLuint)std::stoi(splited[3]) - 1);
                }
            }
            else
                continue;
        }
    }
    if (normals.size() == 0)  // Calculate normal from mesh
    {
        // Smooth normals
        normals.resize(vertices.size(), 0.0);
        for (auto i = 0; i < indices.size(); i += 3) {
            glm::vec3 p0 = {vertices[3 * indices[i]],
                            vertices[3 * indices[i] + 1],
                            vertices[3 * indices[i] + 2]};
            glm::vec3 p1 = {vertices[3 * (indices[i + 1])],
                            vertices[3 * (indices[i + 1]) + 1],
                            vertices[3 * (indices[i + 1]) + 2]};
            glm::vec3 p2 = {vertices[3 * (indices[i + 2])],
                            vertices[3 * (indices[i + 2]) + 1],
                            vertices[3 * (indices[i + 2]) + 2]};
            glm::vec3 normal = glm::normalize(glm::cross((p1 - p0), (p2 - p0)));
            normals[3 * indices[i]] += normal.x;
            normals[3 * indices[i] + 1] += normal.y;
            normals[3 * indices[i] + 2] += normal.z;
            normals[3 * (indices[i + 1])] += normal.x;
            normals[3 * (indices[i + 1]) + 1] += normal.y;
            normals[3 * (indices[i + 1]) + 2] += normal.z;
            normals[3 * (indices[i + 2])] += normal.x;
            normals[3 * (indices[i + 2]) + 1] += normal.y;
            normals[3 * (indices[i + 2]) + 2] += normal.z;
        }
        // normalize
        for (auto i = 0; i < normals.size(); i += 3) {
            float distance = glm::sqrt(normals[i] * normals[i] +
                                       normals[i + 1] * normals[i + 1] +
                                       normals[i + 2] * normals[i + 2]);
            normals[i] /= distance;
            normals[i + 1] /= distance;
            normals[i + 2] /= distance;
        }
    }

    for (size_t i=0; i<vertices.size(); i+=3)
    {
        buf.push_back(vertices[i]);
        buf.push_back(vertices[i+1]);
        buf.push_back(vertices[i+2]);
        buf.push_back(normals[i]);
        buf.push_back(normals[i+1]);
        buf.push_back(normals[i+2]);
    }
    for (size_t i=0; i<indices.size(); i++)
        index.push_back(indices[i]);
}

int main()
{
    LOG::restartLog();
    GAME::World world(100, 100, Rectangle{glm::vec2(-1.0), glm::vec2(1.0)});
    GAME::Object obj1;
    GAME::Object obj2;

    // Load Object
    std::string inputFile = "model/tyra.obj";
    std::vector<float> buf1;
    std::vector<unsigned> index1;
    loadSimpleOBJ(inputFile, buf1, index1);

    obj1.data() = buf1;
    obj1.index() = index1;
    obj1.modelMatrix() = glm::rotate(obj1.modelMatrix(), 3.1415926f, glm::vec3(0.0, 1.0, 0.0));
    world.addStaticObj(obj1);

    inputFile = "model/bunny.obj";
    std::vector<float> buf2;
    std::vector<unsigned> index2;
    loadSimpleOBJ(inputFile, buf2, index2);
    obj2.data() = buf2;
    obj2.index() = index2;
    obj2.modelMatrix() = glm::translate(obj2.modelMatrix(), glm::vec3(-2.0, 0.0, 0.0));
    obj2.modelMatrix() = glm::scale(obj2.modelMatrix(), glm::vec3(10.0));
    world.addStaticObj(obj2);

    GAME::OpenGLRenderer renderer(world);
    renderer.init();
    renderer.runLoop();
}
/*
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <random>
#include <vector>

#include <object.hpp>
#include <rectangle.hpp>

#include <examples/opengl3_example/imgui_impl_glfw_gl3.h>
#include <imgui.h>

#include <AABB.hpp>
#include <AABBQuadTree.hpp>

// //////////////////////
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

std::vector<Object> points;
Quad<5, 10> q(0, Rectangle{glm::vec2(-0.9), glm::vec2(0.9)});

std::vector<QT::AABB> aabbPoints;
QT::AABBQuad<6, 100> aabbQ(0, Rectangle{glm::vec2(-0.9), glm::vec2(0.9)});

static bool isDragging = false;
static bool insertPos = false;

static void mouse_button_callback(GLFWwindow* window, int button, int action,
                                  int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if( action == GLFW_PRESS)
        {
            if (!isDragging)
            {
                isDragging = true;
                insertPos = true;
            }
        }
        else
        {
            isDragging = false;
        }
    }
}


static void cursor_position_callback(GLFWwindow* window, double xpos, double
ypos)
{
    if (isDragging)
    {
        int w, h;
        glfwGetWindowSize(window, &w, &h);
        if (insertPos)
        {
            points.push_back(glm::vec2(2.0*(xpos/w) - 1.0,1.0-(ypos/h)*2.0));
            std::cout<<&(points.back())<<std::endl;
            q.insertObj(&(points.back()));
            insertPos = false;
        }
        else
        {
            //points.back().setLocation(glm::vec2(xpos/w, ypos/h));
        }
    }
}

int main(void)
{

    points.reserve(2000);
    GLFWwindow* window;
    std::random_device rd;
    std::mt19937 mt(rd());
    std::normal_distribution<float> xdist(0.0, 0.2);
    std::normal_distribution<float> ydist(0.0, 0.3);
    std::uniform_real_distribution<float> xsize( 0.0, 0.5);
    std::uniform_real_distribution<float> ysize( 0.0, 0.5);
    for (size_t i=0; i<1000; i++)
    {
        points.push_back(Object{glm::vec2(xdist(mt), ydist(mt))});
        q.insertObj(&(points.back()));
    }

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);


    // Init glew, enable it to avoid ImGui crash on Linux
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
        throw std::runtime_error("Unable to init glew");

    // Setup ImGui binding
    if (!ImGui_ImplGlfwGL3_Init(window, true))
        throw std::runtime_error("Unable to init ImGui");

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    // Generate aabb points
    for (size_t i=0; i<1000; i++)
    {
        glm::vec2 startP{xdist(mt), ydist(mt)};
        glm::vec2 sz{xsize(mt), ysize(mt)};
        aabbPoints.push_back(Rectangle{startP, startP+sz});
    }
    for (size_t i=0; i<aabbPoints.size(); i++)
        aabbQ.insertObj(&(aabbPoints[i]));


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();
        ImGui::Text("Hello, world!");

        // Experiment on draw rectangles!!
        // ///////
        float sz = 50.0;
        //draw_list->AddRect(ImVec2(p.x, p.y), ImVec2(p.x+sz, p.y+sz),
ImColor(ImVec4(1.0, 0.0, 0.0, 1.0)), 1.0f, ~1, 1.0);
        //Rectangle b{glm::vec2(0.0), glm::vec2(60)};
        //Rectangle r1{glm::vec2(10.0, 10.0), glm::vec2(30.0, 20.0)};
        //Rectangle r2{glm::vec2(0.0, 0.0), glm::vec2(50.0, 30.0)};

        //ImGui::Begin("Rectangles");
        //r1.ImDraw(2, b);
        //r2.ImDraw(2, b);
        //ImGui::End();

        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPointSize(5.0);

        for (const auto& pt : points) {
            pt.glDraw();
        }
        glColor3f(1.0, 0.0, 0.0);

        static float scale = 100.0;
        ImGui::Begin("QuadTree");
        ImGui::SliderFloat("Scale", &scale, 0.1, 1000.0, "%.0f");
        for (const auto& aabb: aabbPoints)
        {
            aabb.getBoundingBox().ImDraw(scale, q.getBound(), ImVec4(1.0, 0.0,
0.0, 1.0));
        }
        aabbQ.ImDraw(scale, q.getBound());
        ImGui::End();
        glColor3f(0.0, 1.0, 0.0);
        ImGui::Render();
        glfwSwapBuffers(window);
    }
    ImGui_ImplGlfwGL3_Shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
*/
