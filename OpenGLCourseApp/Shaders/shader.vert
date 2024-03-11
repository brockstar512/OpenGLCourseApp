#version 330                                                                  
                                                                              
layout (location = 0) in vec3 pos;											  
out vec4 vertexColor;                                                        
uniform mat4 model;                                                          
uniform mat4 worldProjection;    
uniform mat4 view;
                                                                              
void main()                                                                   
{                                                                             
    vertexColor = vec4(clamp(pos,0.0f,1.0f),1);                                   
    gl_Position = worldProjection * view * model * vec4(pos, 1.0);				                        
}