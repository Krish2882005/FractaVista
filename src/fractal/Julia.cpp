#include "Julia.hpp"

std::vector<std::string> Julia::getShaderDefines() const
{
	return { "FRACTAL_JULIA" };
}

void Julia::setUniforms(const Shader& shader, const FractalState& state) const
{
	shader.setVec2("juliaC", state.juliaConstant);
}
