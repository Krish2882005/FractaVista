#pragma once

#include <glad/glad.h>

class Texture
{
	public:
		Texture(int width, int height);
		~Texture();

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		void bind(GLuint unit = 0) const;
		void bindImage(GLuint unit) const;

		GLuint getID() const { return m_textureID; }

	private:
		GLuint m_textureID = 0;
};
