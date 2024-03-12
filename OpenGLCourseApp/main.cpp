// OpenGLCourseApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define STB_IMAGE_IMPLEMENTATION
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
#include "Texture.h"
#include "Light.h"
Camera camera;
const float toRadians = 3.14159265f / 180.0f;
GLuint shader;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
MyWindow window;

//textures
Texture brickTexture;
Texture dirtTexture;

//light
Light mainLight;

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
    /*
    0,0 is the bottom left of the texture
    1,0 is the bottom right of the texture
    0,1 is the top left of the texture
    1,1 is the top right of the texture
    //.5 is going to be the center so we are cutting out a tringle 
    */
    GLfloat vertices[] = {
        // vertext coordinates
        //                      texture coordinates
        //x     y   z           u   v
        -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
        0.0f, -1.0f, 1.0f,      0.5f, 0.0f,
        1.0f, -1.0f, 0.0f,      1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,       0.5f, 1.0f,
    };
    /*
    //define points of VAOs
    //i think each of them are the vertex buffers
    //but we are packing additional attributes above
    // we are now adding textures and telling the attribute pointer where to read them based off the index of the array
    //so we can't use this anymore
    GLfloat vertices[] = {
        -1.0f,-1.0f,0.0f,
        0.0f,-1.0f,1.0f, 
        1.0f,-1.0f,0.0f,
        0.0f,1.0f,0.0f
    };
    */



    //center of screen is 0,0 in opengl

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 20, 12);
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 20, 12);
    meshList.push_back(obj2);

}


int main()
{

    window = MyWindow(800, 600);
    window.Init();

    //create triangle
    CreateObject();
    //compile shader
    CreateShader();
    //camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 90.0f, 0.0f, 5.0f, 0.1f);//this was making it go crazy... the world up was inverted and the yaw was inverted

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.075f);

    //create the textures
    brickTexture = Texture("Textures/brick.png");
    brickTexture.LoadTexture();
    dirtTexture = Texture("Textures/dirt.png");
    dirtTexture.LoadTexture();

    mainLight = Light(1.0f,1.0f,1.0f,0.5f);

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformAmbientIntensity = 0, uniformAmbientColour = 0;

    //loop until window closed
    glm::mat4 projection = glm::perspective(45.0f, window.GetBufferWidth() / window.GetBuggerHeight(), 0.1f, 100.0f);
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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//these values are the color/256
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//now it's clearing both the depth and buffer bit via the bitwaise p[erator


        glUseProgram(shader);
        shaderList[0]->UseShader();

        uniformModel = shaderList[0]->GetModelLocation();
        uniformProjection = shaderList[0]->GetProjectionLocation();
        
        uniformView = shaderList[0]->GetViewLocation();
        uniformAmbientIntensity = shaderList[0]->GetAmbientIntensity();
        uniformAmbientColour = shaderList[0]->GetAmbientColorLocation();

        mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour);
        //glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 model(1.0f);
        //modifying the model

        //get the uniform variabel here (1)
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        //pass in the variable
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
        //we will draw the texture onto whatever texture is being references here before we render the texture
        brickTexture.UseTexture();

        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.UseTexture();
        meshList[1]->RenderMesh();



        //this is where you unassign the shader
        glUseProgram(0);

        //there is two buffers... one you can see and one you are drawing to
        window.SwapBuffers();


    }

    //std::cout << "Hello World!\n";

}


    //projection matrix: matrix that converts items into a normalized matrix that determines what is rendered and what is culled (our vertex positions are multiplied by our projection matrix and thats what gives us the normliazed value)

    //opengl: everythihng you assign in open gl will recieve an id for objects stored on gpu. opengl is a graphics plugin api used to interface with the gpu while sdl is a framework.sdl offers access to audio, keyboard, and mouse controlls.GLFW is a utility library for opengl that allows for the things offered by SDL like window management and keyboard/mouses inputs. glew just helps to figure out which version of OpenGL and which extensions are supported and loads their function pointers so you can easily use them.

    //vertex array object (VAOS): VAOs -is an object which contains one or more Vertex Buffer Objects and is designed to store the information for a complete rendered object. 

    //vertex buffer objects (VBOS): a buffer of memory in our gpus video ram. we are going to reserve the mrmory on our device so that we can refence it. once we create a buffer, we bind it (seletced it to work onit), tell it how big the buffer should be to reserve the data for the object, then we draw to it in the loop...VBOS Store the “contents” of vertex attribute or index arrays

    //VAO vs VBO: A VBO is a buffer of memory which the gpu can access. That's all it is. A VAO is an object that stores vertex bindings. This means that when you call glVertexAttribPointer and friends to describe your vertex format that format information gets stored into the currently bound VAO.

    //vertex attribute(this is important for the function: glVertexAttribPointer in mesh): what is contained in the bvertex buffer... what is in it and how its laid/sliced out because we are packing in our buffer. Vertex attribute is a property of a vertex, e.g. its position, colour, texture coordinate, etc. Vertex attributes specify where and how each property is stored in a vertex buffer, so a shader can access them properly.

    //index buffer objects (IBO): (think of a sqaure how a sqaure is two triangles and the twotringles will be using the inside touching vertexs) The concept of Index buffers is that certain vertices are shared across faces, hence can be reused to save space of duplicated verticies.

    //uniforms: A uniform is a global Shader variable declared with the "uniform" storage qualifier. These act as parameters that the user of a shader program can pass to that program.

    //texel: texture pixel on a texture map

    //mipmap: resolotuion limitations for textures. some textures are not intended to be view that close up, so mipmapping determines how far away you are and which size version of that calculated texture to use.

    //shader: code that runs on the GPU.

    //sampler: textures in shaders are accessed via samplers. textures are attached to a texture unit and have an id. the sampler access ther texture unit by the id.

    //phong lighting: These combined create phong lighting model
            //ambient lighting: the lighting that is always present.
            //diffuse lighting: light determined by direction of light soruce. creates faded effect further away from light source.
            //specular lighting: light reflection from the source to the viewers eye.

    //diffuse lighting: cross product between the normal and the light source to determine the lighting. higher the theta the dimmer the diffuse factor
    
    //phong shading: average of the normals on each vector with the use of interpoloation. we can use normalize matrix to scale the normls to prevent skewing

    //specular lighting: we need four things to show reflection... view vector (what angle are we viewing the shape) reflection vector (light vector reflected around normal), normal vector, light vector. we use the light source and the normal to determine the light reflection. then we can get the vewier and determine the theat between the viewer and the light reclection  angle. smaller theta more light. higher theat less light. the specular factor is the dot product to the exponent facor of the shinienss. the shiniess will be kept by the material

    //type of light: 
    // directional light: light without position or source. all light coming as parralel. often represents the sun
    // point light: light bulb. shines in all direction from a specific position.
    // spot light:light emittied from a specific point with a certain range and direction
    // area light: large light area that emits from a specific area

    




