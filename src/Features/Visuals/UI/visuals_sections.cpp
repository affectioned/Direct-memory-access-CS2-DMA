#include "Features/Visuals/UI/visuals_sections.h"

#include "app/Core/globals.h"

#include <imgui.h>

#include <algorithm>
#include <cmath>
#include <utility>

namespace
{
    enum class ItemVisualsGroup {
        Pistols,
        SMGs,
        Rifles,
        Snipers,
        Heavy,
        Gear
    };

    struct ItemVisualsEntry {
        uint16_t id;
        const char* label;
        ItemVisualsGroup group;
    };

    enum class VisualsIcon {
        Enable,
        Preview,
        Box,
        Health,
        Armor,
        Visibility,
        Weapon,
        Skeleton,
        Snap,
        Arrows,
        Flags,
        Item,
        World,
        Bomb,
        Sound,
        Dot
    };

    constexpr ItemVisualsEntry kItemVisualsEntries[] = {
        { 1, "Deagle", ItemVisualsGroup::Pistols },
        { 2, "Dual Berettas", ItemVisualsGroup::Pistols },
        { 3, "Five-SeveN", ItemVisualsGroup::Pistols },
        { 4, "Glock", ItemVisualsGroup::Pistols },
        { 30, "Tec-9", ItemVisualsGroup::Pistols },
        { 32, "P2000", ItemVisualsGroup::Pistols },
        { 36, "P250", ItemVisualsGroup::Pistols },
        { 61, "USP-S", ItemVisualsGroup::Pistols },
        { 63, "CZ75", ItemVisualsGroup::Pistols },
        { 64, "R8", ItemVisualsGroup::Pistols },
        { 17, "MAC-10", ItemVisualsGroup::SMGs },
        { 19, "P90", ItemVisualsGroup::SMGs },
        { 23, "MP5", ItemVisualsGroup::SMGs },
        { 24, "UMP-45", ItemVisualsGroup::SMGs },
        { 26, "PP-Bizon", ItemVisualsGroup::SMGs },
        { 33, "MP7", ItemVisualsGroup::SMGs },
        { 34, "MP9", ItemVisualsGroup::SMGs },
        { 7, "AK-47", ItemVisualsGroup::Rifles },
        { 8, "AUG", ItemVisualsGroup::Rifles },
        { 10, "FAMAS", ItemVisualsGroup::Rifles },
        { 13, "Galil", ItemVisualsGroup::Rifles },
        { 16, "M4A4", ItemVisualsGroup::Rifles },
        { 39, "SG553", ItemVisualsGroup::Rifles },
        { 60, "M4A1-S", ItemVisualsGroup::Rifles },
        { 9, "AWP", ItemVisualsGroup::Snipers },
        { 11, "G3SG1", ItemVisualsGroup::Snipers },
        { 38, "SCAR-20", ItemVisualsGroup::Snipers },
        { 40, "SSG08", ItemVisualsGroup::Snipers },
        { 14, "M249", ItemVisualsGroup::Heavy },
        { 25, "XM1014", ItemVisualsGroup::Heavy },
        { 27, "MAG-7", ItemVisualsGroup::Heavy },
        { 28, "Negev", ItemVisualsGroup::Heavy },
        { 29, "Sawed-Off", ItemVisualsGroup::Heavy },
        { 35, "Nova", ItemVisualsGroup::Heavy },
        { 31, "Zeus", ItemVisualsGroup::Gear },
        { 57, "Healthshot", ItemVisualsGroup::Gear },
    };
    constexpr ImGuiColorEditFlags kPickerFlags =
        ImGuiColorEditFlags_AlphaBar |
        ImGuiColorEditFlags_AlphaPreviewHalf;

    constexpr ImGuiColorEditFlags kInlineColorFlags =
        kPickerFlags | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel;

    constexpr float kSliderWidth = 160.0f;
    constexpr float kVisualsRowHeight = 50.0f;
    constexpr float kVisualsRowGap = 16.0f;
    constexpr float kVisualsColumnGap = 18.0f;
    constexpr float kVisualsRowRounding = 8.0f;
    constexpr float kPi = 3.1415926535f;

    ImU32 ColorU32(int r, int g, int b, int a)
    {
        return IM_COL32(r, g, b, a);
    }

    ImVec2 Pixel(float x, float y)
    {
        return ImVec2(std::floor(x) + 0.5f, std::floor(y) + 0.5f);
    }

    ImVec2 Add(const ImVec2& p, float x, float y)
    {
        return Pixel(p.x + x, p.y + y);
    }

    void DrawGearIcon(ImDrawList* drawList, const ImVec2& center, ImU32 color)
    {
        ImVec2 teeth[16] = {};
        for (int i = 0; i < 16; ++i) {
            const float a = (static_cast<float>(i) * kPi) / 8.0f;
            const float r = (i % 2 == 0) ? 7.4f : 5.7f;
            teeth[i] = Pixel(center.x + std::cos(a) * r, center.y + std::sin(a) * r);
        }
        drawList->AddPolyline(teeth, 16, color, ImDrawFlags_Closed, 1.35f);
        drawList->AddCircle(center, 2.2f, color, 16, 1.25f);
    }

    void DrawSoftIconCircle(ImDrawList* drawList, const ImVec2& center, float radius, ImU32 color)
    {
        drawList->AddCircle(center, radius, color, 28, 1.35f);
    }

    void DrawCornerBoxIcon(ImDrawList* drawList, const ImVec2& p, ImU32 color)
    {
        constexpr float s = 17.0f;
        constexpr float l = 5.0f;
        const ImVec2 q = Add(p, 1.0f, 1.0f);
        drawList->AddLine(Add(q, 0.0f, 0.0f), Add(q, l, 0.0f), color, 1.6f);
        drawList->AddLine(Add(q, 0.0f, 0.0f), Add(q, 0.0f, l), color, 1.6f);
        drawList->AddLine(Add(q, s, 0.0f), Add(q, s - l, 0.0f), color, 1.6f);
        drawList->AddLine(Add(q, s, 0.0f), Add(q, s, l), color, 1.6f);
        drawList->AddLine(Add(q, 0.0f, s), Add(q, l, s), color, 1.6f);
        drawList->AddLine(Add(q, 0.0f, s), Add(q, 0.0f, s - l), color, 1.6f);
        drawList->AddLine(Add(q, s, s), Add(q, s - l, s), color, 1.6f);
        drawList->AddLine(Add(q, s, s), Add(q, s, s - l), color, 1.6f);
    }

    uintptr_t VisualsIconTexture(VisualsIcon icon)
    {
        switch (icon) {
        case VisualsIcon::Enable:
            return g::visualsUiIcons.enableVisuals;
        case VisualsIcon::Preview:
            return g::visualsUiIcons.visualsPreview;
        case VisualsIcon::Box:
            return g::visualsUiIcons.cornerBox;
        case VisualsIcon::Health:
            return g::visualsUiIcons.healthBar;
        case VisualsIcon::Armor:
            return g::visualsUiIcons.armorBar;
        case VisualsIcon::Visibility:
            return g::visualsUiIcons.visibilityColors;
        case VisualsIcon::Weapon:
            return g::visualsUiIcons.weaponLabel;
        case VisualsIcon::Skeleton:
            return g::visualsUiIcons.skeleton;
        case VisualsIcon::Snap:
            return g::visualsUiIcons.snapLines;
        case VisualsIcon::Flags:
            return g::visualsUiIcons.playerFlags;
        case VisualsIcon::World:
            return g::visualsUiIcons.worldVisuals;
        case VisualsIcon::Bomb:
            return g::visualsUiIcons.bombVisuals;
        default:
            return 0;
        }
    }

    void DrawVisualsIcon(ImDrawList* drawList, VisualsIcon icon, const ImVec2& pos, float size, ImU32 color)
    {
        const uintptr_t textureId = VisualsIconTexture(icon);
        if (textureId != 0) {
            drawList->AddImage(
                ImTextureRef(static_cast<ImTextureID>(textureId)),
                Pixel(pos.x, pos.y),
                Pixel(pos.x + size, pos.y + size),
                ImVec2(0.0f, 0.0f),
                ImVec2(1.0f, 1.0f),
                color);
            return;
        }

        constexpr float stroke = 1.65f;
        const ImVec2 c = Add(pos, 10.0f, 10.0f);

        switch (icon) {
        case VisualsIcon::Enable:
            DrawSoftIconCircle(drawList, c, 7.5f, color);
            drawList->AddLine(Add(c, -3.8f, 0.0f), Add(c, -1.0f, 3.0f), color, 1.9f);
            drawList->AddLine(Add(c, -1.0f, 3.0f), Add(c, 5.0f, -4.2f), color, 1.9f);
            break;
        case VisualsIcon::Preview:
            drawList->AddRect(Add(pos, 2.0f, 3.0f), Add(pos, 18.0f, 17.0f), color, 3.5f, 0, stroke);
            drawList->AddCircleFilled(Add(pos, 14.0f, 7.0f), 1.6f, color);
            drawList->AddLine(Add(pos, 5.0f, 15.0f), Add(pos, 9.0f, 10.5f), color, stroke);
            drawList->AddLine(Add(pos, 9.0f, 10.5f), Add(pos, 15.0f, 15.0f), color, stroke);
            break;
        case VisualsIcon::Box:
            DrawCornerBoxIcon(drawList, pos, color);
            break;
        case VisualsIcon::Health:
            drawList->AddRect(Add(c, -6.8f, -6.8f), Add(c, 6.8f, 6.8f), color, 3.0f, 0, 1.35f);
            drawList->AddLine(Add(c, -3.8f, 0.0f), Add(c, 3.8f, 0.0f), color, 1.65f);
            drawList->AddLine(Add(c, 0.0f, -3.8f), Add(c, 0.0f, 3.8f), color, 1.65f);
            break;
        case VisualsIcon::Armor: {
            const ImVec2 pts[] = {
                Add(c, 0.0f, -8.2f),
                Add(c, 6.5f, -5.0f),
                Add(c, 5.2f, 5.0f),
                Add(c, 0.0f, 8.3f),
                Add(c, -5.2f, 5.0f),
                Add(c, -6.5f, -5.0f)
            };
            drawList->AddPolyline(pts, 6, color, ImDrawFlags_Closed, stroke);
            break;
        }
        case VisualsIcon::Visibility:
            drawList->AddCircle(c, 7.6f, color, 30, stroke);
            drawList->AddCircleFilled(Add(c, -3.3f, -2.8f), 1.35f, color, 12);
            drawList->AddCircleFilled(Add(c, 1.2f, -4.0f), 1.35f, color, 12);
            drawList->AddCircleFilled(Add(c, 4.2f, -0.7f), 1.35f, color, 12);
            drawList->AddCircle(Add(c, 1.9f, 3.5f), 2.1f, color, 16, 1.25f);
            break;
        case VisualsIcon::Weapon:
        {
            const ImVec2 pistol[] = {
                Add(pos, 2.5f, 7.0f),
                Add(pos, 14.5f, 7.0f),
                Add(pos, 17.0f, 9.2f),
                Add(pos, 16.0f, 11.0f),
                Add(pos, 10.0f, 11.0f),
                Add(pos, 8.3f, 16.5f),
                Add(pos, 5.2f, 16.5f),
                Add(pos, 6.0f, 11.0f),
                Add(pos, 2.5f, 11.0f)
            };
            drawList->AddPolyline(pistol, 9, color, ImDrawFlags_Closed, stroke);
            drawList->AddLine(Add(pos, 11.7f, 11.0f), Add(pos, 12.5f, 13.5f), color, 1.2f);
            break;
        }
        case VisualsIcon::Skeleton:
            drawList->AddCircle(Add(pos, 10.0f, 4.0f), 2.1f, color, 16, 1.3f);
            drawList->AddLine(Add(pos, 10.0f, 6.8f), Add(pos, 10.0f, 12.6f), color, 1.45f);
            drawList->AddLine(Add(pos, 6.4f, 9.4f), Add(pos, 13.6f, 9.4f), color, 1.45f);
            drawList->AddLine(Add(pos, 10.0f, 12.6f), Add(pos, 7.1f, 17.4f), color, 1.45f);
            drawList->AddLine(Add(pos, 10.0f, 12.6f), Add(pos, 12.9f, 17.4f), color, 1.45f);
            break;
        case VisualsIcon::Snap:
            drawList->AddLine(Add(c, -6.5f, 0.0f), Add(c, -2.7f, 0.0f), color, 1.35f);
            drawList->AddLine(Add(c, 2.7f, 0.0f), Add(c, 6.5f, 0.0f), color, 1.35f);
            drawList->AddLine(Add(c, 0.0f, -6.5f), Add(c, 0.0f, -2.7f), color, 1.35f);
            drawList->AddLine(Add(c, 0.0f, 2.7f), Add(c, 0.0f, 6.5f), color, 1.35f);
            drawList->AddCircle(c, 2.4f, color, 18, 1.25f);
            break;
        case VisualsIcon::Arrows:
            drawList->AddLine(Add(c, -6.8f, 0.0f), Add(c, 6.8f, 0.0f), color, stroke);
            drawList->AddLine(Add(c, 0.0f, -6.8f), Add(c, 0.0f, 6.8f), color, stroke);
            drawList->AddLine(Add(c, 6.8f, 0.0f), Add(c, 3.8f, -3.0f), color, stroke);
            drawList->AddLine(Add(c, 6.8f, 0.0f), Add(c, 3.8f, 3.0f), color, stroke);
            drawList->AddLine(Add(c, -6.8f, 0.0f), Add(c, -3.8f, -3.0f), color, stroke);
            drawList->AddLine(Add(c, -6.8f, 0.0f), Add(c, -3.8f, 3.0f), color, stroke);
            drawList->AddLine(Add(c, 0.0f, -6.8f), Add(c, -3.0f, -3.8f), color, stroke);
            drawList->AddLine(Add(c, 0.0f, -6.8f), Add(c, 3.0f, -3.8f), color, stroke);
            drawList->AddLine(Add(c, 0.0f, 6.8f), Add(c, -3.0f, 3.8f), color, stroke);
            drawList->AddLine(Add(c, 0.0f, 6.8f), Add(c, 3.0f, 3.8f), color, stroke);
            break;
        case VisualsIcon::Flags:
            drawList->AddLine(Add(pos, 5.0f, 3.0f), Add(pos, 5.0f, 18.0f), color, stroke);
            drawList->AddLine(Add(pos, 6.0f, 4.0f), Add(pos, 16.2f, 6.7f), color, stroke);
            drawList->AddLine(Add(pos, 16.2f, 6.7f), Add(pos, 6.0f, 10.5f), color, stroke);
            break;
        case VisualsIcon::Item:
            drawList->AddRect(Add(pos, 5.0f, 5.0f), Add(pos, 15.0f, 15.0f), color, 2.0f, 0, stroke);
            drawList->AddCircleFilled(Add(pos, 15.5f, 4.5f), 1.9f, color);
            break;
        case VisualsIcon::World:
            drawList->AddCircle(c, 7.8f, color, 30, stroke);
            drawList->AddLine(Add(c, -7.8f, 0.0f), Add(c, 7.8f, 0.0f), color, 1.25f);
            drawList->AddLine(Add(c, 0.0f, -7.8f), Add(c, 0.0f, 7.8f), color, 1.25f);
            drawList->AddEllipse(c, ImVec2(4.0f, 7.8f), color, 0.0f, 22, 1.2f);
            break;
        case VisualsIcon::Bomb:
            drawList->AddCircle(Add(pos, 9.6f, 11.6f), 5.8f, color, 28, 1.55f);
            drawList->AddRectFilled(Add(pos, 7.1f, 5.2f), Add(pos, 12.1f, 7.2f), color, 1.0f);
            drawList->AddLine(Add(pos, 12.0f, 6.2f), Add(pos, 16.0f, 3.4f), color, 1.35f);
            drawList->AddLine(Add(pos, 16.0f, 3.4f), Add(pos, 18.0f, 4.4f), color, 1.15f);
            drawList->AddCircleFilled(Add(pos, 9.6f, 11.6f), 1.25f, color);
            break;
        case VisualsIcon::Sound:
            drawList->AddCircleFilled(c, 1.6f, color);
            drawList->AddCircle(c, 4.4f, color, 24, 1.4f);
            drawList->AddCircle(c, 7.6f, color, 28, 1.4f);
            break;
        case VisualsIcon::Dot:
            drawList->AddCircleFilled(c, 4.0f, color);
            break;
        default:
            break;
        }
    }

    bool ToggleSwitch(const char* id, bool* value)
    {
        if (!value)
            return false;

        ImGui::PushID(id);
        const ImVec2 size(38.0f, 20.0f);
        const ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::InvisibleButton("##toggle", size);
        const bool clicked = ImGui::IsItemClicked(ImGuiMouseButton_Left);
        if (clicked)
            *value = !*value;

        const bool hovered = ImGui::IsItemHovered();
        const float t = *value ? 1.0f : 0.0f;
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const ImU32 trackColor = *value
            ? ColorU32(37, 116, 255, hovered ? 255 : 242)
            : ColorU32(54, 62, 74, hovered ? 245 : 218);
        const ImVec2 min = Pixel(pos.x, pos.y);
        const ImVec2 max = Pixel(pos.x + size.x, pos.y + size.y);
        drawList->AddRectFilled(min, max, trackColor, size.y * 0.5f);
        if (*value) {
            drawList->AddRect(
                Pixel(pos.x - 0.5f, pos.y - 0.5f),
                Pixel(pos.x + size.x + 0.5f, pos.y + size.y + 0.5f),
                ColorU32(96, 158, 255, hovered ? 150 : 108),
                size.y * 0.5f,
                0,
                1.0f);
        }
        drawList->AddCircleFilled(
            Pixel(pos.x + 10.0f + t * 18.0f, pos.y + 10.8f),
            7.0f,
            ColorU32(0, 0, 0, *value ? 58 : 38));
        drawList->AddCircleFilled(
            Pixel(pos.x + 10.0f + t * 18.0f, pos.y + 10.0f),
            6.9f,
            ColorU32(240, 246, 255, 255));

        ImGui::PopID();
        return clicked;
    }

    bool GearButton(const char* id)
    {
        ImGui::PushID(id);
        const ImVec2 size(26.0f, 26.0f);
        const ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::InvisibleButton("##gear", size);
        const bool clicked = ImGui::IsItemClicked(ImGuiMouseButton_Left);
        const bool hovered = ImGui::IsItemHovered();

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(
            Pixel(pos.x, pos.y),
            Pixel(pos.x + size.x, pos.y + size.y),
            hovered ? ColorU32(25, 36, 52, 248) : ColorU32(14, 22, 34, 238),
            7.0f);
        drawList->AddRect(
            Pixel(pos.x, pos.y),
            Pixel(pos.x + size.x, pos.y + size.y),
            ColorU32(60, 78, 105, hovered ? 245 : 170),
            7.0f);
        DrawGearIcon(drawList, Pixel(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f), ColorU32(170, 184, 205, hovered ? 255 : 220));

        ImGui::PopID();
        return clicked;
    }

    void ColorRow(const char* id, const char* label, float* color);

    template <typename ExtraFn>
    void DrawOptionRow(const char* id,
                       VisualsIcon icon,
                       const char* label,
                       const char* description,
                       bool* enabled,
                       float* color,
                       ExtraFn&& extraFn,
                       bool showSettings = true,
                       float forcedWidth = 0.0f)
    {
        ImGui::PushID(id);

        const float availableWidth = forcedWidth > 0.0f ? forcedWidth : ImGui::GetContentRegionAvail().x - 2.0f;
        const float rowWidth = std::max(260.0f, availableWidth);
        const ImVec2 cursorPos = ImGui::GetCursorScreenPos();
        const ImVec2 rowPos = Pixel(cursorPos.x, cursorPos.y);
        const ImVec2 rowMax = Pixel(rowPos.x + rowWidth, rowPos.y + kVisualsRowHeight);
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const bool isEnabled = enabled && *enabled;
        const bool rowHovered = ImGui::IsMouseHoveringRect(rowPos, rowMax);

        drawList->AddRectFilled(
            rowPos,
            rowMax,
            rowHovered ? ColorU32(12, 24, 39, 250) : ColorU32(8, 17, 29, 240),
            kVisualsRowRounding);
        drawList->AddRect(
            rowPos,
            rowMax,
            isEnabled ? ColorU32(38, 92, 162, rowHovered ? 135 : 92) : ColorU32(36, 48, 65, rowHovered ? 105 : 72),
            kVisualsRowRounding,
            0,
            0.65f);

        if (isEnabled) {
            drawList->AddRectFilled(
                ImVec2(rowPos.x + 1.0f, rowPos.y + 12.0f),
                ImVec2(rowPos.x + 2.0f, rowMax.y - 12.0f),
                ColorU32(37, 113, 255, 82),
                1.5f);
        }

        constexpr float iconSize = 20.0f;
        const ImVec2 iconPos(rowPos.x + 17.0f, rowPos.y + (kVisualsRowHeight - iconSize) * 0.5f);
        DrawVisualsIcon(drawList, icon, iconPos, iconSize, ColorU32(205, 216, 232, isEnabled ? 245 : 172));

        const ImVec2 labelPos(rowPos.x + 55.0f, rowPos.y + (kVisualsRowHeight - ImGui::GetFontSize()) * 0.5f);
        drawList->AddText(labelPos, ColorU32(226, 234, 246, isEnabled ? 255 : 205), label);
        (void)description;

        const float rightPad = 12.0f;
        const float gearWidth = showSettings ? 32.0f : 0.0f;
        const float toggleX = rowMax.x - rightPad - gearWidth - 38.0f - (showSettings ? 8.0f : 0.0f);
        ImGui::SetCursorScreenPos(ImVec2(toggleX, rowPos.y + (kVisualsRowHeight - 20.0f) * 0.5f));
        ToggleSwitch("toggle", enabled);

        if (showSettings) {
            ImGui::SetCursorScreenPos(ImVec2(rowMax.x - rightPad - 26.0f, rowPos.y + (kVisualsRowHeight - 26.0f) * 0.5f));
            if (GearButton("settings"))
                ImGui::OpenPopup("##cfg");
        }

        ImGui::SetCursorScreenPos(ImVec2(rowPos.x, rowMax.y + kVisualsRowGap));
        ImGui::Dummy(ImVec2(rowWidth, 1.0f));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.0f, 12.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.035f, 0.055f, 0.085f, 0.98f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.16f, 0.24f, 0.36f, 0.82f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.07f, 0.10f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.10f, 0.14f, 0.21f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.20f, 0.48f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.20f, 0.48f, 1.0f, 1.0f));
        ImGui::SetNextWindowSizeConstraints(ImVec2(380.0f, 0.0f), ImVec2(560.0f, FLT_MAX));
        if (ImGui::BeginPopup("##cfg")) {
            ImGui::TextColored(ImVec4(0.86f, 0.92f, 1.0f, 1.0f), "%s Settings", label);
            ImDrawList* popupDrawList = ImGui::GetWindowDrawList();
            const ImVec2 linePos = ImGui::GetCursorScreenPos();
            popupDrawList->AddRectFilled(
                linePos,
                ImVec2(linePos.x + ImGui::GetContentRegionAvail().x, linePos.y + 1.0f),
                ColorU32(52, 105, 186, 138),
                1.0f);
            ImGui::Spacing();
            ImGui::Spacing();

            if (color)
                ColorRow("accent", "Accent Color", color);

            extraFn();

            ImGui::EndPopup();
        }
        ImGui::PopStyleColor(6);
        ImGui::PopStyleVar(4);

        ImGui::PopID();
    }

    template <typename ExtraFn>
    void DrawFeature(const char* id, const char* label, bool* enabled, float* color, ExtraFn&& extraFn)
    {
        DrawOptionRow(id, VisualsIcon::Dot, label, "", enabled, color, std::forward<ExtraFn>(extraFn), true);
    }

    void ColorRow(const char* id, const char* label, float* color)
    {
        ImGui::PushID(id);
        const float rowWidth = std::max(300.0f, ImGui::GetContentRegionAvail().x);
        const float rowHeight = 34.0f;
        const ImVec2 rowPos = Pixel(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
        const ImVec2 rowMax = Pixel(rowPos.x + rowWidth, rowPos.y + rowHeight);
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(rowPos, rowMax, ColorU32(8, 17, 29, 238), 7.0f);
        drawList->AddRect(rowPos, rowMax, ColorU32(41, 58, 82, 120), 7.0f, 0, 0.65f);
        drawList->AddText(
            ImVec2(rowPos.x + 12.0f, rowPos.y + (rowHeight - ImGui::GetFontSize()) * 0.5f),
            ColorU32(226, 234, 246, 235),
            label);
        ImGui::SetCursorScreenPos(ImVec2(rowMax.x - 40.0f, rowPos.y + 5.0f));
        ImGui::SetNextItemWidth(26.0f);
        ImGui::ColorEdit4("##c", color, kInlineColorFlags);
        ImGui::SetCursorScreenPos(ImVec2(rowPos.x, rowMax.y + 7.0f));
        ImGui::Dummy(ImVec2(rowWidth, 1.0f));
        ImGui::PopID();
    }

    bool ToggleSetting(const char* id, const char* label, bool* toggle)
    {
        ImGui::PushID(id);
        const float rowWidth = std::max(300.0f, ImGui::GetContentRegionAvail().x);
        const float rowHeight = 34.0f;
        const ImVec2 rowPos = Pixel(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
        const ImVec2 rowMax = Pixel(rowPos.x + rowWidth, rowPos.y + rowHeight);
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const bool isOn = toggle && *toggle;
        drawList->AddRectFilled(rowPos, rowMax, ColorU32(8, 17, 29, 238), 7.0f);
        drawList->AddRect(
            rowPos,
            rowMax,
            isOn ? ColorU32(38, 92, 162, 102) : ColorU32(41, 58, 82, 116),
            7.0f,
            0,
            0.65f);
        drawList->AddText(
            ImVec2(rowPos.x + 12.0f, rowPos.y + (rowHeight - ImGui::GetFontSize()) * 0.5f),
            ColorU32(226, 234, 246, isOn ? 255 : 218),
            label);
        ImGui::SetCursorScreenPos(ImVec2(rowMax.x - 50.0f, rowPos.y + 7.0f));
        const bool changed = ToggleSwitch("toggle", toggle);
        ImGui::SetCursorScreenPos(ImVec2(rowPos.x, rowMax.y + 7.0f));
        ImGui::Dummy(ImVec2(rowWidth, 1.0f));
        ImGui::PopID();
        return changed;
    }

    void ToggleColorRow(const char* id, const char* label, bool* toggle, float* color)
    {
        ImGui::PushID(id);
        const float rowWidth = std::max(320.0f, ImGui::GetContentRegionAvail().x);
        const float rowHeight = 34.0f;
        const ImVec2 rowPos = Pixel(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
        const ImVec2 rowMax = Pixel(rowPos.x + rowWidth, rowPos.y + rowHeight);
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const bool isOn = toggle && *toggle;
        drawList->AddRectFilled(rowPos, rowMax, ColorU32(8, 17, 29, 238), 7.0f);
        drawList->AddRect(
            rowPos,
            rowMax,
            isOn ? ColorU32(38, 92, 162, 102) : ColorU32(41, 58, 82, 116),
            7.0f,
            0,
            0.65f);
        drawList->AddText(
            ImVec2(rowPos.x + 12.0f, rowPos.y + (rowHeight - ImGui::GetFontSize()) * 0.5f),
            ColorU32(226, 234, 246, isOn ? 255 : 218),
            label);
        ImGui::SetCursorScreenPos(ImVec2(rowMax.x - 92.0f, rowPos.y + 7.0f));
        ToggleSwitch("toggle", toggle);
        ImGui::SetCursorScreenPos(ImVec2(rowMax.x - 40.0f, rowPos.y + 5.0f));
        ImGui::SetNextItemWidth(26.0f);
        ImGui::ColorEdit4("##c", color, kInlineColorFlags);
        ImGui::SetCursorScreenPos(ImVec2(rowPos.x, rowMax.y + 7.0f));
        ImGui::Dummy(ImVec2(rowWidth, 1.0f));
        ImGui::PopID();
    }

    void FloatRow(const char* id, const char* label, float* value, float minValue, float maxValue, const char* fmt)
    {
        ImGui::PushID(id);
        const float rowWidth = std::max(340.0f, ImGui::GetContentRegionAvail().x);
        const float rowHeight = 38.0f;
        const ImVec2 rowPos = Pixel(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
        const ImVec2 rowMax = Pixel(rowPos.x + rowWidth, rowPos.y + rowHeight);
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(rowPos, rowMax, ColorU32(8, 17, 29, 238), 7.0f);
        drawList->AddRect(rowPos, rowMax, ColorU32(41, 58, 82, 116), 7.0f, 0, 0.65f);
        drawList->AddText(
            ImVec2(rowPos.x + 12.0f, rowPos.y + (rowHeight - ImGui::GetFontSize()) * 0.5f),
            ColorU32(226, 234, 246, 230),
            label);
        ImGui::SetCursorScreenPos(ImVec2(rowMax.x - kSliderWidth - 12.0f, rowPos.y + 8.0f));
        ImGui::SetNextItemWidth(kSliderWidth);
        ImGui::SliderFloat("##s", value, minValue, maxValue, fmt);
        ImGui::SetCursorScreenPos(ImVec2(rowPos.x, rowMax.y + 7.0f));
        ImGui::Dummy(ImVec2(rowWidth, 1.0f));
        ImGui::PopID();
    }

    void SizeRow(const char* id, const char* label, float* size, float minValue, float maxValue)
    {
        ImGui::PushID(id);
        const float rowWidth = std::max(340.0f, ImGui::GetContentRegionAvail().x);
        const float rowHeight = 38.0f;
        const ImVec2 rowPos = Pixel(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
        const ImVec2 rowMax = Pixel(rowPos.x + rowWidth, rowPos.y + rowHeight);
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(rowPos, rowMax, ColorU32(8, 17, 29, 238), 7.0f);
        drawList->AddRect(rowPos, rowMax, ColorU32(41, 58, 82, 116), 7.0f, 0, 0.65f);
        drawList->AddText(
            ImVec2(rowPos.x + 12.0f, rowPos.y + (rowHeight - ImGui::GetFontSize()) * 0.5f),
            ColorU32(226, 234, 246, 230),
            label);
        ImGui::SetCursorScreenPos(ImVec2(rowMax.x - kSliderWidth - 12.0f, rowPos.y + 8.0f));
        ImGui::SetNextItemWidth(kSliderWidth);
        ImGui::SliderFloat("##s", size, minValue, maxValue, *size == 0.0f ? "Default" : "%.0f");
        ImGui::SetCursorScreenPos(ImVec2(rowPos.x, rowMax.y + 7.0f));
        ImGui::Dummy(ImVec2(rowWidth, 1.0f));
        ImGui::PopID();
    }

    void FlagSettingsRow(const char* id, const char* label, bool* toggle, float* color, float* size)
    {
        ImGui::PushID(id);
        const float rowWidth = std::max(430.0f, ImGui::GetContentRegionAvail().x);
        const float rowHeight = 38.0f;
        const ImVec2 rowPos = Pixel(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
        const ImVec2 rowMax = Pixel(rowPos.x + rowWidth, rowPos.y + rowHeight);
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const bool isOn = toggle && *toggle;
        drawList->AddRectFilled(rowPos, rowMax, ColorU32(8, 17, 29, 238), 7.0f);
        drawList->AddRect(
            rowPos,
            rowMax,
            isOn ? ColorU32(38, 92, 162, 102) : ColorU32(41, 58, 82, 116),
            7.0f,
            0,
            0.65f);
        drawList->AddText(
            ImVec2(rowPos.x + 12.0f, rowPos.y + (rowHeight - ImGui::GetFontSize()) * 0.5f),
            ColorU32(226, 234, 246, isOn ? 255 : 218),
            label);
        ImGui::SetCursorScreenPos(ImVec2(rowMax.x - 260.0f, rowPos.y + 9.0f));
        ToggleSwitch("toggle", toggle);
        ImGui::SetCursorScreenPos(ImVec2(rowMax.x - 210.0f, rowPos.y + 7.0f));
        ImGui::SetNextItemWidth(26.0f);
        ImGui::ColorEdit4("##c", color, kInlineColorFlags);
        ImGui::SetCursorScreenPos(ImVec2(rowMax.x - 170.0f, rowPos.y + 9.0f));
        ImGui::SetNextItemWidth(158.0f);
        ImGui::SliderFloat("##s", size, 0.0f, 24.0f, *size == 0.0f ? "Default" : "%.0f");
        ImGui::SetCursorScreenPos(ImVec2(rowPos.x, rowMax.y + 7.0f));
        ImGui::Dummy(ImVec2(rowWidth, 1.0f));
        ImGui::PopID();
    }

    const char* ItemVisualsGroupLabel(ItemVisualsGroup group)
    {
        switch (group) {
        case ItemVisualsGroup::Pistols: return "Pistols";
        case ItemVisualsGroup::SMGs: return "SMGs";
        case ItemVisualsGroup::Rifles: return "Rifles";
        case ItemVisualsGroup::Snipers: return "Snipers";
        case ItemVisualsGroup::Heavy: return "Heavy";
        case ItemVisualsGroup::Gear: return "Gear";
        default: break;
        }
        return "Items";
    }

    void SetItemVisualsEnabled(uint16_t id, bool enabled)
    {
        if (id == 0 || id >= 1200)
            return;
        g::visualsItemEnabledMask.set(id, enabled);
    }

    bool IsItemVisualsEnabled(uint16_t id)
    {
        if (id == 0 || id >= 1200)
            return false;
        return g::visualsItemEnabledMask.test(id);
    }

    void SetAllKnownItemsEnabled(bool enabled)
    {
        for (const ItemVisualsEntry& entry : kItemVisualsEntries)
            SetItemVisualsEnabled(entry.id, enabled);
    }

    void RenderItemGroupBlock(ItemVisualsGroup group)
    {
        ImGui::TextDisabled("%s", ItemVisualsGroupLabel(group));
        ImGui::Separator();
        ImGui::Spacing();

        for (const ItemVisualsEntry& entry : kItemVisualsEntries) {
            if (entry.group != group)
                continue;

            bool enabled = IsItemVisualsEnabled(entry.id);
            if (ImGui::Checkbox(entry.label, &enabled))
                SetItemVisualsEnabled(entry.id, enabled);
        }

        ImGui::Spacing();
    }

    float VisualsGridColumnWidth()
    {
        const float availableWidth = std::max(520.0f, ImGui::GetContentRegionAvail().x);
        return std::floor((availableWidth - kVisualsColumnGap) * 0.5f);
    }

    template <typename LeftFn, typename RightFn>
    void RenderVisualsGridPair(float columnWidth, LeftFn&& leftFn, RightFn&& rightFn)
    {
        const ImVec2 start = ImGui::GetCursorScreenPos();
        leftFn(columnWidth);
        ImGui::SetCursorScreenPos(ImVec2(start.x + columnWidth + kVisualsColumnGap, start.y));
        rightFn(columnWidth);
        ImGui::SetCursorScreenPos(ImVec2(start.x, start.y + kVisualsRowHeight + kVisualsRowGap));
    }
}

void ui::tabs::visuals_sections::RenderCoreSection()
{
    if (!g::visualsEnabled) {
        DrawOptionRow("enable", VisualsIcon::Enable, "Enable Visuals", "Enable or disable all Visuals features", &g::visualsEnabled, nullptr, [] {}, false);
        return;
    }

    const float columnWidth = VisualsGridColumnWidth();
    RenderVisualsGridPair(
        columnWidth,
        [](float width) { DrawOptionRow("enable", VisualsIcon::Enable, "Enable Visuals", "Enable or disable all Visuals features", &g::visualsEnabled, nullptr, [] {}, false, width); },
        [](float width) { DrawOptionRow("preview", VisualsIcon::Preview, "Visuals Preview", "Show Visuals elements preview", &g::visualsPreviewOpen, nullptr, [] {}, false, width); });
    RenderVisualsGridPair(
        columnWidth,
        [](float width) { DrawOptionRow("teammates", VisualsIcon::Flags, "Show Teammates", "Render Visuals for players on your team", &g::visualsShowTeammates, nullptr, [] {}, false, width); },
        [](float) {});
}

void ui::tabs::visuals_sections::RenderGeneralSection()
{
    DrawOptionRow("box", VisualsIcon::Box, "Corner Box", "Draw corner boxes around players", &g::visualsBox, g::visualsBoxColor, [] {
        FloatRow("thick", "Thickness", &g::visualsThickness, 0.3f, 2.0f, "%.2f");
    });
    DrawOptionRow("health", VisualsIcon::Health, "Health Bar", "Show players health bar", &g::visualsHealth, g::visualsHealthColor, [] {
        ToggleSetting("value", "Show Value", &g::visualsHealthText);
    });
    DrawOptionRow("armor", VisualsIcon::Armor, "Armor Bar", "Show players armor bar", &g::visualsArmor, g::visualsArmorColor, [] {
        ToggleSetting("value", "Show Value", &g::visualsArmorText);
    });

    DrawOptionRow("vis", VisualsIcon::Visibility, "Visibility Colors", "Color code by visibility", &g::visualsVisibilityColoring, g::visualsVisibleColor, [] {
        ColorRow("occ", "Occluded", g::visualsHiddenColor);
        ToggleSetting("visonly", "Visible Only", &g::visualsVisibleOnly);
    });
}

void ui::tabs::visuals_sections::RenderOptionsGrid()
{
    const float columnWidth = VisualsGridColumnWidth();
    const auto renderPair = [columnWidth](auto&& leftFn, auto&& rightFn) {
        RenderVisualsGridPair(columnWidth, std::forward<decltype(leftFn)>(leftFn), std::forward<decltype(rightFn)>(rightFn));
    };

    renderPair(
        [] (float width) { DrawOptionRow("box", VisualsIcon::Box, "Corner Box", "", &g::visualsBox, g::visualsBoxColor, [] {
                FloatRow("thick", "Thickness", &g::visualsThickness, 0.3f, 2.0f, "%.2f");
            }, true, width); },
        [] (float width) { DrawOptionRow("skeleton", VisualsIcon::Skeleton, "Skeleton", "", &g::visualsSkeleton, g::visualsSkeletonColor, [] {
                ToggleSetting("dots", "Show Dots", &g::visualsSkeletonDots);
                ToggleSetting("head_circle", "Head Circle", &g::visualsSkeletonHeadCircle);
                FloatRow("head_circle_sz", "Head Circle Size", &g::visualsSkeletonHeadCircleSize, 0.05f, 0.30f, "%.3f");
                FloatRow("thick", "Thickness", &g::visualsThickness, 0.3f, 2.0f, "%.2f");
            }, true, width); });

    renderPair(
        [] (float width) { DrawOptionRow("health", VisualsIcon::Health, "Health Bar", "", &g::visualsHealth, g::visualsHealthColor, [] {
                ToggleSetting("value", "Show Value", &g::visualsHealthText);
            }, true, width); },
        [] (float width) { DrawOptionRow("snap", VisualsIcon::Snap, "Snap Lines", "", &g::visualsSnaplines, g::visualsSnaplineColor, [] {
                ToggleSetting("top", "Snap From Top", &g::visualsSnaplineFromTop);
            }, true, width); });

    renderPair(
        [] (float width) { DrawOptionRow("armor", VisualsIcon::Armor, "Armor Bar", "", &g::visualsArmor, g::visualsArmorColor, [] {
                ToggleSetting("value", "Show Value", &g::visualsArmorText);
            }, true, width); },
        [] (float width) { DrawOptionRow("flags", VisualsIcon::Flags, "Player Flags", "", &g::visualsFlags, nullptr, [] {
                FlagSettingsRow("name", "Name", &g::visualsName, g::visualsNameColor, &g::visualsNameFontSize);
                FlagSettingsRow("distance", "Distance", &g::visualsDistance, g::visualsDistanceColor, &g::visualsDistanceSize);
                FlagSettingsRow("blind", "Blind", &g::visualsFlagBlind, g::visualsFlagBlindColor, &g::visualsFlagBlindSize);
                FlagSettingsRow("scoped", "Scoped", &g::visualsFlagScoped, g::visualsFlagScopedColor, &g::visualsFlagScopedSize);
                FlagSettingsRow("defusing", "Defusing", &g::visualsFlagDefusing, g::visualsFlagDefusingColor, &g::visualsFlagDefusingSize);
                FlagSettingsRow("kit", "Kit", &g::visualsFlagKit, g::visualsFlagKitColor, &g::visualsFlagKitSize);
                FlagSettingsRow("money", "Money", &g::visualsFlagMoney, g::visualsFlagMoneyColor, &g::visualsFlagMoneySize);
            }, true, width); });

    renderPair(
        [] (float width) { DrawOptionRow("vis", VisualsIcon::Visibility, "Visibility Colors", "", &g::visualsVisibilityColoring, g::visualsVisibleColor, [] {
                ColorRow("occ", "Occluded", g::visualsHiddenColor);
                ToggleSetting("visonly", "Visible Only", &g::visualsVisibleOnly);
            }, true, width); },
        [] (float width) { DrawOptionRow("world", VisualsIcon::World, "World Visuals", "", &g::visualsWorld, g::visualsWorldColor, [] {
                ToggleSetting("smoke", "Smoke Timer", &g::visualsWorldSmokeTimer);
                ToggleSetting("inferno", "Molotov Timer", &g::visualsWorldInfernoTimer);
                ToggleSetting("decoy", "Decoy Timer", &g::visualsWorldDecoyTimer);
            }, true, width); });

    renderPair(
        [] (float width) { DrawOptionRow("weapon", VisualsIcon::Weapon, "Weapon Label", "", &g::visualsWeapon, nullptr, [] {
                ToggleColorRow("txt", "Label Text", &g::visualsWeaponText, g::visualsWeaponTextColor);
                SizeRow("txtsz", "Text Size", &g::visualsWeaponTextSize, 0.0f, 24.0f);
                ImGui::Separator();
                ToggleColorRow("icon", "Icon Weapon", &g::visualsWeaponIcon, g::visualsWeaponIconColor);
                ToggleSetting("knife", "No Knife", &g::visualsWeaponIconNoKnife);
                SizeRow("iconsz", "Icon Size", &g::visualsWeaponIconSize, 10.0f, 30.0f);
                ImGui::Separator();
                ToggleColorRow("ammo", "Weapon Ammo", &g::visualsWeaponAmmo, g::visualsWeaponAmmoColor);
                SizeRow("ammosz", "Ammo Size", &g::visualsWeaponAmmoSize, 0.0f, 24.0f);
            }, true, width); },
        [] (float width) { DrawOptionRow("bomb", VisualsIcon::Bomb, "Bomb Visuals", "", &g::visualsBombInfo, g::visualsBombColor, [] {
                ToggleSetting("text", "Show Text", &g::visualsBombText);
                ToggleSetting("timer", "Bomb Time", &g::visualsBombTime);
                SizeRow("bmbtxtsz", "Text Size", &g::visualsBombTextSize, 0.0f, 24.0f);
            }, true, width); });

    renderPair(
        [] (float width) { DrawOptionRow("arrows", VisualsIcon::Arrows, "Off-screen Arrows", "", &g::visualsOffscreenArrows, g::visualsOffscreenColor, [] {
                SizeRow("arrowsz", "Arrow Size", &g::visualsOffscreenSize, 8.0f, 32.0f);
            }, true, width); },
        [] (float width) { DrawOptionRow("sound", VisualsIcon::Sound, "Sound Visuals", "", &g::visualsSound, nullptr, [] {
                FloatRow("snd_range", "Range", &g::visualsSoundRange, 200.0f, 4000.0f, "%.0f");
                FloatRow("snd_dur", "Duration", &g::visualsSoundDuration, 0.4f, 4.0f, "%.2f");
                ImGui::Separator();
                ToggleSetting("snd_foot", "Footsteps", &g::visualsSoundFootsteps);
                ToggleSetting("snd_shot", "Shots Fired", &g::visualsSoundShots);
                ToggleSetting("snd_rel", "Reloads", &g::visualsSoundReloads);
            }, true, width); });
}

void ui::tabs::visuals_sections::RenderWeaponSection()
{
    DrawOptionRow("weapon", VisualsIcon::Weapon, "Weapon Label", "Show weapon name or icon", &g::visualsWeapon, nullptr, [] {
        ToggleColorRow("txt", "Label Text", &g::visualsWeaponText, g::visualsWeaponTextColor);
        SizeRow("txtsz", "Text Size", &g::visualsWeaponTextSize, 0.0f, 24.0f);
        ImGui::Separator();
        ToggleColorRow("icon", "Icon Weapon", &g::visualsWeaponIcon, g::visualsWeaponIconColor);
        ToggleSetting("knife", "No Knife", &g::visualsWeaponIconNoKnife);
        SizeRow("iconsz", "Icon Size", &g::visualsWeaponIconSize, 10.0f, 30.0f);
        ImGui::Separator();
        ToggleColorRow("ammo", "Weapon Ammo", &g::visualsWeaponAmmo, g::visualsWeaponAmmoColor);
        SizeRow("ammosz", "Ammo Size", &g::visualsWeaponAmmoSize, 0.0f, 24.0f);
    });
}

void ui::tabs::visuals_sections::RenderPlayerVisualsSection()
{
    DrawOptionRow("skeleton", VisualsIcon::Skeleton, "Skeleton", "Draw player skeleton", &g::visualsSkeleton, g::visualsSkeletonColor, [] {
        ToggleSetting("dots", "Show Dots", &g::visualsSkeletonDots);
        ToggleSetting("head_circle", "Head Circle", &g::visualsSkeletonHeadCircle);
        FloatRow("thick", "Thickness", &g::visualsThickness, 0.3f, 2.0f, "%.2f");
    });

    DrawOptionRow("snap", VisualsIcon::Snap, "Snap Lines", "Draw lines to players", &g::visualsSnaplines, g::visualsSnaplineColor, [] {
        ToggleSetting("top", "Snap From Top", &g::visualsSnaplineFromTop);
        ImGui::Separator();
        ToggleColorRow("arrows", "Screen Arrows", &g::visualsOffscreenArrows, g::visualsOffscreenColor);
        ToggleSetting("sound", "Sound Visuals", &g::visualsSound);
    });

    if (!g::visualsOffscreenArrows)
        g::visualsSound = false;
}

void ui::tabs::visuals_sections::RenderFlagsSection()
{
    DrawOptionRow("flags", VisualsIcon::Flags, "Player Flags", "Show player status flags", &g::visualsFlags, nullptr, [] {
        FlagSettingsRow("name", "Name", &g::visualsName, g::visualsNameColor, &g::visualsNameFontSize);
        FlagSettingsRow("distance", "Distance", &g::visualsDistance, g::visualsDistanceColor, &g::visualsDistanceSize);
        FlagSettingsRow("blind", "Blind", &g::visualsFlagBlind, g::visualsFlagBlindColor, &g::visualsFlagBlindSize);
        FlagSettingsRow("scoped", "Scoped", &g::visualsFlagScoped, g::visualsFlagScopedColor, &g::visualsFlagScopedSize);
        FlagSettingsRow("defusing", "Defusing", &g::visualsFlagDefusing, g::visualsFlagDefusingColor, &g::visualsFlagDefusingSize);
        FlagSettingsRow("kit", "Kit", &g::visualsFlagKit, g::visualsFlagKitColor, &g::visualsFlagKitSize);
        FlagSettingsRow("money", "Money", &g::visualsFlagMoney, g::visualsFlagMoneyColor, &g::visualsFlagMoneySize);
    });
}

void ui::tabs::visuals_sections::RenderItemSection()
{
    DrawOptionRow("item", VisualsIcon::Item, "Item Visuals", "Show selected dropped items", &g::visualsItem, g::visualsWorldColor, [] {
        if (ImGui::Button("Enable All"))
            SetAllKnownItemsEnabled(true);
        ImGui::SameLine();
        if (ImGui::Button("Disable All"))
            SetAllKnownItemsEnabled(false);

        ImGui::Separator();

        ImGui::BeginChild("##itemfilter", ImVec2(0.0f, 380.0f), ImGuiChildFlags_Borders);
        if (ImGui::BeginTable("##itemgrid", 3, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_PadOuterX)) {
            ImGui::TableNextColumn();
            RenderItemGroupBlock(ItemVisualsGroup::Pistols);
            ImGui::Spacing();
            RenderItemGroupBlock(ItemVisualsGroup::SMGs);

            ImGui::TableNextColumn();
            RenderItemGroupBlock(ItemVisualsGroup::Rifles);
            ImGui::Spacing();
            RenderItemGroupBlock(ItemVisualsGroup::Snipers);

            ImGui::TableNextColumn();
            RenderItemGroupBlock(ItemVisualsGroup::Heavy);
            ImGui::Spacing();
            RenderItemGroupBlock(ItemVisualsGroup::Gear);

            ImGui::EndTable();
        }
        ImGui::EndChild();
    });
}

void ui::tabs::visuals_sections::RenderWorldSection()
{
    DrawOptionRow("world", VisualsIcon::World, "World Visuals", "Show world utility timers", &g::visualsWorld, g::visualsWorldColor, [] {
        ToggleSetting("smoke", "Smoke Timer", &g::visualsWorldSmokeTimer);
        ToggleSetting("inferno", "Molotov Timer", &g::visualsWorldInfernoTimer);
        ToggleSetting("decoy", "Decoy Timer", &g::visualsWorldDecoyTimer);
    });

    DrawOptionRow("bomb", VisualsIcon::Bomb, "Bomb Visuals", "Show planted and dropped C4 info", &g::visualsBombInfo, g::visualsBombColor, [] {
        ToggleSetting("text", "Show Text", &g::visualsBombText);
        ToggleSetting("timer", "Bomb Time", &g::visualsBombTime);
        SizeRow("bmbtxtsz", "Text Size", &g::visualsBombTextSize, 0.0f, 24.0f);
    });
}
