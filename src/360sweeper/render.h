#ifndef RENDER_H
#define RENDER_H

#include "StdAfx.h"
#include "globals.h"
#include "textures.h"
#include "board.h"

// -------------------------------------------------------
// RENDER FUNCTION DECLARATIONS
// -------------------------------------------------------
float GetPercentage(float input, float lowCap, float highCap);
float percentToPixelsSlider(float percentage);

void  drawMaShit(float scaleX, float scaleY, float posX, float posY);
void DrawTexturedString(float, float, float, const std::string&);
float GetTextWidth(const std::string&, float);
void RenderHeader(const std::string&);

void  RenderCounter(int value, float startX, float startY, float tileWidthNDC, float tileHeightNDC, bool ignoreGameOver);
void  RenderRAM(int totalKB, float startX, float startY, float tileWidthNDC, float tileHeightNDC);
void  RenderTimer(int value, float startX, float startY, float tileWidthNDC, float tileHeightNDC);
void DrawHint(float offsetX, float offsetY, WORD mask, const char* label);

void  Render(Board& board, int selRow, int selCol, int tileCountRow, int tileCountCol, int flagLeft);

#endif // RENDER_H