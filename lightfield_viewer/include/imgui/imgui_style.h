#pragma once

// #include "IconsFontAwesome.h" // from https://github.com/juliettef/IconFontCppHeaders

namespace ImGui {

    void StyleColorsSober(ImGuiStyle *dst) {

        ImGuiStyle *style = dst ? dst : &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 2.0f;    // Radius of window corners rounding. Set to 0.0f to have rectangular windows
        style->ScrollbarRounding = 3.0f; // Radius of grab corners rounding for scrollbar
        style->GrabRounding = 2.0f;      // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
        style->AntiAliasedLines = true;
        style->AntiAliasedFill = true;
        style->WindowRounding = 2;
        style->ChildRounding = 2;
        style->ScrollbarSize = 16;
        style->ScrollbarRounding = 3;
        style->GrabRounding = 2;
        style->ItemSpacing.x = 10;
        style->ItemSpacing.y = 4;
        style->IndentSpacing = 22;
        style->FramePadding.x = 6;
        style->FramePadding.y = 4;
        style->Alpha = 1.0f;
        style->FrameRounding = 3.0f;

        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.27f, 0.34f, 0.36f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.46f, 0.47f, 0.52f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.50f, 0.41f, 0.20f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.54f, 0.51f, 0.47f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.68f, 0.57f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.42f, 0.51f, 0.63f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.76f, 0.77f, 0.78f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.67f, 0.80f, 0.89f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.51f, 0.60f, 0.62f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.62f, 0.51f, 0.37f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.58f, 0.48f, 0.39f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.61f, 0.49f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.17f, 0.37f, 0.60f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.31f, 0.46f, 0.64f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_Tab] = ImVec4(0.48f, 0.44f, 0.39f, 0.86f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.71f, 0.47f, 0.16f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.63f, 0.37f, 0.15f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.42f, 0.39f, 0.34f, 1.00f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.94f, 0.47f, 0.06f, 0.70f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }
    // generic miniDart theme
    void StyleColorsLightGreen(ImGuiStyle *dst) {
        ImGuiStyle *style = dst ? dst : &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 2.0f;    // Radius of window corners rounding. Set to 0.0f to have rectangular windows
        style->ScrollbarRounding = 3.0f; // Radius of grab corners rounding for scrollbar
        style->GrabRounding = 2.0f;      // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
        style->AntiAliasedLines = true;
        style->AntiAliasedFill = true;
        style->WindowRounding = 2;
        style->ChildRounding = 2;
        style->ScrollbarSize = 16;
        style->ScrollbarRounding = 3;
        style->GrabRounding = 2;
        style->ItemSpacing.x = 10;
        style->ItemSpacing.y = 4;
        style->IndentSpacing = 22;
        style->FramePadding.x = 6;
        style->FramePadding.y = 4;
        style->Alpha = 1.0f;
        style->FrameRounding = 3.0f;

        colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        // colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.93f, 0.93f, 0.93f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.71f, 0.71f, 0.71f, 0.08f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.04f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.71f, 0.71f, 0.71f, 0.55f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.94f, 0.94f, 0.94f, 0.55f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.71f, 0.78f, 0.69f, 0.98f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.82f, 0.78f, 0.78f, 0.51f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.61f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.90f, 0.90f, 0.90f, 0.30f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.92f, 0.92f, 0.78f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.184f, 0.407f, 0.193f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.71f, 0.78f, 0.69f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.725f, 0.805f, 0.702f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.793f, 0.900f, 0.836f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.71f, 0.78f, 0.69f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.71f, 0.78f, 0.69f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.71f, 0.78f, 0.69f, 1.00f);
        colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
        colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.45f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
    }

}; // namespace ImGui