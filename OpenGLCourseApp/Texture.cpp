#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

bool Texture::LoadTexture()
{
	unsigned char* textureData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	if (!textureData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}
	//generate texture. applying an id to reference for it
	glGenTextures(1, &textureID);
	//bind 2d texture type to the generated texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	//s: how it will handle it during the left or right
	//t: how it will handle in when wrapping around the up and down
	///how it will handle if you go over the boarder
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//how to handle the minimap when zooming in or going away
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//these seem like we are passingin which function we want to access as an enum then passing in the arguments but first we have to pass in the type of texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//sets up image data from texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	//free up the raw data since we don't need it anymore
	stbi_image_free(textureData);

	return true;
}

bool Texture::LoadTextureWithAlpha()
{
	unsigned char* textureData = stbi_load(fileLocation, &width, &height, &bitDepth,0);

	if (!textureData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}
	//generate texture. applying an id to reference for it
	glGenTextures(1,&textureID);
	//bind 2d texture type to the generated texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	//s: how it will handle it during the left or right
	//t: how it will handle in when wrapping around the up and down
	///how it will handle if you go over the boarder
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//how to handle the minimap when zooming in or going away
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	//these seem like we are passingin which function we want to access as an enum then passing in the arguments but first we have to pass in the type of texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//sets up image data from texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D,0);

	//free up the raw data since we don't need it anymore
	stbi_image_free(textureData);

	return true;
}




void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	height = 0;
	width = 0;
	bitDepth = 0;
	fileLocation = "";
}

void Texture::UseTexture()
{
	//we are telling the gpu to use this texture to bind to whatever object the gpu is drawing

	//this is referencing the sampler get unit 0
	glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_2D, textureID);
}

Texture::~Texture()
{
}
