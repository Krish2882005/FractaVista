#pragma once

#include <map>
#include <string>
#include <string_view>

#include "FractalTypes.hpp"

struct FractalDefinition
{
		std::string_view name;
		std::string_view shaderDefine;
};

static const std::map<FractalType, FractalDefinition> FractalDefinitions
	= { { FractalType::Mandelbrot, { "Mandelbrot", "FRACTAL_MANDELBROT" } },
		{ FractalType::Julia, { "Julia", "FRACTAL_JULIA" } },
		{ FractalType::BurningShip, { "Burning Ship", "FRACTAL_BURNING_SHIP" } },
		{ FractalType::CubicMandelbrot, { "Cubic Mandelbrot", "FRACTAL_CUBIC_MANDELBROT" } },
		{ FractalType::Tricorn, { "Tricorn", "FRACTAL_TRICORN" } },
		{ FractalType::Newton, { "Newton", "FRACTAL_NEWTON" } } };
