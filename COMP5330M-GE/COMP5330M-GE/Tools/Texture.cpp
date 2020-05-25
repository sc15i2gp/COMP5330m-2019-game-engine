#include "Texture.h"

void Texture::loadImageFile(const std::string & path)
{
	m_data = stbi_load("Textures/water.png", &m_textureWidth, &m_textureHeight, &m_numberOfChannels, 0);
	OutputDebugStringA(std::to_string(m_textureHeight).c_str());
	OutputDebugStringA(std::to_string(m_textureWidth).c_str());
	OutputDebugStringA(std::to_string(m_numberOfChannels).c_str());
}

void Texture::generateTexture(const std::string& path)
{
	glGenTextures(1, &m_textureID);

	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	loadImageFile(path);

	if (m_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_textureWidth, m_textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		OutputDebugStringA("Failed to load texture image!");
	}

	stbi_image_free(m_data);
}

Texture::Texture(const std::string & filePath)
{
	generateTexture(filePath);
}

void Texture::activeTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

GLuint Texture::getTextureID() const
{
	return m_textureID;
}

GLuint Texture::getTextureWidth() const
{
	return m_textureWidth;
}

GLuint Texture::getTextureHieght() const
{
	return m_textureHeight;
}

Texture::~Texture()
{
}
