#version 330                                                                  
in vec4 vertexColor;                                                                 
out vec4 colour;                                                               
in vec2 TexCoord;

uniform sampler2D theTexture;
void main()                                                                   
{                                                                             
    colour = texture(theTexture, TexCoord) * vertexColor;                                                       
}