#include "Mesh.h"
#include <stdio.h>
void Mesh::CreateMesh(GLfloat *vertices, unsigned *indeces, unsigned numVertices, unsigned int numOfIndecies)
{
	indexCount = numOfIndecies;

    //create the vao - ammount of array, where we want to store the address/id of the arrays
    glGenVertexArrays(1, &VAO);
    //bind the vertext array
    glBindVertexArray(VAO);

    //create buffer for the indeices
    glGenBuffers(1, &IBO);
    //bind the buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    //we want the buffer element to bind and the size of the indeces
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces[0]) * numOfIndecies, indeces, GL_STATIC_DRAW);


    //create the buffer on the graphics card
    glGenBuffers(1, &VBO);
    //enter which buffer which is the array buffer and pick which buffer by the id
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //connect buffer data.. the vertices to the array
                              //pass in the size of the array//pass in vertices, pass in how we are going to be interacting the triangle. we are not going to be redrawing it so we are sticking with DL_Static draw
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])* numVertices, vertices, GL_STATIC_DRAW);
    //location         //which value, how many values, what kind of value,normalize, stride, offest for starting value
    // //stride: i think that means splicing data with addiotnal data cause you can data pack by making odds a vluae for one things and even value means another
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //we want to enable an array usage to work
    glEnableVertexAttribArray(0);
    //binding the array to nothing
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //unbind the IBO/EBO after you unbind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //unbinding the array
    glBindVertexArray(0);

    //if you dont unbind the buffer the next time you edit the buffer it could be clashin/interfereing with the previous drawn buffer
}

void Mesh::RenderMesh()
{
    //this is where you can use the shader

    glBindVertexArray(VAO);

    //we are not drawing the arrays anymore
    //glDrawArrays(GL_TRIANGLES,0,3);

    //bind ibo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    //draw the element not based on arrays but based on the element ids
                               //count of indeces//the size of type//because we have bound the slot to element array buffer already we can put 0 or nullptr
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    //unbind
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    glBindVertexArray(0);

}

void Mesh::ClearMesh()
{
    if (IBO != 0)
    {
        //delete buffer
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }
    if (VBO != 0)
    {
        //delete buffer
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (VAO != 0)
    {
        //delete buffer
        glDeleteBuffers(1, &VAO);
        VAO = 0;
    }
    indexCount = 0;
}

Mesh::~Mesh()
{
    ClearMesh();
    printf("Destructor called");
}
