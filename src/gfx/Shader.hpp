#pragma once

#include <set>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
	public:
		Shader();
		~Shader();

		void compileFromPath(const std::string& computePath, const std::vector<std::string>& defines = {});

		void use() const;

		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec2(const std::string& name, const glm::vec2& value) const;
		void setVec2(const std::string& name, const glm::dvec2& value) const;

	private:
		std::string loadSource(const std::string& path, std::set<std::string>& includedFiles, int depth);
		void checkCompileErrors(GLuint shader, const std::string& type);

		GLuint m_programID = 0;
};
