#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <iostream>
#include <array>
#include <random>

#include <rectangle.hpp>
#include <quad.hpp>
#include <object.hpp>

#include <imgui.h>
#include <examples/opengl3_example/imgui_impl_glfw_gl3.h>

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


static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
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
        //draw_list->AddRect(ImVec2(p.x, p.y), ImVec2(p.x+sz, p.y+sz), ImColor(ImVec4(1.0, 0.0, 0.0, 1.0)), 1.0f, ~1, 1.0);
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
            aabb.getBoundingBox().ImDraw(scale, q.getBound(), ImVec4(1.0, 0.0, 0.0, 1.0));
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
