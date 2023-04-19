#pragma once
#include "OGLGraphics.h"
#include "Headers/utilities/Renderer.h"
#include <iostream>
#include <glfw3.h>

#include "IMGUI/imgui.h"
#include "IMGUI/backends/imgui_impl_glfw.h"
#include "IMGUI/backends/imgui_impl_opengl3.h"
#include "state_machines/MenuStateMachine.h"

void InitGLFW();
GLFWwindow* CreateWindow();
bool CheckGlad();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
bool InitOGL(GLFWwindow* a_Window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

bool ButtonPressed;
float ScrollOffset;
bool test = false;
glm::vec2 MousePos;

//Renderer* renderer;
MenuStateMachine* main_state_machine;

int main()
{
    InitGLFW();
    GLFWwindow* window = CreateWindow();
	CheckGlad();
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //renderer = new Renderer();

    ImGuiContext* context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    ImGuiIO& io = ImGui::GetIO();

    main_state_machine = new MenuStateMachine(io, "../Empires(Windows)/Resources/Shaders/proceduralshader.frag", "../Empires(Windows)/Resources/Shaders/proceduralshader.vert");
    //renderer->Init(io, "../Empires(Windows)/Resources/Shaders/proceduralshader.frag", "../Empires(Windows)/Resources/Shaders/proceduralshader.vert");

    while (!glfwWindowShouldClose(window))
    {	
        ScrollOffset = 0;
        glfwPollEvents();
        glfwSwapBuffers(window);

        //glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetScrollCallback(window, scroll_callback);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        //renderer->Update(io, ButtonPressed, MousePos, 0, ScrollOffset);
        main_state_machine->HandleMenu(io, ButtonPressed, MousePos, 0, ScrollOffset);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //delete renderer;
    delete main_state_machine;
	
    return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        ButtonPressed = true;
    else
        ButtonPressed = false;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        test = true;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
    case GLFW_KEY_W:
        main_state_machine->GetRenderer().Input(KeyInput::W);
        break;
    case GLFW_KEY_A:
        main_state_machine->GetRenderer().Input(KeyInput::A);
        break;
    case GLFW_KEY_S:
        main_state_machine->GetRenderer().Input(KeyInput::S);
        break;
    case GLFW_KEY_D:
        main_state_machine->GetRenderer().Input(KeyInput::D);
        break;
    case GLFW_KEY_SPACE:
        main_state_machine->GetRenderer().Input(KeyInput::SPACE);
        break;
    default:
        main_state_machine->GetRenderer().Input(KeyInput::NONE);
        break;
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    MousePos = glm::vec2(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ScrollOffset = static_cast<float>(yoffset);
    
    if (yoffset > 0)
        main_state_machine->GetRenderer().Input(KeyInput::SCROLLUP);
    else 
        main_state_machine->GetRenderer().Input(KeyInput::SCROLLDOWN);
}

void InitGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

GLFWwindow* CreateWindow()
{
    GLFWwindow* window = glfwCreateWindow(800, 600, "Empires", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        _ASSERT(window == nullptr);
    }
    glfwMakeContextCurrent(window);
	
    return window;
}

bool CheckGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool InitOGL(GLFWwindow* a_Window)
{
    InitGLFW();
    a_Window = CreateWindow();
	
    return CheckGlad();
}