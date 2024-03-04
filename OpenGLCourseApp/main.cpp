// OpenGLCourseApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>

//window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader;
//vertex shaders:
//allows us to manipulate the vertices and pass them off to the fragmebnt shader
//version of glsl
static const char* vShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
layout (location = 0) in vec3 pos;											  \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);				  \n\
}";
//fragment shader: 
//handinling each pixel on the screen and how it works with the vertices
//we dont pass anything directly anything to the vertext shader normally
//we pass it to the vertext shader and the fragment shader picks up the result
        //fShader only has 1 output value normally so whater you call the out varaible it will assume that is your output value
static const char* fShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
out vec4 colour;                                                               \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    colour = vec4(1.0, 0.0, 0.0, 1.0);                                         \n\
}";

void CreateTriangle()
{
    //define points of VAOs
    GLfloat vertices[] = {
        -1.0f,-1.0f,0.0f,
        1.0f,-1.0f,0.0f,
        0.0f,1.0f,0.0f
    };
    //center of screen is 0,0 in opengl

    //create the vao - ammount of array, where we want to store the address/id of the arrays
    glGenVertexArrays(1, &VAO);
    //bind the vertext array
    glBindVertexArray(VAO);
    //create the buffer on the graphics car
    glGenBuffers(1, &VBO);
    //enter which buffer which is the array buffer and pick which buffer by the id
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //connect buffer data.. the vertices to the array
                              //pass in the size of the array//pass in vertices, pass in how we are going to be interacting the triangle. we are not going to be redrawing it so we are sticking with DL_Static draw
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    //location         //which value, how many values, what kind of value,normalize, stride, offest for starting value
    // //stride: i think that means splicing data with addiotnal data cause you can data pack by making odds a vluae for one things and even value means another
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    //we want to enable an array usage to work
    glEnableVertexAttribArray(0);
    //binding the array to nothing
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //unbinding the array
    glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    //create the shader based on the type
    GLuint theShader = glCreateShader(shaderType);
    //get the code
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    //determine how many characters it is
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);
    //pass the shader to the shader source wuitg tge type of shader  and tell he program how long and what the cose is
    glShaderSource(theShader, 1, theCode, codeLength);
    //conpile it
    glCompileShader(theShader);


    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}


void CompileShaders()
{
    shader = glCreateProgram();

    if (!shader)
    {
        printf("error creating shadert program");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);

    AddShader(shader, fShader, GL_FRAGMENT_SHADER);


    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    //create the exe on the graphics cards
    //link them 
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader,sizeof(eLog), NULL, eLog);
        printf("Error Linking Program: '%s'\n", eLog);
        return;
    }
    //now validate them
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error Validating Program: '%s'\n", eLog);
        return;
    }
}

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

    //create triangle
    CreateTriangle();
    //compile shader
    CompileShaders();
    //loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        //get and handle user input events
        glfwPollEvents();

        //clear window
        glClearColor(0.0f, 0.0f,0.0f,1.0f);//these values are the color/256
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        //this is where you can use the shader

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES,0,3);

        glBindVertexArray(0);

        //this is where you unassign the shader
        glUseProgram(0);

        //there is two buffers... one you can see and one you are drawing to
        glfwSwapBuffers(mainWindow);


    }
    //std::cout << "Hello World!\n";
}



//x86 is 32 bit