#pragma once

#include <string>

#include <glad/glad.h>

class QuadRenderer
{
	public:
		QuadRenderer();
		~QuadRenderer();

		QuadRenderer(const QuadRenderer&) = delete;
		QuadRenderer& operator=(const QuadRenderer&) = delete;

		void render(GLuint textureID);

	private:
		void checkCompileErrors(GLuint shader, const std::string& type);

		GLuint m_programID = 0;
		GLuint m_vao = 0;
		GLuint m_vbo = 0;
};
