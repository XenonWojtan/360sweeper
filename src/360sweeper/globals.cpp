#include "StdAfx.h"
#include "globals.h"

// D3D core
LPDIRECT3DDEVICE9           g_pd3dDevice  = NULL;
LPDIRECT3DVERTEXBUFFER9     g_pVB         = NULL;
LPDIRECT3DINDEXBUFFER9      g_pIB         = NULL;
LPDIRECT3DVERTEXDECLARATION9 g_pVertexDecl = NULL;
LPDIRECT3DVERTEXSHADER9     g_pVertexShader = NULL;
LPDIRECT3DPIXELSHADER9      g_pPixelShader  = NULL;
D3DXMATRIX                  g_matWorld, g_matView, g_matProj;
bool                        g_bWidescreen = false;
XVIDEO_MODE                 VideoMode;

// Game state
int   gameOver           = 0;
int   gameRunning        = 0;
int   uncoveredSafeCells = 0;
bool  boardGenerated     = false;
bool  previewRender      = false;
bool  openingTile        = false;
bool  pressingStart      = false;
bool  OPENbeingPressed   = false;
bool  smileyClicked      = false;

// Menu state
int   buttonBoard    = 1;
int   buttonMenu     = 1;
int   lastButtonMenu1 = 1;
int   lastButtonMenu2 = 1;
int   scene          = 1;
int   statsScene = 1;
int   tutScene = 0;
// Audio / settings
bool  g_IsMuted                = false;
float soundVol                 = 1.0f;
int   volumepercentage         = 100;
bool  g_VibrationsEnabled      = true;
int   g_VibrationStrengthPercent = 75;
int   vibrationStrength        = 30000;
int   movecooldownms           = 200;
int   holdTime                 = 225;
bool  chordingEnabled          = true;
// Custom board size
int   customRow     = 18;
int   customCol     = 18;
int   maxRow        = 18;
int   maxCol        = 38;
int   customMines   = 20;

int boardSizeReturnButton =1;
// Font / text
float spacing     = 10.0f;
float letterWidth = 20.0f;
float fontScale = 1.0f;
int fontBgR = 118;
int fontBgG = 118;
int fontBgB = 118;

// Input
bool  dpad          = true;
bool  leftjoystick  = true;
WORD  buttonOPEN_Mask  = VIRTUAL_RT;
WORD  buttonFLAG_Mask  = VIRTUAL_LT;
WORD  buttonRESET_Mask = XINPUT_GAMEPAD_START;
WORD  buttonBACK_Mask  = XINPUT_GAMEPAD_BACK;
WORD  buttonA_Mask     = XINPUT_GAMEPAD_A;
WORD  buttonX_Mask     = XINPUT_GAMEPAD_X;
WORD  buttonB_Mask          = XINPUT_GAMEPAD_B;
WORD  buttonY_Mask          = XINPUT_GAMEPAD_Y;
WORD  buttonLB_Mask         = XINPUT_GAMEPAD_LEFT_SHOULDER;
WORD  buttonRB_Mask         = XINPUT_GAMEPAD_RIGHT_SHOULDER;
WORD  buttonL_THUMB_Mask    = XINPUT_GAMEPAD_LEFT_THUMB;
WORD  buttonR_THUMB_Mask    = XINPUT_GAMEPAD_RIGHT_THUMB;
WORD  buttonDPAD_UP_Mask    = XINPUT_GAMEPAD_DPAD_UP;
WORD  buttonDPAD_DOWN_Mask  = XINPUT_GAMEPAD_DPAD_DOWN;
WORD  buttonDPAD_LEFT_Mask  = XINPUT_GAMEPAD_DPAD_LEFT;
WORD  buttonDPAD_RIGHT_Mask = XINPUT_GAMEPAD_DPAD_RIGHT;

bool  isListening   = false;
WORD* actionToRemap = NULL;
DWORD lastMoveTime  = 0;

// Path
std::string GAME_ROOT_PATH = "Content";
std::string fullPath       = "";
std::vector<std::string> packList;
int                      selectedPackIndex = 0;
// Timer
Xbox360Timer myGameTimer;
std::vector<IXAudio2SourceVoice*> g_deadVoices;