// OpenGLCourseApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>

//window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
int main()
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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);

    //create window
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window create failed");
        glfwTerminate();
        return 1;
    }
    //get buffer size information
    int bufferWidth, bufferHeight;
    //window address, buffer width and height address
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //set context for GLEW to use... if we have multiple windows we can switch between windows to identify the context
    glfwMakeContextCurrent(mainWindow);

    //allow modern extension features..
    glewExperimental = GL_TRUE;

    //initialize glew
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initiazlietion failed");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    //setup our viewport size
    //this is the buffer viewport the other width and heighth is for the window
    glViewport(0,0,bufferWidth,bufferHeight);

    //loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        //get and handle user input events
        glfwPollEvents();

        //clear window
        glClearColor(1.0f, 0.0f,0.0f,1.0f);//these values are the color/256
        glClear(GL_COLOR_BUFFER_BIT);
        //there is two buffers... one you can see and one you are drawing to
        glfwSwapBuffers(mainWindow);


    }
    //std::cout << "Hello World!\n";
}



//x86 is 32 bit