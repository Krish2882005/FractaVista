#include "UIManager.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <filesystem>
#include <format>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <ranges>
#include <string>

#include "fractal/FractalDefinition.hpp"
#include "ui/Icons.hpp"
#include "ui/Theme.hpp"
#include "util/FileUtils.hpp"
#include "util/Logger.hpp"

namespace
{
	namespace ui_constants
	{
		// Application Info
		constexpr auto APP_NAME = "FractaVista";
		constexpr auto APP_VERSION = "0.1.0-alpha.1";
		constexpr auto APP_REPO_URL = "https://github.com/Krish2882005/FractaVista";
		constexpr auto APP_REPO_URL_LABEL = "github.com/Krish2882005/FractaVista";

		// Rendering & GL
		constexpr auto GLSL_VERSION = "#version 430";
		constexpr int GRADIENT_TEXTURE_SIZE = 256;
		constexpr ImVec2 TEXTURE_UV_MIN = { 0.0F, 1.0F };
		constexpr ImVec2 TEXTURE_UV_MAX = { 1.0F, 0.0F };

		// Fonts
		constexpr float BASE_FONT_SIZE = 18.0F;
		constexpr float ICON_TO_FONT_SCALE = 2.0F / 3.0F;
		constexpr auto REGULAR_FONT_FILE = "assets/fonts/Poppins/Poppins-Regular.ttf";
		constexpr auto BOLD_FONT_FILE = "assets/fonts/Poppins/Poppins-Bold.ttf";
		constexpr auto ICONS_FONT_FILE = "assets/fonts/fontawesome-free-6.7.2-desktop/otfs/Font Awesome 6 Free-Solid-900.otf";

		// UI Identifiers
		constexpr auto MAIN_HOST_ID = "MainHost";
		constexpr auto MAIN_DOCKSPACE_ID = "MainDockSpace";
		constexpr auto LOGO_CONTAINER_ID = "##LogoContainer";
		constexpr auto ABOUT_POPUP_ID = "About FractaVista";
		constexpr auto PALETTE_INTERACTION_ZONE_ID = "palette_interaction_zone";
		constexpr auto PALETTE_HANDLE_ID = "handle";
		constexpr auto COLOR_EDIT_ID = "##color";

		// Labels (with icons)
		const auto VIEWPORT_WINDOW_TITLE = ICON_FA_IMAGE " Viewport";
		const auto PROPERTIES_WINDOW_TITLE = ICON_FA_SLIDERS " Properties";
		const auto EXPORT_WINDOW_TITLE = ICON_FA_CAMERA " Export";
		const auto COLORING_WINDOW_TITLE = ICON_FA_PALETTE " Coloring";
		const auto FILE_SAVE_PRESET = ICON_FA_FLOPPY_DISK " Save Preset...";
		const auto FILE_LOAD_PRESET = ICON_FA_FOLDER_OPEN " Load Preset...";
		const auto FILE_EXPORT_IMAGE = ICON_FA_FILE_EXPORT " Export Image...";
		const auto FILE_QUIT = ICON_FA_RIGHT_FROM_BRACKET " Quit";
		const auto VIEW_STATUS_BAR = ICON_FA_DESKTOP " Status Bar";
		const auto HELP_ABOUT = ICON_FA_CIRCLE_INFO " About";
		const auto EXPORT_SAVE_BUTTON = ICON_FA_FLOPPY_DISK " Save to File";
		const auto SAVE_TO_FILE_BUTTON = ICON_FA_FLOPPY_DISK " Save to File";

		// Layout & Sizing
		constexpr ImVec2 NO_PADDING = { 0.0F, 0.0F };
		constexpr ImVec2 FILL_SPACE = { 0.0F, 0.0F };
		constexpr float RIGHT_PANEL_SPLIT_RATIO = 0.25F;
		constexpr float BOTTOM_PANEL_SPLIT_RATIO = 0.25F;
		constexpr float MENU_BAR_LOGO_SPACING = 20.0F;
		constexpr float VERTICAL_CENTER_ALIGN_FACTOR = 0.5F;
		constexpr float CLOSE_BUTTON_WIDTH = 120.0F;
		constexpr ImVec2 FULL_WIDTH_BUTTON = { -1.0F, 0.0F };

		// About Modal
		constexpr ImVec4 MUTED_TEXT_COLOR = { 0.6F, 0.6F, 0.6F, 1.0F };
		constexpr ImVec4 HYPERLINK_COLOR = { 0.26F, 0.59F, 0.98F, 1.0F };
		constexpr ImVec4 TRANSPARENT_COLOR = { 0.0F, 0.0F, 0.0F, 0.0F };
		constexpr ImVec2 HYPERLINK_FRAME_PADDING = { 0.0F, 1.0F };
		constexpr float HYPERLINK_UNDERLINE_THICKNESS = 1.0F;
		constexpr int HYPERLINK_STYLE_VARS_TO_POP = 2;
		constexpr int HYPERLINK_STYLE_COLORS_TO_POP = 6;

		// Palette Editor
		constexpr float GRADIENT_PREVIEW_HEIGHT = 25.0F;
		constexpr float PALETTE_EDITOR_INTERACTION_HEIGHT = 40.0F;
		constexpr ImVec2 PALETTE_HANDLE_SIZE = { 20.0F, 20.0F };
		constexpr float PALETTE_HANDLE_VERTICAL_OFFSET = 20.0F;
		constexpr float PALETTE_HANDLE_CENTER_FROM_LEFT = 10.0F;
		constexpr float PALETTE_HANDLE_TRIANGLE_SIZE = 5.0F;
		constexpr float PALETTE_HANDLE_CIRCLE_RADIUS = 3.0F;
		constexpr float PALETTE_HANDLE_CIRCLE_OFFSET = 10.0F;
		constexpr int MIN_PALETTE_STOPS = 2;
		constexpr glm::vec3 DEFAULT_NEW_COLOR_STOP = { 1.0F, 1.0F, 1.0F };

		// Fractal & Controls
		constexpr int MIN_ITERATIONS = 32;
		constexpr int MAX_ITERATIONS = 32768;
		constexpr double ZOOM_SPEED = 1.25;
		constexpr float EPSILON = 1e-6F;
		constexpr double JULIA_PARAM_STEP = 0.001;

		// Formatting Strings
		constexpr auto ZOOM_FORMAT = "%.3e";
		constexpr auto COORD_FORMAT = "%.15f";
		constexpr auto JULIA_PARAM_FORMAT = "%.4f";
		constexpr auto STATUS_BAR_FORMAT = "X: %.6f, Y: %.6f | Zoom: %.2e | Res: %dx%d";

		// Status Bar
		constexpr ImVec2 STATUS_BAR_PADDING = { 12.0F, 5.0F };
		constexpr int STATUS_BAR_STYLES_TO_POP = 2;

		// Exporting
		constexpr auto PNG_EXTENSION = ".png";
		constexpr auto JPG_EXTENSION = ".jpg";
		constexpr auto BMP_EXTENSION = ".bmp";
	} // namespace ui_constants
} // namespace

UIManager::UIManager(SDL_Window* window, SDL_GLContext context) 
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags
		|= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
	m_iniPath = FileUtils::getAbsolutePath("config/FractaVista_UI.ini").string();
	io.IniFilename = m_iniPath.c_str();

	setupFonts();
	Theme::Apply();

	glGenTextures(1, &m_gradientTexture);
	glBindTexture(GL_TEXTURE_2D, m_gradientTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	ImGui_ImplSDL3_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init(ui_constants::GLSL_VERSION);
}

UIManager::~UIManager()
{
	if (m_gradientTexture != 0)
	{
		glDeleteTextures(1, &m_gradientTexture);
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void UIManager::setupFonts()
{
	ImGuiIO& io = ImGui::GetIO();

	const float baseFontSize = ui_constants::BASE_FONT_SIZE;
	const float iconFontSize = baseFontSize * ui_constants::ICON_TO_FONT_SCALE;

	const auto regularFontPath = FileUtils::getAbsolutePath(ui_constants::REGULAR_FONT_FILE);
	const auto boldFontPath = FileUtils::getAbsolutePath(ui_constants::BOLD_FONT_FILE);
	const auto iconFontPath = FileUtils::getAbsolutePath(ui_constants::ICONS_FONT_FILE);

	ImFontConfig iconConfig;
	iconConfig.MergeMode = true;
	iconConfig.PixelSnapH = true;
	iconConfig.GlyphMinAdvanceX = iconFontSize;
	static constexpr std::array<ImWchar, 3> s_iconRanges = { ICON_MIN_FA, ICON_MAX_FA, 0 };

	if (std::filesystem::exists(regularFontPath))
	{
		io.Fonts->AddFontFromFileTTF(regularFontPath.string().c_str(), baseFontSize);
		if (std::filesystem::exists(iconFontPath))
		{
			io.Fonts->AddFontFromFileTTF(iconFontPath.string().c_str(), iconFontSize, &iconConfig, s_iconRanges.data());
		}
	}
	else
	{
		FRACTAL_WARN("Poppins-Regular.ttf not found. Using default ImGui font.");
		io.Fonts->AddFontDefault();
		if (std::filesystem::exists(iconFontPath))
		{
			io.Fonts->AddFontFromFileTTF(iconFontPath.string().c_str(), iconFontSize, &iconConfig, s_iconRanges.data());
		}
	}

	if (std::filesystem::exists(boldFontPath))
	{
		m_fontBold = io.Fonts->AddFontFromFileTTF(boldFontPath.string().c_str(), baseFontSize);
		if (std::filesystem::exists(iconFontPath))
		{
			io.Fonts->AddFontFromFileTTF(iconFontPath.string().c_str(), iconFontSize, &iconConfig, s_iconRanges.data());
		}
	}

	io.Fonts->Build();
}

void UIManager::processEvent(const SDL_Event& event)
{
	ImGui_ImplSDL3_ProcessEvent(&event);
}

void UIManager::update(FractalState& state, UIState& uiState, FractalComputer& computer)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags hostWindowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar
									   | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
									   | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
									   | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ui_constants::NO_PADDING);
	ImGui::Begin(ui_constants::MAIN_HOST_ID, nullptr, hostWindowFlags);
	ImGui::PopStyleVar();

	drawMainMenuBar(uiState);
	setupDockspace(uiState);

	if (uiState.showPropertiesPanel)
		drawPropertiesPanel(state);
	if (uiState.showColoringPanel)
		drawColoringPanel(state);
	if (uiState.showExportPanel)
		drawExportPanel(uiState);
	if (uiState.showAboutModal)
		drawAboutModal(uiState);
	if (uiState.showStatusBar)
		drawStatusBar(state);

	drawViewportPanel(state, uiState, computer.getTextureID());

	ImGui::End();
}

void UIManager::setupDockspace(UIState& uiState)
{
	ImGuiID dockspaceID = ImGui::GetID(ui_constants::MAIN_DOCKSPACE_ID);
	ImGui::DockSpace(dockspaceID, ui_constants::FILL_SPACE, ImGuiDockNodeFlags_None);

	ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockspaceID);
	if (node == nullptr || node->IsEmpty())
	{
		ImGui::DockBuilderRemoveNode(dockspaceID);
		ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);

		ImGuiID dockMainID = dockspaceID;
		ImGuiID dockRight = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Right,
														ui_constants::RIGHT_PANEL_SPLIT_RATIO, nullptr, &dockMainID);
		ImGuiID dockBottom = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Down,
														 ui_constants::BOTTOM_PANEL_SPLIT_RATIO, nullptr, &dockMainID);

		ImGui::DockBuilderDockWindow(ui_constants::VIEWPORT_WINDOW_TITLE, dockMainID);
		ImGui::DockBuilderDockWindow(ui_constants::PROPERTIES_WINDOW_TITLE, dockRight);
		ImGui::DockBuilderDockWindow(ui_constants::COLORING_WINDOW_TITLE, dockRight);
		ImGui::DockBuilderDockWindow(ui_constants::EXPORT_WINDOW_TITLE, dockRight);

		ImGui::DockBuilderFinish(dockspaceID);
	}
}

void UIManager::drawMainMenuBar(UIState& uiState)
{
	if (ImGui::BeginMenuBar())
	{
		const float menuBarHeight = ImGui::GetFrameHeight();
		ImVec2 logoTextSize;

		if (m_fontBold)
		{
			ImGui::PushFont(m_fontBold);
			logoTextSize = ImGui::CalcTextSize(ui_constants::APP_NAME);
			ImGui::PopFont();
		}
		else
		{
			logoTextSize = ImGui::CalcTextSize(ui_constants::APP_NAME);
		}

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ui_constants::TRANSPARENT_COLOR);
		ImGui::BeginChild(ui_constants::LOGO_CONTAINER_ID, ImVec2(logoTextSize.x, menuBarHeight), false,
						  ImGuiWindowFlags_NoScrollWithMouse);

		if (m_fontBold)
			ImGui::PushFont(m_fontBold);

		float logoHeight = ImGui::GetTextLineHeight();
		ImGui::SetCursorPosY((menuBarHeight - logoHeight) * ui_constants::VERTICAL_CENTER_ALIGN_FACTOR);
		ImGui::TextUnformatted(ui_constants::APP_NAME);

		if (m_fontBold)
			ImGui::PopFont();

		ImGui::EndChild();
		ImGui::PopStyleColor();

		ImGui::SameLine(0.0F, ui_constants::MENU_BAR_LOGO_SPACING);

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem(ui_constants::FILE_SAVE_PRESET))
			{
				if (onSavePreset)
					onSavePreset();
			}
			if (ImGui::MenuItem(ui_constants::FILE_LOAD_PRESET))
			{
				if (onLoadPreset)
					onLoadPreset();
			}
			ImGui::Separator();
			if (ImGui::MenuItem(ui_constants::FILE_EXPORT_IMAGE))
			{
				uiState.showExportPanel = true;
			}
			ImGui::Separator();
			if (ImGui::MenuItem(ui_constants::FILE_QUIT))
			{
				if (onQuit)
					onQuit();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem(ui_constants::PROPERTIES_WINDOW_TITLE, nullptr, &uiState.showPropertiesPanel);
			ImGui::MenuItem(ui_constants::COLORING_WINDOW_TITLE, nullptr, &uiState.showColoringPanel);
			ImGui::MenuItem(ui_constants::EXPORT_WINDOW_TITLE, nullptr, &uiState.showExportPanel);
			ImGui::MenuItem(ui_constants::VIEW_STATUS_BAR, nullptr, &uiState.showStatusBar);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem(ui_constants::HELP_ABOUT))
			{
				uiState.showAboutModal = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void UIManager::drawAboutModal(UIState& uiState)
{
	if (uiState.showAboutModal)
		ImGui::OpenPopup(ui_constants::ABOUT_POPUP_ID);
	if (ImGui::BeginPopupModal(ui_constants::ABOUT_POPUP_ID, &uiState.showAboutModal,
							   ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (m_fontBold)
			ImGui::PushFont(m_fontBold);
		ImGui::TextUnformatted(ui_constants::APP_NAME);
		if (m_fontBold)
			ImGui::PopFont();

		ImGui::TextColored(ui_constants::MUTED_TEXT_COLOR, "Version %s", ui_constants::APP_VERSION);
		ImGui::Separator();
		ImGui::TextUnformatted("High-Performance Real-Time Fractal Viewer");
		ImGui::Text("Copyright (c) 2025 Krish Ganatra.");
		ImGui::Separator();
		ImGui::Text("Licensed under the MIT License.");
		ImGui::Text("Source code available at:");
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Text, ui_constants::HYPERLINK_COLOR);
		ImGui::PushStyleColor(ImGuiCol_Button, ui_constants::TRANSPARENT_COLOR);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ui_constants::TRANSPARENT_COLOR);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ui_constants::TRANSPARENT_COLOR);
		ImGui::PushStyleColor(ImGuiCol_Border, ui_constants::TRANSPARENT_COLOR);
		ImGui::PushStyleColor(ImGuiCol_BorderShadow, ui_constants::TRANSPARENT_COLOR);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ui_constants::HYPERLINK_FRAME_PADDING);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

		if (ImGui::Button(ui_constants::APP_REPO_URL_LABEL))
		{
			const std::string command = std::format(
#ifdef _WIN32
				"start {}",
#elif __APPLE__
				"open {}",
#elif __linux__
				"xdg-open {}",
#endif
				ui_constants::APP_REPO_URL);
			system(command.c_str());
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Left-click to open in browser\nRight-click to copy URL");
			ImVec2 textMin = ImGui::GetItemRectMin();
			ImVec2 textMax = ImGui::GetItemRectMax();
			ImGui::GetWindowDrawList()->AddLine(ImVec2(textMin.x, textMax.y), ImVec2(textMax.x, textMax.y),
												ImGui::GetColorU32(ui_constants::HYPERLINK_COLOR),
												ui_constants::HYPERLINK_UNDERLINE_THICKNESS);

			if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
			{
				FRACTAL_INFO("Copying URL to clipboard: {}", ui_constants::APP_REPO_URL);
				ImGui::SetClipboardText(ui_constants::APP_REPO_URL);
			}
		}

		ImGui::PopStyleVar(ui_constants::HYPERLINK_STYLE_VARS_TO_POP);
		ImGui::PopStyleColor(ui_constants::HYPERLINK_STYLE_COLORS_TO_POP);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		float contentRegionWidth = ImGui::GetContentRegionAvail().x;
		ImGui::SetCursorPosX(contentRegionWidth - ui_constants::CLOSE_BUTTON_WIDTH);

		if (ImGui::Button("Close", ImVec2(ui_constants::CLOSE_BUTTON_WIDTH, 0)))
		{
			uiState.showAboutModal = false;
		}

		ImGui::EndPopup();
	}
}

void UIManager::drawViewportPanel(FractalState& state, const UIState& uiState, GLuint textureID)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ui_constants::NO_PADDING);
	ImGui::Begin(ui_constants::VIEWPORT_WINDOW_TITLE);

	ImVec2 panelSize = ImGui::GetContentRegionAvail();
	if (panelSize.x > 0 && panelSize.y > 0)
	{
		if (state.renderWidth != static_cast<int>(panelSize.x) || state.renderHeight != static_cast<int>(panelSize.y))
		{
			state.renderWidth = static_cast<int>(panelSize.x);
			state.renderHeight = static_cast<int>(panelSize.y);
			if (onRequestRedraw)
				onRequestRedraw();
		}

		ImGui::Image(static_cast<ImTextureID>(textureID), panelSize, ui_constants::TEXTURE_UV_MIN,
					 ui_constants::TEXTURE_UV_MAX);

		if (ImGui::IsWindowHovered())
		{
			ImGuiIO& io = ImGui::GetIO();

			CameraController::Input input;
			input.viewportSize = { panelSize.x, panelSize.y };
			input.mousePos = { io.MousePos.x - ImGui::GetItemRectMin().x, io.MousePos.y - ImGui::GetItemRectMin().y };
			input.delta = { io.MouseDelta.x, io.MouseDelta.y };
			input.dragging = ImGui::IsMouseDragging(ImGuiMouseButton_Left);
			input.wheel = io.MouseWheel;

			m_cameraController.apply(state, input);

			if (input.dragging || input.wheel != 0.0)
			{
				if (onRequestRedraw)
					onRequestRedraw();
			}
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void UIManager::drawPropertiesPanel(FractalState& state)
{
	ImGui::Begin(ui_constants::PROPERTIES_WINDOW_TITLE);
	bool changed = false;

	if (ImGui::BeginCombo("Algorithm", FractalDefinitions.at(state.type).name.data()))
	{
		for (const auto& [type, def] : FractalDefinitions)
		{
			if (ImGui::Selectable(def.name.data(), state.type == type))
			{
				state.type = type;
				changed = true;
			}
		}
		ImGui::EndCombo();
	}

	changed |= ImGui::SliderInt("Max Iterations", &state.maxIterations, ui_constants::MIN_ITERATIONS,
								ui_constants::MAX_ITERATIONS);
	changed |= ImGui::InputDouble("Zoom", &state.zoom, 0.0, 0.0, ui_constants::ZOOM_FORMAT);
	changed |= ImGui::InputDouble("Offset X", &state.offset.x, 0.0, 0.0, ui_constants::COORD_FORMAT);
	changed |= ImGui::InputDouble("Offset Y", &state.offset.y, 0.0, 0.0, ui_constants::COORD_FORMAT);

	if (state.type == FractalType::Julia)
	{
		ImGui::SeparatorText("Julia Parameters");
		changed |= ImGui::InputDouble("Constant Real", &state.specificParams.juliaConstant.x,
									  ui_constants::JULIA_PARAM_STEP, 0.0, ui_constants::JULIA_PARAM_FORMAT);
		changed |= ImGui::InputDouble("Constant Imag", &state.specificParams.juliaConstant.y,
									  ui_constants::JULIA_PARAM_STEP, 0.0, ui_constants::JULIA_PARAM_FORMAT);
	}

	if (changed && onRequestRedraw)
		onRequestRedraw();
	ImGui::End();
}

void UIManager::drawColoringPanel(FractalState& state)
{
	ImGui::Begin(ui_constants::COLORING_WINDOW_TITLE);
	bool needsRedraw = false;

	ImGui::SeparatorText("Shading");
	needsRedraw |= ImGui::Checkbox("Smooth Coloring", &state.coloring.useSmoothing);

	needsRedraw |= ImGui::DragScalar("Palette Frequency", ImGuiDataType_Double, &state.coloring.paletteFrequency,
									 0.005f, NULL, NULL, "%.3f");

	ImGui::SeparatorText("Palette Gradient");
	needsRedraw |= drawPaletteEditor(state);

	if (needsRedraw && onRequestRedraw)
	{
		onRequestRedraw();
	}
	ImGui::End();
}

void UIManager::updateGradientTexture(const std::vector<ColorStop>& palette)
{
	if (palette.empty())
		return;

	const int textureSize = ui_constants::GRADIENT_TEXTURE_SIZE;
	std::vector<glm::vec3> gradientData(textureSize);

	auto sortedPalette = palette;
	std::ranges::sort(sortedPalette);

	if (sortedPalette.empty())
		return;

	for (int i = 0; i < textureSize; ++i)
	{
		float t = static_cast<float>(i) / (textureSize - 1);

		if (t <= sortedPalette.front().position)
		{
			gradientData[i] = sortedPalette.front().color;
			continue;
		}
		if (t >= sortedPalette.back().position)
		{
			gradientData[i] = sortedPalette.back().color;
			continue;
		}

		for (size_t j = 0; j < sortedPalette.size() - 1; ++j)
		{
			const auto& s1 = sortedPalette[j];
			const auto& s2 = sortedPalette[j + 1];
			if (t >= s1.position && t <= s2.position)
			{
				float segmentT = (s2.position - s1.position < ui_constants::EPSILON)
									 ? 0.0F
									 : (t - s1.position) / (s2.position - s1.position);
				gradientData[i] = glm::mix(s1.color, s2.color, segmentT);
				break;
			}
		}
	}

	glBindTexture(GL_TEXTURE_2D, m_gradientTexture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, textureSize, 1, 0, GL_RGB, GL_FLOAT, gradientData.data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool UIManager::drawPaletteEditor(FractalState& state)
{
	auto& palette = state.coloring.palette;
	bool paletteChanged = false;

	updateGradientTexture(palette);

	ImGui::Image(static_cast<ImTextureID>(m_gradientTexture),
				 { ImGui::GetContentRegionAvail().x, ui_constants::GRADIENT_PREVIEW_HEIGHT });

	ImGui::InvisibleButton(ui_constants::PALETTE_INTERACTION_ZONE_ID,
						   { ImGui::GetContentRegionAvail().x, ui_constants::PALETTE_EDITOR_INTERACTION_HEIGHT });
	ImVec2 p0 = ImGui::GetItemRectMin();
	ImVec2 p1 = ImGui::GetItemRectMax();
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	if (ImGui::IsItemClicked() && palette.size() < MAX_PALETTE_STOPS)
	{
		float newPosNorm = std::clamp((ImGui::GetMousePos().x - p0.x) / (p1.x - p0.x), 0.0F, 1.0F);
		palette.push_back({ .color = ui_constants::DEFAULT_NEW_COLOR_STOP, .position = newPosNorm });
		std::ranges::sort(palette);
		paletteChanged = true;
	}

	int stopToDelete = -1;
	for (size_t i = 0; i < palette.size(); ++i)
	{
		ImVec2 stopPosOnBar
			= { p0.x + ((p1.x - p0.x) * palette[i].position), p0.y + ui_constants::PALETTE_HANDLE_VERTICAL_OFFSET };

		ImGui::PushID(static_cast<int>(i));
		ImGui::SetCursorScreenPos({ stopPosOnBar.x - ui_constants::PALETTE_HANDLE_CENTER_FROM_LEFT,
									stopPosOnBar.y - ui_constants::PALETTE_HANDLE_VERTICAL_OFFSET });
		ImGui::InvisibleButton(ui_constants::PALETTE_HANDLE_ID, ui_constants::PALETTE_HANDLE_SIZE);

		const ImU32 colHandle = ImGui::GetColorU32(ImGuiCol_Text);
		const ImU32 colHandleHover = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
		const ImU32 handeColor = ImGui::IsItemHovered() || ImGui::IsItemActive() ? colHandleHover : colHandle;

		drawList->AddTriangleFilled({ stopPosOnBar.x - ui_constants::PALETTE_HANDLE_TRIANGLE_SIZE,
									  stopPosOnBar.y - ui_constants::PALETTE_HANDLE_TRIANGLE_SIZE },
									{ stopPosOnBar.x + ui_constants::PALETTE_HANDLE_TRIANGLE_SIZE,
									  stopPosOnBar.y - ui_constants::PALETTE_HANDLE_TRIANGLE_SIZE },
									stopPosOnBar, handeColor);
		drawList->AddCircleFilled({ stopPosOnBar.x, stopPosOnBar.y - ui_constants::PALETTE_HANDLE_CIRCLE_OFFSET },
								  ui_constants::PALETTE_HANDLE_CIRCLE_RADIUS, handeColor);

		if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			palette[i].position = std::clamp((ImGui::GetMousePos().x - p0.x) / (p1.x - p0.x), 0.0F, 1.0F);
			paletteChanged = true;
		}

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && palette.size() > ui_constants::MIN_PALETTE_STOPS)
		{
			stopToDelete = static_cast<int>(i);
		}

		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Pos: %.2F", palette[i].position);
		ImGui::PopID();
	}

	if (stopToDelete != -1)
	{
		palette.erase(palette.begin() + stopToDelete);
		paletteChanged = true;
	}

	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		std::sort(palette.begin(), palette.end());
		paletteChanged = true;
	}

	ImGui::Separator();
	for (size_t i = 0; i < palette.size(); ++i)
	{
		ImGui::PushID(static_cast<int>(i));
		if (ImGui::ColorEdit3(ui_constants::COLOR_EDIT_ID, reinterpret_cast<float*>(&palette[i].color)))
		{
			paletteChanged = true;
		}
		ImGui::SameLine();
		ImGui::Text("Stop %zu", i);
		ImGui::PopID();
	}

	return paletteChanged;
}

void UIManager::drawExportPanel(UIState& uiState)
{
	ImGui::Begin(ui_constants::EXPORT_WINDOW_TITLE);

	ImGui::InputText("Filename", &uiState.screenshotFilename);

	static constexpr std::array<const char*, 3> s_formats = { "PNG", "JPG", "BMP" };
	ImGui::Combo("Format", reinterpret_cast<int*>(&uiState.screenshotFormat), s_formats.data(),
				 static_cast<int>(s_formats.size()));

	static constexpr std::array<const char*, 4> s_factors = { "1x", "2x", "4x", "8x" };
	int factorIdx = static_cast<int>(log2(uiState.supersampleFactor));
	if (ImGui::Combo("Supersample", &factorIdx, s_factors.data(), static_cast<int>(s_factors.size())))
	{
		uiState.supersampleFactor = 1 << factorIdx;
	}

	if (ImGui::Button(ui_constants::SAVE_TO_FILE_BUTTON, ui_constants::FULL_WIDTH_BUTTON))
	{
		if (onRequestScreenshot)
		{
			ScreenshotRequest req;
			req.supersample = uiState.supersampleFactor;
			req.format = uiState.screenshotFormat;

			std::string extension;
			switch (req.format)
			{
				case ScreenshotFormat::PNG:
					extension = ui_constants::PNG_EXTENSION;
					break;
				case ScreenshotFormat::JPG:
					extension = ui_constants::JPG_EXTENSION;
					break;
				case ScreenshotFormat::BMP:
					extension = ui_constants::BMP_EXTENSION;
					break;
			}
			req.filepath = uiState.screenshotFilename + extension;

			onRequestScreenshot(req);
		}
	}
	ImGui::End();
}

void UIManager::drawStatusBar(const FractalState& state)
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking
							 | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing
							 | ImGuiWindowFlags_NoNav;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ui_constants::STATUS_BAR_PADDING);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0F);

	const float height = ImGui::GetTextLineHeight() + (ImGui::GetStyle().WindowPadding.y * 2.0F);

	if (ImGui::BeginViewportSideBar("StatusBar", ImGui::GetMainViewport(), ImGuiDir_Down, height, flags))
	{
		ImGui::Text(ui_constants::STATUS_BAR_FORMAT, state.offset.x, state.offset.y, state.zoom, state.renderWidth,
					state.renderHeight);
		ImGui::End();
	}
	ImGui::PopStyleVar(ui_constants::STATUS_BAR_STYLES_TO_POP);
}

void UIManager::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backupCurrentWindow = SDL_GL_GetCurrentWindow();
		SDL_GLContext backupCurrentContext = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backupCurrentWindow, backupCurrentContext);
	}
}
