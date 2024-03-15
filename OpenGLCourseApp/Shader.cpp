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
    return uniformDirectionLight.uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColorLocation()
{
    return uniformDirectionLight.uniformColor;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
    return uniformDirectionLight.uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation()
{
    return uniformDirectionLight.uniformDirection;
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

void Shader::SetPointLights(PointLight* pLights, unsigned int lightCount)
{
    if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

    glUniform1i(uniformPointLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++)
    {
        pLights[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor,
            uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
            uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
    }
}

void Shader::SetDirectionLight(DirectionalLight* dLight)
{
    dLight->UseLight(uniformDirectionLight.uniformAmbientIntensity, uniformDirectionLight.uniformColor, uniformDirectionLight.uniformDiffuseIntensity, uniformDirectionLight.uniformDirection);
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

    //we get the address when the program first louds up
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = glGetUniformLocation(shaderID, "view");
    uniformDirectionLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.colour");
    uniformDirectionLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    uniformDirectionLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformDirectionLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
    uniformPointLightCount = glGetUniformLocation(shaderID,"pointLightCount");

    //we will have multiple point lights
    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char locBuff[100] = { '\0' };
        //getting the point light we are looking at...and grabbign that specific point lights color
        //getting the id of every point light uniform color in the shader
        //get every items location

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", i);
        uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);


    }
 
}



void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    //create the shader based on the type. returns ID for shader on gpu
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
