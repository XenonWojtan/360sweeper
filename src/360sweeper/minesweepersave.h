//--------------------------------------------------------------------------------------
// MinesweeperSave.h
// Xbox 360 Minesweeper — stats saved via XContent.
//--------------------------------------------------------------------------------------
#ifndef MINESWEEPER_SAVE_H
#define MINESWEEPER_SAVE_H
#include "StdAfx.h"
#include <xtl.h>
#include <xam.h>

// ---- XContent identifiers  ------
extern CHAR g_msSaveRoot[];
extern CHAR g_msSaveFile[];
// ---- Binary format ------------------------------------------------------------------
#define MS_SAVE_MAGIC    0x4D494E45  // 'MINE' — detects corrupt/wrong files
#define MS_SAVE_VERSION  0x00000005  // bump when struct layout changes
#define MS_NO_BEST_TIME  (-1.0f)     // fBestTime sentinel — no win recorded yet

// ---- Difficulty index ---------------------------------------------------------------
enum MSDifficulty
{
    MS_DIFF_BEGINNER     = 0,   //  9x9,  10 mines
    MS_DIFF_INTERMEDIATE = 1,   // 16x16, 40 mines
    MS_DIFF_EXPERT       = 2,   // 16x30, 99 mines
    MS_DIFF_CUSTOM       = 3,   // user board
    MS_DIFF_COUNT        = 4
};

// ---- Per-mode stats ------------------------------
struct MSModeStats
{
    float fBestTime;      
    DWORD dwGamesPlayed;
    DWORD dwGamesWon;
    DWORD dwGamesLost;
    float fReserved[3];
};

// ---- Cross-mode totals --------------------------------------------------------------
struct MSTotals
{
    DWORD dwTotalGamesPlayed;
    DWORD dwTotalGamesWon;
    DWORD dwTotalGamesLost;
    DWORD dwReserved[5];
};

// ---- Full save struct — POD only, written directly to disk -------------------------
struct MinesweeperSaveData
{
    DWORD       dwMagic;
    DWORD       dwVersion;
    MSModeStats modes[MS_DIFF_COUNT];
    MSTotals    totals;
    DWORD       bTutorialSeen;   // 1 = user has completed tutorial, 0 = hasn't
    DWORD       dwReserved[15]; 
};

// ---- Save manager -------------------------------------------------------------------
// Usage:
//   Startup :  g_SaveManager.SetDevice(deviceID);  g_SaveManager.Load();
//   Game end:  g_SaveManager.RecordGame(diff, bWon, timer.GetTimeAsFloat());
//              g_SaveManager.Save();
//   Display :  g_SaveManager.GetBeginnerStats().fBestTime  etc.
// -----------------------------------------------------------------------------
class MinesweeperSaveManager
{
public:
    MinesweeperSaveManager();

    // Call once you have a device ID from XShowDeviceSelectorUI
    void SetDevice( XCONTENTDEVICEID deviceID );

    // Mirrors WriteSaveGame from the ATG sample — writes MinesweeperSaveData
    DWORD Save();

    // Mirrors ReadSaveGame from the ATG sample — reads MinesweeperSaveData
    // Returns ERROR_SUCCESS on success or clean first-run (file not found).
    DWORD Load();

    // Update stats then call Save()
    void RecordGame( MSDifficulty diff, BOOL bWon, float fTime );

    // Zero everything
    void ResetStats();

    // Accessors
    const MSModeStats& GetBeginnerStats()     const { return m_Data.modes[MS_DIFF_BEGINNER];     }
    const MSModeStats& GetIntermediateStats() const { return m_Data.modes[MS_DIFF_INTERMEDIATE]; }
    const MSModeStats& GetExpertStats()       const { return m_Data.modes[MS_DIFF_EXPERT];       }
    const MSModeStats& GetCustomStats()       const { return m_Data.modes[MS_DIFF_CUSTOM];       }
    const MSTotals&    GetTotals()            const { return m_Data.totals;                      }

    const MinesweeperSaveData& GetData() const { return m_Data; }
	bool HasSeenTutorial() const { return m_Data.bTutorialSeen != 0; }
	void SetTutorialSeen( bool seen ) { m_Data.bTutorialSeen = seen ? 1 : 0; }

    // Returns index of first signed-in user (XUSER_INDEX_NONE if nobody signed in)
    static DWORD GetSignedInUser();

    // Returns true if at least one user is currently signed in
    static bool IsUserSignedIn();

    // Call every frame — detects sign-out and account switches.
    // Returns true if the signed-in user changed (including sign-out).
    // When an account switch occurs it resets the device and reloads save data
    // for the new user automatically (caller must re-run XShowDeviceSelectorUI).
    bool PollSignInState( bool& bSignedIn );

private:
    MinesweeperSaveData m_Data;
    XCONTENTDEVICEID    m_DeviceID;

    // Tracks which user slot was active last frame (XUSER_INDEX_NONE = nobody)
    DWORD               m_LastUserIndex;
};

// Global instance — extern this in any file that needs to read stats
extern MinesweeperSaveManager g_SaveManager;

#endif // MINESWEEPER_SAVE_H