#pragma once

#include <string>
#include <vector>

#include "FractalState.hpp"
#include "gfx/Shader.hpp"

class IFractal
{
	public:
		virtual ~IFractal() = default;

		virtual std::vector<std::string> getShaderDefines() const = 0;

		virtual void setUniforms(const Shader& shader, const FractalState& state) const = 0;

		virtual std::string getName() const = 0;
};
