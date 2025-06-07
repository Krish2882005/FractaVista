#include "FractalManager.hpp"

#include <algorithm>
#include <vector>

#include <SDL3_image/SDL_image.h>

#include "BurningShip.hpp"
#include "CubicMandelbrot.hpp"
#include "Julia.hpp"
#include "Mandelbrot.hpp"
#include "Newton.hpp"
#include "Tricorn.hpp"
#include "utils/Logger.hpp"

FractalManager::FractalManager(int width, int height) : m_width(width), m_height(height)
{
	m_texture = std::make_unique<Texture>(width, height);
	initFractals();
}

void FractalManager::initFractals()
{
	m_fractals[FractalType::Mandelbrot] = std::make_unique<Mandelbrot>();
	m_fractals[FractalType::Julia] = std::make_unique<Julia>();
	m_fractals[FractalType::BurningShip] = std::make_unique<BurningShip>();
	m_fractals[FractalType::CubicMandelbrot] = std::make_unique<CubicMandelbrot>();
	m_fractals[FractalType::Tricorn] = std::make_unique<Tricorn>();
	m_fractals[FractalType::Newton] = std::make_unique<Newton>();
	Logger::Info("Initialized ", m_fractals.size(), " fractal algorithms.");
}

Shader& FractalManager::getOrCreateShader(const FractalState& state)
{
	if (m_shaderCache.find(state.type) == m_shaderCache.end())
	{
		Logger::Info("Compiling shader for ", m_fractals.at(state.type)->getName());
		auto shader = std::make_unique<Shader>();
		auto defines = m_fractals.at(state.type)->getShaderDefines();
		shader->compileFromPath("shaders/compute/MainShader.glsl", defines);
		m_shaderCache[state.type] = std::move(shader);
	}
	return *m_shaderCache.at(state.type);
}

void FractalManager::onResize(int newWidth, int newHeight)
{
	m_width = newWidth;
	m_height = newHeight;
	m_texture = std::make_unique<Texture>(newWidth, newHeight);
	Logger::Info("Resized fractal texture to ", newWidth, "x", newHeight);
}

void FractalManager::generate(const FractalState& state)
{
	Shader& shader = getOrCreateShader(state);
	shader.use();

	m_texture->bindImage(0);

	shader.setVec2("fullResolution", glm::vec2(m_width, m_height));
	shader.setVec2("offset", glm::vec2(state.offset));
	shader.setFloat("zoom", static_cast<float>(state.zoom));
	shader.setInt("maxIterations", state.maxIterations);

	m_fractals.at(state.type)->setUniforms(shader, state);

	const int groupSizeX = 16;
	const int groupSizeY = 16;
	glDispatchCompute((m_width + groupSizeX - 1) / groupSizeX, (m_height + groupSizeY - 1) / groupSizeY, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void FractalManager::saveScreenshot(const std::string& path, const std::string& extension, const FractalState& state,
									int supersampleFactor)
{
	Logger::Info("Taking screenshot... Supersample: ", supersampleFactor, "x");
	int ss_width = m_width * supersampleFactor;
	int ss_height = m_height * supersampleFactor;

	Texture ss_texture(ss_width, ss_height);
	Shader& shader = getOrCreateShader(state);

	shader.use();
	ss_texture.bindImage(0);

	shader.setVec2("fullResolution", glm::vec2(ss_width, ss_height));
	shader.setVec2("offset", glm::vec2(state.offset));
	shader.setFloat("zoom", static_cast<float>(state.zoom));
	shader.setInt("maxIterations", state.maxIterations * supersampleFactor);
	m_fractals.at(state.type)->setUniforms(shader, state);

	const int groupSizeX = 16;
	const int groupSizeY = 16;
	glDispatchCompute((ss_width + groupSizeX - 1) / groupSizeX, (ss_height + groupSizeY - 1) / groupSizeY, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	std::vector<unsigned char> buffer(ss_width * ss_height * 4);
	ss_texture.bind();
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());

	const int row_pitch = ss_width * 4;
	for (int y = 0; y < ss_height / 2; ++y)
	{
		auto row1 = buffer.begin() + y * row_pitch;
		auto row2 = buffer.begin() + (ss_height - 1 - y) * row_pitch;
		std::swap_ranges(row1, row1 + row_pitch, row2);
	}

	SDL_Surface* surface
		= SDL_CreateSurfaceFrom(ss_width, ss_height, SDL_PIXELFORMAT_ABGR8888, buffer.data(), row_pitch);

	if (!surface)
	{
		Logger::Error("Failed to create SDL_Surface for screenshot: ", SDL_GetError());
		return;
	}

	bool success = false;

	std::string lowerExt = extension;
	std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), ::tolower);

	if (lowerExt == "png")
	{
		success = IMG_SavePNG(surface, path.c_str());
	}
	else if (lowerExt == "jpg" || lowerExt == "jpeg")
	{
		SDL_Surface* convertedSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGB24);

		if (!convertedSurface)
		{
			Logger::Error("Failed to convert SDL_Surface for screenshot: ", SDL_GetError());
			return;
		}

		success = IMG_SaveJPG(surface, path.c_str(), 95);

		SDL_DestroySurface(convertedSurface);
	}
	else if (lowerExt == "bmp")
	{
		success = SDL_SaveBMP(surface, path.c_str());
	}
	else
	{
		Logger::Error("Unsupported file extension: ", extension);
		SDL_DestroySurface(surface);
		return;
	}

	if (!success)
	{
		Logger::Error("Failed to save screenshot (", extension, "): ", SDL_GetError());
	}
	else
	{
		Logger::Info("Screenshot saved to ", path);
	}

	SDL_DestroySurface(surface);
}
