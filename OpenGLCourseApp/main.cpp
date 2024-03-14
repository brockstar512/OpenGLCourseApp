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
#include "Material.h"

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

//Material
Material shinyMat;
Material dullMat;

//vertex shader
static const char* vShader = "Shaders/shader.vert";
//fragment shader
static const char* fShader = "Shaders/shader.frag";
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 728;

void CreateShader()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFile(vShader,fShader);
    shaderList.push_back(shader1);
}

void CalculateAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticesCount, unsigned int vertexLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vertexLength;
        unsigned int in1 = indices[i + 1] * vertexLength;
        unsigned int in2 = indices[i + 2] * vertexLength;

        //we are making manual lines to vertices so we can use them to calculate the cross product to find the normals
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        //the n1s and n0s are pointing to the start of each rows in vertices... we are adding a normal offset to get the normal of that row
        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;

        //adding the normals in the respectvie place.
        vertices[in0] += normal.x;
        vertices[in0 + 1] += normal.y;
        vertices[in0 + 2] += normal.z;

        vertices[in1] += normal.x;
        vertices[in1 + 1] += normal.y;
        vertices[in1 + 2] += normal.z;

        vertices[in2] += normal.x;
        vertices[in2 + 1] += normal.y;
        vertices[in2 + 2] += normal.z;

        for (size_t i = 0; i < verticesCount / vertexLength; i++)
        {
            //grabbing all the normal offsets
            unsigned int nOffset = i * vertexLength + normalOffset;
            //normalizing them
            glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
            vec = glm::normalize(vec);
            //putting them back in but normalized
            vertices[nOffset] = vec.x;
            vertices[nOffset + 1] = vec.y;
            vertices[nOffset + 2] = vec.z;
        }
    }
}

void CreateObject()
{

    //THE INDICES IS REFERING THE LINE IN THE VERTICES SO 0 IS -1.0f,-1.0f,0.0f AND 3 is 0.0f,1.0f,0.0f
    unsigned int indices[] = {
        0,3,1,
        1,3,2,
        2,3,0,
        0,1,2
    };

    //define points of VAOs
    GLfloat vertices[] = {
        // vertext coordinates
        //                      texture coordinates     normals
        //x     y   z           u   v                   nx, ny nz
        -1.0f, -1.0f, -0.5f,     0.0f, 0.0f,             0.0f,0.0f,0.0f,
        0.0f, -1.0f, 1.0f,      0.5f, 0.0f,             0.0f,0.0f,0.0f,
        1.0f, -1.0f, -0.5f,      1.0f, 0.0f,             0.0f,0.0f,0.0f,
        0.0f, 1.0f, 0.0f,       0.5f, 1.0f,             0.0f,0.0f,0.0f,
    };

    CalculateAverageNormals(indices, 12, vertices,32,8,5);

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj2);

}

int main()
{

    window = MyWindow(WIDTH,HEIGHT);
    window.Init();

    //create triangle
    CreateObject();
    //compile shader
    CreateShader();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.075f);

    //create the textures
    brickTexture = Texture("Textures/brick.png");
    brickTexture.LoadTexture();
    dirtTexture = Texture("Textures/dirt.png");
    dirtTexture.LoadTexture();

    shinyMat = Material(1.0f, 32);
    dullMat = Material(0.3f, 4);

    //mainLight = Light(1.0f,1.0f,1.0f,0.5f);
    mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f,2.0f, -1.0f, -2.0f, 0.3f);

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformAmbientIntensity = 0, uniformAmbientColour = 0,uniformDiffusionIntensity = 0, uniformDirection = 0, uniformEyePos, uniformSpecularIntensity, uniformShinyIntesity;

    //loop until window closed
    glm::mat4 projection = glm::perspective(45.0f, window.GetBufferWidth() / window.GetBuggerHeight(), 0.1f, 100.0f);
    while (!window.getShouldClose())
    {

        //getting deltatime
        GLfloat now = glfwGetTime(); 
        deltaTime = now - lastTime; 
        lastTime = now;

        //get and handle user input events
        glfwPollEvents();

        camera.KeyControl(window.GetKeys(), deltaTime);
        camera.MouseControl(window.GetXChange(), window.GetYChange());


        //clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glUseProgram(shader);
        shaderList[0]->UseShader();

        uniformModel = shaderList[0]->GetModelLocation();
        uniformProjection = shaderList[0]->GetProjectionLocation();
        
        uniformView = shaderList[0]->GetViewLocation();
        uniformAmbientIntensity = shaderList[0]->GetAmbientIntensity();
        uniformAmbientColour = shaderList[0]->GetAmbientColorLocation();
        uniformDiffusionIntensity = shaderList[0]->GetDiffuseIntensityLocation(),
        uniformDirection = shaderList[0]->GetDirectionLocation();
        uniformEyePos = shaderList[0]->GetEyeLocation();
        uniformShinyIntesity = shaderList[0]->GetShininessLocation();
        uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
        //        mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour);
        mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour,uniformDiffusionIntensity,uniformDirection);

        glm::mat4 model(1.0f);
        //modifying the model

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.5f));
        //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));


        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
        glUniform3f(uniformEyePos,camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);


        //we will draw the texture onto whatever texture is being references here before we render the texture
        brickTexture.UseTexture();
        shinyMat.UseMaterial(uniformSpecularIntensity, uniformShinyIntesity);
        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 2.25f, -3.5f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.UseTexture();
        dullMat.UseMaterial(uniformSpecularIntensity, uniformShinyIntesity);
        meshList[1]->RenderMesh();

        //this is where you unassign the shader
        glUseProgram(0);

        //there is two buffers... one you can see and one you are drawing to
        window.SwapBuffers();
    }

    //std::cout << "Hello World!\n";

}

    //vertex shader vs fragment shader... the program passes data to the vertex shader then the vertex shader passes data to the fragment shader

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
                        //specular lighting is how the light reflects off the material.

    //type of light: 
    // directional light: light without position or source. all light coming as parralel. often represents the sun
    // point light: light bulb. shines in all direction from a specific position.
    // spot light:light emittied from a specific point with a certain range and direction
    // area light: large light area that emits from a specific area


    //location vs uniform : uniform all vertex varaibles will be the same, location, the vertex variables will be individual
/*
    in one draw call, you set a uniform variable to a value that is the same for all vertices/fragments you render during this call. E.g. the position of a light, or the view_projection_matrix.

    However, you probably want for every vertex different vertex positions and maybe different texture coordinates etc. For these cases you use for example layout (location = 0) in vec3 aPos;*/

    




