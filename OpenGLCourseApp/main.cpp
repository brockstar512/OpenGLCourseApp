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
#include <vector>
#include "Mesh.h"
#include "Shader.h"
#include "MyWindow.h"
#include "Camera.h"

Camera camera;
const float toRadians = 3.14159265f / 180.0f;
GLuint shader;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
MyWindow window;
//not a gint or gfloat because these are not being passed to the shaders
float currAngle = 0.0f;
float toScale = 0.4f;

//vertex shader
static const char* vShader = "Shaders/shader.vert";
//fragment shader
static const char* fShader = "Shaders/shader.frag";
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
//deltatime = current time - lasttime
//then multiply the camera speed by deltaTime
// 
//three types of angles: pitch -> up and down yaw -> left and right roll-> rotation around the axis

void CreateShader()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFile(vShader,fShader);
    shaderList.push_back(shader1);
}

void CreateObject()
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

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj2);

}


int main()
{

    window = MyWindow(800,600);
    window.Init();

    //create triangle
    CreateObject();
    //compile shader
    CreateShader();
    //camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 90.0f, 0.0f, 5.0f, 0.1f);this was making it go crazy... the world up was inverted and the yaw was inverted

   camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.075f);


    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

    //loop until window closed
    glm::mat4 projection = glm::perspective(45.0f, window.GetBufferWidth()/window.GetBuggerHeight(), 0.1f, 100.0f);
    while (!window.getShouldClose())
    {
        GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
        deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
        lastTime = now;

        //get and handle user input events
        glfwPollEvents();

        camera.KeyControl(window.GetKeys(), deltaTime);
        camera.MouseControl(window.GetXChange(), window.GetYChange());


        //clear window
        glClearColor(0.0f, 0.0f,0.0f,1.0f);//these values are the color/256
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//now it's clearing both the depth and buffer bit via the bitwaise p[erator
        
        
        glUseProgram(shader);
        shaderList[0]->UseShader();

        uniformModel = shaderList[0]->GetModelLocation();
        uniformProjection = shaderList[0]->GetProjectionLocation();
        //get the uniform variabel here (1)
        uniformView = shaderList[0]->GetViewLocation();
        //glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 model(1.0f);
        //modifying the model


        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        //pass in the variable
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));

        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->RenderMesh();

     

        //this is where you unassign the shader
        glUseProgram(0);

        //there is two buffers... one you can see and one you are drawing to
        window.SwapBuffers();


    }
    //std::cout << "Hello World!\n";

    //i could delete pointers here if i want them...
}

