#include "StdAfx.h"
#include "globals.h"
#include "textures.h"
#include "board.h"
#include "render.h"
#include "controls.h"
#include "settings.h"
#include "audio.h"
#include "MinesweeperSave.h"
#include "fontprops.h"

static MSDifficulty GetCurrentDifficulty(int rows, int cols, int mines)
{
    if (rows ==  9 && cols ==  9 && mines ==  10) return MS_DIFF_BEGINNER;
    if (rows == 16 && cols == 16 && mines ==  40) return MS_DIFF_INTERMEDIATE;
    if (rows == 16 && cols == 30 && mines ==  99) return MS_DIFF_EXPERT;
    return MS_DIFF_CUSTOM;
}

// True while no Xbox profile is signed in — render overlay when set
bool g_bNotLoggedIn = false;

// Forward declaration — defined in render.cpp, also declare in render.h
void RenderNotLoggedIn();

//bugs = none;
//graphics = very good;
//fps = much;
//game = work;

//-------------------------------------------------------------------------------------
// Vertex shader
//-------------------------------------------------------------------------------------
const char* g_strVertexShaderProgram =
    " float4x4 matWVP : register(c0); "
    " struct VS_IN { float4 ObjPos : POSITION; float2 Tex : TEXCOORD0; }; "
    " struct VS_OUT { float4 ProjPos : POSITION; float2 Tex : TEXCOORD0; }; "
    " VS_OUT main(VS_IN In) { "
    " VS_OUT Out; "
    " Out.ProjPos = mul(matWVP, In.ObjPos); "
    " Out.Tex = In.Tex; "
    " return Out; "
    " } ";
//-------------------------------------------------------------------------------------
// Pixel shader
//-------------------------------------------------------------------------------------
const char* g_strPixelShaderProgram =
    " sampler2D texSampler : register(s0); "
    " struct PS_IN { float2 Tex : TEXCOORD0; }; "
    " float4 main(PS_IN In) : COLOR { "
    "     return tex2D(texSampler, In.Tex); "
    " }";

float GetUsedMemoryMB() {
    MEMORYSTATUS stat;
    GlobalMemoryStatus(&stat);
    
    double usedBytes = (double)stat.dwTotalPhys - (double)stat.dwAvailPhys;
    return (float)(usedBytes / (1024.0 * 1024.0));
}

//-------------------------------------------------------------------------------------
// InitD3D
//-------------------------------------------------------------------------------------
HRESULT InitD3D()
{
    Direct3D* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pD3D)
        return E_FAIL;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    XGetVideoMode(&VideoMode);
    g_bWidescreen = (VideoMode.fIsWideScreen != 0);

    d3dpp.BackBufferWidth        = min(VideoMode.dwDisplayWidth, 1280);
    d3dpp.BackBufferHeight       = min(VideoMode.dwDisplayHeight, 720);
    d3dpp.BackBufferFormat       = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount        = 1;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

    if (FAILED(pD3D->CreateDevice(
        0,
        D3DDEVTYPE_HAL,
        NULL,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &g_pd3dDevice)))
    {
        return E_FAIL;
    }

    FLOAT fAspect = g_bWidescreen ? (16.0f / 9.0f) : (4.0f / 3.0f);
    D3DXMatrixPerspectiveFovLH(&g_matProj, D3DX_PI / 4, fAspect, 1.0f, 200.0f);

    D3DXVECTOR3 vEyePt(0.0f, 0.0f, -7.0f);	
    D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
    D3DXMatrixLookAtLH(&g_matView, &vEyePt, &vLookatPt, &vUp);

    D3DXMatrixIdentity(&g_matWorld);
	SAFE_RELEASE(pD3D);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    return S_OK;
}

//-------------------------------------------------------------------------------------
// InitScene
//-------------------------------------------------------------------------------------
HRESULT InitScene() {
    // --- Compile vertex shader ---
    LPD3DXBUFFER pVSCode = NULL;
    LPD3DXBUFFER pVSError = NULL;
    HRESULT hr = D3DXCompileShader(g_strVertexShaderProgram, (UINT)strlen(g_strVertexShaderProgram),
                                   NULL, NULL, "main", "vs_2_0", 0, &pVSCode, &pVSError, NULL);
    if (FAILED(hr)) {
        if (pVSError) OutputDebugStringA((char*)pVSError->GetBufferPointer());
        return E_FAIL;
    }
    g_pd3dDevice->CreateVertexShader((DWORD*)pVSCode->GetBufferPointer(), &g_pVertexShader);
    // --- Compile pixel shader ---
    LPD3DXBUFFER pPSCode = NULL;
    LPD3DXBUFFER pPSError = NULL;
    hr = D3DXCompileShader(g_strPixelShaderProgram, (UINT)strlen(g_strPixelShaderProgram),
                            NULL, NULL, "main", "ps_2_0", 0, &pPSCode, &pPSError, NULL);
    if (FAILED(hr)) {
        if (pPSError) OutputDebugStringA((char*)pPSError->GetBufferPointer());
        return E_FAIL;
    }
    g_pd3dDevice->CreatePixelShader((DWORD*)pPSCode->GetBufferPointer(), &g_pPixelShader);

    // --- Vertex declaration ---
    D3DVERTEXELEMENT9 elems[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration(elems, &g_pVertexDecl);

    // --- Cube vertices with UVs ---
    float w = 32.0f / 1280.0f; 
	float h = 32.0f / 720.0f;

	TEXVERTEX verts[] = {
		// x,   y,  z,   u, v
		{-w,  h,  0,   0, 0}, // Top Left
		{ w,  h,  0,   1, 0}, // Top Right
		{ w, -h,  0,   1, 1}, // Bottom Right
		{-w, -h,  0,   0, 1}, // Bottom Left
	};

	WORD indices[] = { 0, 1, 2, 0, 2, 3 };

    // --- Vertex buffer ---
    if (FAILED(g_pd3dDevice->CreateVertexBuffer(sizeof(verts), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &g_pVB, NULL))) return E_FAIL;
    void* pData = NULL;
    g_pVB->Lock(0, sizeof(verts), &pData, 0);
    memcpy(pData, verts, sizeof(verts));
    g_pVB->Unlock();

    // --- Index buffer ---
    if (FAILED(g_pd3dDevice->CreateIndexBuffer(sizeof(indices), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIB, NULL))) return E_FAIL;
    void* pIdxData = NULL;
    g_pIB->Lock(0, sizeof(indices), &pIdxData, 0);
    memcpy(pIdxData, indices, sizeof(indices));
    g_pIB->Unlock();

    // --- Matrices ---
    D3DXMatrixIdentity(&g_matWorld);
    FLOAT fAspect = (VideoMode.fIsWideScreen) ? (16.0f / 9.0f) : (4.0f / 3.0f);
    D3DXMatrixPerspectiveFovLH(&g_matProj, D3DX_PI / 4, fAspect, 1.0f, 200.0f);
    D3DXVECTOR3 eye(0.0f,0.0f,-7.0f);
    D3DXVECTOR3 at(0.0f,0.0f,0.0f);
    D3DXVECTOR3 up(0.0f,1.0f,0.0f);
    D3DXMatrixLookAtLH(&g_matView,&eye,&at,&up);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    return S_OK;
}

void CleanUp() {
    // --- Release Gameplay Textures ---
    CleanUpTextures();

    // --- Release Buffers and Shaders ---
    SAFE_RELEASE(g_pVB);             
    SAFE_RELEASE(g_pIB);             
    SAFE_RELEASE(g_pVertexShader);   
    SAFE_RELEASE(g_pPixelShader);    
    SAFE_RELEASE(g_pVertexDecl);     
    SAFE_RELEASE(g_pd3dDevice);
}

void RestartGame() {
	XLaunchNewImage("game:\\360sweeper.xex", NULL);
}

void ReloadTxt()
{
	loadingText();
	LoadFontProps();
}


void __cdecl main() {
    HRESULT hr;
    IXAudio2* pXAudio2 = NULL;

    UINT32 flags = 0;
#ifdef _DEBUG
    flags |= XAUDIO2_DEBUG_ENGINE;
#endif

    if (FAILED(hr = XAudio2Create(&pXAudio2, flags)))
        ATG::FatalError("Error %#X calling XAudio2Create\n", hr);

    IXAudio2MasteringVoice* pMasteringVoice = NULL;
    if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice)))
        ATG::FatalError("Error %#X calling CreateMasteringVoice\n", hr);

    if (FAILED(InitD3D()))    return;
    if (FAILED(InitScene()))  return;
	//if (DEBUGXENIA) settingsPath = "cache://settingsmeow.txt";
    InitializeSettings();
    InitializeControls();
	InitializeFontProps();
    loadingText();

    // -------------------------------------------------------
    // Sign-in gate — spin here until at least one profile is
    // signed in.  XShowSigninUI prompts the user; we re-check
    // each iteration and only continue once someone is logged in.
    // -------------------------------------------------------
    while ( !MinesweeperSaveManager::IsUserSignedIn() )
    {
        // Show the sign-in blade; 1 = single user, 0 = no special flags
        XShowSigninUI( 1, 0 );
        bool uiDismissed = false;
        while ( !uiDismissed )
        {
            // The sign-in UI is modal on 360; once XUserGetSigninState changes
            // we know it has been dismissed.
            if ( MinesweeperSaveManager::IsUserSignedIn() )
                uiDismissed = true;

            // Keep rendering a black frame so the display isn't frozen
            g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                  D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0 );
            g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
        }
    }

    // -------------------------------------------------------
    // Save — device selector, mirrors ShowDeviceUI() in sample.
    // We fire it once here and poll XHasOverlappedIoCompleted
    // each frame (same as sample's m_bDeviceUIActive loop).
    // -------------------------------------------------------
    XCONTENTDEVICEID g_DeviceID    = XCONTENTDEVICE_ANY;
    XOVERLAPPED      g_DeviceXov   = {0};
    bool             g_DeviceReady = false; 
    bool             g_bResultRecorded = false;

    {
        ULARGE_INTEGER iBytesRequested = {0};
        DWORD dwRet = XShowDeviceSelectorUI(
            MinesweeperSaveManager::GetSignedInUser(),
            XCONTENTTYPE_SAVEDGAME,
            0,
            iBytesRequested,
            &g_DeviceID,
            &g_DeviceXov );
        // ERROR_IO_PENDING means the UI is showing — we poll below each frame.
        // Any other return means it failed immediately; skip saving silently.
        if ( dwRet != ERROR_IO_PENDING )
            g_DeviceReady = false;
    }

    RumbleManager player1Rumble(0);
    srand((unsigned int)time(NULL));
	
    // -------------------------------------------------------
    // Game state
    // -------------------------------------------------------
    int   tileCountRow  = 9;
    int   tileCountCol  = 9;
    int   mineCount     = 10;
    int   selectedRow   = 0;
    int   selectedCol   = 0;
    int   flagLeft      = mineCount;

    myGameTimer.Reset();
    previewRender = false;

    DWORD OPENpressStartTime  = 0;
    DWORD RESETpressStartTime = 0;
    bool  OPENActionTriggered  = false;
    bool  RESETActionTriggered = false;
    DWORD buttonPressStartTime = 0;
    DWORD lastRepeatTime       = 0;
	bool movedThisFrame = false;
	DWORD debugComboPressStart = 0;
	bool  debugComboActive     = false;
    // -------------------------------------------------------
    // Wait-for-release guard — prevents the A press that
    // triggered StartRemapping from being immediately captured
    // as the new binding on the very next frame.
    // -------------------------------------------------------
    bool listeningWaitForRelease = false;
	
    bool lastOPEN = false, lastFLAG = false, lastRESET = false;
    bool lastBACK = false, lastX = false, lastA = false;

    Board myBoard(tileCountRow, tileCountCol, &player1Rumble);
    int totalSafeCells = InitNewGame(myBoard, tileCountRow, tileCountCol, mineCount, player1Rumble);
	
    // -------------------------------------------------------
    // Sound paths
    // -------------------------------------------------------
    const std::string SND_MOVE   = "game:\\" + GAME_ROOT_PATH + "\\Sounds\\move_click.wav";
    const std::string SND_FLAG   = "game:\\" + GAME_ROOT_PATH + "\\Sounds\\flag_place.wav";
    const std::string SND_UNFLAG = "game:\\" + GAME_ROOT_PATH + "\\Sounds\\flag_unplace.wav";
    const std::string SND_RESET  = "game:\\" + GAME_ROOT_PATH + "\\Sounds\\reset.wav";

    // -------------------------------------------------------
    // BOARDSIZE lookup table
    // -------------------------------------------------------
    struct BoardPreset { int rows, cols, mines; };
    static const BoardPreset boardPresets[3] = {
        {  9,  9,  10 },  // Beginner
        { 16, 16,  40 },  // Intermediate
        { 16, 30,  99 },  // Expert
    };

    // -------------------------------------------------------
    // Slider config table
    // -------------------------------------------------------
    struct SliderCfg {
        int* pVal;
        int  minVal;
        int  maxVal;
        int  step;
    };
    SliderCfg sliders[4] = {
		{ &volumepercentage,           0,  200, 2 },  // was 2/198
		{ &g_VibrationStrengthPercent, 0,  100, 1 },  // was 1/99
		{ &movecooldownms,            50,  250, 1 },  // was 51/249
		{ &holdTime,                 150,  300, 1 },  // was 151/299
	};
	
    gameRunning = TITLESCREEN;

    // -------------------------------------------------------
    // Main loop
    // -------------------------------------------------------
    while (true) {
        XINPUT_STATE state;
        if (XInputGetState(0, &state) == ERROR_SUCCESS) {

            // -----------------------------------------------
            // Device selector poll — same as sample's
            // m_bDeviceUIActive / XHasOverlappedIoCompleted block.
            // Once complete, set device and load stats.
            // -----------------------------------------------
            if ( !g_DeviceReady && XHasOverlappedIoCompleted( &g_DeviceXov ) )
			{
                if ( g_DeviceID != XCONTENTDEVICE_ANY )
                {
                    g_SaveManager.SetDevice( g_DeviceID );
                    g_SaveManager.Load();
                    g_DeviceReady = true;
                }
            }

            // -----------------------------------------------
            // Sign-in poll — detect sign-out and account switches.
            // On any change: mark device not ready and re-launch
            // the device selector for the new (or returning) user.
            // -----------------------------------------------
            {
                bool bSignedIn = true;
                if ( g_SaveManager.PollSignInState( bSignedIn ) )
                {
                    // User changed or signed out
                    g_bNotLoggedIn = !bSignedIn;
                    g_DeviceReady  = false;
                    g_bResultRecorded = false;

                    if ( bSignedIn )
                    {
                        // New account signed in — re-launch device selector
                        ZeroMemory( &g_DeviceXov, sizeof(g_DeviceXov) );
                        ULARGE_INTEGER iBytesRequested = {0};
                        DWORD dwRet = XShowDeviceSelectorUI(
                            MinesweeperSaveManager::GetSignedInUser(),
                            XCONTENTTYPE_SAVEDGAME,
                            0,
                            iBytesRequested,
                            &g_DeviceID,
                            &g_DeviceXov );
                        if ( dwRet != ERROR_IO_PENDING )
                            g_DeviceReady = false;
                    }
                }
                else
                {
                    // No change — keep overlay flag in sync
                    g_bNotLoggedIn = !bSignedIn;
                }
            }

            // -----------------------------------------------
            // Remap listening — runs independently of the main
            // input block so isListening does not block pressA
            // from reaching StartRemapping on the trigger frame.
            // listeningWaitForRelease ensures the A press that
            // started the remap isn't captured as the new binding.
            // -----------------------------------------------
            if (isListening) {
                if (listeningWaitForRelease) {
                    bool triggersReleased = state.Gamepad.bLeftTrigger  <= TRIGGER_THRESHOLD
										 && state.Gamepad.bRightTrigger <= TRIGGER_THRESHOLD;
					bool buttonsReleased  = state.Gamepad.wButtons == 0;
					if (triggersReleased && buttonsReleased)
						listeningWaitForRelease = false;
                }
                else {
                    WORD caughtInput = GetCurrentInput(state);
                    if (caughtInput != 0) {
                        *actionToRemap = caughtInput;
                        ResolveConflicts();
                        SaveControls();
                        LoadControls();
                        isListening   = false;
                        actionToRemap = NULL;
                    }
                }
            }

            if ( g_bNotLoggedIn ) {
                // Input is suspended while no profile is signed in.
                // Dead voices still need cleaning up each frame.
                for (int i = 0; i < (int)g_deadVoices.size(); i++)
                    g_deadVoices[i]->DestroyVoice();
                g_deadVoices.clear();
            }
            else if ( !isListening ) {
                // -----------------------------------------------
                // Normal input — only runs when not remapping.
                // -----------------------------------------------
				for (int i = 0; i < (int)g_deadVoices.size(); i++) {
					g_deadVoices[i]->DestroyVoice();
				}
				g_deadVoices.clear();
                // -----------------------------------------------
                // Input reading
                // -----------------------------------------------
                short stickX = state.Gamepad.sThumbLX;
                short stickY = state.Gamepad.sThumbLY;
                if (!leftjoystick) {
                    stickX = state.Gamepad.sThumbRX;
                    stickY = state.Gamepad.sThumbRY;
                }

				DWORD now = GetTickCount();
                const int DEADZONE = 17000;
                bool moveUp    = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)    || (stickY >  DEADZONE);
                bool moveDown  = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)  || (stickY < -DEADZONE);
                bool moveLeft  = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)  || (stickX < -DEADZONE);
                bool moveRight = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) || (stickX >  DEADZONE);
                if (!dpad) {
                    moveUp    = (stickY >  DEADZONE);
                    moveDown  = (stickY < -DEADZONE);
                    moveLeft  = (stickX < -DEADZONE);
                    moveRight = (stickX >  DEADZONE);
                }

                bool pressOPEN  = IsButtonPressed(state, buttonOPEN_Mask);
                bool pressFLAG  = IsButtonPressed(state, buttonFLAG_Mask);
                bool pressRESET = IsButtonPressed(state, buttonRESET_Mask);
                bool pressBACK  = IsButtonPressed(state, buttonBACK_Mask);
                bool pressX     = IsButtonPressed(state, buttonX_Mask);
                bool pressA     = IsButtonPressed(state, buttonA_Mask);

                OPENbeingPressed = pressOPEN;

                // -----------------------------------------------
                // OPEN (uncover) button
                // -----------------------------------------------
                if (pressOPEN && gameRunning == INGAME && (gameOver != -1 || gameRunning != PAUSEMENU)) {
                    if (!lastOPEN) {
                        OPENpressStartTime  = now;
                        OPENActionTriggered = false;
                        openingTile         = true;
                    }
                    if (!OPENActionTriggered && (now - OPENpressStartTime >= (DWORD)holdTime) && gameOver != 1) {
                        previewRender = true;
                    }
                }
                else {
                    previewRender  = false;
                    openingTile    = false;
                    pressingStart  = false;

                    if (lastOPEN && gameRunning == INGAME) {
                        float fTimeBeforeOpen = myGameTimer.GetTimeAsFloat(); // captured before any Stop()
                        if (!OPENActionTriggered) {
                            if (!boardGenerated) {
                                boardGenerated = true;
                                myBoard.genMines(mineCount, selectedRow, selectedCol);
                                myGameTimer.Start();
                            }
                            if (myBoard.getCell(selectedRow, selectedCol).isMine
                                && !myBoard.getCell(selectedRow, selectedCol).isFlagged
                                && !(gameOver == -1 || gameOver == 1)) {
                                myBoard.getCell(selectedRow, selectedCol).isUncovered = true;
                                gameOver = -1;
                                myBoard.revealAllMines();
                                myBoard.getCell(selectedRow, selectedCol).isBoom = true;
                                float fFinalTime = myGameTimer.GetTimeAsFloat();
                                myGameTimer.Stop();
                                vibrateGamepad(player1Rumble);
                                if ( g_DeviceReady && !g_bResultRecorded ) {
                                    g_SaveManager.RecordGame( GetCurrentDifficulty(tileCountRow, tileCountCol, mineCount), FALSE, fFinalTime );
                                    g_SaveManager.Save();
                                    g_bResultRecorded = true;
                                }
                            }
                            else if (myBoard.getCell(selectedRow, selectedCol).isUncovered && chordingEnabled) {
                                myBoard.chord(selectedRow, selectedCol);
                            }
                            else {
                                myBoard.openUp(selectedRow, selectedCol); 
                            }
                        }
                        OPENActionTriggered = false;
                        // Catch loss triggered inside Board::openUp (flood fill hit a mine)
                        if ( g_DeviceReady && !g_bResultRecorded && gameOver == -1 ) {
                            g_SaveManager.RecordGame( GetCurrentDifficulty(tileCountRow, tileCountCol, mineCount), FALSE, fTimeBeforeOpen );
                            g_SaveManager.Save();
                            g_bResultRecorded = true;
                        }
                    }
                }
                player1Rumble.Update();

                // -----------------------------------------------
                // FLAG button
                // -----------------------------------------------
                if (pressFLAG && !lastFLAG
                    && !myBoard.getCell(selectedRow, selectedCol).isUncovered
                    && gameOver == 0 && gameRunning == INGAME)
                {
                    bool& flagged = myBoard.getCell(selectedRow, selectedCol).isFlagged;
                    flagged = !flagged;
                    if (flagged) {
                        myBoard.increaseNeighCountFlags(selectedRow, selectedCol);
                        flagLeft--;
                        PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
                    }
                    else {
                        myBoard.decreaseNeighCountFlags(selectedRow, selectedCol);
                        flagLeft++;
                        PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                    }
                }

                // -----------------------------------------------
                // RESET (smiley) button
                // -----------------------------------------------
                if (pressRESET && gameRunning == INGAME) {
                    if (!lastRESET) {
                        RESETpressStartTime  = now;
                        RESETActionTriggered = false;
                    }
                    pressingStart = true;
                }
                else {
                    pressingStart = false;
                    if (lastRESET && gameRunning == INGAME) {
                        if (!RESETActionTriggered) {
                            if (mineCount > tileCountRow * tileCountCol)
                                mineCount = tileCountRow * tileCountCol;
                            InitNewGame(myBoard, tileCountRow, tileCountCol, mineCount, player1Rumble);
                            flagLeft = mineCount;
                            gameOver = 0;
                            g_bResultRecorded = false;
                            PlayPCM(pXAudio2, SND_RESET.c_str(), soundVol);
                        }
                        RESETActionTriggered = false;
                    }
                }

                // -----------------------------------------------
                // BACK button
                // -----------------------------------------------
                if (pressBACK && !lastBACK) {
                    if (gameRunning == INGAME) {
                        gameRunning = PAUSEMENU;
                        PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                    }
                    else if (gameRunning == OPTIONS || gameRunning == STATISTICS || gameRunning == EXIT ) {
                        if (gameRunning == STATISTICS) statsScene = 1;
                        gameRunning = PAUSEMENU;
                        buttonMenu  = lastButtonMenu1;
                        PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
                    }
                    else if (gameRunning == PAUSEMENU) {
                        gameRunning = INGAME;
                        buttonMenu  = 1;
                        PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
                    }
                    else if (gameRunning == BOARDSIZE) {
                        gameRunning = INGAME;
                        buttonBoard = 1;
                        PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
                    }
					else if (gameRunning == GAMEPLAYTUT || gameRunning == PATTERNS){
						gameRunning = TUTORIAL;
                        buttonBoard = 1;
						buttonMenu  = lastButtonMenu2;
                        PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
					}
					else if (gameRunning == CUSTOMSIZE) {
                        gameRunning = BOARDSIZE;
                        buttonBoard = 1;
                        PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
                    }
                    else if (gameRunning == SETTINGS) {
                        SaveSettings();
                        LoadSettings();
                        gameRunning = OPTIONS;
                        scene       = 1;
                        buttonMenu  = lastButtonMenu2;
                        PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
                    }
					else if (gameRunning == CONTROLS) {
						SaveControls();
						LoadControls();
						gameRunning = OPTIONS;
                        buttonMenu  = lastButtonMenu2;
                        PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
					}
                    else if (gameRunning == CONFIRM || gameRunning == RESTART ||  gameRunning == RESETCONTROLS) {
                        scene       = 2;
						if (gameRunning == CONFIRM) buttonMenu  = 9;
						else if (gameRunning == RESETCONTROLS) buttonMenu  = 10;
						else if (gameRunning == RESTART) buttonMenu  = 11;
                        gameRunning = SETTINGS;
                        PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
                    }
                    else if (gameRunning == TUTORIAL || gameRunning == CREDITS) {
                        gameRunning = OPTIONS;
                        buttonMenu  = lastButtonMenu2;
						tutScene = 0;
                        PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
                    }
                }

                // -----------------------------------------------
                // X button
                // -----------------------------------------------
                if (pressX && !lastX) {
                    if (gameRunning == INGAME) {
                        gameRunning = BOARDSIZE;
                        buttonBoard = 1;
                        PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                    }
                    else if (gameRunning == BOARDSIZE) {
                        gameRunning = INGAME;
                        buttonBoard = 1;
                        PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
                    }
                }

                // -----------------------------------------------
                // A button
                // -----------------------------------------------
                if (pressA && !lastA) {
					if (buttonBoard == 5) {
						buttonBoard = 1;
						gameRunning = CUSTOMSIZE;
						PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
					}
					else if (gameRunning == TITLESCREEN){
						if ( !g_SaveManager.HasSeenTutorial() ) {
							gameRunning = GAMEPLAYTUT;
							g_SaveManager.SetTutorialSeen( true );
							g_SaveManager.Save();
						}
						else gameRunning = PAUSEMENU;
						
						PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
					}
                    else if (gameRunning == BOARDSIZE || gameRunning == CUSTOMSIZE) {
                        int idx = buttonBoard - 1; 
                        if (idx >= 0 && idx < 4) {
							if (idx == 3 || gameRunning == CUSTOMSIZE) {
								if (customRow > maxRow) tileCountRow = maxRow;
								else tileCountRow = customRow;
								if (customCol > maxCol) tileCountCol = maxCol;
								else tileCountCol = customCol;
								mineCount    = customMines;
							}
							else {
								const BoardPreset& p = boardPresets[idx];
								tileCountRow = p.rows;
								tileCountCol = p.cols;
								mineCount    = p.mines;
							}
                            myBoard.ReInit(tileCountRow, tileCountCol);
                            if (mineCount > tileCountRow * tileCountCol)
                                mineCount = tileCountRow * tileCountCol - 1;
                            totalSafeCells = InitNewGame(myBoard, tileCountRow, tileCountCol, mineCount, player1Rumble);
                            flagLeft    = mineCount;
                            gameOver    = 0;
                            g_bResultRecorded = false;
                            gameRunning = INGAME;
                            selectedRow = 0;
                            selectedCol = 0;
                            buttonBoard = 1;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
							
                        }
                    }
                    else if (gameRunning == PAUSEMENU) {
                        if (buttonMenu == 1) {
                            gameRunning = INGAME;
                            buttonMenu  = 1;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 2) {
                            gameRunning     = OPTIONS;
                            lastButtonMenu1 = buttonMenu;
                            buttonMenu      = 1;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 3) {
                            gameRunning     = STATISTICS;
                            lastButtonMenu1 = buttonMenu;
                            buttonMenu      = 1;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 4) {
                            gameRunning     = EXIT;
                            lastButtonMenu1 = buttonMenu;
                            buttonMenu      = 1;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                    }
                    else if (gameRunning == EXIT) {
                        if (buttonMenu == 1){
							gameRunning = PAUSEMENU;
							buttonMenu  = lastButtonMenu1;
							PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
						}
						else if (buttonMenu == 2){
							CleanUp();
							pMasteringVoice->DestroyVoice();
							SAFE_RELEASE(pXAudio2);
							XLaunchNewImage(NULL, NULL);
						}
                    }
					else if (gameRunning == CONTROLS) {
						if (buttonMenu == 1){
							dpad = !dpad;
							SaveControls();
							LoadControls();
							PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
						}
						else if (buttonMenu == 2){
							leftjoystick = !leftjoystick;
							SaveControls();
							LoadControls();
							PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
						}
						else if (buttonMenu >= 3 && buttonMenu <= 8) {
							WORD* remapTargets[6] = {
								&buttonOPEN_Mask,
								&buttonFLAG_Mask,
								&buttonBACK_Mask,
								&buttonRESET_Mask,
								&buttonA_Mask,
								&buttonX_Mask
							};
							StartRemapping(*remapTargets[buttonMenu - 3]);
							listeningWaitForRelease = true;
							PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
						}
					}	
                    else if (gameRunning == CONFIRM) {
                        if (buttonMenu == 1) {
                            RefreshPackList();
                            gameRunning = SETTINGS;
                            buttonMenu  = 9;
                            scene       = 2;
                            LoadSettings();
                            PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 2) {
                            gameRunning = SETTINGS;
                            buttonMenu  = 9;
                            scene       = 2;
                            CreateSettings();
                            LoadSettings();
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                    }
					else if (gameRunning == RESETCONTROLS) {
                        if (buttonMenu == 1) {
                            RefreshPackList();
                            gameRunning = SETTINGS;
                            buttonMenu  = 10;
                            scene       = 2;
                            LoadSettings();
                            PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 2) {
                            gameRunning = SETTINGS;
                            buttonMenu  = 10;
                            scene       = 2;
                            CreateControls();
                            LoadControls();
                            LoadSettings();
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                    }
					else if (gameRunning == RESTART) {
                        if (buttonMenu == 1) {
                            RefreshPackList();
                            gameRunning = SETTINGS;
                            buttonMenu  = 10;
                            scene       = 2;
                            LoadSettings();
                            PlayPCM(pXAudio2, SND_UNFLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 2) {
                            CleanUp();
							pMasteringVoice->DestroyVoice();
							SAFE_RELEASE(pXAudio2);
                            RestartGame();
                        }
                    }
					else if (gameRunning == TUTORIAL) {
						lastButtonMenu2 = buttonMenu;
						if (buttonMenu == 1) {
                            gameRunning     = GAMEPLAYTUT;
                            buttonMenu      = 1;
							tutScene = 0;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 2) {
                            gameRunning     = PATTERNS;
							tutScene = 0;
                            buttonMenu      = 1;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
					}
					else if (gameRunning == GAMEPLAYTUT && tutScene == 14) {
						if (buttonMenu == 1) {
                            gameRunning     = INGAME;
							tutScene        = 0;
                            buttonMenu      = 1;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 2) {
							tutScene        = 0;
                            gameRunning     = PATTERNS;
                            buttonMenu      = 1;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
					}
                    else if (gameRunning == OPTIONS) {
                        if (buttonMenu == 1) {
                            gameRunning     = TUTORIAL;
                            lastButtonMenu2 = buttonMenu;
                            buttonMenu      = 1;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 2) {
                            gameRunning     = CONTROLS;
                            lastButtonMenu2 = buttonMenu;
                            buttonMenu      = 1;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 3) {
                            LoadSettings();
                            RefreshPackList();
                            gameRunning     = SETTINGS;
                            lastButtonMenu2 = buttonMenu;
                            buttonMenu      = 1;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 4) {
                            gameRunning     = CREDITS;
                            lastButtonMenu2 = buttonMenu;
							tutScene = 0;
                            buttonMenu      = 1;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                    }
                    else if (gameRunning == SETTINGS) {
                        if (buttonMenu == 1) {
                            g_IsMuted = !g_IsMuted;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 2) {
                            chordingEnabled = !chordingEnabled;
                            PlayPCM(pXAudio2, chordingEnabled ? SND_UNFLAG.c_str() : SND_FLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 3) {
                            g_VibrationsEnabled = !g_VibrationsEnabled;
                            PlayPCM(pXAudio2, g_VibrationsEnabled ? SND_FLAG.c_str() : SND_UNFLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 8) {
                            GAME_ROOT_PATH = packList[selectedPackIndex];
                            SaveSettings();
                            LoadSettings();
							ReloadTxt();
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                        else if (buttonMenu == 9) {
                            buttonMenu  = 1;
                            gameRunning = CONFIRM;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
						else if (buttonMenu == 10) {
                            buttonMenu  = 1;
                            gameRunning = RESETCONTROLS;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
						else if (buttonMenu == 11) {
                            buttonMenu  = 1;
                            gameRunning = RESTART;
                            PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                        }
                    }
                }

                // BOARDSIZE also triggers on pressRESET
                if (pressRESET && !lastRESET && gameRunning == BOARDSIZE) {
                    int idx = buttonBoard - 1;
                    if (idx >= 0 && idx < 3) {
                        const BoardPreset& p = boardPresets[idx];
                        tileCountRow = p.rows;
                        tileCountCol = p.cols;
                        mineCount    = p.mines;
                        myBoard.ReInit(tileCountRow, tileCountCol);
                        if (mineCount > tileCountRow * tileCountCol)
                            mineCount = tileCountRow * tileCountCol - 1;
                        totalSafeCells = InitNewGame(myBoard, tileCountRow, tileCountCol, mineCount, player1Rumble);
                        flagLeft    = mineCount;
                        gameOver    = 0;
                        g_bResultRecorded = false;
                        gameRunning = INGAME;
                        selectedRow = 0;
                        selectedCol = 0;
                        buttonBoard = 1;
                        PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
                    }
                }

                // -----------------------------------------------
                // Win check
                // -----------------------------------------------
                if (gameRunning == INGAME && gameOver == 0 && flagLeft >= 0 && checkWin(totalSafeCells)) {
                    gameOver = 1;
                    myBoard.revealAllMines();
                    myGameTimer.Stop();
                    if ( g_DeviceReady && !g_bResultRecorded ) {
                        g_SaveManager.RecordGame( GetCurrentDifficulty(tileCountRow, tileCountCol, mineCount), TRUE, myGameTimer.GetTimeAsFloat() );
                        g_SaveManager.Save();
                        g_bResultRecorded = true;
                    }
                }

                // -----------------------------------------------
                // Movement + cooldown
                // -----------------------------------------------
                if (now - lastMoveTime > (DWORD)movecooldownms) {

                    // Up
                    if (moveUp) {	
                        if (gameRunning == INGAME) {
                            selectedRow = max(0, selectedRow - 1);
                            lastMoveTime = now;
                            movedThisFrame = true;  
                        }
                        else if ((gameRunning == PAUSEMENU || gameRunning == OPTIONS) && buttonMenu > 1 && buttonMenu <= 4) {
                            buttonMenu--;
                            lastMoveTime = now;
                            PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
                        }
                        else if (gameRunning == SETTINGS && buttonMenu > 1 && buttonMenu <= 11) {
                            if (buttonMenu == 7) scene--;
                            buttonMenu--;
                            lastMoveTime = now;
                            PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
                        }
						else if (gameRunning == CONTROLS && buttonMenu > 1 && buttonMenu <= 8) {
                            buttonMenu--;
                            lastMoveTime = now;
                            PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
                        }
						else if ((gameRunning == TUTORIAL || (gameRunning == GAMEPLAYTUT && tutScene == 14)) && buttonMenu > 1 && buttonMenu <= 2) {
                            buttonMenu--;
                            lastMoveTime = now;
                            PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
                        }
						else if (gameRunning == CUSTOMSIZE) {
							if (buttonBoard == 1 && customRow < maxRow) {
								customRow++;
								if (customMines > (customRow * customCol) - 1)	
									customMines = (customRow * customCol) - 1;
							}
							else if (buttonBoard == 2 && customCol < maxCol) {
								customCol++;
								if (customMines > (customRow * customCol) - 1)
									customMines = (customRow * customCol) - 1;
							}
							else if (buttonBoard == 3 && customMines < (customRow * customCol) - 1)
								customMines++;
							lastMoveTime = now;
							PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
						}
						else if (gameRunning == BOARDSIZE) {
							if (buttonBoard == 4) { buttonBoard = (boardSizeReturnButton == 2 || boardSizeReturnButton == 3) ? 1 : boardSizeReturnButton; lastMoveTime = now; PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol); }
							else if (buttonBoard == 5) { buttonBoard = boardSizeReturnButton; lastMoveTime = now; PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol); }
						}
						else if (gameRunning == STATISTICS && statsScene > 1) { statsScene--; lastMoveTime = now; PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);}
                    }

                    // Down
                    if (moveDown) {
                        if (gameRunning == INGAME) {
                            selectedRow = min(tileCountRow - 1, selectedRow + 1);
                            lastMoveTime = now;                            
							movedThisFrame = true;  

                        }
                        else if ((gameRunning == PAUSEMENU || gameRunning == OPTIONS) && buttonMenu >= 1 && buttonMenu < 4) {
                            buttonMenu++;
                            lastMoveTime = now;
                            PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
                        }
                        else if (gameRunning == SETTINGS && buttonMenu >= 1 && buttonMenu < 11) {
                            if (buttonMenu == 6) scene++;
                            buttonMenu++;
                            lastMoveTime = now;
                            PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
                        }
						else if ((gameRunning == TUTORIAL || (gameRunning == GAMEPLAYTUT && tutScene == 14)) && buttonMenu >= 1 && buttonMenu < 2) {
                            buttonMenu++;
                            lastMoveTime = now;
                            PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
                        }
						else if (gameRunning == CONTROLS && buttonMenu >= 1 && buttonMenu < 8) {
                            buttonMenu++;
                            lastMoveTime = now;
                            PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
                        }
						else if (gameRunning == CUSTOMSIZE) {
							if (buttonBoard == 1 && customRow > 1) {
								customRow--;
								if (customMines > (customRow * customCol) - 1)
									customMines = (customRow * customCol) - 1;
							}
							else if (buttonBoard == 2 && customCol > 1) {
								customCol--;
								if (customMines > (customRow * customCol) - 1)
									customMines = (customRow * customCol) - 1;
							}
							else if (buttonBoard == 3 && customMines > 0)
								customMines--;
							lastMoveTime = now;
							PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
						}
						else if (gameRunning == BOARDSIZE) {
							if (buttonBoard == 1) { buttonBoard = 4; lastMoveTime = now; PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol); }
							else if (buttonBoard == 2) { buttonBoard = 5; boardSizeReturnButton = 2; lastMoveTime = now; PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol); }
							else if (buttonBoard == 3) { buttonBoard = 5; boardSizeReturnButton = 3; lastMoveTime = now; PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol); }
						}
						else if (gameRunning == STATISTICS && statsScene < 3) { statsScene++; lastMoveTime = now; PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);}
						

                    }

                    // Left
                    if (moveLeft) {
                        if (gameRunning == INGAME) {
                            selectedCol = max(0, selectedCol - 1);
                            lastMoveTime = now;
                            movedThisFrame = true;  
                        }
                        else if (gameRunning == BOARDSIZE && ((buttonBoard > 1 && buttonBoard <= 3) || (buttonBoard > 4 && buttonBoard <= 5))) {
							buttonBoard--;
							lastMoveTime = now;
							PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
						}
						else if (gameRunning == BOARDSIZE && buttonBoard == 4) {
							buttonBoard = 1;
							lastMoveTime = now;
							PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
						}
                        else if ((gameRunning == EXIT || gameRunning == CONFIRM || gameRunning == RESTART || gameRunning == RESETCONTROLS) && buttonMenu == 2) {
                            buttonMenu--;
                            lastMoveTime = now;
                            PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
                        }
						else if ((gameRunning == GAMEPLAYTUT || gameRunning == PATTERNS) && tutScene > 0) {
							tutScene--;
							lastMoveTime = now;
							PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
						}
                        else if (gameRunning == SETTINGS && buttonMenu == 8) {
                            selectedPackIndex--;
                            if (selectedPackIndex < 0) selectedPackIndex = (int)packList.size() - 1;
                            lastMoveTime = now;
                            PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
                        }
						else if (gameRunning == CUSTOMSIZE && buttonBoard > 1 && buttonBoard <= 3) {
							buttonBoard--;
                            lastMoveTime = now;
                            PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
						}
                    }

                    // Right
                    if (moveRight) {
                        if (gameRunning == INGAME) {
                            selectedCol = min(tileCountCol - 1, selectedCol + 1);
                            lastMoveTime = now;
							movedThisFrame = true;  
                        }
                        else if (gameRunning == BOARDSIZE && ((buttonBoard >= 1 && buttonBoard < 3) || (buttonBoard >= 4 && buttonBoard < 5))) {
							buttonBoard++;
							lastMoveTime = now;
							PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
						}
						else if (gameRunning == BOARDSIZE && buttonBoard == 5) {
							buttonBoard = 3;
							lastMoveTime = now;
							PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
						}
                        else if ((gameRunning == EXIT || gameRunning == CONFIRM || gameRunning == RESTART || gameRunning == RESETCONTROLS) && buttonMenu == 1) {
                            buttonMenu++;
                            lastMoveTime = now;
                            PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
                        }
						else if ((gameRunning == GAMEPLAYTUT && tutScene < 14) || (gameRunning == PATTERNS && tutScene < 16)) {
							tutScene++;
							lastMoveTime = now;
							PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
						}
                        else if (gameRunning == SETTINGS && buttonMenu == 8) {
                            selectedPackIndex++;
                            if (selectedPackIndex >= (int)packList.size()) selectedPackIndex = 0;
                            lastMoveTime = now;
                            PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
                        }
						else if (gameRunning == CUSTOMSIZE && buttonBoard >= 1 && buttonBoard < 3) {
							buttonBoard++;
                            lastMoveTime = now;
                            PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
						}
                    }
					if (movedThisFrame){
						PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
						movedThisFrame = false;
					}
					// -----------------------------------------------
					// MEOW
					// -----------------------------------------------
					{
						bool comboHeld =
							(state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)  &&
							(state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) &&
							(state.Gamepad.bLeftTrigger  > 200)                      &&
							(state.Gamepad.bRightTrigger > 200);

						if (comboHeld) {
							if (debugComboPressStart == 0)
								debugComboPressStart = GetTickCount();

							if (!debugComboActive && (GetTickCount() - debugComboPressStart >= 5000)
								&& gameRunning == CREDITS) {
								debugComboActive = true;
								tutScene         = 1;
								PlayPCM(pXAudio2, SND_FLAG.c_str(), soundVol);
							}
						}
						else {
							debugComboPressStart = 0;
							debugComboActive     = false;
						}
					}
                    // -----------------------------------------------
                    // Sliders
                    // -----------------------------------------------
                    if (gameRunning == SETTINGS && buttonMenu >= 4 && buttonMenu <= 7) {
                        int      sliderIdx = buttonMenu - 4; // 0-3
                        SliderCfg& s       = sliders[sliderIdx];

                        if (moveLeft || moveRight) {
                            DWORD currentTime = now;

                            if (buttonPressStartTime == 0) {
                                buttonPressStartTime = currentTime;
                                lastRepeatTime       = currentTime;

                                if (moveLeft  && *s.pVal > s.minVal)  *s.pVal -= s.step;
                                else if (moveRight && *s.pVal < s.maxVal) *s.pVal += s.step;

                                PlayPCM(pXAudio2, SND_MOVE.c_str(), soundVol);
                            }

                            DWORD holdDuration = currentTime - buttonPressStartTime;
                            DWORD currentDelay = (holdDuration > 750) ? 30 : 180;

                            if (currentTime - lastRepeatTime > currentDelay) {
                                if (moveLeft  && *s.pVal > s.minVal)  *s.pVal -= s.step;
                                else if (moveRight && *s.pVal < s.maxVal) *s.pVal += s.step;

                                lastRepeatTime = currentTime;
                            }
                        }
                        else {
                            buttonPressStartTime = 0;
                        }
                    }
                }

                lastOPEN  = pressOPEN;
                lastFLAG  = pressFLAG;
                lastRESET = pressRESET;
                lastBACK  = pressBACK;
                lastX     = pressX;
                lastA     = pressA;
            }
        }

        // -------------------------------------------------------
        // Render
        // -------------------------------------------------------
        if ( g_bNotLoggedIn )
        {
            g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);
            g_pd3dDevice->BeginScene();
            RenderNotLoggedIn();
            g_pd3dDevice->EndScene();
        }
        else
        {
            g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                D3DCOLOR_ARGB(255, fontBgR, fontBgG, fontBgB), 1.0f, 0);
            g_pd3dDevice->BeginScene();
            Render(myBoard, selectedRow, selectedCol, tileCountRow, tileCountCol, flagLeft);
            g_pd3dDevice->EndScene();
        }
        g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        OPENbeingPressed = false;
    }

    CleanUp();
    pMasteringVoice->DestroyVoice();
    SAFE_RELEASE(pXAudio2);
}