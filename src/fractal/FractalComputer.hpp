#pragma once

#include <map>
#include <memory>
#include <string>

#include "FractalState.hpp"
#include "gfx/Shader.hpp"
#include "gfx/Texture.hpp"
#include "ui/UIState.hpp"
#include <glad/gl.h>

// Maximum number of color stops we support in the palette.
constexpr int MAX_PALETTE_STOPS = 16;

class FractalComputer
{
	public:
		FractalComputer(int width, int height);
		~FractalComputer();

		void generate(const FractalState& state);
		void onResize(int newWidth, int newHeight);
		void saveScreenshot(const ScreenshotRequest& request, const FractalState& state);

		[[nodiscard]] GLuint getTextureID() const { return m_texture->getID(); }

	private:
		Shader& getOrCreateShader(FractalType type);
		void updatePaletteUBO(const ColoringParams& coloring);

		int m_width;
		int m_height;
		std::unique_ptr<Texture> m_texture;

		std::map<FractalType, std::unique_ptr<Shader>> m_shaderCache;

		GLuint m_paletteUBO = 0;
};
