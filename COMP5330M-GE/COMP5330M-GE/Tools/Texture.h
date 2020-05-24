#pragma once

#include "Framework/OpenGL.h"
#include<string>

/*A texture class which enables us to read image files, and use them in our Game Engine*/
class Texture
{

private:

	GLuint m_textureWidth;
	GLuint m_textureHeight;

	GLuint m_textureID;

public:

	Texture(const std::string& filePath);

	void activeTexture();

	GLuint getTextureID() const;
	GLuint getTextureWidth() const;
	GLuint getTextureHieght() const;


	~Texture();
};
