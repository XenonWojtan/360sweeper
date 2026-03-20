#include "StdAfx.h"
#include "globals.h"
#include "textures.h"
#include "controls.h"
// -------------------------------------------------------
// TEXTURE DEFINITIONS
// -------------------------------------------------------

// UI / gameplay
LPDIRECT3DTEXTURE9 g_pboard_size_windowTexture          = NULL;
LPDIRECT3DTEXTURE9 g_pbuttonTexture                     = NULL;
LPDIRECT3DTEXTURE9 g_pbuttonexitTexture                 = NULL;
LPDIRECT3DTEXTURE9 g_pbuttonexitselectedTexture         = NULL;
LPDIRECT3DTEXTURE9 g_pbuttonmenuTexture                 = NULL;
LPDIRECT3DTEXTURE9 g_pbuttonmenuselectedTexture         = NULL;
LPDIRECT3DTEXTURE9 g_pbuttonselectedTexture             = NULL;
LPDIRECT3DTEXTURE9 g_pcheckboxTexture                   = NULL;
LPDIRECT3DTEXTURE9 g_pcheckboxcheckedTexture            = NULL;
LPDIRECT3DTEXTURE9 g_pcheckboxselectedTexture           = NULL;
LPDIRECT3DTEXTURE9 g_pcheckboxselectedcheckedTexture    = NULL;
LPDIRECT3DTEXTURE9 g_pflagTexture                       = NULL;
LPDIRECT3DTEXTURE9 g_pflagMissplaceTexture              = NULL;
LPDIRECT3DTEXTURE9 g_phighlightTexture                  = NULL;
LPDIRECT3DTEXTURE9 g_pmineTexture                       = NULL;
LPDIRECT3DTEXTURE9 g_pmineRedTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pquestCovTexture                   = NULL;
LPDIRECT3DTEXTURE9 g_psettings_windowTexture            = NULL;
LPDIRECT3DTEXTURE9 g_psmileyClickedTexture              = NULL;
LPDIRECT3DTEXTURE9 g_psmileyDEFTexture                  = NULL;
LPDIRECT3DTEXTURE9 g_psmileyLossTexture                 = NULL;
LPDIRECT3DTEXTURE9 g_psmileyOpeningTexture              = NULL;
LPDIRECT3DTEXTURE9 g_psmileyWinTexture                  = NULL;
LPDIRECT3DTEXTURE9 g_plogoTexture = NULL;

// Tiles
LPDIRECT3DTEXTURE9 g_ptileTexture                       = NULL;
LPDIRECT3DTEXTURE9 g_ptile0Texture                      = NULL;
LPDIRECT3DTEXTURE9 g_ptile1Texture                      = NULL;
LPDIRECT3DTEXTURE9 g_ptile2Texture                      = NULL;
LPDIRECT3DTEXTURE9 g_ptile3Texture                      = NULL;
LPDIRECT3DTEXTURE9 g_ptile4Texture                      = NULL;
LPDIRECT3DTEXTURE9 g_ptile5Texture                      = NULL;
LPDIRECT3DTEXTURE9 g_ptile6Texture                      = NULL;
LPDIRECT3DTEXTURE9 g_ptile7Texture                      = NULL;
LPDIRECT3DTEXTURE9 g_ptile8Texture                      = NULL;

// Borders
LPDIRECT3DTEXTURE9 g_pBorders_bottomTexture             = NULL;
LPDIRECT3DTEXTURE9 g_pBorders_leftTexture               = NULL;
LPDIRECT3DTEXTURE9 g_pBorders_leftBottomCornerTexture   = NULL;
LPDIRECT3DTEXTURE9 g_pBorders_leftTopCornerTexture      = NULL;
LPDIRECT3DTEXTURE9 g_pBorders_leftTopCorner1Texture     = NULL;
LPDIRECT3DTEXTURE9 g_pBorders_rightTexture              = NULL;
LPDIRECT3DTEXTURE9 g_pBorders_rightBottomCornerTexture  = NULL;
LPDIRECT3DTEXTURE9 g_pBorders_rightTopCornerTexture     = NULL;
LPDIRECT3DTEXTURE9 g_pBorders_rightTopCorner1Texture    = NULL;
LPDIRECT3DTEXTURE9 g_pBorders_topTexture                = NULL;
LPDIRECT3DTEXTURE9 g_pBorders_top1Texture               = NULL;

// Number display (HUD counters)
LPDIRECT3DTEXTURE9 g_pnum_blankTexture                  = NULL;
LPDIRECT3DTEXTURE9 g_pnum_minusTexture                  = NULL;
LPDIRECT3DTEXTURE9 g_pnum_num0Texture                   = NULL;
LPDIRECT3DTEXTURE9 g_pnum_num1Texture                   = NULL;
LPDIRECT3DTEXTURE9 g_pnum_num2Texture                   = NULL;
LPDIRECT3DTEXTURE9 g_pnum_num3Texture                   = NULL;
LPDIRECT3DTEXTURE9 g_pnum_num4Texture                   = NULL;
LPDIRECT3DTEXTURE9 g_pnum_num5Texture                   = NULL;
LPDIRECT3DTEXTURE9 g_pnum_num6Texture                   = NULL;
LPDIRECT3DTEXTURE9 g_pnum_num7Texture                   = NULL;
LPDIRECT3DTEXTURE9 g_pnum_num8Texture                   = NULL;
LPDIRECT3DTEXTURE9 g_pnum_num9Texture                   = NULL;

// Slider / bar UI
LPDIRECT3DTEXTURE9 g_psliderTexture                     = NULL;
LPDIRECT3DTEXTURE9 g_pbarTexture                        = NULL;
LPDIRECT3DTEXTURE9 g_pbarselectedTexture                = NULL;
LPDIRECT3DTEXTURE9 g_psliderselectedTexture             = NULL;
LPDIRECT3DTEXTURE9 g_pboardhighlightTexture             = NULL;
LPDIRECT3DTEXTURE9 g_pbordercounterTexture = NULL;
LPDIRECT3DTEXTURE9 g_parrowTexture      = NULL;
LPDIRECT3DTEXTURE9 g_parrowLeftTexture  = NULL;
LPDIRECT3DTEXTURE9 g_parrowRightTexture = NULL;
LPDIRECT3DTEXTURE9 g_parrowUpTexture    = NULL;



// Button icons
LPDIRECT3DTEXTURE9 g_picons_aiconTexture            = NULL;
LPDIRECT3DTEXTURE9 g_picons_backTexture             = NULL;
LPDIRECT3DTEXTURE9 g_picons_biconTexture            = NULL;
LPDIRECT3DTEXTURE9 g_picons_dpadTexture             = NULL;
LPDIRECT3DTEXTURE9 g_picons_lbTexture               = NULL;
LPDIRECT3DTEXTURE9 g_picons_lclickTexture           = NULL;
LPDIRECT3DTEXTURE9 g_picons_ltTexture               = NULL;
LPDIRECT3DTEXTURE9 g_picons_rbTexture               = NULL;
LPDIRECT3DTEXTURE9 g_picons_rclickTexture           = NULL;
LPDIRECT3DTEXTURE9 g_picons_rtTexture               = NULL;
LPDIRECT3DTEXTURE9 g_picons_startTexture            = NULL;
LPDIRECT3DTEXTURE9 g_picons_tileChoosingTextTexture = NULL;
LPDIRECT3DTEXTURE9 g_picons_tileControlsTexture     = NULL;
LPDIRECT3DTEXTURE9 g_picons_xiconTexture            = NULL;
LPDIRECT3DTEXTURE9 g_picons_yiconTexture            = NULL;

// Patterns
LPDIRECT3DTEXTURE9 g_ptutorial_boardtopTexture            = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_scene1Texture              = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_scene2Texture              = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_scene3Texture              = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_scene4Texture              = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_scene44Texture             = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_scene5Texture              = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_scene6Texture              = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_scene7Texture              = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_scene8Texture              = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_scene9Texture              = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_11scene1Texture   = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_11scene2Texture   = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_11scene3Texture   = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_11scene4Texture   = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_11scene5Texture   = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_121scene1Texture  = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_121scene2Texture  = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_121scene3Texture  = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_1221scene1Texture = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_1221scene2Texture = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_1221scene3Texture = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_12scene1Texture   = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_12scene2Texture   = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_12scene3Texture   = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_12scene4Texture   = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_12scene5Texture   = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_patternB1Texture  = NULL;
LPDIRECT3DTEXTURE9 g_ptutorial_patterns_patternB2Texture  = NULL;

// -------------------------------------------------------
// FONT TEXTURE DEFINITIONS
// -------------------------------------------------------

// Digits
LPDIRECT3DTEXTURE9 g_p0Texture                          = NULL;
LPDIRECT3DTEXTURE9 g_p1Texture                          = NULL;
LPDIRECT3DTEXTURE9 g_p2Texture                          = NULL;
LPDIRECT3DTEXTURE9 g_p3Texture                          = NULL;
LPDIRECT3DTEXTURE9 g_p4Texture                          = NULL;
LPDIRECT3DTEXTURE9 g_p5Texture                          = NULL;
LPDIRECT3DTEXTURE9 g_p6Texture                          = NULL;
LPDIRECT3DTEXTURE9 g_p7Texture                          = NULL;
LPDIRECT3DTEXTURE9 g_p8Texture                          = NULL;
LPDIRECT3DTEXTURE9 g_p9Texture                          = NULL;

// Lowercase letters
LPDIRECT3DTEXTURE9 g_paTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pbTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pcTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pdTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_peTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pfTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pgTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_phTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_piTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pjTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pkTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_plTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pmTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pnTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_poTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_ppTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pqTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_prTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_psTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_ptTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_puTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pvTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pwTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pxTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pyTexture                          = NULL;
LPDIRECT3DTEXTURE9 g_pzTexture                          = NULL;

// Uppercase letters
LPDIRECT3DTEXTURE9 g_pupper_ATexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_BTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_CTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_DTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_ETexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_FTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_GTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_HTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_ITexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_JTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_KTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_LTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_MTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_NTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_OTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_PTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_QTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_RTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_STexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_TTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_UTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_VTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_WTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_XTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_YTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pupper_ZTexture                    = NULL;

// Symbols
LPDIRECT3DTEXTURE9 g_pampersandTexture                  = NULL;
LPDIRECT3DTEXTURE9 g_pasteriskTexture                   = NULL;
LPDIRECT3DTEXTURE9 g_patTexture                         = NULL;
LPDIRECT3DTEXTURE9 g_pbrace_closeTexture                = NULL;
LPDIRECT3DTEXTURE9 g_pbrace_openTexture                 = NULL;
LPDIRECT3DTEXTURE9 g_pbracket_closeTexture              = NULL;
LPDIRECT3DTEXTURE9 g_pbracket_openTexture               = NULL;
LPDIRECT3DTEXTURE9 g_pcaretTexture                      = NULL;
LPDIRECT3DTEXTURE9 g_pcolonTexture                      = NULL;
LPDIRECT3DTEXTURE9 g_pcommaTexture                      = NULL;
LPDIRECT3DTEXTURE9 g_pdollarTexture                     = NULL;
LPDIRECT3DTEXTURE9 g_pequalsTexture                     = NULL;
LPDIRECT3DTEXTURE9 g_pexclamationTexture                = NULL;
LPDIRECT3DTEXTURE9 g_pgreater_thanTexture               = NULL;
LPDIRECT3DTEXTURE9 g_phashTexture                       = NULL;
LPDIRECT3DTEXTURE9 g_pless_thanTexture                  = NULL;
LPDIRECT3DTEXTURE9 g_pminusTexture                      = NULL;
LPDIRECT3DTEXTURE9 g_pparen_closeTexture                = NULL;
LPDIRECT3DTEXTURE9 g_pparen_openTexture                 = NULL;
LPDIRECT3DTEXTURE9 g_ppercentTexture                    = NULL;
LPDIRECT3DTEXTURE9 g_pperiodTexture                     = NULL;
LPDIRECT3DTEXTURE9 g_ppipeTexture                       = NULL;
LPDIRECT3DTEXTURE9 g_pplusTexture                       = NULL;
LPDIRECT3DTEXTURE9 g_pquestionTexture                   = NULL;
LPDIRECT3DTEXTURE9 g_psemicolonTexture                  = NULL;
LPDIRECT3DTEXTURE9 g_punderscoreTexture                 = NULL;
LPDIRECT3DTEXTURE9 g_pslashTexture                      = NULL;

LPDIRECT3DTEXTURE9* tekstury[] = {
	&g_pboard_size_windowTexture,
	&g_pbuttonTexture,
	&g_pbuttonexitTexture,
	&g_pbuttonexitselectedTexture,
	&g_pbuttonmenuTexture,
	&g_pbuttonmenuselectedTexture,
	&g_pbuttonselectedTexture,
	&g_pcheckboxTexture,
	&g_pcheckboxcheckedTexture,
	&g_pcheckboxselectedTexture,
	&g_pcheckboxselectedcheckedTexture,
	&g_pflagTexture,
	&g_pflagMissplaceTexture,
	&g_phighlightTexture,
	&g_pmineTexture,
	&g_pmineRedTexture,
	&g_pquestCovTexture,
	&g_psettings_windowTexture,
	&g_psmileyClickedTexture,
	&g_psmileyDEFTexture,
	&g_psmileyLossTexture,
	&g_psmileyOpeningTexture,
	&g_psmileyWinTexture,
	&g_plogoTexture,
	&g_ptileTexture,
	&g_ptile0Texture,
	&g_ptile1Texture,
	&g_ptile2Texture,
	&g_ptile3Texture,
	&g_ptile4Texture,
	&g_ptile5Texture,
	&g_ptile6Texture,
	&g_ptile7Texture,
	&g_ptile8Texture,
	&g_pBorders_bottomTexture,
	&g_pBorders_leftTexture,
	&g_pBorders_leftBottomCornerTexture,
	&g_pBorders_leftTopCornerTexture,
	&g_pBorders_leftTopCorner1Texture,
	&g_pBorders_rightTexture,
	&g_pBorders_rightBottomCornerTexture,
	&g_pBorders_rightTopCornerTexture,
	&g_pBorders_rightTopCorner1Texture,
	&g_pBorders_topTexture,
	&g_pBorders_top1Texture,
	&g_pnum_blankTexture,
	&g_pnum_minusTexture,
	&g_pnum_num0Texture,
	&g_pnum_num1Texture,
	&g_pnum_num2Texture,
	&g_pnum_num3Texture,
	&g_pnum_num4Texture,
	&g_pnum_num5Texture,
	&g_pnum_num6Texture,
	&g_pnum_num7Texture,
	&g_pnum_num8Texture,
	&g_pnum_num9Texture,
	&g_psliderTexture,
	&g_pbarTexture,
	&g_pbarselectedTexture,
	&g_psliderselectedTexture,
	&g_pboardhighlightTexture,
	&g_pbordercounterTexture,
	&g_parrowTexture,
	&g_parrowLeftTexture,
	&g_parrowRightTexture,
	&g_parrowUpTexture,
	&g_picons_aiconTexture,
	&g_picons_backTexture,
	&g_picons_biconTexture,
	&g_picons_dpadTexture,
	&g_picons_lbTexture,
	&g_picons_lclickTexture,
	&g_picons_ltTexture,
	&g_picons_rbTexture,
	&g_picons_rclickTexture,
	&g_picons_rtTexture,
	&g_picons_startTexture,
	&g_picons_tileChoosingTextTexture,
	&g_picons_tileControlsTexture,
	&g_picons_xiconTexture,
	&g_picons_yiconTexture,
	&g_ptutorial_boardtopTexture,
	&g_ptutorial_scene1Texture,
	&g_ptutorial_scene2Texture,
	&g_ptutorial_scene3Texture,
	&g_ptutorial_scene4Texture,
	&g_ptutorial_scene44Texture,
	&g_ptutorial_scene5Texture,
	&g_ptutorial_scene6Texture,
	&g_ptutorial_scene7Texture,
	&g_ptutorial_scene8Texture,
	&g_ptutorial_scene9Texture,
	&g_ptutorial_patterns_11scene1Texture,
	&g_ptutorial_patterns_11scene2Texture,
	&g_ptutorial_patterns_11scene3Texture,
	&g_ptutorial_patterns_11scene4Texture,
	&g_ptutorial_patterns_11scene5Texture,
	&g_ptutorial_patterns_121scene1Texture,
	&g_ptutorial_patterns_121scene2Texture,
	&g_ptutorial_patterns_121scene3Texture,
	&g_ptutorial_patterns_1221scene1Texture,
	&g_ptutorial_patterns_1221scene2Texture,
	&g_ptutorial_patterns_1221scene3Texture,
	&g_ptutorial_patterns_12scene1Texture,
	&g_ptutorial_patterns_12scene2Texture,
	&g_ptutorial_patterns_12scene3Texture,
	&g_ptutorial_patterns_12scene4Texture,
	&g_ptutorial_patterns_12scene5Texture,
	&g_ptutorial_patterns_patternB1Texture,
	&g_ptutorial_patterns_patternB2Texture,
	&g_p0Texture,
	&g_p1Texture,
	&g_p2Texture,
	&g_p3Texture,
	&g_p4Texture,
	&g_p5Texture,
	&g_p6Texture,
	&g_p7Texture,
	&g_p8Texture,
	&g_p9Texture,
	&g_paTexture,
	&g_pbTexture,
	&g_pcTexture,
	&g_pdTexture,
	&g_peTexture,
	&g_pfTexture,
	&g_pgTexture,
	&g_phTexture,
	&g_piTexture,
	&g_pjTexture,
	&g_pkTexture,
	&g_plTexture,
	&g_pmTexture,
	&g_pnTexture,
	&g_poTexture,
	&g_ppTexture,
	&g_pqTexture,
	&g_prTexture,
	&g_psTexture,
	&g_ptTexture,
	&g_puTexture,
	&g_pvTexture,
	&g_pwTexture,
	&g_pxTexture,
	&g_pyTexture,
	&g_pzTexture,
	&g_pupper_ATexture,
	&g_pupper_BTexture,
	&g_pupper_CTexture,
	&g_pupper_DTexture,
	&g_pupper_ETexture,
	&g_pupper_FTexture,
	&g_pupper_GTexture,
	&g_pupper_HTexture,
	&g_pupper_ITexture,
	&g_pupper_JTexture,
	&g_pupper_KTexture,
	&g_pupper_LTexture,
	&g_pupper_MTexture,
	&g_pupper_NTexture,
	&g_pupper_OTexture,
	&g_pupper_PTexture,
	&g_pupper_QTexture,
	&g_pupper_RTexture,
	&g_pupper_STexture,
	&g_pupper_TTexture,
	&g_pupper_UTexture,
	&g_pupper_VTexture,
	&g_pupper_WTexture,
	&g_pupper_XTexture,
	&g_pupper_YTexture,
	&g_pupper_ZTexture,
	&g_pampersandTexture,
	&g_pasteriskTexture,
	&g_patTexture,
	&g_pbrace_closeTexture,
	&g_pbrace_openTexture,
	&g_pbracket_closeTexture,
	&g_pbracket_openTexture,
	&g_pcaretTexture,
	&g_pcolonTexture,
	&g_pcommaTexture,
	&g_pdollarTexture,
	&g_pequalsTexture,
	&g_pexclamationTexture,
	&g_pgreater_thanTexture,
	&g_phashTexture,
	&g_pless_thanTexture,
	&g_pminusTexture,
	&g_pparen_closeTexture,
	&g_pparen_openTexture,
	&g_ppercentTexture,
	&g_pperiodTexture,
	&g_ppipeTexture,
	&g_pplusTexture,
	&g_pquestionTexture,
	&g_psemicolonTexture,
	&g_punderscoreTexture,
	&g_pslashTexture
};
const int txtCount = sizeof(tekstury) / sizeof(tekstury[0]);

const char* paths[] = {
	"board_size_window.png",
	"button.png",
	"buttonexit.png",
	"buttonexitselected.png",
	"buttonmenu.png",
	"buttonmenuselected.png",
	"buttonselected.png",
	"checkbox.png",
	"checkboxchecked.png",
	"checkboxselected.png",
	"checkboxselectedchecked.png",
	"flag.png",
	"flagMissplace.png",
	"highlight.png",
	"mine.png",
	"mineRed.png",
	"questCov.png",
	"settings_window.png",
	"smileyClicked.png",
	"smileyDEF.png",
	"smileyLoss.png",
	"smileyOpening.png",
	"smileyWin.png",
	"logo.png",
	"tile.png",
	"tile0.png",
	"tile1.png",
	"tile2.png",
	"tile3.png",
	"tile4.png",
	"tile5.png",
	"tile6.png",
	"tile7.png",
	"tile8.png",
	"Borders\\bottom.png",
	"Borders\\left.png",
	"Borders\\leftBottomCorner.png",
	"Borders\\leftTopCorner.png",
	"Borders\\leftTopCorner1.png",
	"Borders\\right.png",
	"Borders\\rightBottomCorner.png",
	"Borders\\rightTopCorner.png",
	"Borders\\rightTopCorner1.png",
	"Borders\\top.png",
	"Borders\\top1.png",
	"num\\blank.png",
	"num\\minus.png",
	"num\\num0.png",
	"num\\num1.png",
	"num\\num2.png",
	"num\\num3.png",
	"num\\num4.png",
	"num\\num5.png",
	"num\\num6.png",
	"num\\num7.png",
	"num\\num8.png",
	"num\\num9.png",
	"slider.png",
	"bar.png",
	"barselected.png",
	"sliderselected.png",
	"boardhighlight.png",
	"bordercounter.png",
	"arrow.png",
	"arrowLeft.png",
	"arrowRight.png",
	"arrowUp.png",
	"icons\\aicon.png",
	"icons\\back.png",
	"icons\\bicon.png",
	"icons\\dpad.png",
	"icons\\lb.png",
	"icons\\lclick.png",
	"icons\\lt.png",
	"icons\\rb.png",
	"icons\\rclick.png",
	"icons\\rt.png",
	"icons\\start.png",
	"icons\\tileChoosingText.png",
	"icons\\tileControls.png",
	"icons\\xicon.png",
	"icons\\yicon.png",
	"tutorial\\boardtop.png",
	"tutorial\\scene1.png",
	"tutorial\\scene2.png",
	"tutorial\\scene3.png",
	"tutorial\\scene4.png",
	"tutorial\\scene44.png",
	"tutorial\\scene5.png",
	"tutorial\\scene6.png",
	"tutorial\\scene7.png",
	"tutorial\\scene8.png",
	"tutorial\\scene9.png",
	"tutorial\\patterns\\11scene1.png",
	"tutorial\\patterns\\11scene2.png",
	"tutorial\\patterns\\11scene3.png",
	"tutorial\\patterns\\11scene4.png",
	"tutorial\\patterns\\11scene5.png",
	"tutorial\\patterns\\121scene1.png",
	"tutorial\\patterns\\121scene2.png",
	"tutorial\\patterns\\121scene3.png",
	"tutorial\\patterns\\1221scene1.png",
	"tutorial\\patterns\\1221scene2.png",
	"tutorial\\patterns\\1221scene3.png",
	"tutorial\\patterns\\12scene1.png",
	"tutorial\\patterns\\12scene2.png",
	"tutorial\\patterns\\12scene3.png",
	"tutorial\\patterns\\12scene4.png",
	"tutorial\\patterns\\12scene5.png",
	"tutorial\\patterns\\patternB1.png",
	"tutorial\\patterns\\patternB2.png",
	"Font\\0.png",
	"Font\\1.png",
	"Font\\2.png",
	"Font\\3.png",
	"Font\\4.png",
	"Font\\5.png",
	"Font\\6.png",
	"Font\\7.png",
	"Font\\8.png",
	"Font\\9.png",
	"Font\\a.png",
	"Font\\b.png",
	"Font\\c.png",
	"Font\\d.png",
	"Font\\e.png",
	"Font\\f.png",
	"Font\\g.png",
	"Font\\h.png",
	"Font\\i.png",
	"Font\\j.png",
	"Font\\k.png",
	"Font\\l.png",
	"Font\\m.png",
	"Font\\n.png",
	"Font\\o.png",
	"Font\\p.png",
	"Font\\q.png",
	"Font\\r.png",
	"Font\\s.png",
	"Font\\t.png",
	"Font\\u.png",
	"Font\\v.png",
	"Font\\w.png",
	"Font\\x.png",
	"Font\\y.png",
	"Font\\z.png",
	"Font\\upper_A.png",
	"Font\\upper_B.png",
	"Font\\upper_C.png",
	"Font\\upper_D.png",
	"Font\\upper_E.png",
	"Font\\upper_F.png",
	"Font\\upper_G.png",
	"Font\\upper_H.png",
	"Font\\upper_I.png",
	"Font\\upper_J.png",
	"Font\\upper_K.png",
	"Font\\upper_L.png",
	"Font\\upper_M.png",
	"Font\\upper_N.png",
	"Font\\upper_O.png",
	"Font\\upper_P.png",
	"Font\\upper_Q.png",
	"Font\\upper_R.png",
	"Font\\upper_S.png",
	"Font\\upper_T.png",
	"Font\\upper_U.png",
	"Font\\upper_V.png",
	"Font\\upper_W.png",
	"Font\\upper_X.png",
	"Font\\upper_Y.png",
	"Font\\upper_Z.png",
	"Font\\ampersand.png",
	"Font\\asterisk.png",
	"Font\\at.png",
	"Font\\brace_close.png",
	"Font\\brace_open.png",
	"Font\\bracket_close.png",
	"Font\\bracket_open.png",
	"Font\\caret.png",
	"Font\\colon.png",
	"Font\\comma.png",
	"Font\\dollar.png",
	"Font\\equals.png",
	"Font\\exclamation.png",
	"Font\\greater_than.png",
	"Font\\hash.png",
	"Font\\less_than.png",
	"Font\\minus.png",
	"Font\\paren_close.png",
	"Font\\paren_open.png",
	"Font\\percent.png",
	"Font\\period.png",
	"Font\\pipe.png",
	"Font\\plus.png",
	"Font\\question.png",
	"Font\\semicolon.png",
	"Font\\underscore.png",
	"Font\\slash.png"
};
// -------------------------------------------------------
// loadTextureFromFile
// -------------------------------------------------------
LPDIRECT3DTEXTURE9 loadTextureFromFile(const char* filePath)
{
    HRESULT hr;
    IDirect3DTexture9* texture = NULL;

    hr = D3DXCreateTextureFromFileExA(
        g_pd3dDevice,
        filePath,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        1,
        0,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT,
        D3DX_FILTER_NONE,
        D3DX_FILTER_NONE,
        0,
        NULL,
        NULL,
        &texture
    );

    if (FAILED(hr) || texture == NULL) {
        char buf[256];
        sprintf(buf, "Failed to load texture: %s (HRESULT: 0x%08X)\n", filePath, hr);
        OutputDebugStringA(buf);
        // Fallback magenta texture
        g_pd3dDevice->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL);
        D3DLOCKED_RECT lr;
        texture->LockRect(0, &lr, NULL, 0);
        *((DWORD*)lr.pBits) = 0xFFFF00FF;
        texture->UnlockRect(0);
        return texture;
    }
    return texture;
}

// -------------------------------------------------------
// CleanUpTextures
// -------------------------------------------------------
void CleanUpTextures()
{
	for (int i = 0; i< txtCount; i++){
		if (*tekstury[i] != NULL) {
			(*tekstury[i])->Release();
			*tekstury[i] = NULL;
		}
	}
}

// -------------------------------------------------------
// GetNumberTexture
// -------------------------------------------------------
LPDIRECT3DTEXTURE9 GetNumberTexture(int digit)
{
    switch (digit) {
        case 0: return g_pnum_num0Texture;
        case 1: return g_pnum_num1Texture;
        case 2: return g_pnum_num2Texture;
        case 3: return g_pnum_num3Texture;
        case 4: return g_pnum_num4Texture;
        case 5: return g_pnum_num5Texture;
        case 6: return g_pnum_num6Texture;
        case 7: return g_pnum_num7Texture;
        case 8: return g_pnum_num8Texture;
        case 9: return g_pnum_num9Texture;
        default: return g_pnum_num0Texture;
    }
}


// -------------------------------------------------------
// GetIconTexture
// -------------------------------------------------------
LPDIRECT3DTEXTURE9 StringToButtonIcon(std::string name)
{
    name = Sanitize(name);
    if (name == "LT")         return g_picons_ltTexture;
	if (name == "RT")         return g_picons_rtTexture;
	if (name == "A")          return g_picons_aiconTexture;
	if (name == "B")          return g_picons_biconTexture;
	if (name == "X")          return g_picons_xiconTexture;
	if (name == "Y")          return g_picons_yiconTexture;
	if (name == "START")      return g_picons_startTexture;
	if (name == "BACK")       return g_picons_backTexture;
	if (name == "LB")         return g_picons_lbTexture;
	if (name == "RB")         return g_picons_rbTexture;
	if (name == "L_THUMB")    return g_picons_lclickTexture;
	if (name == "R_THUMB")    return g_picons_rclickTexture;
	if (name == "DPAD_UP")    return g_picons_dpadTexture;
	if (name == "DPAD_DOWN")  return g_picons_dpadTexture;
	if (name == "DPAD_LEFT")  return g_picons_dpadTexture;
	if (name == "DPAD_RIGHT") return g_picons_dpadTexture;
    return g_pquestCovTexture;
}
// -------------------------------------------------------
// getCheckboxtexture
// -------------------------------------------------------
LPDIRECT3DTEXTURE9 getCheckboxtexture(int i, bool checkboxtriggered)
{
    if (i == buttonMenu - 1 && checkboxtriggered)
        return g_pcheckboxselectedcheckedTexture;
    else if (i == buttonMenu - 1 && !checkboxtriggered)
        return g_pcheckboxselectedTexture;
    else if (i != buttonMenu - 1 && checkboxtriggered)
        return g_pcheckboxcheckedTexture;
    else
        return g_pcheckboxTexture;
}

// -------------------------------------------------------
// GetTextureFromChar
// -------------------------------------------------------
LPDIRECT3DTEXTURE9 GetTextureFromChar(char c)
{
    switch (c) {
        // Digits
        case '0': return g_p0Texture;
        case '1': return g_p1Texture;
        case '2': return g_p2Texture;
        case '3': return g_p3Texture;
        case '4': return g_p4Texture;
        case '5': return g_p5Texture;
        case '6': return g_p6Texture;
        case '7': return g_p7Texture;
        case '8': return g_p8Texture;
        case '9': return g_p9Texture;
        // Lowercase
        case 'a': return g_paTexture;
        case 'b': return g_pbTexture;
        case 'c': return g_pcTexture;
        case 'd': return g_pdTexture;
        case 'e': return g_peTexture;
        case 'f': return g_pfTexture;
        case 'g': return g_pgTexture;
        case 'h': return g_phTexture;
        case 'i': return g_piTexture;
        case 'j': return g_pjTexture;
        case 'k': return g_pkTexture;
        case 'l': return g_plTexture;
        case 'm': return g_pmTexture;
        case 'n': return g_pnTexture;
        case 'o': return g_poTexture;
        case 'p': return g_ppTexture;
        case 'q': return g_pqTexture;
        case 'r': return g_prTexture;
        case 's': return g_psTexture;
        case 't': return g_ptTexture;
        case 'u': return g_puTexture;
        case 'v': return g_pvTexture;
        case 'w': return g_pwTexture;
        case 'x': return g_pxTexture;
        case 'y': return g_pyTexture;
        case 'z': return g_pzTexture;
        // Uppercase
        case 'A': return g_pupper_ATexture;
        case 'B': return g_pupper_BTexture;
        case 'C': return g_pupper_CTexture;
        case 'D': return g_pupper_DTexture;
        case 'E': return g_pupper_ETexture;
        case 'F': return g_pupper_FTexture;
        case 'G': return g_pupper_GTexture;
        case 'H': return g_pupper_HTexture;
        case 'I': return g_pupper_ITexture;
        case 'J': return g_pupper_JTexture;
        case 'K': return g_pupper_KTexture;
        case 'L': return g_pupper_LTexture;
        case 'M': return g_pupper_MTexture;
        case 'N': return g_pupper_NTexture;
        case 'O': return g_pupper_OTexture;
        case 'P': return g_pupper_PTexture;
        case 'Q': return g_pupper_QTexture;
        case 'R': return g_pupper_RTexture;
        case 'S': return g_pupper_STexture;
        case 'T': return g_pupper_TTexture;
        case 'U': return g_pupper_UTexture;
        case 'V': return g_pupper_VTexture;
        case 'W': return g_pupper_WTexture;
        case 'X': return g_pupper_XTexture;
        case 'Y': return g_pupper_YTexture;
        case 'Z': return g_pupper_ZTexture;
        // Symbols
        case '&': return g_pampersandTexture;
        case '*': return g_pasteriskTexture;
        case '@': return g_patTexture;
        case '}': return g_pbrace_closeTexture;
        case '{': return g_pbrace_openTexture;
        case ']': return g_pbracket_closeTexture;
        case '[': return g_pbracket_openTexture;
        case '^': return g_pcaretTexture;
        case ':': return g_pcolonTexture;
        case ',': return g_pcommaTexture;
        case '$': return g_pdollarTexture;
        case '=': return g_pequalsTexture;
        case '!': return g_pexclamationTexture;
        case '>': return g_pgreater_thanTexture;
        case '#': return g_phashTexture;
        case '<': return g_pless_thanTexture;
        case '-': return g_pminusTexture;
        case ')': return g_pparen_closeTexture;
        case '(': return g_pparen_openTexture;
        case '%': return g_ppercentTexture;
        case '.': return g_pperiodTexture;
        case '|': return g_ppipeTexture;
        case '+': return g_pplusTexture;
        case '?': return g_pquestionTexture;
        case ';': return g_psemicolonTexture;
        case '_': return g_punderscoreTexture;
        case '/': return g_pslashTexture;
        default:  return NULL;
    }
}

// -------------------------------------------------------
// loadingText  — loads all textures from the current pack
// -------------------------------------------------------

static void LoadTex(LPDIRECT3DTEXTURE9& var, const char* relativePath)
{
    fullPath = "game:\\" + GAME_ROOT_PATH + "\\" + relativePath;
    var = loadTextureFromFile(fullPath.c_str());
    if (!var) {
        char buf[256];
        sprintf(buf, "%s not found\n", relativePath);
        OutputDebugStringA(buf);
    }
}

void loadingText()
{
    CleanUpTextures();
	for (int i = 0; i< txtCount; i++){
		LoadTex(*tekstury[i],        paths[i]);
	}
}