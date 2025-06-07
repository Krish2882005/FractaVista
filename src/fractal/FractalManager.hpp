#pragma once

#include <map>
#include <memory>
#include <string>

#include "FractalState.hpp"
#include "IFractal.hpp"
#include "gfx/Shader.hpp"
#include "gfx/Texture.hpp"

class FractalManager
{
	public:
		FractalManager(int width, int height);

		void generate(const FractalState& state);
		void onResize(int newWidth, int newHeight);
		void saveScreenshot(const std::string& path, const std::string& extension, const FractalState& state,
							int supersampleFactor);

		GLuint getTexture() const { return m_texture->getID(); }
		const auto& getFractalMap() const { return m_fractals; }

	private:
		void initFractals();
		Shader& getOrCreateShader(const FractalState& state);

		int m_width, m_height;
		std::unique_ptr<Texture> m_texture;

		std::map<FractalType, std::unique_ptr<IFractal>> m_fractals;
		std::map<FractalType, std::unique_ptr<Shader>> m_shaderCache;
};
