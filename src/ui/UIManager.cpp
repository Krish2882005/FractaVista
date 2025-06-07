#include "UIManager.hpp"

#include <filesystem>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <vector>

#include "fractal/FractalType.hpp"

UIManager::UIManager(SDL_Window* window, SDL_GLContext context)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init("#version 330");
}

UIManager::~UIManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void UIManager::processEvent(const SDL_Event& event)
{
	ImGui_ImplSDL3_ProcessEvent(&event);
}

bool UIManager::wantsCaptureMouse() const
{
	return ImGui::GetIO().WantCaptureMouse;
}

bool UIManager::wantsCaptureKeyboard() const
{
	return ImGui::GetIO().WantCaptureKeyboard;
}

void UIManager::update(FractalState& state, const std::map<FractalType, std::unique_ptr<IFractal>>& fractals)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("FractaVista Controls");

	std::string currentName = fractals.at(state.type)->getName();
	if (ImGui::BeginCombo("Algorithm", currentName.c_str()))
	{
		for (const auto& pair : fractals)
		{
			bool is_selected = (state.type == pair.first);
			if (ImGui::Selectable(pair.second->getName().c_str(), is_selected))
			{
				if (state.type != pair.first)
				{
					state.type = pair.first;
					state.needsUpdate = true;
				}
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();
	ImGui::Text("Parameters");

	bool changed = false;
	changed |= ImGui::SliderInt("Max Iterations", &state.maxIterations, 64, 8192);

	changed |= ImGui::DragScalar("Zoom", ImGuiDataType_Double, &state.zoom, 100.0f, NULL, NULL, "%.1f");
	changed |= ImGui::DragScalar("Offset X", ImGuiDataType_Double, &state.offset.x, 0.01, NULL, NULL, "%.8f");
	changed |= ImGui::DragScalar("Offset Y", ImGuiDataType_Double, &state.offset.y, 0.01, NULL, NULL, "%.8f");

	if (state.type == FractalType::Julia)
	{
		ImGui::Separator();
		ImGui::Text("Julia Parameters");
		changed |= ImGui::DragScalar("Constant Real", ImGuiDataType_Double, &state.juliaConstant.x, 0.001, NULL, NULL,
									 "%.4f");
		changed |= ImGui::DragScalar("Constant Imag", ImGuiDataType_Double, &state.juliaConstant.y, 0.001, NULL, NULL,
									 "%.4f");
	}

	if (changed)
	{
		state.needsUpdate = true;
	}

	ImGui::Separator();
	ImGui::Text("Export");

	ImGui::InputText("Filename", m_screenshotPathBuffer, IM_ARRAYSIZE(m_screenshotPathBuffer));

	const char* extensionOptions[] = { "png", "jpg", "bmp" };
	ImGui::Combo("Format", &m_screenshotExtensionIndex, extensionOptions, IM_ARRAYSIZE(extensionOptions));

	const char* supersampleOptions[] = { "1x (Normal)", "2x", "4x", "8x" };
	ImGui::Combo("Supersample", &m_screenshotFactorIndex, supersampleOptions, IM_ARRAYSIZE(supersampleOptions));

	if (ImGui::Button("Save Screenshot"))
	{
		if (onRequestScreenshot)
		{
			namespace fs = std::filesystem;

			std::string extension = extensionOptions[m_screenshotExtensionIndex];
			int factor = 1 << m_screenshotFactorIndex;

			fs::path filepath(m_screenshotPathBuffer);
			filepath.replace_extension("." + extension); // This safely removes existing .ext and appends new one

			onRequestScreenshot(filepath.string(), extension, factor);
		}
	}

	if (ImGui::Button("Quit"))
	{
		if (onQuit)
			onQuit();
	}

	ImGui::End();
}

void UIManager::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
