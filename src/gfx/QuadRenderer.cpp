#include "QuadRenderer.hpp"

#include <stdexcept>

#include "utils/Logger.hpp"

QuadRenderer::QuadRenderer()
{
	const char* vertSrc = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoords;
        out vec2 TexCoords;
        void main() {
            TexCoords = aTexCoords;
            gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
        }
    )";
	const char* fragSrc = R"(
        #version 330 core
        out vec4 FragColor;
        in vec2 TexCoords;
        uniform sampler2D screenTexture;
        void main() {
            FragColor = texture(screenTexture, TexCoords);
        }
    )";

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertSrc, NULL);
	glCompileShader(vertShader);
	checkCompileErrors(vertShader, "VERTEX");

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSrc, NULL);
	glCompileShader(fragShader);
	checkCompileErrors(fragShader, "FRAGMENT");

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertShader);
	glAttachShader(m_programID, fragShader);
	glLinkProgram(m_programID);
	checkCompileErrors(m_programID, "PROGRAM");

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	float quadVertices[] = { -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,

							 -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,	 -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,	 1.0f, 1.0f };

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
}

QuadRenderer::~QuadRenderer()
{
	if (m_programID != 0)
		glDeleteProgram(m_programID);
	if (m_vbo != 0)
		glDeleteBuffers(1, &m_vbo);
	if (m_vao != 0)
		glDeleteVertexArrays(1, &m_vao);
}

void QuadRenderer::render(GLuint textureID)
{
	glUseProgram(m_programID);
	glUniform1i(glGetUniformLocation(m_programID, "screenTexture"), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void QuadRenderer::checkCompileErrors(GLuint shader, const std::string& type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			Logger::Error("Shader compilation error of type: ", type, "\n", infoLog);
			throw std::runtime_error("Shader compilation failed.");
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			Logger::Error("Program linking error of type: ", type, "\n", infoLog);
			throw std::runtime_error("Shader program linking failed.");
		}
	}
}
