#version 330                                                                  
                                                                              
layout (location = 0) in vec3 pos;	
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 vertexColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;                                                          
uniform mat4 worldProjection;    
uniform mat4 view;
                                                                              
void main()                                                                   
{                                                                             
    vertexColor = vec4(clamp(pos,0.0f,1.0f),1);                                   
    gl_Position = worldProjection * view * model * vec4(pos, 1.0);		
    TexCoord = tex;
    Normal = mat3(transpose(inverse(model))) * norm;

    FragPos = (model * vec4(pos, 1.0)).xyz;
}