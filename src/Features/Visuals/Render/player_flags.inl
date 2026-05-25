if (g::visualsFlags || g::visualsDistance) {
    const float flagStartX = boxLeft + boxWidth + 6.0f;
    float flagY = boxTop;
    char moneyBuf[16] = {};
    char distBuf[16] = {};

    struct FlagEntry { const char* text; ImU32 color; float size; };
    FlagEntry flagList[8] = {};
    int flagCount = 0;

    if (g::visualsFlags) {
        if (p.flashed && g::visualsFlagBlind)
            flagList[flagCount++] = { "Blind", ColorToImU32(g::visualsFlagBlindColor), g::visualsFlagBlindSize };
        if (p.scoped && g::visualsFlagScoped)
            flagList[flagCount++] = { "Scoped", ColorToImU32(g::visualsFlagScopedColor), g::visualsFlagScopedSize };
        if (p.defusing && g::visualsFlagDefusing)
            flagList[flagCount++] = { "Defusing", ColorToImU32(g::visualsFlagDefusingColor), g::visualsFlagDefusingSize };
        if (p.hasDefuser && g::visualsFlagKit)
            flagList[flagCount++] = { "Kit", ColorToImU32(g::visualsFlagKitColor), g::visualsFlagKitSize };
        if (p.money > 0 && g::visualsFlagMoney) {
            snprintf(moneyBuf, sizeof(moneyBuf), "$%d", p.money);
            flagList[flagCount++] = { moneyBuf, ColorToImU32(g::visualsFlagMoneyColor), g::visualsFlagMoneySize };
        }
    }

    
    if (g::visualsDistance) {
        const float dx = renderPlayerPos.x - renderLocalPos.x;
        const float dy = renderPlayerPos.y - renderLocalPos.y;
        const float dz = renderPlayerPos.z - renderLocalPos.z;
        const float distanceUnits = static_cast<float>(std::hypot(std::hypot(dx, dy), dz));
        const float distanceMeters = distanceUnits / 39.37f;
        if (std::isfinite(distanceMeters) && distanceMeters < 10000.0f) {
            snprintf(distBuf, sizeof(distBuf), "%.0fm", distanceMeters);
            flagList[flagCount++] = { distBuf, distanceCol, g::visualsDistanceSize };
        }
    }

    for (int f = 0; f < flagCount; ++f) {
        if (!flagList[f].text || flagList[f].text[0] == '\0')
            continue;
        float fontSize = flagList[f].size > 0.0f ? flagList[f].size : 0.0f;
        DrawTextShadow(drawList, g::fontComicSans, fontSize, ImVec2(flagStartX, flagY), flagList[f].color, flagList[f].text);
        float lineH = fontSize > 0.0f ? fontSize : ImGui::GetFontSize();
        flagY += lineH + 1.0f;
    }
}
