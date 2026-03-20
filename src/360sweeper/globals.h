#ifndef GLOBALS_H
#define GLOBALS_H

#include "StdAfx.h"

// -------------------------------------------------------
// DEFINES
// -------------------------------------------------------
#define VIRTUAL_LT 0x9000
#define VIRTUAL_RT 0x9001
#define TRIGGER_THRESHOLD 150

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// -------------------------------------------------------
// ENUMS
// -------------------------------------------------------
enum Level {
	TITLESCREEN= -2,
    BOARDSIZE  = -1,
    INGAME     =  0,
    PAUSEMENU  =  1,
    OPTIONS    =  2,
    STATISTICS =  3,
    CREDITS    =  4,
    TUTORIAL   =  5,
    CONTROLS   =  6,
    SETTINGS   =  7,
    EXIT       =  8,
    CONFIRM    =  9,
	RESTART    =  10,
	TITLE      =  11,
	CUSTOMSIZE =  12,
	GAMEPLAYTUT = 13,
	PATTERNS   =  14,
	RESETCONTROLS = 15,
};
// -------------------------------------------------------
// STRUCTS
// -------------------------------------------------------
struct TEXVERTEX {
    float x, y, z;
    float u, v;
};
#define D3DFVF_TEXVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)

// -------------------------------------------------------
// CLASSES
// -------------------------------------------------------
class Xbox360Timer {
public:
    Xbox360Timer() {
        QueryPerformanceFrequency(&m_frequency);
        m_frozenTime = 0;
        m_isStopped  = true;
        Reset();
    }
    void Start() {
        m_isStopped = false;
        QueryPerformanceCounter(&m_startTime);
    }
    void Stop() {
        if (!m_isStopped) {
            m_frozenTime = GetTimeAsInt();
            m_isStopped  = true;
        }
    }
    void Reset() {
        QueryPerformanceCounter(&m_startTime);
        m_frozenTime = 0;
        m_isStopped  = true;
    }
    int GetTimeAsInt() {
        if (m_isStopped) return m_frozenTime;
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        long long elapsedTicks = currentTime.QuadPart - m_startTime.QuadPart;
        return (int)((elapsedTicks * 100) / m_frequency.QuadPart);
    }
    float GetTimeAsFloat() {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        long long elapsedTicks = currentTime.QuadPart - m_startTime.QuadPart;
        return (float)elapsedTicks / (float)m_frequency.QuadPart;
    }
private:
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_startTime;
    int  m_frozenTime;
    bool m_isStopped;
};

class RumbleManager {
private:
    DWORD    m_controllerIndex;
    ULONGLONG m_endTime;
    bool     m_isActive;
public:
    RumbleManager(DWORD index) : m_controllerIndex(index), m_endTime(0), m_isActive(false) {}
    void Pulse(WORD leftMotor, WORD rightMotor, DWORD durationMs) {
        XINPUT_VIBRATION vib;
        vib.wLeftMotorSpeed  = leftMotor;
        vib.wRightMotorSpeed = rightMotor;
        if (XInputSetState(m_controllerIndex, &vib) == ERROR_SUCCESS) {
            m_endTime  = GetTickCount() + durationMs;
            m_isActive = true;
        }
    }
    void Update() {
        if (m_isActive && GetTickCount() >= m_endTime) Stop();
    }
    void Stop() {
        XINPUT_VIBRATION stopVib;
        stopVib.wLeftMotorSpeed  = 0;
        stopVib.wRightMotorSpeed = 0;
        XInputSetState(m_controllerIndex, &stopVib);
        m_isActive = false;
    }
};

// -------------------------------------------------------
// NDC CONSTANTS
// -------------------------------------------------------
static const float X1pix         = (1.0f / 1280.0f) * 2.0f;
static const float Y1pix         = (1.0f / 720.0f)  * 2.0f;
static const float tileWidthNDC  = (32.0f / 1280.0f) * 2.0f;
static const float tileHeightNDC = (32.0f / 720.0f)  * 2.0f;
static const float offset24pxNDC = (24.0f / 720.0f)  * 2.0f;
static const float Xoffset24pxNDC= (24.0f / 1280.0f) * 2.0f;
static const float MAX_VOLUME_LIMIT = 2.0f;
// -------------------------------------------------------
// EXTERN GLOBALS
// -------------------------------------------------------

// D3D core
extern LPDIRECT3DDEVICE9          g_pd3dDevice;
extern LPDIRECT3DVERTEXBUFFER9    g_pVB;
extern LPDIRECT3DINDEXBUFFER9     g_pIB;
extern LPDIRECT3DVERTEXDECLARATION9 g_pVertexDecl;
extern LPDIRECT3DVERTEXSHADER9    g_pVertexShader;
extern LPDIRECT3DPIXELSHADER9     g_pPixelShader;
extern D3DXMATRIX                 g_matWorld, g_matView, g_matProj;
extern bool                       g_bWidescreen;
extern XVIDEO_MODE                VideoMode;

// Game state
extern int   gameOver;
extern int   gameRunning;
extern int   uncoveredSafeCells;
extern bool  boardGenerated;
extern bool  previewRender;
extern bool  openingTile;
extern bool  pressingStart;
extern bool  OPENbeingPressed;
extern bool  smileyClicked;

// Menu state
extern int   buttonBoard;
extern int   buttonMenu;
extern int   lastButtonMenu1;
extern int   lastButtonMenu2;
extern int   boardSizeReturnButton;
extern int   scene;
extern int   statsScene;
extern int   tutScene;

// Audio / settings
extern bool  g_IsMuted;
extern float soundVol;
extern int   volumepercentage;
extern bool  g_VibrationsEnabled;
extern int   g_VibrationStrengthPercent;
extern int   vibrationStrength;
extern int   movecooldownms;
extern int   holdTime;
extern bool  chordingEnabled;

// Custom board
extern int   customRow;
extern int   customCol;
extern int   customMines;

extern int   maxRow;
extern int   maxCol;
// Font / text
extern float spacing;
extern float letterWidth;
extern float fontScale;
extern int fontBgR;
extern int fontBgG;
extern int fontBgB;

// Input
extern bool  dpad;
extern bool  leftjoystick;
extern WORD  buttonOPEN_Mask;
extern WORD  buttonFLAG_Mask;
extern WORD  buttonRESET_Mask;
extern WORD  buttonBACK_Mask;
extern WORD  buttonA_Mask;
extern WORD  buttonX_Mask;
extern WORD  buttonB_Mask;
extern WORD  buttonY_Mask;
extern WORD  buttonLB_Mask;
extern WORD  buttonRB_Mask;
extern WORD  buttonL_THUMB_Mask;
extern WORD  buttonR_THUMB_Mask;
extern WORD  buttonDPAD_UP_Mask;
extern WORD  buttonDPAD_DOWN_Mask;
extern WORD  buttonDPAD_LEFT_Mask;
extern WORD  buttonDPAD_RIGHT_Mask;
extern bool  isListening;
extern WORD* actionToRemap;
extern DWORD lastMoveTime;

// Path
extern std::string GAME_ROOT_PATH;
extern std::string fullPath;
extern std::vector<std::string> packList;
extern int                      selectedPackIndex;
// Timer (one global instance)
extern Xbox360Timer myGameTimer;

extern std::vector<IXAudio2SourceVoice*> g_deadVoices;

#endif