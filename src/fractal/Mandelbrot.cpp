#include "Mandelbrot.hpp"

std::vector<std::string> Mandelbrot::getShaderDefines() const
{
	return { "FRACTAL_MANDELBROT" };
}

void Mandelbrot::setUniforms(const Shader& shader, const FractalState& state) const
{
}
