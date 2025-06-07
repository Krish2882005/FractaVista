#include "Tricorn.hpp"

std::vector<std::string> Tricorn::getShaderDefines() const
{
	return { "FRACTAL_TRICORN" };
}

void Tricorn::setUniforms(const Shader& shader, const FractalState& state) const
{
}
