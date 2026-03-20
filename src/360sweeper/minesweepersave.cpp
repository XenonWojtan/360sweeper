//--------------------------------------------------------------------------------------
// MinesweeperSave.cpp
//--------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "MinesweeperSave.h"

// MinesweeperSave.cpp
CHAR g_msSaveRoot[] = "save";
CHAR g_msSaveFile[] = "save:\\360sweeper.dat";
// Global instance
MinesweeperSaveManager g_SaveManager;

//--------------------------------------------------------------------------------------
MinesweeperSaveManager::MinesweeperSaveManager()
    : m_DeviceID( XCONTENTDEVICE_ANY )
    , m_LastUserIndex( XUSER_INDEX_NONE )
{
    ResetStats();
}

//--------------------------------------------------------------------------------------
void MinesweeperSaveManager::SetDevice( XCONTENTDEVICEID deviceID )
{
    m_DeviceID = deviceID;
}

//--------------------------------------------------------------------------------------
// GetSignedInUser — equivalent of ATG::SignIn::GetSignedInUser()
//--------------------------------------------------------------------------------------
DWORD MinesweeperSaveManager::GetSignedInUser()
{
    for ( DWORD i = 0; i < 4; ++i )
        if ( XUserGetSigninState( i ) != eXUserSigninState_NotSignedIn )
            return i;
    return XUSER_INDEX_NONE; // nobody signed in
}

//--------------------------------------------------------------------------------------
// IsUserSignedIn — returns true if at least one user slot is signed in
//--------------------------------------------------------------------------------------
bool MinesweeperSaveManager::IsUserSignedIn()
{
    return ( GetSignedInUser() != XUSER_INDEX_NONE );
}

//--------------------------------------------------------------------------------------
// PollSignInState — call every frame.
// Sets bSignedIn to reflect current state.
// Returns true when the active user changed (sign-out OR account switch).
// On an account switch: resets device + in-memory stats so the caller knows
// it must re-run XShowDeviceSelectorUI for the new profile.
//--------------------------------------------------------------------------------------
bool MinesweeperSaveManager::PollSignInState( bool& bSignedIn )
{
    DWORD currentUser = GetSignedInUser();
    bSignedIn = ( currentUser != XUSER_INDEX_NONE );

    if ( currentUser == m_LastUserIndex )
        return false; // no change

    // Something changed — save any pending data under the OLD user before switching
    if ( m_LastUserIndex != XUSER_INDEX_NONE && m_DeviceID != XCONTENTDEVICE_ANY )
        Save();

    // Reset device — caller must re-run device selector for the new user
    m_DeviceID     = XCONTENTDEVICE_ANY;
    m_LastUserIndex = currentUser;

    if ( bSignedIn )
    {
        // New user signed in: clear stats in memory until device is chosen + loaded
        ResetStats();
    }

    return true; 
}

//--------------------------------------------------------------------------------------
// Save — mirrors Sample::WriteSaveGame exactly.
// szFileName   -> MS_CONTENT_FILENAME  (same role as g_szSaveRoot / szFileName arg)
// szDisplayName-> L"Minesweeper"
// file written -> g_msSaveFile  (same role as g_szSaveGame)
// data written -> MinesweeperSaveData struct instead of dummy text
//--------------------------------------------------------------------------------------
DWORD MinesweeperSaveManager::Save()
{
    // Stamp header before writing
    m_Data.dwMagic   = MS_SAVE_MAGIC;
    m_Data.dwVersion = MS_SAVE_VERSION;

    // ---- exact sample boilerplate ------------------------------------------------
    HANDLE hEventComplete = CreateEvent( NULL, FALSE, FALSE, NULL );
    if ( hEventComplete == NULL )
        return GetLastError();

    XOVERLAPPED xov = {0};
    xov.hEvent = hEventComplete;

    XCONTENT_DATA contentData = {0};
    strcpy_s ( contentData.szFileName,    "360sweeper"  );
    wcscpy_s ( contentData.szDisplayName, L"360sweeper" );
    contentData.dwContentType = XCONTENTTYPE_SAVEDGAME;
    contentData.DeviceID      = m_DeviceID;

    DWORD dwErr = XContentCreate( GetSignedInUser(), g_msSaveRoot, &contentData,
                                  XCONTENTFLAG_CREATEALWAYS, NULL, NULL, &xov );
    if ( dwErr != ERROR_IO_PENDING )
    {
        CloseHandle( hEventComplete );
        return dwErr;
    }

    if ( XGetOverlappedResult( &xov, NULL, TRUE ) == ERROR_SUCCESS )
    {
        HANDLE hFile = CreateFile( g_msSaveFile, GENERIC_WRITE, 0,
                                   NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

        if ( hFile != INVALID_HANDLE_VALUE )
        {
            DWORD dwWritten;
            if ( WriteFile( hFile, (VOID*)&m_Data, sizeof(MinesweeperSaveData),
                            &dwWritten, NULL ) == 0 )
            {
                CloseHandle( hFile );
                XContentClose( g_msSaveRoot, &xov );
                XGetOverlappedResult( &xov, NULL, TRUE );
                CloseHandle( hEventComplete );
                return GetLastError();
            }
            CloseHandle( hFile );
        }
        else
        {
            XContentClose( g_msSaveRoot, &xov );
            XGetOverlappedResult( &xov, NULL, TRUE );
            CloseHandle( hEventComplete );
            return GetLastError();
        }
    }

    XContentClose( g_msSaveRoot, &xov );
    XGetOverlappedResult( &xov, NULL, TRUE );
    CloseHandle( hEventComplete );

    return ERROR_SUCCESS;
}

//--------------------------------------------------------------------------------------
// Load — mirrors Sample::ReadSaveGame exactly.
// On first run (package not found) we reset to defaults and return ERROR_SUCCESS.
//--------------------------------------------------------------------------------------
DWORD MinesweeperSaveManager::Load()
{
    HANDLE hEventComplete = CreateEvent( NULL, FALSE, FALSE, NULL );
    if ( hEventComplete == NULL )
        return GetLastError();

    XOVERLAPPED xov = {0};
    xov.hEvent = hEventComplete;

    XCONTENT_DATA contentData = {0};
    strcpy_s ( contentData.szFileName,    "360sweeper"  );
    wcscpy_s ( contentData.szDisplayName, L"360sweeper" );
    contentData.dwContentType = XCONTENTTYPE_SAVEDGAME;
    contentData.DeviceID      = m_DeviceID;

    DWORD dwErr = XContentCreate( GetSignedInUser(), g_msSaveRoot, &contentData,
                                  XCONTENTFLAG_OPENEXISTING, NULL, NULL, &xov );
    if ( dwErr != ERROR_IO_PENDING )
    {
        CloseHandle( hEventComplete );
        // Package doesn't exist yet — first run
        ResetStats();
        return ERROR_SUCCESS;
    }

    dwErr = XGetOverlappedResult( &xov, NULL, TRUE );
    if ( dwErr == ERROR_SUCCESS )
    {
        HANDLE hFile = CreateFile( g_msSaveFile, GENERIC_READ,
                                   FILE_SHARE_READ, NULL, OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL, NULL );

        if ( hFile != INVALID_HANDLE_VALUE )
        {
            MinesweeperSaveData tempData = {0};
            DWORD dwRead = 0;

            if ( ReadFile( hFile, (VOID*)&tempData, sizeof(MinesweeperSaveData),
                           &dwRead, NULL ) == 0 )
            {
                CloseHandle( hFile );
                XContentClose( g_msSaveRoot, &xov );
                XGetOverlappedResult( &xov, NULL, TRUE );
                CloseHandle( hEventComplete );
                return GetLastError();
            }

            CloseHandle( hFile );

            // Validate magic and version — reset cleanly if corrupt/old
            if ( dwRead == sizeof(MinesweeperSaveData)
                 && tempData.dwMagic   == MS_SAVE_MAGIC
                 && tempData.dwVersion == MS_SAVE_VERSION )
            {
                m_Data = tempData;
            }
            else
            {
                ResetStats();
            }
        }
        else
        {
            // File missing inside package — first run
            ResetStats();
        }
    }
    else
    {
        // XContentCreate with OPENEXISTING failed — first run or no device
        XContentClose( g_msSaveRoot, &xov );
        XGetOverlappedResult( &xov, NULL, TRUE );
        CloseHandle( hEventComplete );
        ResetStats();
        return ERROR_SUCCESS;
    }

    XContentClose( g_msSaveRoot, &xov );
    XGetOverlappedResult( &xov, NULL, TRUE );
    CloseHandle( hEventComplete );

    return ERROR_SUCCESS;
}

//--------------------------------------------------------------------------------------
void MinesweeperSaveManager::RecordGame( MSDifficulty diff, BOOL bWon, float fTime )
{
    if ( diff < 0 || diff >= MS_DIFF_COUNT ) return;

    MSModeStats& mode = m_Data.modes[diff];
    mode.dwGamesPlayed++;

    if ( bWon )
    {
        mode.dwGamesWon++;
        if ( mode.fBestTime < 0.0f || fTime < mode.fBestTime )
            mode.fBestTime = fTime;
    }
    else
    {
        mode.dwGamesLost++;
    }

    m_Data.totals.dwTotalGamesPlayed++;
    if ( bWon ) m_Data.totals.dwTotalGamesWon++;
    else        m_Data.totals.dwTotalGamesLost++;
}

//--------------------------------------------------------------------------------------
void MinesweeperSaveManager::ResetStats()
{
    ZeroMemory( &m_Data, sizeof(MinesweeperSaveData) );
    m_Data.dwMagic        = MS_SAVE_MAGIC;
    m_Data.dwVersion      = MS_SAVE_VERSION;
    m_Data.bTutorialSeen  = 0;
    for ( int i = 0; i < MS_DIFF_COUNT; ++i )
        m_Data.modes[i].fBestTime = MS_NO_BEST_TIME;
}