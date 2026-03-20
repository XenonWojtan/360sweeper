#include "StdAfx.h"
#include "globals.h"
#include "textures.h"
#include "board.h"
#include "render.h"
#include "MinesweeperSave.h"
#include "controls.h"
// -------------------------------------------------------
// GetPercentage
// -------------------------------------------------------
float GetPercentage(float input, float lowCap, float highCap)
{
    if (highCap <= lowCap)
        return (input >= lowCap) ? 100.0f : 0.0f;

    float result = (input - lowCap) / (highCap - lowCap);
    if (result < 0.0f) result = 0.0f;
    if (result > 1.0f) result = 1.0f;
    return result * 100.0f;
}

// -------------------------------------------------------
// percentToPixelsSlider
// -------------------------------------------------------
float percentToPixelsSlider(float percentage)
{
    float centerX = (1280.0f * X1pix) / 2.0f - (640.0f * X1pix);
    float basePos = centerX - (50.0f * X1pix) / 2.0f;
    float maxOffset = (391.0f * X1pix) / 2.0f;
    float multiplier = (percentage - 50.0f) / 50.0f;
    return basePos + (multiplier * maxOffset);
}

// -------------------------------------------------------
// drawMaShit
// -------------------------------------------------------
void drawMaShit(float scaleX, float scaleY, float posX, float posY)
{
    D3DXMATRIX matWorld, matScale, matTrans;
    D3DXMatrixScaling(&matScale, scaleX, scaleY, 1.0f);
    D3DXMatrixTranslation(&matTrans, posX, posY, 0.0f);
    D3DXMatrixMultiply(&matWorld, &matScale, &matTrans);
    g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
    g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

// -------------------------------------------------------
// DrawTexturedString
// -------------------------------------------------------
void DrawTexturedString(float posX, float posY, float scale, const std::string& text)
{
	float s = scale * fontScale;
    for (size_t i = 0; i < text.length(); i++) {
        char c = text[i];
        
        float snappedX = floorf(posX / X1pix + 0.5f) * X1pix;
        float snappedY = floorf(posY / Y1pix + 0.5f) * Y1pix;
        
        if (c == ' ') {
            posX += spacing * X1pix * s;
            continue;
        }

        LPDIRECT3DTEXTURE9 pTexture = GetTextureFromChar(c);
        if (pTexture != NULL) {
            g_pd3dDevice->SetTexture(0, pTexture);
            drawMaShit(s, s, snappedX, snappedY);
            posX += letterWidth * X1pix * s;
        }
    }
}

// -------------------------------------------------------
// GetTextWidth
// -------------------------------------------------------
float GetTextWidth(const std::string& text, float scale)
{
	float s = scale * fontScale;
    float totalWidth = 0.0f;
    for (size_t i = 0; i < text.length(); i++) {
        if (text[i] == ' ')
            totalWidth += spacing * X1pix * s;
        else
            totalWidth += letterWidth * X1pix * s;
    }
    return totalWidth;
}

// -------------------------------------------------------
// RenderHeader
// -------------------------------------------------------
void RenderHeader(const std::string& text)
{
    float centerX  = (1280.0f * X1pix) / 2.0f - (640.0f * X1pix);
    float centerY  = (640.0f  * Y1pix) / 2.0f - (320.0f * Y1pix);
    float scale    = 1.0f;
    float textWidth = GetTextWidth(text, scale);
    float posYtext  = centerY - 37.0f * Y1pix + 256.0f * Y1pix - 7.5f * Y1pix;
    float posXtext  = centerX - textWidth / 2.0f + 10.0f * X1pix;
    DrawTexturedString(posXtext, posYtext, scale, text);
}

// -------------------------------------------------------
// RenderCounter
// -------------------------------------------------------
void RenderCounter(int value, float startX, float startY, float tileWidthNDC, float tileHeightNDC, bool ignoreGameOver)
{
    bool isNegative = (value < 0);
    int  absValue   = abs(value);

    if (value < -99) absValue = 99;
    if (value > 999) absValue = 999;

    int digits[3];
    digits[0] = absValue / 100;
    digits[1] = (absValue / 10) % 10;
    digits[2] = absValue % 10;

    // matBase = matScale * matTextureOffset -- constant for all digits, built once
    D3DXMATRIX matScale, matTextureOffset, matBase;
    D3DXMatrixScaling(&matScale, 1.0f, 2.0f, 1.0f);
    D3DXMatrixTranslation(&matTextureOffset, -0.1f * tileWidthNDC, 0.1f * tileHeightNDC, 0.0f);
    D3DXMatrixMultiply(&matBase, &matScale, &matTextureOffset);

    for (int i = 0; i < 3; i++) {
        float digitPosX   = startX + (i * tileWidthNDC * 0.81f);
        float counterPosY = startY + (1.4f * tileHeightNDC);

        D3DXMATRIX matTrans, matFinal;
        D3DXMatrixTranslation(&matTrans, digitPosX, counterPosY, 0.0f);
        D3DXMatrixMultiply(&matFinal, &matBase, &matTrans);

        if (gameOver == 1 && !ignoreGameOver) {
            g_pd3dDevice->SetTexture(0, g_pnum_num0Texture);
            g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matFinal, 4);
            g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
        }
        else {
            LPDIRECT3DTEXTURE9 pTex = (isNegative && i == 0)
                ? g_pnum_minusTexture
                : GetNumberTexture(digits[i]);
            g_pd3dDevice->SetTexture(0, pTex);
            g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matFinal, 4);
            g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
        }
    }
}

// -------------------------------------------------------
// RenderRAM
// -------------------------------------------------------
void RenderRAM(int totalKB, float startX, float startY, float tileWidthNDC, float tileHeightNDC)
{
    if (totalKB < 0)      totalKB = 0;
    if (totalKB > 999999) totalKB = 999999;

    int digits[6];
    digits[0] = (totalKB / 100000) % 10;
    digits[1] = (totalKB / 10000)  % 10;
    digits[2] = (totalKB / 1000)   % 10;
    digits[3] = (totalKB / 100)    % 10;
    digits[4] = (totalKB / 10)     % 10;
    digits[5] =  totalKB           % 10;

    // matBase = matScale * matTextureOffset -- constant for all digits, built once
    D3DXMATRIX matScale, matTextureOffset, matBase;
    D3DXMatrixScaling(&matScale, 1.0f, 2.0f, 1.0f);
    D3DXMatrixTranslation(&matTextureOffset, -0.1f * tileWidthNDC, 0.1f * tileHeightNDC, 0.0f);
    D3DXMatrixMultiply(&matBase, &matScale, &matTextureOffset);

    for (int i = 0; i < 6; i++) {
        float digitPosX   = startX - (i * tileWidthNDC * 0.81f);
        float counterPosY = startY + (1.4f * tileHeightNDC);

        D3DXMATRIX matTrans, matFinal;
        D3DXMatrixTranslation(&matTrans, digitPosX, counterPosY, 0.0f);
        D3DXMatrixMultiply(&matFinal, &matBase, &matTrans);

        g_pd3dDevice->SetTexture(0, GetNumberTexture(digits[i]));
        g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matFinal, 4);
        g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
    }
}

// -------------------------------------------------------
// RenderTimer
// -------------------------------------------------------
void RenderTimer(int value, float startX, float startY, float tileWidthNDC, float tileHeightNDC)
{
    int totalSeconds = value / 100;
    if (totalSeconds < 0)   totalSeconds = 0;
    if (totalSeconds > 999) totalSeconds = 999;

    int digits[3];
    digits[0] = totalSeconds % 10;
    digits[1] = (totalSeconds / 10) % 10;
    digits[2] = totalSeconds / 100;

    // matBase = matScale * matTextureOffset -- constant for all digits, built once
    D3DXMATRIX matScale, matTextureOffset, matBase;
    D3DXMatrixScaling(&matScale, 1.0f, 2.0f, 1.0f);
    D3DXMatrixTranslation(&matTextureOffset, -0.1f * tileWidthNDC, 0.1f * tileHeightNDC, 0.0f);
    D3DXMatrixMultiply(&matBase, &matScale, &matTextureOffset);

    for (int i = 0; i < 3; i++) {
        float digitPosX   = startX - (i * tileWidthNDC * 0.81f);
        float counterPosY = startY + (1.4f * tileHeightNDC);

        D3DXMATRIX matTrans, matFinal;
        D3DXMatrixTranslation(&matTrans, digitPosX, counterPosY, 0.0f);
        D3DXMatrixMultiply(&matFinal, &matBase, &matTrans);

        g_pd3dDevice->SetTexture(0, GetNumberTexture(digits[i]));
        g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matFinal, 4);
        g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
    }
}

void DrawHint(float offsetX, float offsetY, WORD mask, const char* label)
{
    float hintX = (640.0f - 100.0f) * X1pix + offsetX;
    float hintY = -(320.0f - 3.0f) * Y1pix + offsetY;
    const float iconScale  = 1.0f;
    const float labelScale = 0.6f;
    float textOffY = hintY - 12.0f * Y1pix;
	
    LPDIRECT3DTEXTURE9 pIcon;
    std::string        btnName;
    if (mask == NULL) {
        pIcon   = g_pquestCovTexture;
        btnName = "";
    } else {
        btnName = Sanitize(ButtonMaskToString(mask));
        pIcon   = StringToButtonIcon(btnName);
    }
    if (pIcon != NULL) {
        bool isABXY      = (btnName == "A" || btnName == "B" || btnName == "X" || btnName == "Y");
        bool isLTRT      = (btnName == "LT" || btnName == "RT");
        bool isStartBack = (btnName == "START" || btnName == "BACK");
        bool isDPAD      = (btnName == "DPAD_UP" || btnName == "DPAD_DOWN" || btnName == "DPAD_LEFT" || btnName == "DPAD_RIGHT");
        bool isThumb     = (btnName == "L_THUMB" || btnName == "R_THUMB");
        bool isBumper    = (btnName == "LB" || btnName == "RB");

        float btnScale = isABXY ? iconScale : isThumb ? 1.5f : isBumper ? 1.45f : 1.85f;
        if (isStartBack) btnScale -= 0.1f;

        float btnX = hintX - (isLTRT     ? 16.0f * X1pix : 0.0f)
                           - (isStartBack ?  2.0f * X1pix : 0.0f)
                           - (isDPAD     ? 12.0f * X1pix : 0.0f);
        float btnY = hintY - 10.0f * Y1pix
                           + (isLTRT  ? 12.0f * Y1pix : 0.0f)
                           + (isDPAD  ? 12.0f * Y1pix : 0.0f)
                           + (isThumb ?  2.0f * Y1pix : 0.0f);

        g_pd3dDevice->SetTexture(0, pIcon);
        drawMaShit(btnScale, btnScale, btnX, btnY);
    }

    float textX = hintX + 29.0f * X1pix;
    DrawTexturedString(textX, textOffY, labelScale, label);
}

// -------------------------------------------------------
// Render
// -------------------------------------------------------
void Render(Board& board, int selRow, int selCol, int tileCountRow, int tileCountCol, int flagLeft)
{
    const float centerX = 0.0f;
    const float centerY = 0.0f;

    float totalWindowHeight = (tileCountRow * tileHeightNDC) + (3.0f * tileHeightNDC) + offset24pxNDC;
    float totalGridWidth    = tileCountCol * tileWidthNDC;
    float startX            = -(totalGridWidth / 2.0f) + (tileWidthNDC / 2.0f);
    float visualCenterY     = totalWindowHeight / 2.0f;
    float topSectionHeight  = (2.0f * tileHeightNDC) + offset24pxNDC + (tileHeightNDC / 2.0f);
    float startY            = visualCenterY - topSectionHeight;
    float scaleX            = 1.0f;
    float scaleY            = 1.0f;

    g_pd3dDevice->SetVertexDeclaration(g_pVertexDecl);
    g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(TEXVERTEX));
    g_pd3dDevice->SetIndices(g_pIB);
    g_pd3dDevice->SetVertexShader(g_pVertexShader);
    g_pd3dDevice->SetPixelShader(g_pPixelShader);
	//-----------------------------------------------
	// TITLE SCREEN
	//-----------------------------------------------
	if (gameRunning == TITLESCREEN) {
		const float logoY  = centerY + 60.0f * Y1pix;
		const float textY  = centerY - 180.0f * Y1pix;

		g_pd3dDevice->SetTexture(0, g_plogoTexture);
		drawMaShit(32.0f, 4.0f, centerX - 305.0f/2 *X1pix, logoY);

		const float labelScale = 0.8f;
		const char* part1      = "Press";
		const char* part2      = "to start";

		float part1W  = GetTextWidth(part1, labelScale);
		float part2W  = GetTextWidth(part2, labelScale);
		float iconW   = 20.0f * X1pix;   
		float totalW  = part1W + iconW + part2W;
		float curX    = centerX - (totalW / 2.0f) - 1.0f * X1pix;

		// "Press "
		DrawTexturedString(curX, textY, labelScale, part1);
		curX += part1W + 4.0f * X1pix;

		// A button icon
		curX += 7.0f * X1pix;
		g_pd3dDevice->SetTexture(0, StringToButtonIcon(ButtonMaskToString(buttonA_Mask)));
		drawMaShit(1.0f, 1.0f, curX, textY + 3.0f *Y1pix);
		curX += iconW + 7.0F * X1pix;

		// " to start"
		DrawTexturedString(curX, textY, labelScale, part2);
	}
    //-----------------------------------------------
    // BOARD SIZE MENU
    //-----------------------------------------------
    else if (gameRunning == BOARDSIZE) {
        float posX, posY, posXtext, posYtext, textWidth;
        int   i;

        static const char* boardLabels[5] = { "Beginner", "Intermediate", "Expert", "Custom", "Change size"};

        scaleY = 8.0f;
        scaleX = 16.0f;
        posX   = centerX - ((43.0f * X1pix) / 2.0f);
        posY   = centerY + ((37.0f * Y1pix) / 2.0f);

        g_pd3dDevice->SetTexture(0, g_pboard_size_windowTexture);
        drawMaShit(scaleX, scaleY, posX, posY);

        posYtext  = centerY + 37.0f * Y1pix * 2.0f + 5.0f * Y1pix;
        posXtext  = centerX - 43.0f * X1pix * 2.0f;
        DrawTexturedString(posXtext, posYtext, 1.0f, "Board size");

        scaleY   = 2.0f;
        scaleX   = 4.0f;
        posX     = centerX - (138.0f * X1pix);
        posY     = posY - (5.0f * Y1pix);
        posYtext = posY - 10.0f * Y1pix;

        for (i = 0; i < 3; i++) {
            posX = (centerX - (138.0f * X1pix)) + (i * (128.0f + 10.0f) * X1pix);

            g_pd3dDevice->SetTexture(0,
                (i == buttonBoard - 1) ? g_pbuttonselectedTexture : g_pbuttonTexture);
            drawMaShit(scaleX, scaleY, posX, posY);

            textWidth = GetTextWidth(boardLabels[i], 0.5f);
            posXtext  = posX - (textWidth / 2.0f) + 10.0f * X1pix / 2.0f;
            DrawTexturedString(posXtext, posYtext, 0.5f, boardLabels[i]);
        }
		posX = (centerX - (138.0f * X1pix));
		posY -= 53.0f * Y1pix;
		posYtext -= 53.0f * Y1pix;
        for (i = 0; i < 2; i++) {
            posX = (centerX - (138.0f * X1pix)) + (i * (128.0f + 10.0f) * X1pix) + 64.0f*X1pix;

            g_pd3dDevice->SetTexture(0,
                (i + 3 == buttonBoard - 1 ) ? g_pbuttonselectedTexture : g_pbuttonTexture);
            drawMaShit(scaleX, scaleY, posX, posY);

            textWidth = GetTextWidth(boardLabels[i+3], 0.5f);
            posXtext  = posX - (textWidth / 2.0f) + 10.0f * X1pix / 2.0f;
            DrawTexturedString(posXtext, posYtext, 0.5f, boardLabels[i+3]);
        }
		if (buttonBoard == 5) DrawHint(-20.0f * X1pix, 0.0f, buttonA_Mask, "Select");
		else DrawHint(-20.0f * X1pix, 0.0f, buttonA_Mask, "Play");
		DrawHint(-120.0f * X1pix, 0.0f, buttonX_Mask,   "Back");
    }
	//-----------------------------------------------
    // CUSTOM SIZE MENU
    //-----------------------------------------------
    if (gameRunning == CUSTOMSIZE) {
        float posX, posY, posXtext, posYtext, textWidth;
        int   i;
		

        static const char* boardLabels[4] = { "Rows", "Columns", "Mines", "Confirm"};

        scaleY = 8.0f;
        scaleX = 16.0f;
        posX   = centerX - ((43.0f * X1pix) / 2.0f);
        posY   = centerY + ((37.0f * Y1pix) / 2.0f);

        g_pd3dDevice->SetTexture(0, g_pboard_size_windowTexture);
        drawMaShit(scaleX, scaleY, posX, posY);

        posYtext  = centerY + 37.0f * Y1pix * 2.0f + 5.0f * Y1pix;
        posXtext  = centerX - 43.0f * X1pix * 2.0f;
		float mmeow = GetTextWidth("Custom board size", 1.0f);
		posXtext  = centerX - mmeow / 2.0f + 10.0f * X1pix;
        DrawTexturedString(posXtext, posYtext, 1.0f, "Custom board size");

        scaleY   = 2.0f;
        scaleX   = 4.0f;
        posX     = centerX - (138.0f * X1pix);
        posY     = posY - (5.0f * Y1pix);
        posYtext = posY - 10.0f * Y1pix;

        for (i = 0; i < 3; i++) {
            posX = (centerX - (138.0f * X1pix)) + (i * (128.0f + 10.0f) * X1pix);

            textWidth = GetTextWidth(boardLabels[i], 0.8f);
            posXtext  = posX - (textWidth / 2.0f) + 10.0f * X1pix / 2.0f;
			float tempX = posXtext;
			float tempY = posY - 96.0f * Y1pix;
            DrawTexturedString(posXtext, posYtext, 0.8f, boardLabels[i]);
			
			
			
			
			if (i == 0){
				tempX -= 3.0f*X1pix;
				if (buttonBoard == 1) {g_pd3dDevice->SetTexture(0, g_pboardhighlightTexture); drawMaShit(4.0f, 2.0f, tempX + 6.0f *X1pix, tempY+92.0f/2.0f * Y1pix - 4.0f *Y1pix);}
				else {
					g_pd3dDevice->SetTexture(0, g_pbordercounterTexture);
					drawMaShit(4.0f, 2.0f, tempX + 3.0f * X1pix, tempY+92.0f/2.0f * Y1pix);
				}
				RenderCounter(customRow,tempX,tempY,tileWidthNDC,tileHeightNDC, true);
				
				
			}
			else if (i == 1) {
				tempX = posXtext + textWidth/4.0f - 5.0f*X1pix;
				if (buttonBoard == 2) {g_pd3dDevice->SetTexture(0, g_pboardhighlightTexture); drawMaShit(4.0f, 2.0f, tempX + 6.0f *X1pix, tempY+92.0f/2.0f * Y1pix - 4.0f *Y1pix);}
				else {
					g_pd3dDevice->SetTexture(0, g_pbordercounterTexture);
					drawMaShit(4.0f, 2.0f, tempX + 3.0f * X1pix, tempY+92.0f/2.0f * Y1pix);
				}
				RenderCounter(customCol,tempX,tempY,tileWidthNDC,tileHeightNDC, true);
				
			}
			else if (i == 2) {
				tempX += 5.0f*X1pix;
				if (buttonBoard == 3) {g_pd3dDevice->SetTexture(0, g_pboardhighlightTexture); drawMaShit(4.0f, 2.0f, tempX + 6.0f *X1pix, tempY+92.0f/2.0f * Y1pix - 4.0f *Y1pix);}
				else {
					g_pd3dDevice->SetTexture(0, g_pbordercounterTexture);
					drawMaShit(4.0f, 2.0f, tempX + 3.0f * X1pix, tempY+92.0f/2.0f * Y1pix);
				}
				RenderCounter(customMines,tempX,tempY,tileWidthNDC,tileHeightNDC, true);

			}
			
        }
		DrawHint(-20.0f * X1pix, 0.0f, buttonA_Mask, "Play");
		DrawHint(-120.0f * X1pix, 0.0f, buttonBACK_Mask,   "Back");
	}
    //-----------------------------------------------
    // PAUSE MENU
    //-----------------------------------------------
    else if (gameRunning == PAUSEMENU) {
        float posX, posY, posXtext, textWidth;
        int   i;

        static const char* pauseLabels[4] = { "Play game", "Options", "Statistics", "Exit game" };
        const float stepY = 64.0f * Y1pix + 10.0f * Y1pix;

        scaleY = 2.0f;
        scaleX = 16.0f;
        posX   = centerX - ((162.0f * X1pix) / 2.0f);
        posY   = centerY + (68.0f * Y1pix) + stepY;

        for (i = 0; i < 4; i++) {
            g_pd3dDevice->SetTexture(0,
                (i == buttonMenu - 1) ? g_pbuttonmenuselectedTexture : g_pbuttonmenuTexture);
            drawMaShit(scaleX, scaleY, posX, posY);

            textWidth = GetTextWidth(pauseLabels[i], 1.0f);
            posXtext  = centerX - (textWidth / 2.0f) + 10.0f * X1pix;
            DrawTexturedString(posXtext, posY, 1.0f, pauseLabels[i]);

            posY -= stepY;
        }
		DrawHint(-20.0f * X1pix, 0.0f, buttonA_Mask, "Select");
		DrawHint(-120.0f * X1pix, 0.0f, buttonBACK_Mask,   "Back");
    }

    //-----------------------------------------------
    // OPTIONS MENU
    //-----------------------------------------------
    else if (gameRunning == OPTIONS) {
        float posX, posY, posXtext, textWidth;
        int   i;

        static const char* optionLabels[4] = { "How to play", "Controls", "Settings", "Credits" };
        const float stepY = 64.0f * Y1pix + 10.0f * Y1pix;

        scaleY = 2.0f;
        scaleX = 16.0f;
        posX   = centerX - ((162.0f * X1pix) / 2.0f);
        posY   = centerY + (68.0f * Y1pix) + stepY;

        for (i = 0; i < 4; i++) {
            g_pd3dDevice->SetTexture(0,
                (i == buttonMenu - 1) ? g_pbuttonmenuselectedTexture : g_pbuttonmenuTexture);
            drawMaShit(scaleX, scaleY, posX, posY);

            textWidth = GetTextWidth(optionLabels[i], 1.0f);
            posXtext  = centerX - (textWidth / 2.0f) + 10.0f * X1pix;
            DrawTexturedString(posXtext, posY, 1.0f, optionLabels[i]);

            posY -= stepY;
        }
		DrawHint(-20.0f * X1pix, 0.0f, buttonA_Mask, "Select");
		DrawHint(-120.0f * X1pix, 0.0f, buttonBACK_Mask,   "Back");
    }

    //-----------------------------------------------
    // EXIT / CONFIRM DIALOG
    //-----------------------------------------------
    else if (gameRunning == EXIT || gameRunning == CONFIRM || gameRunning == RESTART || gameRunning == RESETCONTROLS) {
        float posX, posY, posXtext, posYtext, textWidth;
        int   i;

        const char* confirmLabel = (gameRunning == EXIT) ? "Exit" :
								   (gameRunning == RESTART) ? "Restart" : 
								   "Reset";

        scaleY = 8.0f;
        scaleX = 16.0f;
        posX   = centerX - ((43.0f * X1pix) / 2.0f);
        posY   = centerY + ((37.0f * Y1pix) / 2.0f);

        g_pd3dDevice->SetTexture(0, g_pboard_size_windowTexture);
        drawMaShit(scaleX, scaleY, posX, posY);

        scaleY = 4.0f;
        scaleX = 8.0f;
        posX   = centerX - (138.0f * X1pix);
        posY   = posY - ((55.0f * Y1pix) / 4.0f);

        for (i = 0; i < 2; i++) {
            posX += (i > 0) ? (200.0f * X1pix + 10.0f * X1pix) : 0.0f;

            g_pd3dDevice->SetTexture(0,
                (i == buttonMenu - 1) ? g_pbuttonexitselectedTexture : g_pbuttonexitTexture);
            drawMaShit(scaleX, scaleY, posX, posY);

            posYtext = posY - (73.0f * Y1pix) / 2.0f + ((55.0f * Y1pix) / 4.0f) - 10.0f * Y1pix / 2.0f;

            {
                const char* lbl = (i == 0) ? "Cancel" : confirmLabel;
                textWidth = GetTextWidth(lbl, 1.0f);
                posXtext  = posX - (textWidth / 2.0f) + (191.0f * X1pix) / 4.0f - 10.0f * X1pix / 2.0f;
                DrawTexturedString(posXtext, posYtext, 1.0f, lbl);
            }

            posX = centerX - (138.0f * X1pix);
        }

        textWidth = GetTextWidth("Are you sure?", 1.0f);
        posYtext  = centerY - 37.0f * Y1pix + 116.5f * Y1pix;
        posXtext  = centerX - textWidth / 2.0f + 10.0f * X1pix;
        DrawTexturedString(posXtext, posYtext, 1.0f, "Are you sure?");
		DrawHint(-20.0f * X1pix, 0.0f, buttonA_Mask, "Select");
		DrawHint(-120.0f * X1pix, 0.0f, buttonBACK_Mask,   "Back");
    }

    //-----------------------------------------------
    // SHARED WINDOW BACKGROUND + HEADER
    // (SETTINGS / CREDITS / CONTROLS / TUTORIAL / STATISTICS)
    //-----------------------------------------------
    else if (gameRunning == SETTINGS  || gameRunning == CREDITS   ||
             gameRunning == CONTROLS  || gameRunning == TUTORIAL  ||
             gameRunning == STATISTICS || gameRunning == GAMEPLAYTUT ||
			 gameRunning == PATTERNS)
    {
        float posX = centerX - ((43.0f * X1pix) / 2.0f);
        float posY = centerY;

        scaleY = 16.0f;
        scaleX = 16.0f;

        g_pd3dDevice->SetTexture(0, g_psettings_windowTexture);
        drawMaShit(scaleX, scaleY, posX, posY);

        {
            const char* hdr = "";
            if      (gameRunning == SETTINGS)   hdr = "Settings";
            else if (gameRunning == STATISTICS) hdr = "Statistics";
            else if (gameRunning == CREDITS)    hdr = "Credits";
            else if (gameRunning == TUTORIAL)   hdr = "How to play";
            else if (gameRunning == CONTROLS)   hdr = "Controls";
            RenderHeader(hdr);
        }
    }
	//-----------------------------------------------
	// TUTORIAL PAGE
	//-----------------------------------------------
	if (gameRunning == TUTORIAL) {
		float posX, posY, posXtext, textWidth;
		int   i;

		static const char* tutorialLabels[2] = { "Gameplay", "Patterns" };
		const float stepY = 64.0f * Y1pix + 10.0f * Y1pix;

		scaleY = 2.0f;
		scaleX = 16.0f;
		posX   = centerX - ((162.0f * X1pix) / 2.0f);
		posY   = centerY + (68.0f * Y1pix) + stepY - 30.0f * Y1pix;

		for (i = 0; i < 2; i++) {
			g_pd3dDevice->SetTexture(0,
				(i == buttonMenu - 1) ? g_pbuttonmenuselectedTexture : g_pbuttonmenuTexture);
			drawMaShit(scaleX, scaleY, posX, posY);

			textWidth = GetTextWidth(tutorialLabels[i], 1.0f);
			posXtext  = centerX - (textWidth / 2.0f) + 10.0f * X1pix;
			DrawTexturedString(posXtext, posY, 1.0f, tutorialLabels[i]);

			posY -= stepY;
		}
		DrawHint(-20.0f * X1pix, 0.0f, buttonA_Mask, "Select");
		DrawHint(-120.0f * X1pix, 0.0f, buttonBACK_Mask, "Back");
	}
	//-----------------------------------------------
	// GAMEPLAYTUT PAGE
	//-----------------------------------------------
	if (gameRunning == GAMEPLAYTUT) {
		RenderHeader("Gameplay tutorial");
		float posX = centerX - ((43.0f * X1pix) / 2.0f) - 40.0f * X1pix;
		float posY = centerY + 85.0f * Y1pix;
		
		const float textAreaX = centerX - (170.0f * X1pix);
		const float textAreaY = centerY - (20.0f * Y1pix);
		const float lineStep  = 24.0f * Y1pix;
		const float textScale = 0.5f;
		int i = 0;
		if (tutScene != 0) {
			g_pd3dDevice->SetTexture(0, g_parrowLeftTexture);
            drawMaShit(1.0f, 1.0f, centerX - 180.0f *X1pix, posY - 285.0f *Y1pix);
		}
		if (tutScene != 14) {
			g_pd3dDevice->SetTexture(0, g_parrowRightTexture);
            drawMaShit(1.0f, 1.0f, centerX + 180.0f *X1pix, posY - 285.0f *Y1pix);
		}
		if (tutScene == 0 || tutScene ==  1 || tutScene ==  2) {
			if (tutScene !=  2) {
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix, textScale, "Welcome to 360sweeper.");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep, textScale, "If you already know how to play, click back.");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 2.0f, textScale, "This is a tutorial on how to play the");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 3.0f, textScale, "game. The goal of the game is to open");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 4.0f, textScale, "all of the safe tiles without opening");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 5.0f, textScale, "mines. You can flag tiles you suspect of");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 6.0f, textScale, "being mines.");
			}
			else {
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix, textScale, "See here how a lot of the board opened,");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep, textScale, "thats because we opened a 0 if that");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 2.0f, textScale, "happens all of its adjacent cells will");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 3.0f, textScale, "open too.");
			}
			if (tutScene == 0) DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 7.0f, textScale, "To open a tile click LT and to flag RT");
			LPDIRECT3DTEXTURE9 pTopTex = (tutScene == 0) ? g_ptutorial_scene1Texture :
									 g_ptutorial_scene2Texture ;
			g_pd3dDevice->SetTexture(0, pTopTex);
			drawMaShit(8.0f, 4.0f,centerX, posY - 30.0f* Y1pix - lineStep * 7.0f);
		}
		else if (tutScene ==  3 || tutScene ==  4 || tutScene ==  5 || tutScene ==  6) {
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix, textScale, "The logic of minesweeeper is that every");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep, textScale, "tile has a number in it. The number");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 2.0f, textScale, "indicates how many mines are surrounding");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 3.0f, textScale, "that tile. Thus if a number is touching");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 4.0f, textScale, "the same number of cells then they all");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 5.0f, textScale, "must be mines.");
			
			if (tutScene ==  4) DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 6.0f, textScale, "Look at this 1.");
			else if (tutScene ==  5) { DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 6.0f, textScale, "Its touching all these cells but"); 
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 7.0f, textScale, "only one is unopened."); }
			else if (tutScene ==  6) {DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 6.0f, textScale, "Since its a 1 the unopened cell must be");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 7.0f, textScale, "a mine then, flag it!");}
			LPDIRECT3DTEXTURE9 pTopTex = (tutScene ==  2) ? NULL :
									 (tutScene ==  3) ? g_ptutorial_scene2Texture :
									 (tutScene ==  4) ? g_ptutorial_scene3Texture :
									 (tutScene ==  5) ? g_ptutorial_scene4Texture :
													   g_ptutorial_scene44Texture;
			g_pd3dDevice->SetTexture(0, pTopTex);
			drawMaShit(8.0f, 4.0f,centerX, posY - 30.0f* Y1pix - lineStep * 7.0f);
		}
		else if (tutScene ==   7 || tutScene ==   8 || tutScene ==   9) {
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix, textScale, "If a number is touching the same number of");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep, textScale, "flags as itself, all of its adjacent tiles");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 2.0f, textScale, "can be opened.");
			if (tutScene ==   7) DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 3.0f, textScale, "Look:");
			if (tutScene ==   8 || tutScene ==   9 ) DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 3.0f, textScale, "This 1 is touching one flag.");
			if (tutScene ==   9) DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 4.0f, textScale, "So the blue cells can be opened.");
			
			LPDIRECT3DTEXTURE9 pTopTex = (tutScene ==   7) ? g_ptutorial_scene5Texture :
									 (tutScene ==   8) ? g_ptutorial_scene6Texture :
									 (tutScene ==   9) ? g_ptutorial_scene7Texture :
													   g_ptutorial_scene8Texture;
			g_pd3dDevice->SetTexture(0, pTopTex);
			drawMaShit(8.0f, 4.0f,centerX, posY - 30.0f* Y1pix - lineStep * 7.0f);
		}
		else if (tutScene ==   10 || tutScene ==   11 || tutScene ==   12) {
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix, 1.0f, "Chording");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep, textScale, "When a number is touching the same number");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 2.0f, textScale, "of flags, you can click it and all of the");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 3.0f, textScale, "other cells next to it will be opened.");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 4.0f, textScale, "Careful! If your flagging is wrong a mine");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 5.0f, textScale, "can be opened. You can turn this feature");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 6.0f, textScale, "off in settings.");
			
			if (tutScene ==   10) DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 7.0f, textScale, "Look the case from the last slide:");
			else if (tutScene ==   11 || tutScene ==   12) { DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 7.0f, textScale, "This 1 has one flag next to it"); 
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 8.0f, textScale, "so when we click it the blue cells will");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 9.0f, textScale, "uncover."); } 
			LPDIRECT3DTEXTURE9 pTopTex = (tutScene ==   10 || tutScene ==   11) ? g_ptutorial_scene7Texture :
															 g_ptutorial_scene8Texture ;
			g_pd3dDevice->SetTexture(0, pTopTex);
			if (tutScene ==   11 || tutScene ==   12)drawMaShit(8.0f, 4.0f,centerX, posY - 30.0f* Y1pix - lineStep * 9.0f);
			else drawMaShit(8.0f, 4.0f,centerX, posY - 30.0f* Y1pix - lineStep * 7.0f);
		}
		else if (tutScene ==   13) {
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix, textScale, "The board beside the tiles has also two");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep, textScale, "counters and a face. The left counter");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 2.0f, textScale, "indicates the amount of mines there are");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 3.0f, textScale, "in the board and it decreases when you");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 4.0f, textScale, "place flags. The right counter is a timer");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 5.0f, textScale, "showing how much time has passed since");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 6.0f, textScale, "the game started. The face indicates the");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 7.0f, textScale, "state of the game.");
			g_pd3dDevice->SetTexture(0, g_ptutorial_boardtopTexture);
			drawMaShit(16.0f, 4.0f, textAreaX+ 80.0f * X1pix, posY  - lineStep * 8.0f);
		}
		else if (tutScene == 14) {
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix, 1.0f, "Tips:");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep, textScale, "Chording can help save time in solving a");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 2.0f, textScale, "board since you can open more then one");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 3.0f, textScale, "cell with a single click.");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 4.0f, textScale, "Start the game by opening corners -");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 5.0f, textScale, "they have the biggest chance to be a");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 6.0f, textScale, "opening (0).");
			static const char* gameLabels[2] = { "Play game", "Patterns" };
			const float stepY = 64.0f * Y1pix + 10.0f * Y1pix;
			scaleY = 2.0f;
			scaleX = 16.0f;
			float tempposX   = centerX - ((162.0f * X1pix) / 2.0f);
			float tempposY   = centerY + (68.0f * Y1pix) + stepY - 200.0f * Y1pix;

			for (i = 0; i < 2; i++) {
				g_pd3dDevice->SetTexture(0,
					(i == buttonMenu - 1) ? g_pbuttonmenuselectedTexture : g_pbuttonmenuTexture);
				drawMaShit(scaleX, scaleY, tempposX, tempposY);

				float textWidth = GetTextWidth(gameLabels[i], 1.0f);
				float posXtext  = centerX - (textWidth / 2.0f) + 10.0f * X1pix;
				DrawTexturedString(posXtext, tempposY, 1.0f, gameLabels[i]);

				tempposY -= stepY;
			}

		}
		DrawHint(0.0f, 20.0f * Y1pix, buttonBACK_Mask, "Back");
	}
	//-----------------------------------------------
	// PATTERNS PAGE
	//-----------------------------------------------
	if (gameRunning == PATTERNS) {
		RenderHeader("Patterns");
		float posX = centerX - ((43.0f * X1pix) / 2.0f) - 40.0f * X1pix;
		float posY = centerY + 80.0f * Y1pix;
		const float textAreaX = centerX - (170.0f * X1pix);
		const float textAreaY = centerY - (20.0f * Y1pix);
		const float lineStep  = 24.0f * Y1pix;
		const float textScale = 0.5f;
		if (tutScene != 0) {
			g_pd3dDevice->SetTexture(0, g_parrowLeftTexture);
            drawMaShit(1.0f, 1.0f, centerX - 180.0f *X1pix, posY - 285.0f *Y1pix);
		}
		if (tutScene != 16) {
			g_pd3dDevice->SetTexture(0, g_parrowRightTexture);
            drawMaShit(1.0f, 1.0f, centerX + 180.0f *X1pix, posY - 285.0f *Y1pix);
		}
		if (tutScene == 0 || tutScene ==   1  || tutScene ==   2  || tutScene ==   3  || tutScene ==   4) {
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix, 1.0f, "1-1 pattern");
			
			if (tutScene ==   1 || tutScene ==   2) DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 1.0f, textScale, "- Look at this 1");
			if (tutScene ==   2) {
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 2.0f, textScale, "- Its touches the two blue tiles so there");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 3.0f, textScale, "must be a mine in one of them");
			}
			if (tutScene ==   3 || tutScene ==   4) DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 1.0f, textScale, "- Now look at this other 1");
			if (tutScene ==   4) {
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 2.0f, textScale, "- It also touches the blue tiles, so");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 3.0f, textScale, "it already has a mine and the purple");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 4.0f, textScale, "cell can be safely opened");
			}
			LPDIRECT3DTEXTURE9 pTopTex = (tutScene ==   0) ? g_ptutorial_patterns_11scene1Texture :
									 (tutScene ==   1) ? g_ptutorial_patterns_11scene2Texture :
									 (tutScene ==   2) ? g_ptutorial_patterns_11scene3Texture :
									 (tutScene ==   3) ? g_ptutorial_patterns_11scene4Texture :
													   g_ptutorial_patterns_11scene5Texture;
			g_pd3dDevice->SetTexture(0, pTopTex);
			drawMaShit(8.0f, 4.0f,centerX, posY - 30.0f* Y1pix - lineStep * 7.0f);
		}
		if (tutScene == 5 || tutScene ==   6  || tutScene ==   7  || tutScene ==   8  || tutScene ==   9) {
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix, 1.0f, "1-2 pattern");
			if (tutScene ==   6 || tutScene ==   7) DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 1.0f, textScale, "- Look at this 1");
			if (tutScene ==   7) {
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 2.0f, textScale, "- Its touches the two blue tiles so there");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 3.0f, textScale, "must be a mine in one of them");
			}
			if (tutScene ==   8 || tutScene ==   9) DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 1.0f, textScale, "- Now look at this 2");
			if (tutScene ==   9) {
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 2.0f, textScale, "- The first mine is in the blue cells,");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 3.0f, textScale, "therefore, the second mine is in the");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 4.0f, textScale, "purple cell.");
			}
			
			LPDIRECT3DTEXTURE9 pTopTex = (tutScene ==   5) ? g_ptutorial_patterns_12scene1Texture :
									 (tutScene ==   6) ? g_ptutorial_patterns_12scene2Texture :
									 (tutScene ==   7) ? g_ptutorial_patterns_12scene3Texture :
									 (tutScene ==   8) ? g_ptutorial_patterns_12scene4Texture :
													   g_ptutorial_patterns_12scene5Texture;
			g_pd3dDevice->SetTexture(0, pTopTex);
			drawMaShit(8.0f, 4.0f,centerX, posY - 30.0f* Y1pix - lineStep * 7.0f);
		}
		if (tutScene == 10 || tutScene ==   11  || tutScene ==   12 ) {
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix, 1.0f, "1-2-1 pattern");
			if (tutScene ==   10 || tutScene ==   11 || tutScene == 12) {
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 1.0f, textScale, "- The 1-2-1 pattern has one solution");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 2.0f, textScale, "its just a combination of two 1-2");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 3.0f, textScale, "patterns");
			}
			if (tutScene ==   10 || tutScene ==   11 || tutScene == 12) DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 4.0f, textScale, "- 1-2 pattern from the left");
			if (tutScene ==   11 || tutScene == 12) 	DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 5.0f, textScale, "- 1-2 pattern from the right");
			if (tutScene ==   12) DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 6.0f, textScale, "- Here is the final result");
			
			LPDIRECT3DTEXTURE9 pTopTex = (tutScene ==   10) ? g_ptutorial_patterns_121scene1Texture :
									 (tutScene ==   11) ? g_ptutorial_patterns_121scene2Texture :
													   g_ptutorial_patterns_121scene3Texture;
			g_pd3dDevice->SetTexture(0, pTopTex);
			drawMaShit(8.0f, 4.0f,centerX, posY - 30.0f* Y1pix - lineStep * 7.0f);
		}
		if (tutScene == 13 || tutScene ==   14  || tutScene ==   15 ) {
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix, 1.0f, "1-2-2-1 pattern");
			
			if (tutScene ==   13 || tutScene ==   14 || tutScene == 15) {
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 1.0f, textScale, "- The 1-2-2-1 pattern has one solution");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 2.0f, textScale, "its also just a combination of two 1-2");
				DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 3.0f, textScale, "patterns");
			}
			if (tutScene ==   13 || tutScene ==   14 || tutScene == 15) DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 4.0f, textScale, "- 1-2 pattern from the left");
			if (tutScene ==   14 || tutScene == 15) 	DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 5.0f, textScale, "- 1-2 pattern from the right");
			if (tutScene ==   15) DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 6.0f, textScale, "- Here is the final result");
			LPDIRECT3DTEXTURE9 pTopTex = (tutScene ==   13) ? g_ptutorial_patterns_1221scene1Texture :
									 (tutScene ==   14) ? g_ptutorial_patterns_1221scene2Texture :
													   g_ptutorial_patterns_1221scene3Texture;
			g_pd3dDevice->SetTexture(0, pTopTex);
			drawMaShit(8.0f, 4.0f,centerX, posY - 30.0f* Y1pix - lineStep * 7.0f);
		}
		if (tutScene == 16) {
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix, textScale, "Well that are all the patterns I prepared.");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 1.0f, textScale, "if you want to study more visit:");
			DrawTexturedString(textAreaX- 10.0f * X1pix, posY + 60.0f* Y1pix - lineStep * 2.0f, 0.6f, "minesweeper.online/help/patterns");
		}
		DrawHint(0.0f, 20.0f * Y1pix, buttonBACK_Mask, "Back");

	}

		
    //-----------------------------------------------
    // STATISTICS PAGE
    //-----------------------------------------------
    if (gameRunning == STATISTICS) {
        const MSModeStats& beg  = g_SaveManager.GetBeginnerStats();
        const MSModeStats& mid  = g_SaveManager.GetIntermediateStats();
        const MSModeStats& xpt  = g_SaveManager.GetExpertStats();
		const MSModeStats& cus  = g_SaveManager.GetCustomStats();
        const MSTotals&    tot  = g_SaveManager.GetTotals();

        // scene 1 = Total + Beginner
        // scene 2 = Intermediate + Expert

        const float colKey    = centerX - (160.0f * X1pix);
        const float colVal    = centerX + (160.0f * X1pix);
        const float lineStep  = 30.0f * Y1pix;
        const float blockStep = 45.0f * Y1pix;
        const float scaleMode = 0.9f;
        const float scaleStat = 0.8f;

        const float startY    = centerY + (145.0f * Y1pix) - 0.5f * Y1pix;

        char buf[32];

        #define STAT_LINE(posY, label, valStr) \
            DrawTexturedString(colKey, (posY), scaleStat, (label)); \
            DrawTexturedString(colVal - GetTextWidth((valStr), scaleStat), (posY), scaleStat, (valStr));

        #define FMT_BEST(fbt, buf) \
            if ((fbt) >= 0.0f) { \
                int _s = (int)(fbt); int _f = (int)(((fbt) - _s) * 10.0f); \
                sprintf((buf), "%d.%ds", _s, _f); \
            } else { sprintf((buf), "-"); }

        #define FMT_WINPCT(won, played, buf) \
            if ((played) > 0) sprintf((buf), "%d%%", (int)(100.0f * (won) / (played))); \
            else              sprintf((buf), "-");
		if (statsScene != 1){
			g_pd3dDevice->SetTexture(0, g_parrowUpTexture);
			drawMaShit(1.0f, 1.0f,176.0f *X1pix,155.3125f * Y1pix);
		}
		if (statsScene != 3){
			g_pd3dDevice->SetTexture(0, g_parrowTexture);
			drawMaShit(1.0f, 1.0f,176.0f *X1pix,- 360.0f *X1pix);
		}
        if (statsScene == 1) {
            float posY = startY;

            // --- Total ---
            // Combined best times: sum of all non-negative best times
            float combinedBest = 0.0f;
            bool  anyBest      = false;
            if (beg.fBestTime >= 0.0f) { combinedBest += beg.fBestTime; anyBest = true; }
            if (mid.fBestTime >= 0.0f) { combinedBest += mid.fBestTime; anyBest = true; }
            if (xpt.fBestTime >= 0.0f) { combinedBest += xpt.fBestTime; anyBest = true; }

            DrawTexturedString(colKey, posY, scaleMode, "Total:");
            posY -= lineStep;

            if (anyBest) {
                int _s = (int)combinedBest; int _f = (int)((combinedBest - _s) * 10.0f);
                sprintf(buf, "%d.%ds", _s, _f);
            } else { sprintf(buf, "-"); }
            STAT_LINE(posY, "  Best times:", buf); posY -= lineStep;

            sprintf(buf, "%d", (int)tot.dwTotalGamesPlayed);
            STAT_LINE(posY, "  Games played:", buf); posY -= lineStep;

            sprintf(buf, "%d", (int)tot.dwTotalGamesWon);
            STAT_LINE(posY, "  Games won:", buf);    posY -= lineStep;

            sprintf(buf, "%d", (int)tot.dwTotalGamesLost);
            STAT_LINE(posY, "  Games lost:", buf);   posY -= lineStep;

            FMT_WINPCT(tot.dwTotalGamesWon, tot.dwTotalGamesPlayed, buf);
            STAT_LINE(posY, "  Win%:", buf);
            posY -= blockStep;

            // --- Beginner ---
            DrawTexturedString(colKey, posY, scaleMode, "Beginner:");
			posY -= lineStep;

            FMT_BEST(beg.fBestTime, buf);
            STAT_LINE(posY, "  Best time:", buf);    posY -= lineStep;

            sprintf(buf, "%d", (int)beg.dwGamesPlayed);
            STAT_LINE(posY, "  Games played:", buf); posY -= lineStep;

            sprintf(buf, "%d", (int)beg.dwGamesWon);
            STAT_LINE(posY, "  Games won:", buf);    posY -= lineStep;

            sprintf(buf, "%d", (int)beg.dwGamesLost);
            STAT_LINE(posY, "  Games lost:", buf);   posY -= lineStep;

            FMT_WINPCT(beg.dwGamesWon, beg.dwGamesPlayed, buf);
            STAT_LINE(posY, "  Win%:", buf);
        }
        else if (statsScene == 2) {

            float posY = startY;

            // --- Intermediate ---
            DrawTexturedString(colKey, posY, scaleMode, "Intermediate:");
            posY -= lineStep;

            FMT_BEST(mid.fBestTime, buf);
            STAT_LINE(posY, "  Best time:", buf);    posY -= lineStep;

            sprintf(buf, "%d", (int)mid.dwGamesPlayed);
            STAT_LINE(posY, "  Games played:", buf); posY -= lineStep;

            sprintf(buf, "%d", (int)mid.dwGamesWon);
            STAT_LINE(posY, "  Games won:", buf);    posY -= lineStep;

            sprintf(buf, "%d", (int)mid.dwGamesLost);
            STAT_LINE(posY, "  Games lost:", buf);   posY -= lineStep;

            FMT_WINPCT(mid.dwGamesWon, mid.dwGamesPlayed, buf);
            STAT_LINE(posY, "  Win%:", buf);
            posY -= blockStep;

            // --- Expert --- 
            DrawTexturedString(colKey, posY, scaleMode, "Expert:");
			posY -= lineStep;

            FMT_BEST(xpt.fBestTime, buf);
            STAT_LINE(posY, "  Best time:", buf);    posY -= lineStep;

            sprintf(buf, "%d", (int)xpt.dwGamesPlayed);
            STAT_LINE(posY, "  Games played:", buf); posY -= lineStep;

            sprintf(buf, "%d", (int)xpt.dwGamesWon);
            STAT_LINE(posY, "  Games won:", buf);    posY -= lineStep;

            sprintf(buf, "%d", (int)xpt.dwGamesLost);
            STAT_LINE(posY, "  Games lost:", buf);   posY -= lineStep;

            FMT_WINPCT(xpt.dwGamesWon, xpt.dwGamesPlayed, buf);
            STAT_LINE(posY, "  Win%:", buf);
        }
		else if (statsScene == 3) {
			float posY = startY;

            // --- Custom ---
            DrawTexturedString(colKey, posY, scaleMode, "Custom:");
            posY -= lineStep;

            FMT_BEST(cus.fBestTime, buf);
            STAT_LINE(posY, "  Best time:", buf);    posY -= lineStep;

            sprintf(buf, "%d", (int)cus.dwGamesPlayed);
            STAT_LINE(posY, "  Games played:", buf); posY -= lineStep;

            sprintf(buf, "%d", (int)cus.dwGamesWon);
            STAT_LINE(posY, "  Games won:", buf);    posY -= lineStep;

            sprintf(buf, "%d", (int)cus.dwGamesLost);
            STAT_LINE(posY, "  Games lost:", buf);   posY -= lineStep;

            FMT_WINPCT(cus.dwGamesWon, cus.dwGamesPlayed, buf);
            STAT_LINE(posY, "  Win%:", buf);
            posY -= blockStep;
		}
		
        #undef STAT_LINE
        #undef FMT_BEST
        #undef FMT_WINPCT
		
		DrawHint(0.0f, 0.0f, buttonBACK_Mask,   "Back");
    }
	 //-----------------------------------------------
    // CREDITS PAGE
    //-----------------------------------------------
    if (gameRunning == CREDITS) {
		float posX = centerX - ((43.0f * X1pix) / 2.0f) - 40.0f * X1pix;
		float posY = centerY + 80.0f * Y1pix;
		const float textAreaX = centerX - (170.0f * X1pix);
		const float textAreaY = centerY - (20.0f * Y1pix);
		const float lineStep  = 24.0f * Y1pix;
		const float textScale = 0.6f;
		const float offset    = 15.0f * X1pix;
		if (tutScene == 0) {
			DrawTexturedString(textAreaX- offset, posY + 60.0f* Y1pix, 0.9f, "Made by:");
			DrawTexturedString(textAreaX- offset, posY + 60.0f* Y1pix - lineStep, textScale, "Wojtan");
			DrawTexturedString(textAreaX- offset, posY + 55.0f* Y1pix - lineStep * 2.0f, textScale, "All credits are on the game's github:");
			DrawTexturedString(textAreaX- offset, posY + 55.0f* Y1pix - lineStep * 3.0f, textScale, "github.com/XenonWojtan/360sweeper");
			DrawTexturedString(textAreaX- offset, posY + 55.0f* Y1pix - lineStep * 4.0f, textScale, "(future updates there too)");
		}

		else if (tutScene == 1 )DrawTexturedString(textAreaX- offset, posY + 60.0f* Y1pix, textScale, "meow :3 - some silly cat");
		DrawHint(0.0f, 0.0f, buttonBACK_Mask,   "Back");
			
	}
    //-----------------------------------------------
    // SETTINGS PAGE
    //-----------------------------------------------
    if (gameRunning == SETTINGS) {
        float        posX, posY, posXtext, posYtext, textWidth, scale;
        float        a        = 2.0f;
        bool         checkboxtriggered = false;
        int          i;
        D3DXMATRIX   matWorld, matScale, matTrans;

        posX   = centerX - (190.0f * X1pix);
        posY   = centerY + (155.0f * Y1pix);
        scaleY = 2.0f;
        scaleX = 2.0f;

        if (scene == 1) {
            static const char* checkLabels[3] = {
                "Mute sounds", "Chording disabled", "Vibrations enabled"
            };

            for (i = 0; i < 3; i++) {
                posY = centerY + (155.0f * Y1pix)
                     - (i * 33.0f * Y1pix) - (i * 20.0f * Y1pix) - 10.0f * Y1pix;

                if      (i == 0) checkboxtriggered = g_IsMuted;
                else if (i == 1) checkboxtriggered = !chordingEnabled;
                else             checkboxtriggered = g_VibrationsEnabled;

                posXtext = posX + (53.0f * X1pix);
                posYtext = posY - 12.5f * Y1pix;
                DrawTexturedString(posXtext, posYtext, 0.8f, checkLabels[i]);

                g_pd3dDevice->SetTexture(0, getCheckboxtexture(i, checkboxtriggered));
                drawMaShit(scaleX, scaleY, posX, posY);
            }

            // Volume slider
            posX  = centerX - (123.0f * X1pix) / 2.0f;
            posY  = centerY + (155.0f * Y1pix)
                  - (3.0f * 33.0f * Y1pix) - (3.0f * 20.0f * Y1pix) - 20.0f * Y1pix;
            scaleY = 1.0f * a;
            scaleX = 8.0f * a;
            g_pd3dDevice->SetTexture(0, g_pbarTexture);
            drawMaShit(scaleX, scaleY, posX, posY);

            scaleY = 1.0f * a;
            scaleX = 1.0f * a;
            posY  -= 3.0f * Y1pix;
            posX   = percentToPixelsSlider(volumepercentage / 2.0f);
            g_pd3dDevice->SetTexture(0, (buttonMenu == 4) ? g_psliderselectedTexture : g_psliderTexture);
            drawMaShit(scaleX, scaleY, posX, posY);

            {
                char volLabel[32];
                sprintf(volLabel, "Volume:%d", volumepercentage);

                scale     = 0.9f;
                textWidth = GetTextWidth(volLabel, scale);
                posXtext  = centerX - (textWidth / 2.0f) + 10.0f * X1pix;
                posYtext  = posY - 5.0f * Y1pix;
                DrawTexturedString(posXtext, posYtext, scale, volLabel);
            }

            // Vibration strength slider
            posX  = centerX - (123.0f * X1pix) / 2.0f;
            posY  = centerY + (155.0f * Y1pix)
                  - (4.0f * 33.0f * Y1pix) - (4.0f * 20.0f * Y1pix)
                  - 13.0f * Y1pix - 20.0f * Y1pix;
            scaleY = 1.0f * a;
            scaleX = 8.0f * a;
            g_pd3dDevice->SetTexture(0, g_pbarTexture);
            drawMaShit(scaleX, scaleY, posX, posY);

            scaleY = 1.0f * a;
            scaleX = 1.0f * a;
            posY  -= 3.0f * Y1pix;
            posX   = percentToPixelsSlider(g_VibrationStrengthPercent / 1.0f);
            g_pd3dDevice->SetTexture(0, (buttonMenu == 5) ? g_psliderselectedTexture : g_psliderTexture);
            drawMaShit(scaleX, scaleY, posX, posY);

            {
                char vibLabel[48];
                sprintf(vibLabel, "Vibration strength:%d", g_VibrationStrengthPercent);

                scale     = 0.85f;
                textWidth = GetTextWidth(vibLabel, scale);
                posXtext  = centerX - (textWidth / 2.0f) + 10.0f * X1pix;
                posYtext  = posY - 5.0f * Y1pix;
                DrawTexturedString(posXtext, posYtext, scale, vibLabel);
            }

            // Move cooldown slider
            posX  = centerX - (123.0f * X1pix) / 2.0f;
            posY  = centerY + (155.0f * Y1pix)
                  - (5.0f * 33.0f * Y1pix) - (5.0f * 20.0f * Y1pix)
                  - 13.0f * Y1pix * 2.0f - 20.0f * Y1pix;
            scaleY = 1.0f * a;
            scaleX = 8.0f * a;
            g_pd3dDevice->SetTexture(0, g_pbarTexture);
            drawMaShit(scaleX, scaleY, posX, posY);

            scaleY = 1.0f * a;
            scaleX = 1.0f * a;
            posY  -= 4.0f * Y1pix;
            posX   = percentToPixelsSlider(GetPercentage(movecooldownms / 1.0f, 50.0f, 250.0f));
            g_pd3dDevice->SetTexture(0, (buttonMenu == 6) ? g_psliderselectedTexture : g_psliderTexture);
            drawMaShit(scaleX, scaleY, posX, posY);

            {
                char msLabel[40];
                sprintf(msLabel, "Move cooldown:%d", movecooldownms);

                scale     = 0.85f;
                textWidth = GetTextWidth(msLabel, scale);
                posXtext  = centerX - (textWidth / 2.0f) + 10.0f * X1pix;
                posYtext  = posY - 5.0f * Y1pix;
                DrawTexturedString(posXtext, posYtext, scale, msLabel);
            }
			g_pd3dDevice->SetTexture(0, g_parrowTexture);
            drawMaShit(1.0f, 1.0f,centerX + 158.5f *X1pix,- 360.0f *X1pix);

        }
        else if (scene == 2) {
            float posX2, posY2, posXtext2, posYtext2, textWidth2, scale2;

            // Hold time slider
            posX2  = centerX - (123.0f * X1pix) / 2.0f;
            posY2  = centerY + (155.0f * Y1pix) - (20.0f * Y1pix);
            scaleY = 1.0f * a;
            scaleX = 8.0f * a;
            g_pd3dDevice->SetTexture(0, g_pbarTexture);
            drawMaShit(scaleX, scaleY, posX2, posY2-25.0f*Y1pix);
			g_pd3dDevice->SetTexture(0, g_parrowUpTexture);
            drawMaShit(1.0f, 1.0f,158.5f *X1pix,155.3125f * Y1pix);
            scaleY = 1.0f * a;
            scaleX = 1.0f * a;
            posY2 -= 4.0f * Y1pix;
            posX2  = percentToPixelsSlider(GetPercentage(holdTime / 1.0f, 150.0f, 300.0f));
            g_pd3dDevice->SetTexture(0, (buttonMenu == 7) ? g_psliderselectedTexture : g_psliderTexture);
            drawMaShit(scaleX, scaleY, posX2, posY2-25.0f*Y1pix);

            {
                char prevLabel[48];
                sprintf(prevLabel, "Hold time to preview:%d", holdTime);

                scale2     = 0.8f;
                textWidth2 = GetTextWidth(prevLabel, scale2);
                posXtext2  = centerX - (textWidth2 / 2.0f) + 10.0f * X1pix;
                posYtext2  = posY2 - 5.0f * Y1pix;
                DrawTexturedString(posXtext2, posYtext2-25.0f*Y1pix, scale2, prevLabel);
            }

            // Texture pack button
            scaleY = 2.0f;
            scaleX = 16.0f;
            posY2  = centerY + (155.0f * Y1pix) - (110.0f * Y1pix)+ 15.0f * Y1pix;
            posX2  = centerX - ((162.0f * X1pix) / 2.0f);
            g_pd3dDevice->SetTexture(0, (buttonMenu == 8) ? g_pbuttonmenuselectedTexture : g_pbuttonmenuTexture);
            drawMaShit(scaleX, scaleY, posX2, posY2-25.0f*Y1pix);

            {
                const char* packName = packList[selectedPackIndex].c_str();
                scale2     = 0.6f;
                textWidth2 = GetTextWidth("Texture pack:", scale2);
                posXtext2  = posX2 - (textWidth2 / 2.0f);
                DrawTexturedString(posXtext2, posY2-25.0f*Y1pix, scale2, "Texture pack:");
                posXtext2 += textWidth2;
                DrawTexturedString(posXtext2, posY2-25.0f*Y1pix, scale2, packName);
            }

            // Reset to default button
            scaleY = 2.0f;
            scaleX = 16.0f;
            posY2  = centerY + (155.0f * Y1pix) - (180.0f * Y1pix) + 15.0f * Y1pix;
            posX2  = centerX - ((162.0f * X1pix) / 2.0f);
            g_pd3dDevice->SetTexture(0, (buttonMenu == 9) ? g_pbuttonmenuselectedTexture : g_pbuttonmenuTexture);
            drawMaShit(scaleX, scaleY, posX2, posY2-25.0f*Y1pix);

            scale2     = 0.85f;
            textWidth2 = GetTextWidth("Reset to default", scale2);
            posXtext2  = centerX - (textWidth2 / 2.0f) + 10.0f * X1pix;
            DrawTexturedString(posXtext2, posY2-25.0f*Y1pix, scale2, "Reset to default");

			scaleY = 2.0f;
            scaleX = 16.0f;
            posY2  = centerY + (155.0f * Y1pix) - (250.0f * Y1pix)+ 15.0f * Y1pix;
            posX2  = centerX - ((162.0f * X1pix) / 2.0f);
            g_pd3dDevice->SetTexture(0, (buttonMenu == 10) ? g_pbuttonmenuselectedTexture : g_pbuttonmenuTexture);
            drawMaShit(scaleX, scaleY, posX2, posY2-25.0f*Y1pix);

            scale2     = 0.85f;
            textWidth2 = GetTextWidth("Reset controls", scale2);
            posXtext2  = centerX - (textWidth2 / 2.0f) + 10.0f * X1pix;
            DrawTexturedString(posXtext2, posY2-25.0f*Y1pix, scale2, "Reset controls");

			scaleY = 2.0f;
            scaleX = 16.0f;
            posY2  = centerY + (155.0f * Y1pix) - (320.0f * Y1pix)+ 15.0f * Y1pix;
            posX2  = centerX - ((162.0f * X1pix) / 2.0f);
            g_pd3dDevice->SetTexture(0, (buttonMenu == 11) ? g_pbuttonmenuselectedTexture : g_pbuttonmenuTexture);
            drawMaShit(scaleX, scaleY, posX2, posY2-25.0f*Y1pix);

            scale2     = 0.85f;
            textWidth2 = GetTextWidth("Restart game", scale2);
            posXtext2  = centerX - (textWidth2 / 2.0f) + 10.0f * X1pix;
            DrawTexturedString(posXtext2, posY2-25.0f*Y1pix, scale2, "Restart game");
        }
		DrawHint(-20.0f * X1pix, 0.0f, buttonA_Mask, "Select");
		DrawHint(-120.0f * X1pix, 0.0f, buttonBACK_Mask,   "Back");
    }
	//-----------------------------------------------
	// CONTROLS PAGE
	//-----------------------------------------------
	if (gameRunning == CONTROLS) {
		float      posX, posY, posXtext, posYtext;
		float      a = 2.0f;
		bool       checkboxtriggered = false;
		int        i;
		D3DXMATRIX matWorld, matScale, matTrans;

		static const char* ctrlLabels[3] = {
			"Dpad movement", "Left analog movement", "Right analog movement"
		};

		posX   = centerX - (190.0f * X1pix);
		posY   = centerY + (155.0f * Y1pix);
		scaleY = 2.0f;
		scaleX = 2.0f;

		for (i = 0; i < 2; i++) {
			posY = centerY + (155.0f * Y1pix)
				 - (i * 33.0f * Y1pix) - (i * 20.0f * Y1pix) - 10.0f * Y1pix;

			checkboxtriggered = (i == 0) ? dpad : true;

			posXtext = posX + (53.0f * X1pix) - 7.0f * X1pix;
			posYtext = posY - 12.5f * Y1pix;
			int U = i;
			if (i == 0) posXtext += 2.0f * X1pix;
			if (i == 1 && !leftjoystick) U = 2;
			DrawTexturedString(posXtext, posYtext, 0.8f, ctrlLabels[U]);

			g_pd3dDevice->SetTexture(0, getCheckboxtexture(i, checkboxtriggered));
			drawMaShit(scaleX, scaleY, posX, posY);
		}

		// --- Button assignment rows ---
		struct ActionRow {
			const char* label;
			WORD        mask;
		};

		ActionRow actionRows[6] = {
			{ "Open tile",   buttonOPEN_Mask  },
			{ "Flag tile",   buttonFLAG_Mask  },
			{ "Back",        buttonBACK_Mask  },
			{ "Reset",       buttonRESET_Mask },
			{ "Confirm",     buttonA_Mask     },
			{ "Board menu",  buttonX_Mask     }
		};
		//espaniol hola
		const float rowStartY  = centerY + (155.0f * Y1pix) - (115.0f * Y1pix);
		const float rowStepY   = 45.0f * Y1pix;
		const float iconScale  = 1.0f;
		const float labelScale = 0.8f;
		const float bgScale    = 1.3f;
		const float labelColX = centerX + (30.0f  * X1pix);
		const float iconColX  = centerX - (178.0f * X1pix);
		const float nameColX  = iconColX + (40.0f * X1pix);

		for (i = 0; i < 6; i++) {
			float rowY    = rowStartY - (i * rowStepY);
			float textOffY = rowY - 12.0f * Y1pix;

			// Action label
			DrawTexturedString(labelColX, textOffY, labelScale, actionRows[i].label);

			
			if (buttonMenu == i+3 && isListening) {
				LPDIRECT3DTEXTURE9 pBG = g_picons_tileChoosingTextTexture;
				g_pd3dDevice->SetTexture(0, pBG);
				drawMaShit(bgScale, bgScale, iconColX, rowY - 10.0f * Y1pix);
			}
			else if (buttonMenu == i+3) {
				LPDIRECT3DTEXTURE9 pBG = g_picons_tileControlsTexture;
				g_pd3dDevice->SetTexture(0, pBG);
				drawMaShit(bgScale, bgScale, iconColX, rowY - 10.0f * Y1pix);
			}
			// Button icon
			std::string rawName  = ButtonMaskToString(actionRows[i].mask);
			std::string btnName  = Sanitize(rawName);
			LPDIRECT3DTEXTURE9 pIcon = StringToButtonIcon(btnName);
			if (pIcon != NULL && !(isListening && buttonMenu == i + 3)) {
				bool isABXY      = (btnName == "A" || btnName == "B" || btnName == "X" || btnName == "Y");
				bool isLTRT      = (btnName == "LT" || btnName == "RT");
				bool isStartBack = (btnName == "START" || btnName == "BACK");
				bool isDPAD      = (btnName == "DPAD_UP" || btnName == "DPAD_DOWN" || btnName == "DPAD_LEFT" || btnName == "DPAD_RIGHT");
				bool isThumb     = (btnName == "L_THUMB" || btnName == "R_THUMB");
				bool isBumper    = (btnName == "LB" || btnName == "RB");
				float btnScale = isABXY ? iconScale : isThumb ? 1.5f : isBumper ? 1.45f : 1.85f;
				if (isStartBack) btnScale -= 0.1f;
				float btnX = iconColX - (isLTRT ? 16.0f * X1pix : 0.0f) - (isStartBack ? 2.0f * X1pix : 0.0f) - (isDPAD ? 12.0f * X1pix : 0.0f);
				float btnY = rowY - 10.0f * Y1pix + (isLTRT ? 12.0f * Y1pix : 0.0f) + (isDPAD ? 12.0f * Y1pix : 0.0f) + (isThumb ? 2.0f * Y1pix : 0.0f);
				g_pd3dDevice->SetTexture(0, pIcon);
				drawMaShit(btnScale, btnScale, btnX, btnY);
			}

			// Button name text
			DrawTexturedString(nameColX, textOffY, labelScale, rawName);
		}
		if (isListening) DrawHint(-20.0f * X1pix, 0.0f, NULL, "Remap");
		
		else {
			DrawHint(-120.0f * X1pix, 0.0f, buttonBACK_Mask,   "Back");
			DrawHint(-20.0f * X1pix, 0.0f, buttonA_Mask, "Select");
		}
	}

    //-----------------------------------------------
    // INGAME
    //-----------------------------------------------
    if (gameRunning == INGAME) {

        IDirect3DTexture9* tileNumTex[9] = {
            g_ptile0Texture, g_ptile1Texture, g_ptile2Texture,
            g_ptile3Texture, g_ptile4Texture, g_ptile5Texture,
            g_ptile6Texture, g_ptile7Texture, g_ptile8Texture
        };

        IDirect3DTexture9* pLastTex = NULL;

        // PASS 1: Tiles
        {
            int r, c;
            for (r = 0; r < tileCountRow; r++) {
                for (c = 0; c < tileCountCol; c++) {
                    const cell&        curCell = board.getCell(r, c);
                    IDirect3DTexture9* pTex    = NULL;
                    float              posX    = startX + (c * tileWidthNDC);
                    float              posY    = startY - (r * tileHeightNDC);
                    D3DXMATRIX         matWorld;

                    if (curCell.isFlagged && !curCell.isMine && (gameOver == -1 || gameOver == 1)) {
                        pTex = g_pflagMissplaceTexture;
                    } else if (curCell.isFlagged) {
                        pTex = g_pflagTexture;
                    } else if (!curCell.isUncovered) {
                        pTex = g_ptileTexture;
                    } else if (!curCell.isMine) {
                        pTex = tileNumTex[curCell.minesAround];
                    } else if (curCell.isBoom) {
                        pTex = g_pmineRedTexture;
                    } else if (gameOver == 1) {
                        pTex = g_pflagTexture;
                    } else {
                        pTex = g_pmineTexture;
                    }

                    if (pTex != pLastTex) {
                        g_pd3dDevice->SetTexture(0, pTex);
                        pLastTex = pTex;
                    }

                    D3DXMatrixTranslation(&matWorld, posX, posY, 0.0f);
                    g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                    g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

                    if (r == selRow && c == selCol) {
                        g_pd3dDevice->SetTexture(0, g_phighlightTexture);
                        g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                        g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
                        // restore cache -- next tile must re-set its texture
                        pLastTex = NULL;
                    }
                }
            }
        }

        // PASS 2: Borders
        {
            D3DXMATRIX matWorld;
            float      posX, posY, adjPosY;
            int        c, r;

            pLastTex = NULL;

            // Top edge
			g_pd3dDevice->SetTexture(0, g_pBorders_topTexture);
			pLastTex = g_pBorders_topTexture;
			scaleY = 4.0f;
			for (c = 1; c < tileCountCol - 1; c++) {
				posX    = startX + (c * tileWidthNDC);
				posY    = startY + tileHeightNDC + (1.5f * tileHeightNDC);
				drawMaShit(scaleX, scaleY, posX, posY);
			}

            // Bottom edge
            g_pd3dDevice->SetTexture(0, g_pBorders_bottomTexture);
            pLastTex = g_pBorders_bottomTexture;
            for (c = 1; c < tileCountCol - 1; c++) {
                posX = startX + (c * tileWidthNDC);
                posY = startY - (tileCountRow * tileHeightNDC);
                D3DXMatrixTranslation(&matWorld, posX, posY, 0.0f);
                g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
            }

            // Left edge
            g_pd3dDevice->SetTexture(0, g_pBorders_leftTexture);
            pLastTex = g_pBorders_leftTexture;
            for (r = 1; r < tileCountRow - 1; r++) {
                posX = startX - tileWidthNDC;
                posY = startY - (r * tileHeightNDC);
                D3DXMatrixTranslation(&matWorld, posX, posY, 0.0f);
                g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
            }

            // Right edge
            g_pd3dDevice->SetTexture(0, g_pBorders_rightTexture);
            pLastTex = g_pBorders_rightTexture;
            for (r = 1; r < tileCountRow - 1; r++) {
                posX = startX + (tileCountCol * tileWidthNDC);
                posY = startY - (r * tileHeightNDC);
                D3DXMatrixTranslation(&matWorld, posX, posY, 0.0f);
                g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
            }

            // Top-left corner
            {
                D3DXMATRIX matScale, matTrans;
                scaleY  = 4.0f;
                posX    = startX - tileWidthNDC;
                posY    = startY + tileHeightNDC;
                adjPosY = posY + (1.5f * tileHeightNDC);

                g_pd3dDevice->SetTexture(0, g_pBorders_leftTopCornerTexture);
                drawMaShit(scaleX, scaleY, posX, adjPosY);

                posX    = startX;
                adjPosY = (startY + tileHeightNDC) + (1.5f * tileHeightNDC);
                D3DXMatrixScaling(&matScale, 1.0f, 4.0f, 1.0f);
                D3DXMatrixTranslation(&matTrans, posX, adjPosY, 0.0f);
                D3DXMatrixMultiply(&matWorld, &matScale, &matTrans);
                g_pd3dDevice->SetTexture(0, g_pBorders_topTexture);
                g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

                g_pd3dDevice->SetTexture(0, g_pBorders_leftTexture);
                posX = startX - tileWidthNDC;
                posY = startY;
                D3DXMatrixTranslation(&matWorld, posX, posY, 0.0f);
                g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
            }

            // Top-right corner
            {
                int lastC = tileCountCol - 1;
                scaleY    = 4.0f;
                posX      = startX + ((lastC + 1) * tileWidthNDC);
                posY      = startY + tileHeightNDC;
                adjPosY   = posY + (1.5f * tileHeightNDC);

                g_pd3dDevice->SetTexture(0, g_pBorders_rightTopCornerTexture);
                drawMaShit(scaleX, scaleY, posX, adjPosY);

                g_pd3dDevice->SetTexture(0, g_pBorders_topTexture);
                posX    = startX + (lastC * tileWidthNDC);
                adjPosY = (startY + tileHeightNDC) + (1.5f * tileHeightNDC);
                drawMaShit(scaleX, scaleY, posX, adjPosY);

                g_pd3dDevice->SetTexture(0, g_pBorders_rightTexture);
                posX = startX + ((lastC + 1) * tileWidthNDC);
                posY = startY;
                D3DXMatrixTranslation(&matWorld, posX, posY, 0.0f);
                g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
            }

            // Bottom-left corner
            {
                int lastR = tileCountRow - 1;
                posX = startX - tileWidthNDC;
                posY = startY - ((lastR + 1) * tileHeightNDC);

                g_pd3dDevice->SetTexture(0, g_pBorders_leftBottomCornerTexture);
                D3DXMatrixTranslation(&matWorld, posX, posY, 0.0f);
                g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

                g_pd3dDevice->SetTexture(0, g_pBorders_bottomTexture);
                posX = startX;
                D3DXMatrixTranslation(&matWorld, posX, posY, 0.0f);
                g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

                g_pd3dDevice->SetTexture(0, g_pBorders_leftTexture);
                posX = startX - tileWidthNDC;
                posY = startY - (lastR * tileHeightNDC);
                D3DXMatrixTranslation(&matWorld, posX, posY, 0.0f);
                g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
            }

            // Bottom-right corner
            {
                int lastR = tileCountRow - 1;
                int lastC = tileCountCol - 1;
                posX = startX + ((lastC + 1) * tileWidthNDC);
                posY = startY - ((lastR + 1) * tileHeightNDC);

                g_pd3dDevice->SetTexture(0, g_pBorders_rightBottomCornerTexture);
                D3DXMatrixTranslation(&matWorld, posX, posY, 0.0f);
                g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

                g_pd3dDevice->SetTexture(0, g_pBorders_bottomTexture);
                posX = startX + (lastC * tileWidthNDC);
                D3DXMatrixTranslation(&matWorld, posX, posY, 0.0f);
                g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

                g_pd3dDevice->SetTexture(0, g_pBorders_rightTexture);
                posX = startX + ((lastC + 1) * tileWidthNDC);
                posY = startY - (lastR * tileHeightNDC);
                D3DXMatrixTranslation(&matWorld, posX, posY, 0.0f);
                g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
            }
        } // end border block

        // PASS 3: UI elements
        if (tileCountCol > 3) {
			RenderCounter(flagLeft, startX + 5.0f * X1pix, startY + tileHeightNDC, tileWidthNDC, tileHeightNDC, false);
			g_pd3dDevice->SetTexture(0, g_pbordercounterTexture);
			drawMaShit(4.0f, 2.0f, startX + 3.0f * X1pix + 5.0f * X1pix, startY + tileHeightNDC+92.0f/2.0f * Y1pix);
		}

        // Smiley :)
		if (tileCountCol >= 8) {
        
            D3DXMATRIX         matScale, matTrans, matTextureOffset, matFinal;
            IDirect3DTexture9* pSmiley;
            float              digitPosX   = startX + (0.5f * totalGridWidth - Xoffset24pxNDC) + 4.0f * X1pix;
            float              counterPosY = startY + (2.35f * tileHeightNDC) - 1.0f * Y1pix;

            D3DXMatrixScaling(&matScale, 2.0f, 2.0f, 1.0f);
            D3DXMatrixTranslation(&matTextureOffset, -0.1f * tileWidthNDC, 0.1f * tileHeightNDC, 0.0f);
            D3DXMatrixTranslation(&matTrans, digitPosX, counterPosY, 0.0f);
            D3DXMatrixMultiply(&matFinal, &matScale, &matTextureOffset);
            D3DXMatrixMultiply(&matFinal, &matFinal, &matTrans);

            if (pressingStart)       pSmiley = g_psmileyClickedTexture;
            else if (gameOver == 1)  pSmiley = g_psmileyWinTexture;
            else if (gameOver == -1) pSmiley = g_psmileyLossTexture;
            else if (openingTile)    pSmiley = g_psmileyOpeningTexture;
            else                     pSmiley = g_psmileyDEFTexture;

            g_pd3dDevice->SetTexture(0, pSmiley);
            g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matFinal, 4);
            g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
        
		}

        // Timer
        if (tileCountCol > 5) {
			int time = myGameTimer.GetTimeAsInt();
			RenderTimer(time, startX + ((tileCountCol - 1) * tileWidthNDC - 5.0f * X1pix),
						startY + tileHeightNDC, tileWidthNDC, tileHeightNDC);
			g_pd3dDevice->SetTexture(0, g_pbordercounterTexture);
			drawMaShit(4.0f, 2.0f, startX + ((tileCountCol - 1) * tileWidthNDC) + 3.0f * X1pix - 52*X1pix - 5.0f * X1pix, startY + tileHeightNDC+92.0f/2.0f * Y1pix);
		}

        // Preview highlight
        if (previewRender && gameOver == 0) {
            const cell& selCell = board.getCell(selRow, selCol);
            D3DXMATRIX  matWorld;
            float       posX, posY;

            if (!selCell.isFlagged) {
                if (!selCell.isUncovered) {
                    posX = startX + (selCol * tileWidthNDC);
                    posY = startY - (selRow * tileHeightNDC);
                    D3DXMatrixTranslation(&matWorld, posX, posY, 0.0f);

                    g_pd3dDevice->SetTexture(0, g_ptile0Texture);
                    g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                    g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

                    g_pd3dDevice->SetTexture(0, g_phighlightTexture);
                    g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                    g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
                }
                else {
                    int deltaR, deltaC;
                    g_pd3dDevice->SetTexture(0, g_ptile0Texture);
                    for (deltaR = -1; deltaR <= 1; deltaR++) {
                        for (deltaC = -1; deltaC <= 1; deltaC++) {
                            int nRow = selRow + deltaR;
                            int nCol = selCol + deltaC;
                            if (deltaR == 0 && deltaC == 0) continue;
                            if (board.isValid(nRow, nCol)) {
                                const cell& nbr = board.getCell(nRow, nCol);
                                if (!nbr.isUncovered && !nbr.isFlagged) {
                                    posX = startX + (nCol * tileWidthNDC);
                                    posY = startY - (nRow * tileHeightNDC);
                                    D3DXMatrixTranslation(&matWorld, posX, posY, 0.0f);
                                    g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&matWorld, 4);
                                    g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
                                }
                            }
                        }
                    }
                }
            }
        }
		// Bottom-right corner hint: board menu button icon + "Menu" text
		DrawHint(0.0f, 0.0f, buttonBACK_Mask, "Menu");
		DrawHint(-120.0f * X1pix, 0.0f, buttonX_Mask,   "Board");

    } // end INGAME

} // end Render()

// -------------------------------------------------------
// RenderNotLoggedIn
// Called instead of Render() when no profile is signed in.
// Assumes BeginScene has already been called on a black clear.
// -------------------------------------------------------
void RenderNotLoggedIn()
{	
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                D3DCOLOR_ARGB(255, fontBgR, fontBgG, fontBgB), 1.0f, 0);
    const float centerX = 0.0f;
    const float centerY = 0.0f;

    g_pd3dDevice->SetVertexDeclaration(g_pVertexDecl);
    g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(TEXVERTEX));
    g_pd3dDevice->SetIndices(g_pIB);
    g_pd3dDevice->SetVertexShader(g_pVertexShader);
    g_pd3dDevice->SetPixelShader(g_pPixelShader);

    // Line 1
    {
        const char* line1    = "Please log into an account";
        float        w1       = GetTextWidth(line1, 1.0f);
        float        posXtext = centerX - w1 / 2.0f + 10.0f * X1pix;
        float        posYtext = centerY + 30.0f * Y1pix;
        DrawTexturedString(posXtext, posYtext, 1.0f, line1);
    }

    // Line 2
    {
        const char* line2    = "to continue playing";
        float        w2       = GetTextWidth(line2, 1.0f);
        float        posXtext = centerX - w2 / 2.0f + 10.0f * X1pix;
        float        posYtext = centerY - 10.0f * Y1pix;
        DrawTexturedString(posXtext, posYtext, 1.0f, line2);
    }
}