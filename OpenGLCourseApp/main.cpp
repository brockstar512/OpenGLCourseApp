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
//window dimensions

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
    obj1->CreateMesh(vertices, indices,12,12);
    meshList.push_back(obj1);

}


int main()
{

    window = MyWindow(800,600);
    window.Init();

    //create triangle
    CreateObject();
    //compile shader
    CreateShader();

    GLuint uniformProjection = 0, uniformModel = 0;

    //loop until window closed
    glm::mat4 projection = glm::perspective(45.0f, window.GetBufferWidth()/window.GetBuggerHeight(), 0.1f, 100.0f);
    while (!window.getShouldClose())
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
        shaderList[0]->UseShader();

        uniformModel = shaderList[0]->GetModelLocation();
        uniformProjection = shaderList[0]->GetProjectionLocation();
        //glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 model(1.0f);
        //modifying the model
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));//push the trangle away from you
        //put it back into the model after you changed the model...rotate on the y axix
        model = glm::rotate(model, currAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(toScale, toScale, 1.0f));


        //assign uniform shader
        //glUniform1f(uniformXMove, triOffset);
        //we dont put it the model directly. we put in the address of the model
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));


        meshList[0]->RenderMesh();
        //this is where you unassign the shader
        glUseProgram(0);

        //there is two buffers... one you can see and one you are drawing to
        window.SwapBuffers();


    }
    //std::cout << "Hello World!\n";
}


/*
* interpolation:
* how the fragment shader interpolates the points between the vertices. happens during the rasterization stage during the render. 
quickly estimatng/calculating with normal map.
* 
* 
* index draws:
* predefined indexes to reuse so we dont have to defin every vertices.
* //this basically builds a library of points that we can reused existing vertices.. this is helpful when you think of a model that is made up of triangles
* if you have to duplicate each vertices in your buffer that is to be so many vertices so instead we just use one index for a point in a triangle
* 
* projections:
* how things are visualized. view to change from view space to clip space use coordinat systems
* previously we used local space
* we are going to use projection to make view space which is the position of vertices in world space
* view space position in the world in relation to the camera by mutiplying it by view matrix
* clip space is clipping  out what we dont want to see
* screen space is the final image and projected onto the window itself


frustum is the dined are we want to see. its the truncated pyramid

//***a uniformed variable will be the same instance for every vertex***


//vertex shaders:
//allows us to manipulate the vertices and pass them off to the fragmebnt shader
// using the out keyword lets up output/return a value from the shader to the cpp file
// //i think the vertex is saying to each vertext... whatever your pos is... take the clamped value color of that point



//fragment shader:
//handinling each pixel on the screen and how it works with the vertices
//we dont pass anything directly anything to the vertext shader normally
//we pass it to the vertext shader and the fragment shader picks up the result
        //fShader only has 1 output value normally so whater you call the out varaible it will assume that is your output value
*/
