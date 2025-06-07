#include "Shader.hpp"

#include <fstream>
#include <sstream>

#include "utils/Logger.hpp"

Shader::Shader() = default;

Shader::~Shader()
{
	if (m_programID != 0)
	{
		glDeleteProgram(m_programID);
	}
}

std::string Shader::loadSource(const std::string& path, std::set<std::string>& includedFiles, int depth)
{
	if (includedFiles.count(path))
	{
		return ""; // Already included, prevent recursion
	}
	includedFiles.insert(path);

	std::ifstream file(path);
	if (!file.is_open())
	{
		Logger::Error("Could not open shader file: ", path);
		return "";
	}

	std::stringstream ss;
	std::string line;
	while (getline(file, line))
	{
		// Skip #version directives in any included files (non-top-level)
		if (depth > 0 && line.find("#version") != std::string::npos)
		{
			continue;
		}

		size_t startPos = line.find_first_not_of(" \t");

		// Check if the line, after trimming, starts with #include
		if (startPos != std::string::npos && line.substr(startPos).rfind("#include", 0) == 0)
		{
			// It's an include directive. Extract the path from between the quotes.
			size_t firstQuote = line.find('"', startPos);
			size_t lastQuote = line.find('"', firstQuote + 1);

			if (firstQuote != std::string::npos && lastQuote != std::string::npos)
			{
				std::string includePath = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
				std::string currentDir = path.substr(0, path.find_last_of("/\\"));

				// Recursively load the included file's content. The returned string
				// already has newlines, so we append it directly.
				ss << loadSource(currentDir + "/" + includePath, includedFiles, depth + 1);
			}
		}
		else
		{
			// It's a regular line of code. Append it and a newline character.
			ss << line << '\n';
		}
	}
	return ss.str();
}

void Shader::compileFromPath(const std::string& computePath, const std::vector<std::string>& defines)
{
	std::set<std::string> includedFiles;
	std::string computeCode = loadSource(computePath, includedFiles, 0);

	std::string versionLine;
	std::string codeWithoutVersion;

	// Extract #version directive to ensure it's always at the top
	size_t versionPos = computeCode.find("#version");
	if (versionPos != std::string::npos)
	{
		size_t lineEnd = computeCode.find('\n', versionPos);
		versionLine = computeCode.substr(versionPos, lineEnd - versionPos + 1);
		codeWithoutVersion = computeCode.substr(lineEnd + 1);
	}
	else
	{
		throw std::runtime_error("Shader must have a #version directive!");
	}

	// Build final source with defines injected after #version
	std::stringstream finalSource;
	finalSource << versionLine;
	for (const auto& def : defines)
	{
		finalSource << "#define " << def << "\n";
	}
	finalSource << codeWithoutVersion;

	std::string finalCodeStr = finalSource.str();
	const char* c_str = finalCodeStr.c_str();

	GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, &c_str, NULL);
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

void Shader::checkCompileErrors(GLuint shader, const std::string& type)
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

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);
}
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(m_programID, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setVec2(const std::string& name, const glm::dvec2& value) const
{
	glm::vec2 float_val = value;
	glUniform2fv(glGetUniformLocation(m_programID, name.c_str()), 1, glm::value_ptr(float_val));
}
