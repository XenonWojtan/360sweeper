#ifndef CONTROLS_H
#define CONTROLS_H

#include "StdAfx.h"
#include "globals.h"

// -------------------------------------------------------
// CONTROLS FUNCTION DECLARATIONS
// -------------------------------------------------------
std::string Sanitize(std::string str);
bool        StringToBool(std::string value);
WORD        StringToButtonMask(std::string name);
std::string ButtonMaskToString(WORD mask);

void ResolveConflicts();
void CreateControls();
void SaveControls();
void LoadControls();
void InitializeControls();

bool IsButtonPressed(const XINPUT_STATE& state, WORD mask);
WORD GetCurrentInput(const XINPUT_STATE& state);
void StartRemapping(WORD& actionMask);

#endif // CONTROLS_H