#include "BurningShip.hpp"

std::vector<std::string> BurningShip::getShaderDefines() const
{
	return { "FRACTAL_BURNING_SHIP" };
}

void BurningShip::setUniforms(const Shader& shader, const FractalState& state) const
{
}
