#include "StdAfx.h"
#include "globals.h"
#include "controls.h"

// -------------------------------------------------------
// Sanitize
// -------------------------------------------------------
std::string Sanitize(std::string str)
{
    size_t found = str.find_last_not_of(" \r\n\t");
    if (found != std::string::npos) str.erase(found + 1);

    size_t first = str.find_first_not_of(" \r\n\t");
    if (std::string::npos == first) return "";
    str = str.substr(first);

    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] >= 'a' && str[i] <= 'z') str[i] -= 32;
    }
    return str;
}

// -------------------------------------------------------
// StringToBool
// -------------------------------------------------------
bool StringToBool(std::string value)
{
    value = Sanitize(value);
    return (value == "TRUE");
}

// -------------------------------------------------------
// StringToButtonMask
// -------------------------------------------------------
WORD StringToButtonMask(std::string name)
{
    name = Sanitize(name);
    if (name == "LT")         return VIRTUAL_LT;
    if (name == "RT")         return VIRTUAL_RT;
    if (name == "A")          return XINPUT_GAMEPAD_A;
    if (name == "B")          return XINPUT_GAMEPAD_B;
    if (name == "X")          return XINPUT_GAMEPAD_X;
    if (name == "Y")          return XINPUT_GAMEPAD_Y;
    if (name == "START")      return XINPUT_GAMEPAD_START;
    if (name == "BACK")       return XINPUT_GAMEPAD_BACK;
    if (name == "LB")         return XINPUT_GAMEPAD_LEFT_SHOULDER;
    if (name == "RB")         return XINPUT_GAMEPAD_RIGHT_SHOULDER;
    if (name == "L_THUMB")    return XINPUT_GAMEPAD_LEFT_THUMB;
    if (name == "R_THUMB")    return XINPUT_GAMEPAD_RIGHT_THUMB;
    if (name == "DPAD_UP")    return XINPUT_GAMEPAD_DPAD_UP;
    if (name == "DPAD_DOWN")  return XINPUT_GAMEPAD_DPAD_DOWN;
    if (name == "DPAD_LEFT")  return XINPUT_GAMEPAD_DPAD_LEFT;
    if (name == "DPAD_RIGHT") return XINPUT_GAMEPAD_DPAD_RIGHT;
    return 0;
}

// -------------------------------------------------------
// ButtonMaskToString
// -------------------------------------------------------
std::string ButtonMaskToString(WORD mask)
{
    if (mask == VIRTUAL_LT)                    return "LT";
    if (mask == VIRTUAL_RT)                    return "RT";
    if (mask == XINPUT_GAMEPAD_A)              return "A";
    if (mask == XINPUT_GAMEPAD_B)              return "B";
    if (mask == XINPUT_GAMEPAD_X)              return "X";
    if (mask == XINPUT_GAMEPAD_Y)              return "Y";
    if (mask == XINPUT_GAMEPAD_START)          return "START";
    if (mask == XINPUT_GAMEPAD_BACK)           return "BACK";
    if (mask == XINPUT_GAMEPAD_LEFT_SHOULDER)  return "LB";
    if (mask == XINPUT_GAMEPAD_RIGHT_SHOULDER) return "RB";
    if (mask == XINPUT_GAMEPAD_LEFT_THUMB)     return "L_THUMB";
    if (mask == XINPUT_GAMEPAD_RIGHT_THUMB)    return "R_THUMB";
    if (mask == XINPUT_GAMEPAD_DPAD_UP)        return "DPAD_UP";
    if (mask == XINPUT_GAMEPAD_DPAD_DOWN)      return "DPAD_DOWN";
    if (mask == XINPUT_GAMEPAD_DPAD_LEFT)      return "DPAD_LEFT";
    if (mask == XINPUT_GAMEPAD_DPAD_RIGHT)     return "DPAD_RIGHT";
    return "A";
}

// -------------------------------------------------------
// ResolveConflicts
// -------------------------------------------------------
void ResolveConflicts()
{
    WORD* masks[] = {
        &buttonBACK_Mask, &buttonRESET_Mask, &buttonA_Mask,
        &buttonX_Mask,    &buttonOPEN_Mask,  &buttonFLAG_Mask
    };

    WORD defaults[] = {
        (WORD)XINPUT_GAMEPAD_BACK,  (WORD)XINPUT_GAMEPAD_START,
        (WORD)XINPUT_GAMEPAD_A,     (WORD)XINPUT_GAMEPAD_X,
        (WORD)VIRTUAL_RT,           (WORD)VIRTUAL_LT
    };

    const int count = 6;

    for (int i = 0; i < count; ++i) {
        bool conflict = false;

        if (*masks[i] == 0) {
            conflict = true;
        } else {
            for (int j = 0; j < count; ++j) {
                if (i == j) continue;
                if (*masks[i] == *masks[j]) {
                    conflict = true;
                    break;
                }
            }
        }

        if (conflict)
            *masks[i] = defaults[i];
    }
}

// -------------------------------------------------------
// CreateControls
// -------------------------------------------------------
void CreateControls()
{
    buttonOPEN_Mask  = VIRTUAL_RT;
    buttonFLAG_Mask  = VIRTUAL_LT;
    buttonBACK_Mask  = XINPUT_GAMEPAD_BACK;
    buttonRESET_Mask = XINPUT_GAMEPAD_START;
    buttonA_Mask     = XINPUT_GAMEPAD_A;
    buttonX_Mask     = XINPUT_GAMEPAD_X;
    dpad             = true;
    leftjoystick     = true;

    std::ofstream outFile("game:\\controls.txt", std::ios::trunc);
    if (outFile.is_open()) {
        outFile << "ActionBack=BACK"             << std::endl;
        outFile << "ActionReset=START"           << std::endl;
        outFile << "ActionConfirm=A"             << std::endl;
        outFile << "ActionBoardMenu=X"           << std::endl;
        outFile << "OpenTile=RT"                 << std::endl;
        outFile << "FlagTile=LT"                 << std::endl;
        outFile << "dpadMovement=true"           << std::endl;
        outFile << "leftStickMovement=true"      << std::endl;
        outFile.close();
    }
}

// -------------------------------------------------------
// SaveControls
// -------------------------------------------------------
void SaveControls()
{
    ResolveConflicts();
    std::ofstream outFile("game:\\controls.txt", std::ios::trunc);
    if (outFile.is_open()) {
        outFile << "ActionBack="        << ButtonMaskToString(buttonBACK_Mask)  << std::endl;
        outFile << "ActionReset="       << ButtonMaskToString(buttonRESET_Mask) << std::endl;
        outFile << "ActionConfirm="     << ButtonMaskToString(buttonA_Mask)     << std::endl;
        outFile << "ActionBoardMenu="   << ButtonMaskToString(buttonX_Mask)     << std::endl;
        outFile << "OpenTile="          << ButtonMaskToString(buttonOPEN_Mask)  << std::endl;
        outFile << "FlagTile="          << ButtonMaskToString(buttonFLAG_Mask)  << std::endl;
        outFile << "dpadMovement="      << (dpad        ? "true" : "false")     << std::endl;
        outFile << "leftStickMovement=" << (leftjoystick ? "true" : "false")    << std::endl;
        outFile.close();
    }
}

// -------------------------------------------------------
// LoadControls
// -------------------------------------------------------
void LoadControls()
{
    std::ifstream inFile("game:\\controls.txt");
    std::string line;

    if (inFile.is_open()) {
        while (std::getline(inFile, line)) {
            if (!line.empty() && line[line.size() - 1] == '\r')
                line.erase(line.size() - 1);

            size_t eqPos = line.find('=');
            if (eqPos == std::string::npos) continue;

            std::string key   = Sanitize(line.substr(0, eqPos));
            std::string value = Sanitize(line.substr(eqPos + 1));

            if      (key == "ACTIONOPEN")      buttonOPEN_Mask  = StringToButtonMask(value);
            else if (key == "ACTIONBACK")      buttonBACK_Mask  = StringToButtonMask(value);
            else if (key == "ACTIONRESET")     buttonRESET_Mask = StringToButtonMask(value);
            else if (key == "ACTIONCONFIRM")   buttonA_Mask     = StringToButtonMask(value);
            else if (key == "ACTIONBOARDMENU") buttonX_Mask     = StringToButtonMask(value);
            else if (key == "OPENTILE")        buttonOPEN_Mask  = StringToButtonMask(value);
            else if (key == "FLAGTILE")        buttonFLAG_Mask  = StringToButtonMask(value);
            else if (key == "DPADMOVEMENT")    dpad             = StringToBool(value);
            else if (key == "LEFTSTICKMOVEMENT") leftjoystick   = StringToBool(value);
        }
        inFile.close();
    }
    ResolveConflicts();
}

// -------------------------------------------------------
// InitializeControls
// -------------------------------------------------------
void InitializeControls()
{
    const char* filePath = "game:\\controls.txt";
    std::ifstream fileCheck(filePath);

    if (fileCheck.is_open()) {
        fileCheck.close();
        OutputDebugString("File found! Proceeding to load...\n");
        LoadControls();
    }
    else {
        OutputDebugString("Controls file NOT found! Creating defaults...\n");
        CreateControls();
    }
}

// -------------------------------------------------------
// IsButtonPressed
// -------------------------------------------------------
bool IsButtonPressed(const XINPUT_STATE& state, WORD mask)
{
    if (mask == VIRTUAL_LT)
        return state.Gamepad.bLeftTrigger > TRIGGER_THRESHOLD;
    if (mask == VIRTUAL_RT)
        return state.Gamepad.bRightTrigger > TRIGGER_THRESHOLD;
    return (state.Gamepad.wButtons & mask) != 0;
}

// -------------------------------------------------------
// GetCurrentInput
// -------------------------------------------------------
WORD GetCurrentInput(const XINPUT_STATE& state)
{
    if (state.Gamepad.bLeftTrigger  > TRIGGER_THRESHOLD ) return VIRTUAL_LT;
    if (state.Gamepad.bRightTrigger > TRIGGER_THRESHOLD ) return VIRTUAL_RT;

    WORD buttons[] = {
        XINPUT_GAMEPAD_A,           XINPUT_GAMEPAD_B,
        XINPUT_GAMEPAD_X,           XINPUT_GAMEPAD_Y,
        XINPUT_GAMEPAD_START,       XINPUT_GAMEPAD_BACK,
        XINPUT_GAMEPAD_LEFT_SHOULDER,  XINPUT_GAMEPAD_RIGHT_SHOULDER,
        XINPUT_GAMEPAD_LEFT_THUMB,     XINPUT_GAMEPAD_RIGHT_THUMB,
        XINPUT_GAMEPAD_DPAD_UP,     XINPUT_GAMEPAD_DPAD_DOWN,
        XINPUT_GAMEPAD_DPAD_LEFT,   XINPUT_GAMEPAD_DPAD_RIGHT
    };

    for (int i = 0; i < 14; ++i) {
        if (state.Gamepad.wButtons & buttons[i])
            return buttons[i];
    }
    return 0;
}

// -------------------------------------------------------
// StartRemapping
// -------------------------------------------------------
void StartRemapping(WORD& actionMask)
{
    isListening   = true;
    actionToRemap = &actionMask;
}