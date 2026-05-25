
if (g::visualsName && p.name[0] != '\0') {
    ImFont* nameFont = GetVisualsNameFont();
    const float nameFontSize = (g::visualsNameFontSize > 4.0f) ? g::visualsNameFontSize : ImGui::GetFontSize();
    ImVec2 textSize = (nameFont ? nameFont->CalcTextSizeA(nameFontSize, FLT_MAX, 0.0f, p.name, nullptr) : ImGui::CalcTextSize(p.name));
    float textX = screenHead.x - textSize.x * 0.5f;
    float textY = boxTop - textSize.y - 4.0f;

    drawList->AddText(nameFont, nameFontSize, ImVec2(textX + 1.0f, textY + 1.0f), IM_COL32(0, 0, 0, 210), p.name);
    drawList->AddText(nameFont, nameFontSize, ImVec2(textX, textY), nameCol, p.name);
}
