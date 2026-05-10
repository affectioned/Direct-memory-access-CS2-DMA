namespace radar {

void ResetActiveMapCalibration()
{
    g::radarWorldRotationDeg = 0.0f;
    g::radarWorldScale = 1.0f;
    g::radarWorldOffsetX = 0.0f;
    g::radarWorldOffsetY = 0.0f;

    std::string mapKey;
    {
        std::lock_guard<std::mutex> lock(s_activeMapMutex);
        mapKey = s_activeMapKey;
        s_activeMapBaseOffsetX = 0.0f;
        s_activeMapBaseOffsetY = 0.0f;
        s_lastSavedMapRotation = 0.0f;
        s_lastSavedMapScale = 1.0f;
        s_lastSavedMapOffsetX = 0.0f;
        s_lastSavedMapOffsetY = 0.0f;
        s_lastMapPersistTime = std::chrono::steady_clock::now();
    }

    if (!mapKey.empty())
        SaveRadarCalibrationForMap(mapKey, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

}
