#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include <glad/gl.h>
#include <glm/glm.hpp>

class Shader
{
	public:
		Shader();
		~Shader();

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;

		void compileFromPath(const std::filesystem::path& computePath, const std::vector<std::string>& defines = {});
		void use() const;
		void bindUBO(std::string_view blockName, GLuint bindingPoint);

		void setBool(std::string_view name, bool value) const;
		void setInt(std::string_view name, int value) const;
		void setFloat(std::string_view name, float value) const;
		void setDouble(std::string_view name, double value) const;
		void setVec2(std::string_view name, const glm::vec2& value) const;
		void setVec2(std::string_view name, const glm::dvec2& value) const;
		void setVec3(std::string_view name, const glm::vec3& value) const;
		void setVec3(std::string_view name, const glm::dvec3& value) const;

	private:
		static void checkCompileErrors(GLuint shader, std::string_view type);
		GLuint m_programID = 0;
};
