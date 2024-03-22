#include "Shader.h"

Shader::Shader()
{
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
    spotLightCount = 0;
    pointLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode,fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
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
    return uniformProjection;
}
GLuint Shader::GetModelLocation()
{
    return uniformModel;
}
GLuint Shader::GetViewLocation()
{
    return uniformView;
}
GLuint Shader::GetAmbientColourLocation()
{
    return uniformDirectionalLight.uniformColour;
}
GLuint Shader::GetAmbientIntensityLocation()
{
    return uniformDirectionalLight.uniformAmbientIntensity;
}
GLuint Shader::GetDiffuseIntensityLocation()
{
    return uniformDirectionalLight.uniformDiffuseIntensity;
}
GLuint Shader::GetDirectionLocation()
{
    return uniformDirectionalLight.uniformDirection;
}
GLuint Shader::GetSpecularIntensityLocation()
{
    return uniformSpecularIntensity;
}
GLuint Shader::GetShininessLocation()
{
    return uniformShininess;
}
GLuint Shader::GetEyePositionLocation()
{
    return uniformEyePosition;
}

void Shader::SetPointLights(PointLight* pLights, unsigned int lightCount)
{
    if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

    glUniform1i(uniformPointLightCount, lightCount);

    //each spotlight will have different uniform variables but the vertices on the spotlight will have the same uniform vaibles
    for (size_t i = 0; i < lightCount; i++)
    {
        pLights[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour,
            uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
            uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
    }
}

void Shader::SetSpotLights(SpotLight* sLights, unsigned int lightCount)
{
    if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

    glUniform1i(uniformSpotLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++)
    {
        sLights[i].UseLight(uniformSpotLights[i].uniformAmbientIntensity, uniformSpotLights[i].uniformColour,
            uniformSpotLights[i].uniformDiffuseIntensity, uniformSpotLights[i].uniformPosition, uniformSpotLights[i].uniformDirection,
            uniformSpotLights[i].uniformConstant, uniformSpotLights[i].uniformLinear, uniformSpotLights[i].uniformExponent, 
            uniformSpotLights[i].uniformEdge);
    }

}

void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
    dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColour,
        uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
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
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformView = glGetUniformLocation(shaderID, "view");
    uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", i);
        uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

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

    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        char locBuff[100] = { '\0' };
        
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.colour", i);
        uniformSpotLights[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
        uniformSpotLights[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
        uniformSpotLights[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
        uniformSpotLights[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
        uniformSpotLights[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
        uniformSpotLights[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
        uniformSpotLights[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
        uniformSpotLights[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
        uniformSpotLights[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
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
