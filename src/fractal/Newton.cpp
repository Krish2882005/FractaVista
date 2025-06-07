#include "Newton.hpp"

std::vector<std::string> Newton::getShaderDefines() const
{
	return { "FRACTAL_NEWTON" };
}

void Newton::setUniforms(const Shader& shader, const FractalState& state) const
{
}
