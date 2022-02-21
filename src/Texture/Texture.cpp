#include "Texture.h"
#include <stb/stb_image.h>

Texture::Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType)
{
	// Assigns the type of texture to the texture object
	type = texType;

	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	
	// Stores the width, height, and number of color channels of the image file
	int imageWidth, imageHeight, numColorChannels;
	// Reads the image file and stores in in bytes
	unsigned char* bytes = stbi_load(image, &imageWidth, &imageHeight, &numColorChannels, 0);

	// Checks if texture was be loaded
	if (stbi_failure_reason())
	{
		std::cout << "STBI FAILED! REASON: " << stbi_failure_reason();
		return;
	}
	
	// Generates an OpenGL texture
	glGenTextures(1, &ID);
	// Assigns the texture to a texture unit
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(type, ID);

	// Configures the type of algorithm to resize the image
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configures the way the texture is reapeated (if it does at all)
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/*
	* For GL_CLAMP_TO_BORDERS define a flat color for filling
	*	float flatColor[] = {1.f, 1.f, 1.f, 1.f};
	*	glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, flatColor);
	*/

	// Assigns the image to the OpenGL Texture Object
	glTexImage2D(type, 0, GL_RGBA, imageWidth, imageHeight, 0, format, pixelType, bytes);

	// Deletes the image data, since it is already in the OpenGL Texture Object
	stbi_image_free(bytes);

	// Unbinds the OpenGL Texture Object so that it isn't accidentally modified
	glBindTexture(type, 0);

	// Generate MipMap of the current Texture
	glGenerateMipmap(type);
}

Texture::~Texture() {}

void Texture::texUnit(Shader* shader, const char* uniform, GLuint unit)
{
	// Gets the uniform's location
	GLuint texUni = glGetUniformLocation(shader->ID, uniform);
	// Activates Shader to change the value of a uniform
	shader->Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	// Assigns the texture to a texture unit
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, ID);
}


void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}