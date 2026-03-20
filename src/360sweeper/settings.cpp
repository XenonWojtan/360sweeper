#include "StdAfx.h"
#include "globals.h"
#include "settings.h"

// -------------------------------------------------------
// BoolToString
// -------------------------------------------------------
std::string BoolToString(bool value)
{
    return value ? "true" : "false";
}

// -------------------------------------------------------
// CreateSettings
// -------------------------------------------------------
void CreateSettings()
{
    volumepercentage           = 100;
    soundVol                   = 1.0f;
    g_IsMuted                  = false;
    chordingEnabled            = true;
    g_VibrationsEnabled        = true;
    g_VibrationStrengthPercent = 75;
    vibrationStrength          = (65535 * 75) / 100;
    movecooldownms             = 200;
    holdTime                   = 225;
    GAME_ROOT_PATH             = "Content";

    std::ofstream outFile("game:\\settingsmeow.txt", std::ios::trunc);
    if (outFile.is_open()) {
        outFile << "volume=100\n";
        outFile << "muted=false\n";
        outFile << "chording enabled=true\n";
        outFile << "vibrations=true\n";
        outFile << "vibrations strength=75\n";
        outFile << "move cooldown=200\n";
        outFile << "hold time to preview=225\n";
        outFile << "texture pack=Content\n";
        outFile.flush();
        outFile.close();
    }
}

// -------------------------------------------------------
// SaveSettings
// -------------------------------------------------------
void SaveSettings()
{
    std::ofstream outFile("game:\\settingsmeow.txt", std::ios::trunc);
    if (outFile.is_open()) {
        outFile << "volume="                << ToString(volumepercentage)           << std::endl;
        outFile << "muted="                 << BoolToString(g_IsMuted)              << std::endl;
        outFile << "chording enabled="      << BoolToString(chordingEnabled)        << std::endl;
        outFile << "vibrations="            << BoolToString(g_VibrationsEnabled)    << std::endl;
        outFile << "vibrations strength="   << ToString(g_VibrationStrengthPercent) << std::endl;
        outFile << "move cooldown="         << ToString(movecooldownms)             << std::endl;
        outFile << "hold time to preview="  << ToString(holdTime)                   << std::endl;
        outFile << "texture pack="          << GAME_ROOT_PATH                       << std::endl;
        outFile.close();
    }
}

// -------------------------------------------------------
// LoadSettings
// -------------------------------------------------------
void LoadSettings()
{
    std::ifstream inFile("game:\\settingsmeow.txt");
    std::string line;

    if (inFile.is_open()) {
        while (std::getline(inFile, line)) {
            if (!line.empty() && line[line.size() - 1] == '\r')
                line.erase(line.size() - 1);

            size_t eqPos = line.find('=');
            if (eqPos == std::string::npos) continue;

            std::string key   = line.substr(0, eqPos);
            std::string value = line.substr(eqPos + 1);

            if (key == "volume") {
                volumepercentage = atoi(value.c_str());
                if (volumepercentage > 200) volumepercentage = 200;
                if (volumepercentage < 0)   volumepercentage = 0;
                soundVol = (volumepercentage / 100.0f);
            }
            else if (key == "muted") {
                g_IsMuted = (value == "true");
                OutputDebugString("Setting Applied: Mute toggled.\n");
            }
            else if (key == "chording enabled") {
                chordingEnabled = (value == "true");
                OutputDebugString("Setting Applied: Chording toggled.\n");
            }
            else if (key == "vibrations") {
                g_VibrationsEnabled = (value == "true");
                OutputDebugString("Setting Applied: Vibrations toggled.\n");
            }
            else if (key == "vibrations strength") {
                g_VibrationStrengthPercent = atoi(value.c_str());
                if (g_VibrationStrengthPercent > 100) g_VibrationStrengthPercent = 100;
                vibrationStrength = 65535 * g_VibrationStrengthPercent / 100;
                OutputDebugString("Setting Applied: Vibration strength set.\n");
            }
            else if (key == "move cooldown") {
                int val = atoi(value.c_str());
                if (val < 50)  val = 50;
                if (val > 250) val = 250;
                movecooldownms = val;
                OutputDebugString("Setting Applied: Move cooldown set.\n");
            }
            else if (key == "hold time to preview") {
                int val = atoi(value.c_str());
                if (val < 150) val = 150;
                if (val > 300) val = 300;
                holdTime = val;
                OutputDebugString("Setting Applied: Hold time set.\n");
            }
            else if (key == "texture pack") {
                GAME_ROOT_PATH = value.c_str();
                OutputDebugString("Setting Applied: Textures set.\n");
            }
        }
        inFile.close();
        OutputDebugString("SUCCESS: Settings loaded from game.\n");
    }
    else {
        OutputDebugString("WARNING: settingsmeow.txt not found. Using defaults.\n");
    }
}

// -------------------------------------------------------
// InitializeSettings
// -------------------------------------------------------
void InitializeSettings()
{
    const char* filePath = "game:\\settingsmeow.txt";
    std::ifstream fileCheck(filePath);

    if (fileCheck.is_open()) {
        fileCheck.close();
        OutputDebugString("File found! Proceeding to load...\n");
        LoadSettings();
    }
    else {
        OutputDebugString("Settings file NOT found! Creating defaults...\n");
        CreateSettings();
    }
}

// -------------------------------------------------------
// GetTexturePacks
// -------------------------------------------------------
std::vector<std::string> GetTexturePacks()
{
    std::vector<std::string> packs;
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("game:\\*", &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                std::string folderName = findFileData.cFileName;
                if (folderName != "." && folderName != "..") {
                    packs.push_back(folderName);
                }
            }
        } while (FindNextFile(hFind, &findFileData));
        FindClose(hFind);
    }
    return packs;
}

// -------------------------------------------------------
// RefreshPackList
// -------------------------------------------------------
void RefreshPackList()
{
    packList = GetTexturePacks();
    selectedPackIndex = 0;

    for (int i = 0; i < (int)packList.size(); i++) {
        if (packList[i] == GAME_ROOT_PATH) {
            selectedPackIndex = i;
            break;
        }
    }
}