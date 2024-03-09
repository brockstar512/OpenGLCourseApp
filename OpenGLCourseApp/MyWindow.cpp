#include "MyWindow.h"

MyWindow::MyWindow()
{
}

MyWindow::MyWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

int MyWindow::Init()
{

    //initialize glfw
    if (!glfwInit())
    {
        printf("GLFW initialization failed");
        glfwTerminate();
        return 1;
    }
    //set up glfw window properties
    //opengl version... we are using 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //preventing any deprecated libs to be used... no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //allowing forwards compatabilty
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //create window
    window = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
    if (!window)
    {
        printf("GLFW window create failed");
        glfwTerminate();
        return 1;
    }

    //window address, buffer width and height address
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    //set context for GLEW to use... if we have multiple windows we can switch between windows to identify the context
    glfwMakeContextCurrent(window);

    //allow modern extension features..
    glewExperimental = GL_TRUE;

    //initialize glew
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initiazlietion failed");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    //initailze depth test so the screen can determine which vertext should be behind vs in front
    glEnable(GL_DEPTH_TEST);
    //setup our viewport size
    //this is the buffer viewport the other width and heighth is for the window
    glViewport(0, 0, bufferWidth, bufferHeight);
}

MyWindow::~MyWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
