if (g::visualsSound) {
    const float durationSec = std::clamp(g::visualsSoundDuration, 0.25f, 5.0f);
    const float rangeWorld = std::clamp(g::visualsSoundRange, 100.0f, 6000.0f);
    const uint64_t durationUs = static_cast<uint64_t>(durationSec * 1000000.0f);
    const uint64_t minCreatedUs = (nowUs > durationUs) ? (nowUs - durationUs) : 0;

    SoundEvent eventBuffer[kSoundEventRingSize] = {};
    const int eventCount = CopySoundEvents(eventBuffer, kSoundEventRingSize, minCreatedUs);

    for (int idx = 0; idx < eventCount; ++idx) {
        const SoundEvent& evt = eventBuffer[idx];
        if (evt.type == SoundEventType::Footstep && !g::visualsSoundFootsteps) continue;
        if (evt.type == SoundEventType::Shot && !g::visualsSoundShots) continue;
        if (evt.type == SoundEventType::Reload && !g::visualsSoundReloads) continue;

        const float dx = evt.position.x - localPos.x;
        const float dy = evt.position.y - localPos.y;
        const float dz = evt.position.z - localPos.z;
        const float distSq = dx * dx + dy * dy + dz * dz;
        if (distSq > rangeWorld * rangeWorld) continue;

        const uint64_t ageUs = (nowUs > evt.createdUs) ? (nowUs - evt.createdUs) : 0;
        const float t = std::clamp(static_cast<float>(ageUs) / static_cast<float>(durationUs), 0.0f, 1.0f);

        float ringStartWorld = 24.0f;
        float ringEndWorld = 140.0f;
        int rgbR = 90, rgbG = 200, rgbB = 255;
        switch (evt.type) {
            case SoundEventType::Footstep:
                ringStartWorld = 22.0f;
                ringEndWorld = 130.0f;
                rgbR = 90; rgbG = 220; rgbB = 255;
                break;
            case SoundEventType::Shot:
                ringStartWorld = 32.0f;
                ringEndWorld = 200.0f;
                rgbR = 255; rgbG = 150; rgbB = 70;
                break;
            case SoundEventType::Reload:
                ringStartWorld = 24.0f;
                ringEndWorld = 140.0f;
                rgbR = 255; rgbG = 220; rgbB = 90;
                break;
        }

        const float radiusWorld = ringStartWorld + (ringEndWorld - ringStartWorld) * t;
        const float alphaScale = std::clamp(1.0f - t, 0.0f, 1.0f);
        const int alpha = static_cast<int>(245.0f * alphaScale);
        if (alpha <= 4) continue;

        const ScreenPos centerSp = WorldToScreen(evt.position, viewMatrix, screenW, screenH);
        if (centerSp.onScreen) {
            const ImU32 dotColor = IM_COL32(rgbR, rgbG, rgbB, alpha);
            const ImU32 dotShadow = IM_COL32(0, 0, 0, alpha / 2);
            drawList->AddCircleFilled(ImVec2(centerSp.x, centerSp.y), 3.6f, dotShadow, 16);
            drawList->AddCircleFilled(ImVec2(centerSp.x, centerSp.y), 2.4f, dotColor, 16);
        }

        constexpr int kRingSegments = 32;
        ImVec2 ringPoints[kRingSegments];
        bool ringPointOnScreen[kRingSegments] = {};
        bool ringHasOnScreen = false;
        for (int s = 0; s < kRingSegments; ++s) {
            const float ang = (static_cast<float>(s) / static_cast<float>(kRingSegments)) * (2.0f * std::numbers::pi_v<float>);
            const Vector3 worldPt = {
                evt.position.x + std::cos(ang) * radiusWorld,
                evt.position.y + std::sin(ang) * radiusWorld,
                evt.position.z,
            };
            const ScreenPos sp = WorldToScreen(worldPt, viewMatrix, screenW, screenH);
            ringPoints[s] = sp.onScreen ? ImVec2(sp.x, sp.y) : ImVec2(0.0f, 0.0f);
            ringPointOnScreen[s] = sp.onScreen;
            if (sp.onScreen) ringHasOnScreen = true;
        }
        if (!ringHasOnScreen) continue;

        const ImU32 ringColor = IM_COL32(rgbR, rgbG, rgbB, alpha);
        const ImU32 ringShadow = IM_COL32(0, 0, 0, alpha / 3);
        for (int s = 0; s < kRingSegments; ++s) {
            const int sNext = (s + 1) % kRingSegments;
            if (!ringPointOnScreen[s] || !ringPointOnScreen[sNext])
                continue;
            drawList->AddLine(ringPoints[s], ringPoints[sNext], ringShadow, 3.4f);
            drawList->AddLine(ringPoints[s], ringPoints[sNext], ringColor, 2.2f);
        }
    }
}
