#include "CubicMandelbrot.hpp"

std::vector<std::string> CubicMandelbrot::getShaderDefines() const
{
	return { "FRACTAL_CUBIC_MANDELBROT" };
}

void CubicMandelbrot::setUniforms(const Shader& shader, const FractalState& state) const
{
}
