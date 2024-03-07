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
GLuint VAO, VBO, shader, uniformModel;
//not a gint or gfloat because these are not being passed to the shaders
bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.05f;
float currAngle = 0.0f;
float toScale = 0.0f;
//glm::mat4 model(1.0f);
// glm::mat4 model = glm::mat4(1.0f);
// 
// 
//***a uniformed variable will be the same instance for every vertex***
//***the variable (pos) will be passed htrough every vertex***
//vertex shaders:
//allows us to manipulate the vertices and pass them off to the fragmebnt shader
//version of glsl
static const char* vShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
layout (location = 0) in vec3 pos;											  \n\
                                                                              \n\
uniform mat4 model;                                                          \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    gl_Position = model * vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);				  \n\
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

        //move tri
        if (direction)
        {
            triOffset += triIncrement;
        }
        else {
            triOffset -= triIncrement;
        }

        if (abs(triOffset) >= triMaxOffset)
        {
            direction = !direction;
        }
        currAngle += 1.0f;
        if (currAngle >= 360)
        {
            currAngle -= 360;
        }
        toScale += 0.05f;
        if (toScale >= 2)
        {
            toScale = 0.0f;
        }

        //clear window
        glClearColor(0.0f, 0.0f,0.0f,1.0f);//these values are the color/256
        glClear(GL_COLOR_BUFFER_BIT);
        

        glUseProgram(shader);

        //glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 model(1.0f);
        //modifying the model
        //put it back into the model after you changed the model
       // model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
                    // degrees in radians //which axis you want to rotate it on
        //model = glm::rotate(model, currAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        //^the order matters... if these are flipped, it will rotate the world then will be tranlated it around the distored world
        //if the triangle size is being morphed and not consitent it is because we are not using a projection matrix
        model = glm::scale(model, glm::vec3(toScale, toScale, 1.0f));
        //REMEMBER THAT ORDER MATTERS BECAUSE THE TRANSFORMATIONS WILL BE COMPOUNDED AND THE SCALES WILL EFFECT THE TRANSLATIONS AND ROTATIONS


        //assign uniform shader
        //glUniform1f(uniformXMove, triOffset);
        //we dont put it the model directly. we put in the address of the model
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));

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

/*
GLM: gl math library
vectors:
    -magnitude and direction
    -add and subrtact vectores = [x1+x2,y1+y2,z1+z2] or [x1-x2,y1-y2,z1-z2]
    -multiplaying vector of scaler value expands the magnitude... |V| is magnitude
    -dot product: returns a scaler value [x1*x2,y1*y2,z1*z2] or v1 * v2 = |v1|*|v2| * cos(theta)
    -Magnitude:magnitude = sqrt(vx^2 + vy^2 + vz^2)
    -get the unit vector (the normalized value): vector/|v|
    -cross product: finds the right angle of two vectors. find a right angle to both vectors (ordering does matter) helps you also find the normal
matrix
    -group if i X j values
    -i is rows, j is columns
    -we use them to handle model transforms
    -adding and subtracting matrices you just overlap the corresponding values ie v1x+v2x or v1x-v2x
    -multiplying by scaler: multiply each value by the scaler
    -multiplying and the dot product: find the dot product of each row x column... so <(x1 * x1) + (y1 * 2x1) +(z1 * x3)> [this location, 0]
                                                                                                                          [0        ,     0]
                                                                                   so <(x1 * y1) + (y1 * 2y) +(z1 * 3y)>  [0, this location]
                                                                                                                          [0        ,     0]
    -vectors are just matrices with a single column. multiplying matrices by a vector will create a modfied version of that matrx. it will be [(c1*r1)+(c2*r2)+(c3*r3)]
                                                                                                                                              [(2c1*r1)+(2c2*r2)+(2c3*r3)] 
    -tranforms:
        identity matrix: all values are 0 except the the places diagonal starting from the top left to the bottom right. 
        the identity matrix is the starting point for all other transforms. 
        [1,0,0,0]      [x]
        [0,1,0,0]   *  [y]
        [0,0,1,0]      [z]
        [0,0,0,1]      [1]
        translation:translation moves the vector. take the foruth column and put in how far you want to move the matrix and multiply the identity matrix (with those values in the 4 column)
        by your vector.
        [1,0,0,X]      [x]= [x + X]
        [0,1,0,Y]   *  [y]= [y + Y]
        [0,0,1,Z]      [z]= [z + Z]
        [0,0,0,1]      [1]= [1]
        scaling: resize a vector
        [SX,0,0,0]      [x] = [SX * x]
        [0,SY,0,0]   *  [y] = [SY * y]
        [0,0,SZ,0]      [z] = [SZ * z]
        [0,0,0, 1]      [1] = [1]
        rotation: rotating around the origin axis
        x rotation
                [1,0,0,X]                        *  [x] = [x]
                [0, cos(theta), -sin(theta),0]   *  [y] = [cos(theta)*y + sin(theta) * z]
                [0, sin(theta),  cos(theta),0]   *  [z] = [sin(theta)*y + cos(theta) * z]
                [0,0,0,1]                        *  [1]= [1]
        y rotation (did not finish these)
                [1,0,0,X]                        *  [x] = [x]
                [0, cos(theta), -sin(theta),0]   *  [y] = [cos(theta)*x + sing(theta)]
                [0,1,0,0]                        *  [z] = [z + Z]
                [0,0,0,1]                        *  [1]= [1]
        z rotation
                [1,0,0,X]                        *  [x] = [x]
                [0, cos(theta), -sin(theta),0]   *  [y] = [cos(theta)*x + sing(theta)]
                [0,1,0,0]                        *  [z] = [z + Z]
                [0,0,0,1]                        *  [1]= [1]
        combining 
        

        order matters so keep in mind what you are doing. for example when you are applying scale and moving transform make sure you apply the transform first, then the scale
        otherwise you are scaling the transpform not the item.


        unform variables
            variable that is unified and not applied to a particular vertiex. will have a location id in the shader.in order to apply something to the unioform variabel we have t
            get shader id then apply it to the id


        */
