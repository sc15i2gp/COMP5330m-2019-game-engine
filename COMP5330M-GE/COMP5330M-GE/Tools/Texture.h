#pragma once

#include "Framework/OpenGL.h"
#include "stb/stb_image.h"
#include<string>

/*A texture class which enables us to read image files, and use them in our Game Engine*/
class Texture
{

private:

	int m_numberOfChannels;
	int m_textureWidth;
	int m_textureHeight;

	GLuint m_textureID;
	unsigned char* m_data;

	void loadImageFile(const std::string& path);
	void generateTexture(const std::string& path);

public:

	Texture();
	Texture(const std::string& image);

	void activeTexture();

	GLuint getTextureID() const;
	GLuint getTextureWidth() const;
	GLuint getTextureHieght() const;


	~Texture();
};
