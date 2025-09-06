#pragma once

#include <glad/gl.h>

class Texture
{
	public:
		Texture(int width, int height);
		~Texture();

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;

		void bind(GLuint unit = 0) const;
		void bindImage(GLuint unit) const;
		void resize(int newWidth, int newHeight);

		[[nodiscard]] GLuint getID() const { return m_textureID; }
		[[nodiscard]] int getWidth() const { return m_width; }
		[[nodiscard]] int getHeight() const { return m_height; }

	private:
		GLuint m_textureID = 0;
		int m_width = 0;
		int m_height = 0;
};
