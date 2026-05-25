{
    auto soundIsEnemyAlive = [&](int idx) -> bool {
        if (idx < 0 || idx >= 64) return false;
        if (!pawns[idx]) return false;
        if (pawns[idx] == localPawn) return false;
        if (lifeStates[idx] != 0) return false;
        if (teams[idx] != 2 && teams[idx] != 3) return false;
        if (s_localTeam != 0 && teams[idx] == s_localTeam) return false;
        return true;
    };

    if (g::visualsSound) {
        const bool hasShotsOffset = ofs.C_CSPlayerPawn_m_iShotsFired > 0;
        const bool hasReloadOffset = ofs.C_CSWeaponBase_m_bInReload > 0;

        int shotsFiredPerPlayer[64] = {};
        uint8_t inReloadPerPlayer[64] = {};
        bool shotsQueued[64] = {};
        bool reloadQueued[64] = {};
        bool queuedSound = false;

        if (hasShotsOffset || hasReloadOffset) {
            for (int i = 0; i < 64; ++i) {
                if (!soundIsEnemyAlive(i)) {
                    s_soundPrevStateValid[i] = false;
                    continue;
                }
                if (hasShotsOffset) {
                    mem.AddScatterReadRequest(
                        handle,
                        pawns[i] + ofs.C_CSPlayerPawn_m_iShotsFired,
                        &shotsFiredPerPlayer[i],
                        sizeof(int));
                    shotsQueued[i] = true;
                    queuedSound = true;
                }
                if (hasReloadOffset && activeWeapons[i]) {
                    mem.AddScatterReadRequest(
                        handle,
                        activeWeapons[i] + ofs.C_CSWeaponBase_m_bInReload,
                        &inReloadPerPlayer[i],
                        sizeof(uint8_t));
                    reloadQueued[i] = true;
                    queuedSound = true;
                }
            }
        }

        bool soundScatterOk = true;
        if (queuedSound)
            soundScatterOk = mem.ExecuteReadScatter(handle);

        const uint64_t soundNowUs = TickNowUs();
        for (int i = 0; i < 64; ++i) {
            if (!soundIsEnemyAlive(i)) {
                s_soundPrevStateValid[i] = false;
                continue;
            }

            const Vector3 emitPos = positions[i];
            const int curShots = (soundScatterOk && shotsQueued[i]) ? shotsFiredPerPlayer[i] : s_prevShotsFired[i];
            const uint8_t curInReload = (soundScatterOk && reloadQueued[i]) ? inReloadPerPlayer[i] : s_prevInReload[i];

            if (s_soundPrevStateValid[i] && soundScatterOk) {
                if (g::visualsSoundShots && shotsQueued[i] && curShots > s_prevShotsFired[i])
                    PushSoundEvent(SoundEventType::Shot, emitPos, soundNowUs);
                if (g::visualsSoundReloads && reloadQueued[i] && curInReload && !s_prevInReload[i])
                    PushSoundEvent(SoundEventType::Reload, emitPos, soundNowUs);
            }
            if (soundScatterOk) {
                if (shotsQueued[i]) s_prevShotsFired[i] = curShots;
                if (reloadQueued[i]) s_prevInReload[i] = curInReload;
            }
            s_soundPrevStateValid[i] = true;

            if (g::visualsSoundFootsteps) {
                const float vx = velocities[i].x;
                const float vy = velocities[i].y;
                const float speed2D = std::sqrt(vx * vx + vy * vy);
                if (speed2D >= visuals::intervals::kSoundFootstepSpeedThreshold &&
                    (soundNowUs - s_lastFootstepEmitUs[i]) >= visuals::intervals::kSoundFootstepEmitIntervalUs) {
                    PushSoundEvent(SoundEventType::Footstep, emitPos, soundNowUs);
                    s_lastFootstepEmitUs[i] = soundNowUs;
                }
            }
        }
    } else {
        for (int i = 0; i < 64; ++i)
            s_soundPrevStateValid[i] = false;
    }
}
