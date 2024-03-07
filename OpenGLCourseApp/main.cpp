// OpenGLCourseApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;
GLuint VAO, VBO, IBO, shader, uniformModel;
//not a gint or gfloat because these are not being passed to the shaders
bool direction = true;
float currAngle = 0.0f;
float toScale = 0.4f;
//glm::mat4 model(1.0f);
// glm::mat4 model = glm::mat4(1.0f);
// 
// 
//***a uniformed variable will be the same instance for every vertex***
//***the variable (pos) will be passed htrough every vertex***
//vertex shaders:
//allows us to manipulate the vertices and pass them off to the fragmebnt shader
// using the out keyword lets up output/return a value from the shader to the cpp file
// //i think the vertex is saying to each vertext... whatever your pos is... take the clamped value color of that point
//version of glsl
static const char* vShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
layout (location = 0) in vec3 pos;											  \n\
out vec4 vertexColor;                                                        \n\
uniform mat4 model;                                                          \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    vertexColor = vec4(clamp(pos,0.0f,1.0f),1);                                   \n\
    gl_Position = model * vec4(pos, 1.0);				                        \n\
}";

//fragment shader: 
//handinling each pixel on the screen and how it works with the vertices
//we dont pass anything directly anything to the vertext shader normally
//we pass it to the vertext shader and the fragment shader picks up the result
        //fShader only has 1 output value normally so whater you call the out varaible it will assume that is your output value
static const char* fShader = "                                                \n\
#version 330                                                                  \n\
in vec4 vertexColor;                                                                 \n\
out vec4 colour;                                                               \n\
                                                                             \n\
void main()                                                                   \n\
{                                                                             \n\
    colour = vertexColor;                                                       \n\
}";

void CreateTriangle()
{
    //place which point to place in which order based on indeces
    //we are saying the 0 index in vertices is our first point
    //then draw the 3rd index, then draw the 1st index.
    //each line is a side of our pyramid. 
    //THE INDICES IS REFERING THE LINE IN THE VERTICES SO 0 IS -1.0f,-1.0f,0.0f AND 3 is 0.0f,1.0f,0.0f
    //the second index in indeces (2,3,0,) is the front facing side... 
    unsigned int indices[] = {
        0,3,1,
        1,3,2,
        2,3,0,//this  right bottom point(1.0f,-1.0f,0.0f) top point(0.0f,1.0f,0.0f) left bottom point(-1.0f,-1.0f,0.0f,)
        0,1,2
    };


    //define points of VAOs
    GLfloat vertices[] = {
        -1.0f,-1.0f,0.0f,
        0.0f,-1.0f,1.0f,
        1.0f,-1.0f,0.0f,
        0.0f,1.0f,0.0f
    };
    //center of screen is 0,0 in opengl

    //create the vao - ammount of array, where we want to store the address/id of the arrays
    glGenVertexArrays(1, &VAO);
    //bind the vertext array
    glBindVertexArray(VAO);

    //create buffer for the indeices
    glGenBuffers(1,&IBO);
    //bind the buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
    //we want the buffer element to bind and the size of the indeces
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,GL_STATIC_DRAW);
    
    
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

    //unbind the IBO/EBO after you unbind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    //unbinding the array
    glBindVertexArray(0);


}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    //create the shader based on the type... glCreateShader: creates an empty shader object and returns a unsigned value by which it can be referenced as like an ID
    //so theShader is an alias value for the shader id on the graphics card.
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

    //get location/id of uniform variable
    uniformModel = glGetUniformLocation(shader, "model");
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

    //initailze depth test so the screen can determine which vertext should be behind vs in front
    glEnable(GL_DEPTH_TEST);
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

        currAngle += 1.0f;
        if (currAngle >= 360)
        {
            currAngle -= 360;
        }

        //clear window
        glClearColor(0.0f, 0.0f,0.0f,1.0f);//these values are the color/256
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//now it's clearing both the depth and buffer bit via the bitwaise p[erator
        

        glUseProgram(shader);

        //glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 model(1.0f);
        //modifying the model
        //put it back into the model after you changed the model...rotate on the y axix
        model = glm::rotate(model, currAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(toScale, toScale, 1.0f));


        //assign uniform shader
        //glUniform1f(uniformXMove, triOffset);
        //we dont put it the model directly. we put in the address of the model
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));

        //this is where you can use the shader

        glBindVertexArray(VAO);

        //we are not drawing the arrays anymore
        //glDrawArrays(GL_TRIANGLES,0,3);

        //bind ibo
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
        //draw the element not based on arrays but based on the element ids
                                   //count of indeces//the size of type
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        //unbind
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        

        glBindVertexArray(0);

        //this is where you unassign the shader
        glUseProgram(0);

        //there is two buffers... one you can see and one you are drawing to
        glfwSwapBuffers(mainWindow);


    }
    //std::cout << "Hello World!\n";
}



//x86 is 32 bit

/*
* interpolation:
* how the fragment shader interpolates the points between the vertices. happens during the rasterization stage during the render. 
quickly estimatng/calculating with normal map.
* 
* 
* index draws:
* predefined indexes to reuse so we dont have to defin every vertices
* 
* projections:
* how things are visualized. view to change from view space to clip space using coordinat systems
* previously we used local space
* we are going to use projection to make view space which is the position of vertices in world space
* view space position in the world in relation to the camera by mutiplying it by view matrix
* clip space is clipping  out what we dont want to see
* screen space is the final image and projected onto the window itself


frustum is the dined are we want to see. its the truncated pyramid
*/
