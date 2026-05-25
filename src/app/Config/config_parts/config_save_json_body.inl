        json root = json::object();

        json& visuals = EnsureSection(root, "Visuals");
        visuals["Enabled"] = g::visualsEnabled;
        visuals["Box"] = g::visualsBox;
        visuals["Health"] = g::visualsHealth;
        visuals["HealthText"] = g::visualsHealthText;
        visuals["Armor"] = g::visualsArmor;
        visuals["ArmorText"] = g::visualsArmorText;
        visuals["Name"] = g::visualsName;
        visuals["NameFontSize"] = g::visualsNameFontSize;
        visuals["Weapon"] = g::visualsWeapon;
        visuals["WeaponText"] = g::visualsWeaponText;
        visuals["WeaponTextSize"] = g::visualsWeaponTextSize;
        SaveColor(visuals, "WeaponTextColor", g::visualsWeaponTextColor);
        visuals["WeaponIcon"] = g::visualsWeaponIcon;
        visuals["WeaponIconNoKnife"] = g::visualsWeaponIconNoKnife;
        visuals["WeaponIconSize"] = g::visualsWeaponIconSize;
        SaveColor(visuals, "WeaponIconColor", g::visualsWeaponIconColor);
        visuals["WeaponAmmo"] = g::visualsWeaponAmmo;
        visuals["WeaponAmmoSize"] = g::visualsWeaponAmmoSize;
        SaveColor(visuals, "WeaponAmmoColor", g::visualsWeaponAmmoColor);
        visuals["Distance"] = g::visualsDistance;
        visuals["DistanceSize"] = g::visualsDistanceSize;
        visuals["Skeleton"] = g::visualsSkeleton;
        visuals["SkeletonDots"] = g::visualsSkeletonDots;
        visuals["Snaplines"] = g::visualsSnaplines;
        visuals["SnapFromTop"] = g::visualsSnaplineFromTop;
        visuals["VisibilityColoring"] = g::visualsVisibilityColoring;
        visuals["VisibleOnly"] = g::visualsVisibleOnly;
        visuals["ShowTeammates"] = g::visualsShowTeammates;
        visuals["OffscreenArrows"] = g::visualsOffscreenArrows;
        visuals["Sound"] = g::visualsSound;
        visuals["SoundFootsteps"] = g::visualsSoundFootsteps;
        visuals["SoundShots"] = g::visualsSoundShots;
        visuals["SoundReloads"] = g::visualsSoundReloads;
        visuals["SoundRange"] = g::visualsSoundRange;
        visuals["SoundDuration"] = g::visualsSoundDuration;
        visuals["Flags"] = g::visualsFlags;
        visuals["Item"] = g::visualsItem;
        visuals["FlagBlind"] = g::visualsFlagBlind;
        SaveColor(visuals, "FlagBlindColor", g::visualsFlagBlindColor);
        visuals["FlagBlindSize"] = g::visualsFlagBlindSize;
        visuals["FlagScoped"] = g::visualsFlagScoped;
        SaveColor(visuals, "FlagScopedColor", g::visualsFlagScopedColor);
        visuals["FlagScopedSize"] = g::visualsFlagScopedSize;
        visuals["FlagDefusing"] = g::visualsFlagDefusing;
        SaveColor(visuals, "FlagDefusingColor", g::visualsFlagDefusingColor);
        visuals["FlagDefusingSize"] = g::visualsFlagDefusingSize;
        visuals["FlagKit"] = g::visualsFlagKit;
        SaveColor(visuals, "FlagKitColor", g::visualsFlagKitColor);
        visuals["FlagKitSize"] = g::visualsFlagKitSize;
        visuals["FlagMoney"] = g::visualsFlagMoney;
        SaveColor(visuals, "FlagMoneyColor", g::visualsFlagMoneyColor);
        visuals["FlagMoneySize"] = g::visualsFlagMoneySize;
        visuals["World"] = g::visualsWorld;
        visuals["WorldProjectiles"] = g::visualsWorldProjectiles;
        visuals["WorldSmokeTimer"] = g::visualsWorldSmokeTimer;
        visuals["WorldInfernoTimer"] = g::visualsWorldInfernoTimer;
        visuals["WorldDecoyTimer"] = g::visualsWorldDecoyTimer;
        visuals["WorldExplosiveTimer"] = g::visualsWorldExplosiveTimer;
        visuals["BombInfo"] = g::visualsBombInfo;
        visuals["BombText"] = g::visualsBombText;
        visuals["BombTime"] = g::visualsBombTime;
        visuals["BombTextSize"] = g::visualsBombTextSize;
        visuals["BombTimerX"] = g::visualsBombTimerX;
        visuals["BombTimerY"] = g::visualsBombTimerY;
        visuals["OffscreenSize"] = g::visualsOffscreenSize;
        SaveColor(visuals, "BoxColor", g::visualsBoxColor);
        SaveColor(visuals, "HealthColor", g::visualsHealthColor);
        SaveColor(visuals, "VisibleColor", g::visualsVisibleColor);
        SaveColor(visuals, "HiddenColor", g::visualsHiddenColor);
        SaveColor(visuals, "ArmorColor", g::visualsArmorColor);
        SaveColor(visuals, "NameColor", g::visualsNameColor);
        SaveColor(visuals, "DistanceColor", g::visualsDistanceColor);
        SaveColor(visuals, "SkeletonColor", g::visualsSkeletonColor);
        SaveColor(visuals, "SnaplineColor", g::visualsSnaplineColor);
        SaveColor(visuals, "OffscreenColor", g::visualsOffscreenColor);
        SaveColor(visuals, "FlagColor", g::visualsFlagColor);
        SaveColor(visuals, "WorldColor", g::visualsWorldColor);
        SaveColor(visuals, "BombColor", g::visualsBombColor);
        SaveColor(visuals, "SoundColor", g::visualsSoundColor);
        visuals["Thickness"] = g::visualsThickness;
        {
            json hiddenIds = json::array();
            for (size_t itemId = 1; itemId < 1200; ++itemId) {
                if (!g::visualsItemEnabledMask.test(itemId))
                    hiddenIds.push_back(static_cast<int>(itemId));
            }
            visuals["ItemHiddenIds"] = std::move(hiddenIds);
        }

        json& radar = EnsureSection(root, "Radar");
        radar["Enabled"] = g::radarEnabled;
        radar["Mode"] = g::radarMode;
        radar["ShowLocalDot"] = g::radarShowLocalDot;
        radar["ShowAngles"] = g::radarShowAngles;
        radar["ShowCrosshair"] = g::radarShowCrosshair;
        radar["ShowBomb"] = g::radarShowBomb;
        radar["Size"] = g::radarSize;
        radar["DotSize"] = g::radarDotSize;
        radar["WorldRotationDeg"] = g::radarWorldRotationDeg;
        radar["WorldScale"] = g::radarWorldScale;
        radar["WorldOffsetX"] = g::radarWorldOffsetX;
        radar["WorldOffsetY"] = g::radarWorldOffsetY;
        radar["StaticFlipX"] = g::radarStaticFlipX;
        SaveColor(radar, "BgColor", g::radarBgColor);
        SaveColor(radar, "BorderColor", g::radarBorderColor);
        SaveColor(radar, "DotColor", g::radarDotColor);
        SaveColor(radar, "BombColor", g::radarBombColor);
        SaveColor(radar, "AngleColor", g::radarAngleColor);
        radar["SpectatorList"] = g::radarSpectatorList;
        radar["SpectatorListX"] = g::radarSpectatorListX;
        radar["SpectatorListY"] = g::radarSpectatorListY;

        json& webRadar = EnsureSection(root, "WEBRadar");
        webRadar["Enabled"] = g::webRadarEnabled;
        webRadar["Port"] = g::webRadarPort;
        webRadar["MapOverride"] = g::webRadarMapOverride;

        json& screen = EnsureSection(root, "Screen");
        screen["VSync"] = g::vsyncEnabled;
        screen["FPSLimit"] = g::fpsLimit;

        json& ui = EnsureSection(root, "UI");
        ui["VisualsPreviewOpen"] = g::visualsPreviewOpen;
        ui["RadarCalibrationOpen"] = g::radarCalibrationOpen;
        ui["WebRadarQrOpen"] = g::webRadarQrOpen;
        ui["WebRadarDebugOpen"] = g::webRadarDebugOpen;
        ui["MenuToggleKey"] = g::menuToggleKey;

        std::filesystem::path path(jsonPath);
        std::error_code ec;
        std::filesystem::create_directories(path.parent_path(), ec);

        std::ofstream file(path, std::ios::binary | std::ios::trunc);
        if (!file.is_open())
            return false;

        file << root.dump(4);
        return file.good();
