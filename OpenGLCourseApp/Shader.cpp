#include "Shader.h"

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode,fragmentCode);
}

void Shader::CreateFromFile(const char* vertexLocation, const char* fragmentLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);

    const char* vertextCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();
    CompileShader(vertextCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open())
    {
        printf("fail to read %s! File doesn't exist ",fileLocation);
        return "";
    }
    std::string line = "";
    while (!fileStream.eof())
    {
        
        std::getline(fileStream, line);
        
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

GLuint Shader::GetProjectionLocation()
{
    return uniformProjection;;
}

GLuint Shader::GetModelLocation()
{
    return uniformModel;
}

GLuint Shader::GetViewLocation()
{
    return uniformView;
}

GLuint Shader::GetAmbientIntensity()
{
    return uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColorLocation()
{
    return uniformAmbientColor ;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
    return uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation()
{
    return uniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation()
{
    return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation()
{
    return uniformShininess;
}

GLuint Shader::GetEyeLocation()
{
    return uniformEyePosition;
}

void Shader::UseShader()
{
    glUseProgram(shaderID);
}

void Shader::ClearShader()
{
    if (shaderID != 0) {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
    uniformModel = 0;
    uniformProjection = 0;
}

Shader::~Shader()
{
    ClearShader();
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    shaderID = glCreateProgram();

    if (!shaderID)
    {
        printf("error creating shadert program");
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);

    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);


    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    //create the exe on the graphics cards
    //link them 
    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error Linking Program: '%s'\n", eLog);
        return;
    }
    //now validate them
    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error Validating Program: '%s'\n", eLog);
        return;
    }
    //__GRAB VALUES FROM SHADERS___

    //get location/id of uniform variable
    uniformModel = glGetUniformLocation(shaderID, "model");
    //get the address/id of the worldProjection and place it in the uniformProjection variable
    uniformProjection = glGetUniformLocation(shaderID, "worldProjection");//this is to get the order when vertices are ontop if eachother
    //we get the uniform view from the shader from view.... the view variable is the uniform variable we place inthe shader.vert file
    uniformView = glGetUniformLocation(shaderID, "view");

    //if there is a problem with the rendering and the log does not say so, there is a high chance it has to do here, where the string literal is misspelled
    uniformAmbientColor = glGetUniformLocation(shaderID, "directionalLight.colour");

    uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");

    uniformDirection = glGetUniformLocation(shaderID,"directionalLight.direction");

    uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.diffuseIntensity");

    uniformSpecularIntensity = glGetUniformLocation(shaderID,"material.specularIntensity");

    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
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
