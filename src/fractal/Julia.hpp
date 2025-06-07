#pragma once
#include "IFractal.hpp"

class Julia : public IFractal
{
	public:
		std::vector<std::string> getShaderDefines() const override;
		void setUniforms(const Shader& shader, const FractalState& state) const override;
		std::string getName() const override { return "Julia"; }
};
