#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "fractal/FractalState.hpp"

enum class ScreenshotFormat
{
	PNG,
	JPG,
	BMP
};

struct ScreenshotRequest
{
		std::filesystem::path filepath;
		ScreenshotFormat format = ScreenshotFormat::PNG;
		int supersample = 1;
};

struct Keyframe
{
		double time;
		FractalState state;
};

struct UIState
{
		bool showPropertiesPanel = true;
		bool showColoringPanel = true;
		bool showExportPanel = true;
		bool showStatusBar = true;
		bool showAnimationPanel = true;
		bool showAboutModal = false;

		std::string screenshotFilename = "fractavistas_shot";
		ScreenshotFormat screenshotFormat = ScreenshotFormat::PNG;
		int supersampleFactor = 1;
};
