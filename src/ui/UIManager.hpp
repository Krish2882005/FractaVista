#pragma once

#include <functional>
#include <imgui.h>
#include <string>

#include <SDL3/SDL.h>

#include "UIState.hpp"
#include "fractal/FractalComputer.hpp"
#include "fractal/FractalState.hpp"
#include "ui/CameraController.hpp"

class UIManager
{
	public:
		UIManager(SDL_Window* window, SDL_GLContext context);
		~UIManager();

		void processEvent(const SDL_Event& event);
		void update(FractalState& state, UIState& uiState, FractalComputer& computer);
		void render();

		// Callbacks to request actions from the main Application class.
		std::function<void()> onRequestRedraw;
		std::function<void(const ScreenshotRequest&)> onRequestScreenshot;
		std::function<void()> onQuit;
		std::function<void()> onSavePreset;
		std::function<void()> onLoadPreset;

	private:
		void setupFonts();
		void setupDockspace(UIState& uiState);

		void drawMainMenuBar(UIState& uiState);
		void drawAboutModal(UIState& uiState);
		void drawViewportPanel(FractalState& state, const UIState& uiState, GLuint textureID);
		void drawPropertiesPanel(FractalState& state);
		void drawColoringPanel(FractalState& state);
		bool drawPaletteEditor(FractalState& state);
		void drawExportPanel(UIState& uiState);
		void drawStatusBar(const FractalState& state);

		ImFont* m_fontBold = nullptr;

		GLuint m_gradientTexture = 0;
		void updateGradientTexture(const std::vector<ColorStop>& palette);

		CameraController m_cameraController;

		std::string m_iniPath;
};
