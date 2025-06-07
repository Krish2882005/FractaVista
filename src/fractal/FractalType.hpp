#pragma once

#include <string>
#include <vector>

enum class FractalType
{
	Mandelbrot,
	Julia,
	BurningShip,
	CubicMandelbrot,
	Tricorn,
	Newton
};

const std::vector<std::pair<FractalType, std::string>> FRACTAL_TYPE_NAMES
	= { { FractalType::Mandelbrot, "Mandelbrot" },	  { FractalType::Julia, "Julia" },
		{ FractalType::BurningShip, "Burning Ship" }, { FractalType::CubicMandelbrot, "Cubic Mandelbrot" },
		{ FractalType::Tricorn, "Tricorn" },		  { FractalType::Newton, "Newton" } };
