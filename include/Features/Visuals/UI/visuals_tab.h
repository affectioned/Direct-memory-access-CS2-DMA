#pragma once

#include "app/UI/MenuShell/tab_page.h"

namespace ui::tabs {
class VisualsTab final : public ui::ITabPage {
public:
    const char* Label() const override;
    void Render(MenuState& state, IStatusSink& statusSink) override;
};
}
