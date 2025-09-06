#include "Texture.hpp"

#include <utility>

Texture::Texture(int width, int height) : m_width(width), m_height(height)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

Texture::~Texture()
{
	if (m_textureID != 0)
	{
		glDeleteTextures(1, &m_textureID);
	}
}

Texture::Texture(Texture&& other) noexcept
	: m_textureID(std::exchange(other.m_textureID, 0)), m_width(std::exchange(other.m_width, 0)),
	  m_height(std::exchange(other.m_height, 0))
{
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	if (this != &other)
	{
		if (m_textureID != 0)
		{
			glDeleteTextures(1, &m_textureID);
		}
		m_textureID = std::exchange(other.m_textureID, 0);
		m_width = std::exchange(other.m_width, 0);
		m_height = std::exchange(other.m_height, 0);
	}
	return *this;
}

void Texture::bind(GLuint unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::bindImage(GLuint unit) const
{
	glBindImageTexture(unit, m_textureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
}

void Texture::resize(int newWidth, int newHeight)
{
	if (m_width == newWidth && m_height == newHeight)
	{
		return;
	}

	m_width = newWidth;
	m_height = newHeight;

	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}
