#ifndef SETTINGS_H
#define SETTINGS_H

#include "StdAfx.h"
#include "globals.h"

// -------------------------------------------------------
// SETTINGS FUNCTION DECLARATIONS
// -------------------------------------------------------
template <typename T>
std::string ToString(T value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string BoolToString(bool value);

void CreateSettings();
void SaveSettings();
void LoadSettings();
void InitializeSettings();

std::vector<std::string> GetTexturePacks();
void                     RefreshPackList();

#endif // SETTINGS_H