if (g::visualsOffscreenArrows) {
    const float px = viewMatrix[0][0]*renderPlayerPos.x + viewMatrix[0][1]*renderPlayerPos.y + viewMatrix[0][2]*renderPlayerPos.z + viewMatrix[0][3];
    const float py = viewMatrix[1][0]*renderPlayerPos.x + viewMatrix[1][1]*renderPlayerPos.y + viewMatrix[1][2]*renderPlayerPos.z + viewMatrix[1][3];
    float dirX = px;
    float dirY = -py;
    const float len = static_cast<float>(std::hypot(dirX, dirY));
    if (len > 0.001f) {
        dirX /= len;
        dirY /= len;

        constexpr float arrowSize = 14.0f;
        const float radius = (std::min(screenW, screenH) * 0.47f) - arrowSize * 2.5f;
        const ImVec2 center(screenW * 0.5f, screenH * 0.5f);
        const ImVec2 base(center.x + dirX * radius, center.y + dirY * radius);
        const float perpX = -dirY;
        const float perpY = dirX;
        const ImVec2 tip(base.x + dirX * arrowSize, base.y + dirY * arrowSize);
        const ImVec2 left(
            base.x - dirX * arrowSize * 0.85f + perpX * arrowSize * 0.70f,
            base.y - dirY * arrowSize * 0.85f + perpY * arrowSize * 0.70f);
        const ImVec2 right(
            base.x - dirX * arrowSize * 0.85f - perpX * arrowSize * 0.70f,
            base.y - dirY * arrowSize * 0.85f - perpY * arrowSize * 0.70f);
        ImU32 renderArrowCol = offscreenCol;
        if (g::visualsVisibilityColoring)
            renderArrowCol = effectiveVisible ? visibleCol : hiddenCol;
        drawList->AddTriangleFilled(tip, left, right, renderArrowCol);
        drawList->AddTriangle(tip, left, right, IM_COL32(0, 0, 0, 220), 2.0f);
    }
}
