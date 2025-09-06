#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include <regex>
#include <set>
#include <sstream>
#include <string>

#include <SDL3/SDL_filesystem.h>

#include "Logger.hpp"

namespace FileUtils
{
	inline std::filesystem::path getAbsolutePath(const std::filesystem::path& relativePath)
	{
		FRACTAL_TRACE("Getting path for %s\n", relativePath.string().c_str());

		const char* basePathCStr = SDL_GetBasePath();
		if (!basePathCStr)
		{
			FRACTAL_TRACE("SDL_GetBasePath returned null; using relative path %s\n", relativePath.string().c_str());
			return relativePath;
		}

		std::filesystem::path basePath(basePathCStr);

		auto fullPath = basePath / relativePath;

		std::filesystem::path genericPath = fullPath.lexically_normal();

		FRACTAL_TRACE("Resolved asset path: %s\n", genericPath.string().c_str());

		return genericPath;
	}

	inline std::optional<std::string> readFile(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
		{
			FRACTAL_TRACE("File does not exist: {}", path.string());
			return std::nullopt;
		}

		std::ifstream file(path);
		if (!file.is_open())
		{
			FRACTAL_ERROR("Failed to open file: {}", path.string());
			return std::nullopt;
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string content = buffer.str();
		FRACTAL_TRACE("Successfully read file: {}", path.string());
		return content;
	}

	namespace detail
	{
		inline void loadShaderSourceRecursive(const std::filesystem::path& path, std::stringstream& ss,
											  std::set<std::string>& includedFiles, int depth = 0)
		{
			if (includedFiles.count(path.string()))
			{
				FRACTAL_TRACE("Skipping already included file: {}", path.string());
				return;
			}
			FRACTAL_TRACE("Loading shader file: {}", path.string());

			auto fileContent = readFile(path);
			if (!fileContent)
			{
				FRACTAL_ERROR("Could not read shader file: {}", path.string());
				return;
			}

			includedFiles.insert(path.string());

			std::stringstream contentStream(*fileContent);
			std::string line;
			const std::regex includeRegex(R"(^\s*#include\s+\"([^\"]+)\")");

			while (std::getline(contentStream, line))
			{
				std::smatch match;
				if (std::regex_search(line, match, includeRegex) && match.size() > 1)
				{
					std::filesystem::path includePath = path.parent_path() / match[1].str();
					FRACTAL_TRACE("Found include directive: {} -> {}", line, includePath.string());
					loadShaderSourceRecursive(includePath, ss, includedFiles, depth + 1);
				}
				else
				{
					ss << line << '\n';
				}
			}

			if (depth > 0)
			{
				FRACTAL_TRACE("Loaded shader: {}", path.filename().string());
			}
		}
	} // namespace detail

	inline std::optional<std::string> loadShaderSource(const std::filesystem::path& path)
	{
		FRACTAL_TRACE("Starting shader source load for {}", path.string());

		std::stringstream ss;
		std::set<std::string> includedFiles;
		detail::loadShaderSourceRecursive(path, ss, includedFiles);

		auto source = ss.str();
		if (source.empty())
		{
			FRACTAL_TRACE("Shader source is empty after loading: {}", path.string());
			return std::nullopt;
		}

		size_t numIncludes = includedFiles.size() > 0 ? includedFiles.size() - 1 : 0;

		if (numIncludes > 0)
		{
			FRACTAL_TRACE("Loaded shader: {} + {} includes", path.filename().string(), numIncludes);
		}
		else
		{
			FRACTAL_TRACE("Loaded shader: {}", path.filename().string());
		}

		return source;
	}

} // namespace FileUtils
