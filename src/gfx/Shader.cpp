#include "Shader.hpp"

#include <set>
#include <stdexcept>
#include <utility>

#include <glm/gtc/type_ptr.hpp>

#include "util/FileUtils.hpp"
#include "util/Logger.hpp"

Shader::Shader() = default;

Shader::~Shader()
{
	if (m_programID != 0)
	{
		glDeleteProgram(m_programID);
	}
}

Shader::Shader(Shader&& other) noexcept : m_programID(std::exchange(other.m_programID, 0))
{
}

Shader& Shader::operator=(Shader&& other) noexcept
{
	if (this != &other)
	{
		if (m_programID != 0)
		{
			glDeleteProgram(m_programID);
		}
		m_programID = std::exchange(other.m_programID, 0);
	}
	return *this;
}

void Shader::compileFromPath(const std::filesystem::path& computePath, const std::vector<std::string>& defines)
{
	std::set<std::string> includedFiles;
	auto optionalComputeCode = FileUtils::loadShaderSource(computePath);
	if (!optionalComputeCode)
	{
		throw std::runtime_error(std::format("Failed to load shader source for path: {}", computePath.string()));
	}

	std::string computeCode = *optionalComputeCode;

	size_t versionPos = computeCode.find("#version");
	if (versionPos == std::string::npos)
	{
		throw std::runtime_error("Shader must have a #version directive!");
	}
	size_t lineEnd = computeCode.find('\n', versionPos);

	std::string defineBlock;
	for (const auto& def : defines)
	{
		defineBlock += std::format("#define {}\n", def);
	}
	computeCode.insert(lineEnd + 1, defineBlock);

	const char* computeCodeCStr = computeCode.c_str();

	GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, &computeCodeCStr, nullptr);
	glCompileShader(computeShader);
	checkCompileErrors(computeShader, "COMPUTE");

	m_programID = glCreateProgram();
	glAttachShader(m_programID, computeShader);
	glLinkProgram(m_programID);
	checkCompileErrors(m_programID, "PROGRAM");

	glDeleteShader(computeShader);
}

void Shader::use() const
{
	glUseProgram(m_programID);
}

void Shader::bindUBO(std::string_view blockName, GLuint bindingPoint)
{
	GLuint blockIndex = glGetUniformBlockIndex(m_programID, blockName.data());
	if (blockIndex == GL_INVALID_INDEX)
	{
		return;
	}
	glUniformBlockBinding(m_programID, blockIndex, bindingPoint);
}

void Shader::checkCompileErrors(GLuint shader, std::string_view type)
{
	GLint success{};
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			throw std::runtime_error(std::format("Shader compilation error of type: {}\n{}", type, infoLog));
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			throw std::runtime_error(std::format("Program linking error of type: {}\n{}", type, infoLog));
		}
	}
}

void Shader::setBool(std::string_view name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_programID, name.data()), static_cast<int>(value));
}

void Shader::setInt(std::string_view name, int value) const
{
	glUniform1i(glGetUniformLocation(m_programID, name.data()), value);
}

void Shader::setFloat(std::string_view name, float value) const
{
	glUniform1f(glGetUniformLocation(m_programID, name.data()), value);
}

void Shader::setDouble(std::string_view name, double value) const
{
	glUniform1d(glGetUniformLocation(m_programID, name.data()), value);
}

void Shader::setVec2(std::string_view name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(m_programID, name.data()), 1, glm::value_ptr(value));
}

void Shader::setVec2(std::string_view name, const glm::dvec2& value) const
{
	glUniform2dv(glGetUniformLocation(m_programID, name.data()), 1, glm::value_ptr(value));
}

void Shader::setVec3(std::string_view name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(m_programID, name.data()), 1, glm::value_ptr(value));
}

void Shader::setVec3(std::string_view name, const glm::dvec3& value) const
{
	glUniform3dv(glGetUniformLocation(m_programID, name.data()), 1, glm::value_ptr(value));
}
