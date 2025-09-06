#include "FractalComputer.hpp"

#include <algorithm>
#include <cstdint>
#include <span>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "FractalDefinition.hpp"
#include "util/FileUtils.hpp"
#include "util/Logger.hpp"

// UBO data structure for color palette, matching std140 layout
struct alignas(16) ColorStopUBOData
{
		glm::vec3 Color;
		float Position;
};

struct PaletteUBOData
{
		int NumStops;
		int Padding[3]; // Explicit padding for std140
		ColorStopUBOData Stops[MAX_PALETTE_STOPS];
};

FractalComputer::FractalComputer(int width, int height) : m_width(width), m_height(height)
{
	m_texture = std::make_unique<Texture>(width, height);

	glGenBuffers(1, &m_paletteUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, m_paletteUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PaletteUBOData), nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_paletteUBO);

	FRACTAL_INFO("FractalComputer initialized with texture size {}x{}.", width, height);
}

FractalComputer::~FractalComputer()
{
	if (m_paletteUBO != 0)
	{
		glDeleteBuffers(1, &m_paletteUBO);
	}
}

void FractalComputer::updatePaletteUBO(const ColoringParams& coloring)
{
	PaletteUBOData uboData{};
	uboData.NumStops = std::min(static_cast<int>(coloring.palette.size()), MAX_PALETTE_STOPS);

	for (int i = 0; i < uboData.NumStops; ++i)
	{
		uboData.Stops[i].Color = coloring.palette[i].color;
		uboData.Stops[i].Position = coloring.palette[i].position;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, m_paletteUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PaletteUBOData), &uboData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Shader& FractalComputer::getOrCreateShader(FractalType type)
{
	if (!m_shaderCache.contains(type))
	{
		const auto& def = FractalDefinitions.at(type);
		FRACTAL_INFO("Compiling shader for '{}'...", def.name);

		auto shader = std::make_unique<Shader>();
		auto shaderPath = FileUtils::getAbsolutePath("assets/shaders/MainShader.glsl");
		shader->compileFromPath(shaderPath, { std::string(def.shaderDefine) });

		shader->use();
		shader->bindUBO("Palette", 0);

		m_shaderCache[type] = std::move(shader);
	}
	return *m_shaderCache.at(type);
}

void FractalComputer::onResize(int newWidth, int newHeight)
{
	if (m_width == newWidth && m_height == newHeight)
		return;
	if (newWidth <= 0 || newHeight <= 0)
		return;

	m_width = newWidth;
	m_height = newHeight;
	m_texture->resize(newWidth, newHeight);
	FRACTAL_INFO("Resized fractal texture to {}x{}.", newWidth, newHeight);
}

void FractalComputer::generate(const FractalState& state)
{
	onResize(state.renderWidth, state.renderHeight);

	Shader& shader = getOrCreateShader(state.type);
	shader.use();

	updatePaletteUBO(state.coloring);
	m_texture->bindImage(0);

	shader.setVec2("fullResolution", glm::dvec2{ static_cast<double>(m_width), static_cast<double>(m_height) });
	shader.setVec2("offset", state.offset);
	shader.setDouble("zoom", state.zoom);
	shader.setInt("maxIterations", state.maxIterations);
	shader.setBool("useSmoothing", state.coloring.useSmoothing);
	shader.setDouble("paletteFrequency", state.coloring.paletteFrequency);

	if (state.type == FractalType::Julia)
	{
		shader.setVec2("juliaC", state.specificParams.juliaConstant);
	}

	const int groupSize = 16;
	glDispatchCompute((m_width + groupSize - 1) / groupSize, (m_height + groupSize - 1) / groupSize, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void FractalComputer::saveScreenshot(const ScreenshotRequest& request, const FractalState& state)
{
	FRACTAL_INFO("Taking screenshot... Supersample: {}x, Path: {}", request.supersample, request.filepath.string());

	const int ssWidth = state.renderWidth * request.supersample;
	const int ssHeight = state.renderHeight * request.supersample;

	if (ssWidth <= 0 || ssHeight <= 0)
	{
		FRACTAL_ERROR("Cannot save screenshot with invalid dimensions ({}x{}).", ssWidth, ssHeight);
		return;
	}

	Texture ssTexture(ssWidth, ssHeight);
	Shader& shader = getOrCreateShader(state.type);

	shader.use();
	updatePaletteUBO(state.coloring);
	ssTexture.bindImage(0);

	shader.setVec2("fullResolution", glm::dvec2{ static_cast<double>(ssWidth), static_cast<double>(ssHeight) });
	shader.setVec2("offset", state.offset);
	shader.setDouble("zoom", state.zoom);
	shader.setInt("maxIterations", state.maxIterations);
	shader.setBool("useSmoothing", state.coloring.useSmoothing);
	shader.setDouble("paletteFrequency", state.coloring.paletteFrequency);

	if (state.type == FractalType::Julia)
	{
		shader.setVec2("juliaC", state.specificParams.juliaConstant);
	}

	const int groupSize = 16;
	glDispatchCompute((ssWidth + groupSize - 1) / groupSize, (ssHeight + groupSize - 1) / groupSize, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	std::vector<unsigned char> buffer(ssWidth * ssHeight * 4);
	ssTexture.bind();
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());

	const int rowPitch = ssWidth * 4;
	for (int y = 0; y < ssHeight / 2; ++y)
	{
		std::span<uint8_t> bufferSpan(buffer.data(), buffer.size());

		const size_t offset1 = static_cast<size_t>(y) * static_cast<size_t>(rowPitch);
		const size_t offset2 = static_cast<size_t>(ssHeight - 1 - y) * static_cast<size_t>(rowPitch);

		auto row1 = bufferSpan.subspan(offset1, rowPitch);
		auto row2 = bufferSpan.subspan(offset2, rowPitch);

		std::swap_ranges(row1.begin(), row1.end(), row2.begin());
	}

	SDL_Surface* surface = SDL_CreateSurfaceFrom(ssWidth, ssHeight, SDL_PIXELFORMAT_ABGR8888, buffer.data(), rowPitch);
	if (!surface)
	{
		FRACTAL_ERROR("Failed to create SDL_Surface for screenshot: {}", SDL_GetError());
		return;
	}

	bool success = false;
	std::string pathStr = request.filepath.string();

	switch (request.format)
	{
		case ScreenshotFormat::PNG:
			success = IMG_SavePNG(surface, pathStr.c_str());
			break;
		case ScreenshotFormat::JPG:
		{
			SDL_Surface* rgbSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGB24);
			if (rgbSurface)
			{
				success = IMG_SaveJPG(rgbSurface, pathStr.c_str(), 95);
				SDL_DestroySurface(rgbSurface);
			}
			else
			{
				FRACTAL_ERROR("Failed to convert surface for JPG saving: {}", SDL_GetError());
			}
		}
		break;
		case ScreenshotFormat::BMP:
			success = SDL_SaveBMP(surface, pathStr.c_str());
			break;
	}

	if (!success)
	{
		FRACTAL_ERROR("Failed to save screenshot: {}", SDL_GetError());
	}
	else
	{
		FRACTAL_INFO("Screenshot saved to {}", request.filepath.string());
	}

	SDL_DestroySurface(surface);
}
