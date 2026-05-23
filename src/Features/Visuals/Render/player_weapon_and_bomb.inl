
if (g::visualsWeapon) {
    bottomTextY += 2.0f;
    
    
    
    const uint16_t weaponIconId = p.weaponIconId;
    const char* weaponIcon = WeaponIconFromItemId(weaponIconId);
    const bool hasWeaponVisualAsset = WeaponVisualKeyFromItemId(weaponIconId) != nullptr;
    const char* weaponIconFallback = WeaponIconFallbackTokenFromItemId(weaponIconId);
    const char* weaponName = WeaponNameFromItemId(p.weaponId);
    if (g::visualsWeaponIcon && weaponName) {
        if (weaponIcon && g::fontWeaponIcons) {
            drawBottomLabel(weaponIcon, ColorToImU32(g::visualsWeaponIconColor), false, true, g::fontWeaponIcons, g::visualsWeaponIconSize);
        } else if (hasWeaponVisualAsset && weaponIconFallback) {
            drawBottomLabel(weaponIconFallback, ColorToImU32(g::visualsWeaponIconColor), false, true, g::fontSegoeBold, g::visualsWeaponIconSize - 1.0f);
        }
    }
    if (g::visualsWeaponText)
        drawBottomLabel(weaponName, ColorToImU32(g::visualsWeaponTextColor), false, true, g::fontComicSans, g::visualsWeaponTextSize);
    if (g::visualsBombInfo && g::visualsBombText && p.hasBomb && !bombState.dropped && !bombState.planted)
        drawBottomLabel("Bomb", bombCol, false, true, nullptr, g::visualsBombTextSize);
}
