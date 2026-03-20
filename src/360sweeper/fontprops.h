#ifndef FONTPROPS_H
#define FONTPROPS_H
#include "StdAfx.h"
#include "globals.h"

template <typename T>
std::string FontPropsToString(T value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void CreateFontProps();
void LoadFontProps();
void InitializeFontProps();

#endif // FONTPROPS_H