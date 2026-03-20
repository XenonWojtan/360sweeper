#ifndef TEXTURES_H
#define TEXTURES_H

#include "StdAfx.h"

// -------------------------------------------------------
// TEXTURE DECLARATIONS
// -------------------------------------------------------

// UI / gameplay
extern LPDIRECT3DTEXTURE9 g_pboard_size_windowTexture;
extern LPDIRECT3DTEXTURE9 g_pbuttonTexture;
extern LPDIRECT3DTEXTURE9 g_pbuttonexitTexture;
extern LPDIRECT3DTEXTURE9 g_pbuttonexitselectedTexture;
extern LPDIRECT3DTEXTURE9 g_pbuttonmenuTexture;
extern LPDIRECT3DTEXTURE9 g_pbuttonmenuselectedTexture;
extern LPDIRECT3DTEXTURE9 g_pbuttonselectedTexture;
extern LPDIRECT3DTEXTURE9 g_pcheckboxTexture;
extern LPDIRECT3DTEXTURE9 g_pcheckboxcheckedTexture;
extern LPDIRECT3DTEXTURE9 g_pcheckboxselectedTexture;
extern LPDIRECT3DTEXTURE9 g_pcheckboxselectedcheckedTexture;
extern LPDIRECT3DTEXTURE9 g_pflagTexture;
extern LPDIRECT3DTEXTURE9 g_pflagMissplaceTexture;
extern LPDIRECT3DTEXTURE9 g_phighlightTexture;
extern LPDIRECT3DTEXTURE9 g_pmineTexture;
extern LPDIRECT3DTEXTURE9 g_pmineRedTexture;
extern LPDIRECT3DTEXTURE9 g_pquestCovTexture;
extern LPDIRECT3DTEXTURE9 g_psettings_windowTexture;
extern LPDIRECT3DTEXTURE9 g_psmileyClickedTexture;
extern LPDIRECT3DTEXTURE9 g_psmileyDEFTexture;
extern LPDIRECT3DTEXTURE9 g_psmileyLossTexture;
extern LPDIRECT3DTEXTURE9 g_psmileyOpeningTexture;
extern LPDIRECT3DTEXTURE9 g_psmileyWinTexture;
extern LPDIRECT3DTEXTURE9 g_plogoTexture;

// Tiles
extern LPDIRECT3DTEXTURE9 g_ptileTexture;
extern LPDIRECT3DTEXTURE9 g_ptile0Texture;
extern LPDIRECT3DTEXTURE9 g_ptile1Texture;
extern LPDIRECT3DTEXTURE9 g_ptile2Texture;
extern LPDIRECT3DTEXTURE9 g_ptile3Texture;
extern LPDIRECT3DTEXTURE9 g_ptile4Texture;
extern LPDIRECT3DTEXTURE9 g_ptile5Texture;
extern LPDIRECT3DTEXTURE9 g_ptile6Texture;
extern LPDIRECT3DTEXTURE9 g_ptile7Texture;
extern LPDIRECT3DTEXTURE9 g_ptile8Texture;

// Borders
extern LPDIRECT3DTEXTURE9 g_pBorders_bottomTexture;
extern LPDIRECT3DTEXTURE9 g_pBorders_leftTexture;
extern LPDIRECT3DTEXTURE9 g_pBorders_leftBottomCornerTexture;
extern LPDIRECT3DTEXTURE9 g_pBorders_leftTopCornerTexture;
extern LPDIRECT3DTEXTURE9 g_pBorders_leftTopCorner1Texture;
extern LPDIRECT3DTEXTURE9 g_pBorders_rightTexture;
extern LPDIRECT3DTEXTURE9 g_pBorders_rightBottomCornerTexture;
extern LPDIRECT3DTEXTURE9 g_pBorders_rightTopCornerTexture;
extern LPDIRECT3DTEXTURE9 g_pBorders_rightTopCorner1Texture;
extern LPDIRECT3DTEXTURE9 g_pBorders_topTexture;
extern LPDIRECT3DTEXTURE9 g_pBorders_top1Texture;

// Number display (HUD counters)
extern LPDIRECT3DTEXTURE9 g_pnum_blankTexture;
extern LPDIRECT3DTEXTURE9 g_pnum_minusTexture;
extern LPDIRECT3DTEXTURE9 g_pnum_num0Texture;
extern LPDIRECT3DTEXTURE9 g_pnum_num1Texture;
extern LPDIRECT3DTEXTURE9 g_pnum_num2Texture;
extern LPDIRECT3DTEXTURE9 g_pnum_num3Texture;
extern LPDIRECT3DTEXTURE9 g_pnum_num4Texture;
extern LPDIRECT3DTEXTURE9 g_pnum_num5Texture;
extern LPDIRECT3DTEXTURE9 g_pnum_num6Texture;
extern LPDIRECT3DTEXTURE9 g_pnum_num7Texture;
extern LPDIRECT3DTEXTURE9 g_pnum_num8Texture;
extern LPDIRECT3DTEXTURE9 g_pnum_num9Texture;

// Slider / bar UI
extern LPDIRECT3DTEXTURE9 g_psliderTexture;
extern LPDIRECT3DTEXTURE9 g_pbarTexture;
extern LPDIRECT3DTEXTURE9 g_pbarselectedTexture;
extern LPDIRECT3DTEXTURE9 g_psliderselectedTexture;
extern LPDIRECT3DTEXTURE9 g_pboardhighlightTexture;
extern LPDIRECT3DTEXTURE9 g_pbordercounterTexture;
extern LPDIRECT3DTEXTURE9 g_parrowTexture;
extern LPDIRECT3DTEXTURE9 g_parrowLeftTexture;
extern LPDIRECT3DTEXTURE9 g_parrowRightTexture;
extern LPDIRECT3DTEXTURE9 g_parrowUpTexture;



// Button icons (A/B/X/Y)
extern LPDIRECT3DTEXTURE9 g_picons_aiconTexture;
extern LPDIRECT3DTEXTURE9 g_picons_backTexture;
extern LPDIRECT3DTEXTURE9 g_picons_biconTexture;
extern LPDIRECT3DTEXTURE9 g_picons_dpadTexture;
extern LPDIRECT3DTEXTURE9 g_picons_lbTexture;
extern LPDIRECT3DTEXTURE9 g_picons_lclickTexture;
extern LPDIRECT3DTEXTURE9 g_picons_ltTexture;
extern LPDIRECT3DTEXTURE9 g_picons_rbTexture;
extern LPDIRECT3DTEXTURE9 g_picons_rclickTexture;
extern LPDIRECT3DTEXTURE9 g_picons_rtTexture;
extern LPDIRECT3DTEXTURE9 g_picons_startTexture;
extern LPDIRECT3DTEXTURE9 g_picons_tileChoosingTextTexture;
extern LPDIRECT3DTEXTURE9 g_picons_tileControlsTexture;
extern LPDIRECT3DTEXTURE9 g_picons_xiconTexture;
extern LPDIRECT3DTEXTURE9 g_picons_yiconTexture;

// Patterns
extern LPDIRECT3DTEXTURE9 g_ptutorial_boardtopTexture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_scene1Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_scene2Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_scene3Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_scene4Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_scene44Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_scene5Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_scene6Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_scene7Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_scene8Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_scene9Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_11scene1Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_11scene2Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_11scene3Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_11scene4Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_11scene5Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_121scene1Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_121scene2Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_121scene3Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_1221scene1Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_1221scene2Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_1221scene3Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_12scene1Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_12scene2Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_12scene3Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_12scene4Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_12scene5Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_patternB1Texture;
extern LPDIRECT3DTEXTURE9 g_ptutorial_patterns_patternB2Texture;

// -------------------------------------------------------
// FONT TEXTURES
// -------------------------------------------------------

// Digits
extern LPDIRECT3DTEXTURE9 g_p0Texture;
extern LPDIRECT3DTEXTURE9 g_p1Texture;
extern LPDIRECT3DTEXTURE9 g_p2Texture;
extern LPDIRECT3DTEXTURE9 g_p3Texture;
extern LPDIRECT3DTEXTURE9 g_p4Texture;
extern LPDIRECT3DTEXTURE9 g_p5Texture;
extern LPDIRECT3DTEXTURE9 g_p6Texture;
extern LPDIRECT3DTEXTURE9 g_p7Texture;
extern LPDIRECT3DTEXTURE9 g_p8Texture;
extern LPDIRECT3DTEXTURE9 g_p9Texture;

// Lowercase letters
extern LPDIRECT3DTEXTURE9 g_paTexture;
extern LPDIRECT3DTEXTURE9 g_pbTexture;
extern LPDIRECT3DTEXTURE9 g_pcTexture;
extern LPDIRECT3DTEXTURE9 g_pdTexture;
extern LPDIRECT3DTEXTURE9 g_peTexture;
extern LPDIRECT3DTEXTURE9 g_pfTexture;
extern LPDIRECT3DTEXTURE9 g_pgTexture;
extern LPDIRECT3DTEXTURE9 g_phTexture;
extern LPDIRECT3DTEXTURE9 g_piTexture;
extern LPDIRECT3DTEXTURE9 g_pjTexture;
extern LPDIRECT3DTEXTURE9 g_pkTexture;
extern LPDIRECT3DTEXTURE9 g_plTexture;
extern LPDIRECT3DTEXTURE9 g_pmTexture;
extern LPDIRECT3DTEXTURE9 g_pnTexture;
extern LPDIRECT3DTEXTURE9 g_poTexture;
extern LPDIRECT3DTEXTURE9 g_ppTexture;
extern LPDIRECT3DTEXTURE9 g_pqTexture;
extern LPDIRECT3DTEXTURE9 g_prTexture;
extern LPDIRECT3DTEXTURE9 g_psTexture;
extern LPDIRECT3DTEXTURE9 g_ptTexture;
extern LPDIRECT3DTEXTURE9 g_puTexture;
extern LPDIRECT3DTEXTURE9 g_pvTexture;
extern LPDIRECT3DTEXTURE9 g_pwTexture;
extern LPDIRECT3DTEXTURE9 g_pxTexture;
extern LPDIRECT3DTEXTURE9 g_pyTexture;
extern LPDIRECT3DTEXTURE9 g_pzTexture;

// Uppercase letters
extern LPDIRECT3DTEXTURE9 g_pupper_ATexture;
extern LPDIRECT3DTEXTURE9 g_pupper_BTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_CTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_DTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_ETexture;
extern LPDIRECT3DTEXTURE9 g_pupper_FTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_GTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_HTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_ITexture;
extern LPDIRECT3DTEXTURE9 g_pupper_JTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_KTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_LTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_MTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_NTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_OTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_PTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_QTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_RTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_STexture;
extern LPDIRECT3DTEXTURE9 g_pupper_TTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_UTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_VTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_WTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_XTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_YTexture;
extern LPDIRECT3DTEXTURE9 g_pupper_ZTexture;

// Symbols
extern LPDIRECT3DTEXTURE9 g_pampersandTexture;
extern LPDIRECT3DTEXTURE9 g_pasteriskTexture;
extern LPDIRECT3DTEXTURE9 g_patTexture;
extern LPDIRECT3DTEXTURE9 g_pbrace_closeTexture;
extern LPDIRECT3DTEXTURE9 g_pbrace_openTexture;
extern LPDIRECT3DTEXTURE9 g_pbracket_closeTexture;
extern LPDIRECT3DTEXTURE9 g_pbracket_openTexture;
extern LPDIRECT3DTEXTURE9 g_pcaretTexture;
extern LPDIRECT3DTEXTURE9 g_pcolonTexture;
extern LPDIRECT3DTEXTURE9 g_pcommaTexture;
extern LPDIRECT3DTEXTURE9 g_pdollarTexture;
extern LPDIRECT3DTEXTURE9 g_pequalsTexture;
extern LPDIRECT3DTEXTURE9 g_pexclamationTexture;
extern LPDIRECT3DTEXTURE9 g_pgreater_thanTexture;
extern LPDIRECT3DTEXTURE9 g_phashTexture;
extern LPDIRECT3DTEXTURE9 g_pless_thanTexture;
extern LPDIRECT3DTEXTURE9 g_pminusTexture;
extern LPDIRECT3DTEXTURE9 g_pparen_closeTexture;
extern LPDIRECT3DTEXTURE9 g_pparen_openTexture;
extern LPDIRECT3DTEXTURE9 g_ppercentTexture;
extern LPDIRECT3DTEXTURE9 g_pperiodTexture;
extern LPDIRECT3DTEXTURE9 g_ppipeTexture;
extern LPDIRECT3DTEXTURE9 g_pplusTexture;
extern LPDIRECT3DTEXTURE9 g_pquestionTexture;
extern LPDIRECT3DTEXTURE9 g_psemicolonTexture;
extern LPDIRECT3DTEXTURE9 g_punderscoreTexture;
extern LPDIRECT3DTEXTURE9 g_pslashTexture;

// -------------------------------------------------------
// FUNCTION DECLARATIONS
// -------------------------------------------------------
LPDIRECT3DTEXTURE9 loadTextureFromFile(const char* filePath);
void               loadingText();
void               CleanUpTextures();
LPDIRECT3DTEXTURE9 GetNumberTexture(int digit);
LPDIRECT3DTEXTURE9 StringToButtonIcon(std::string name);
LPDIRECT3DTEXTURE9 getCheckboxtexture(int i, bool checkboxtriggered);
LPDIRECT3DTEXTURE9 GetTextureFromChar(char c);

#endif // TEXTURES_H