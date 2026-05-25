#include "Features/Visuals/UI/visuals_tab.h"

#include "Features/Visuals/UI/visuals_sections.h"
#include "app/Core/globals.h"
#include "app/UI/MenuShell/menu_state.h"
#include "app/UI/MenuShell/tab_page.h"

#include <imgui.h>

namespace ui {
void RenderVisualsPreview();
}

const char* ui::tabs::VisualsTab::Label() const
{
    return "Visuals";
}

void ui::tabs::VisualsTab::Render(MenuState& state, IStatusSink& statusSink)
{
    (void)state;
    (void)statusSink;

    ImGui::BeginChild("##visualschild", ImVec2(0, 0), ImGuiChildFlags_Borders);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(8.0f, 0.0f));

    visuals_sections::RenderCoreSection();

    if (g::visualsEnabled)
        visuals_sections::RenderOptionsGrid();

    ImGui::PopStyleVar(2);
    ImGui::EndChild();

    ui::RenderVisualsPreview();
}
