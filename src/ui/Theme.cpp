#pragma once

#include "Theme.hpp"

#include <imgui.h>

void Theme::Apply()
{
	ImGuiStyle& style = ImGui::GetStyle();
	auto& colors = style.Colors;

	// Primary Colors
	ImVec4 bgPrimary = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
	ImVec4 bgSecondary = ImVec4(0.17f, 0.17f, 0.19f, 1.00f);
	ImVec4 bgTertiary = ImVec4(0.19f, 0.19f, 0.21f, 1.00f);

	// Accent Colors (Nord blues)
	ImVec4 accentPrimary = ImVec4(136 / 255.f, 192 / 255.f, 208 / 255.f, 1.00f);
	ImVec4 accentHover = ImVec4(129 / 255.f, 161 / 255.f, 193 / 255.f, 1.00f);
	ImVec4 accentActive = ImVec4(143 / 255.f, 188 / 255.f, 219 / 255.f, 1.00f);

	// Text Colors
	ImVec4 textPrimary = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	ImVec4 textDisabled = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

	// Border Color
	ImVec4 borderColor = ImVec4(0.205f, 0.205f, 0.225f, 1.00f);

	// Spacing & rounding
	style.WindowPadding = ImVec2(12, 12);
	style.FramePadding = ImVec2(8, 5);
	style.ItemSpacing = ImVec2(10, 6);
	style.ItemInnerSpacing = ImVec2(6, 4);
	style.ScrollbarSize = 12.0f;
	style.GrabMinSize = 12.0f;
	style.WindowRounding = 6.0f;
	style.FrameRounding = 6.0f;
	style.PopupRounding = 6.0f;
	style.ScrollbarRounding = 6.0f;
	style.GrabRounding = 6.0f;
	style.TabRounding = 6.0f;

	// Borders
	style.FrameBorderSize = 1.0f;
	style.TabBorderSize = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;

	// Core colors
	colors[ImGuiCol_Text] = textPrimary;
	colors[ImGuiCol_TextDisabled] = textDisabled;
	colors[ImGuiCol_WindowBg] = bgPrimary;
	colors[ImGuiCol_ChildBg] = bgPrimary;
	colors[ImGuiCol_PopupBg] = bgSecondary;
	colors[ImGuiCol_Border] = borderColor;
	colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

	// Frames & widgets
	colors[ImGuiCol_FrameBg] = bgSecondary;
	colors[ImGuiCol_FrameBgHovered] = bgTertiary;
	colors[ImGuiCol_FrameBgActive] = bgTertiary;
	colors[ImGuiCol_CheckMark] = accentPrimary;
	colors[ImGuiCol_SliderGrab] = accentPrimary;
	colors[ImGuiCol_SliderGrabActive] = accentActive;
	colors[ImGuiCol_Button] = accentPrimary;
	colors[ImGuiCol_ButtonHovered] = accentHover;
	colors[ImGuiCol_ButtonActive] = accentActive;

	// Titles & menus
	colors[ImGuiCol_TitleBg] = bgPrimary;
	colors[ImGuiCol_TitleBgActive] = bgSecondary;
	colors[ImGuiCol_TitleBgCollapsed] = bgPrimary;
	colors[ImGuiCol_MenuBarBg] = bgSecondary;

	// Scrollbars
	colors[ImGuiCol_ScrollbarBg] = bgSecondary;
	colors[ImGuiCol_ScrollbarGrab] = bgTertiary;
	colors[ImGuiCol_ScrollbarGrabHovered] = accentPrimary;
	colors[ImGuiCol_ScrollbarGrabActive] = accentActive;

	// Headers
	colors[ImGuiCol_Header] = bgTertiary;
	colors[ImGuiCol_HeaderHovered] = accentHover;
	colors[ImGuiCol_HeaderActive] = accentPrimary;

	// Separators
	colors[ImGuiCol_Separator] = borderColor;
	colors[ImGuiCol_SeparatorHovered] = accentPrimary;
	colors[ImGuiCol_SeparatorActive] = accentActive;

	// Tabs
	colors[ImGuiCol_Tab] = bgSecondary;
	colors[ImGuiCol_TabHovered] = accentHover;
	colors[ImGuiCol_TabActive] = accentPrimary;
	colors[ImGuiCol_TabUnfocused] = bgSecondary;
	colors[ImGuiCol_TabUnfocusedActive] = bgTertiary;

	// Docking
	colors[ImGuiCol_DockingPreview] = ImVec4(accentPrimary.x, accentPrimary.y, accentPrimary.z, 0.5f);
	colors[ImGuiCol_DockingEmptyBg] = bgPrimary;

	// Plotting
	colors[ImGuiCol_PlotLines] = accentPrimary;
	colors[ImGuiCol_PlotLinesHovered] = accentHover;
	colors[ImGuiCol_PlotHistogram] = accentPrimary;
	colors[ImGuiCol_PlotHistogramHovered] = accentHover;
	colors[ImGuiCol_TextSelectedBg] = ImVec4(accentPrimary.x, accentPrimary.y, accentPrimary.z, 0.35f);

	// Misc
	colors[ImGuiCol_DragDropTarget] = ImVec4(accentPrimary.x, accentPrimary.y, accentPrimary.z, 0.90f);
	colors[ImGuiCol_NavHighlight] = accentPrimary;
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
}
