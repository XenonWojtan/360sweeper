#include "StdAfx.h"
#include "globals.h"
#include "board.h"

// -------------------------------------------------------
// BOARD — constructor
// -------------------------------------------------------
Board::Board(int r, int c, RumbleManager* pRumble)
    : row(r), col(c), cells(r * c), m_pRumble(pRumble)
{
}

// -------------------------------------------------------
// Board::ReInit
// -------------------------------------------------------
void Board::ReInit(int newRows, int newCols)
{
    // 1. Clear old data and force memory release
    std::vector<cell>().swap(cells);

    // 2. Update dimensions
    row = newRows;
    col = newCols;

    // 3. Allocate new grid
    cells.resize(row * col);

    // 4. Reset game state globals
    gameOver           = 0;
    uncoveredSafeCells = 0;
	
    reset();
}

// -------------------------------------------------------
// Board::SetRumbleManager
// -------------------------------------------------------
void Board::SetRumbleManager(RumbleManager* pRumble)
{
    m_pRumble = pRumble;
}

// -------------------------------------------------------
// Board::setMine
// -------------------------------------------------------
void Board::setMine(int r, int c)
{
    if (r >= 0 && r < row && c >= 0 && c < col)
        cells[r * col + c].isMine = true;
}

// -------------------------------------------------------
// Board::setUncovered
// -------------------------------------------------------
void Board::setUncovered(int r, int c)
{
    if (r >= 0 && r < row && c >= 0 && c < col)
        cells[r * col + c].isUncovered = true;
}

// -------------------------------------------------------
// Board::getCell
// -------------------------------------------------------
cell& Board::getCell(int r, int c)
{
    return cells[r * col + c];
}

// -------------------------------------------------------
// Board::isValid
// -------------------------------------------------------
bool Board::isValid(int r, int c)
{
    return (r >= 0 && r < row && c >= 0 && c < col);
}

// -------------------------------------------------------
// Board::increaseNeighCount
// -------------------------------------------------------
void Board::increaseNeighCount(int rowS, int colS)
{
    for (int deltaR = -1; deltaR <= 1; deltaR++) {
        for (int deltaC = -1; deltaC <= 1; deltaC++) {
            if (deltaR == 0 && deltaC == 0) continue;
            int nRow = rowS + deltaR;
            int nCol = colS + deltaC;
            if (isValid(nRow, nCol))
                cells[nRow * col + nCol].minesAround++;
        }
    }
}

// -------------------------------------------------------
// Board::increaseNeighCountFlags
// -------------------------------------------------------
void Board::increaseNeighCountFlags(int rowS, int colS)
{
    for (int deltaR = -1; deltaR <= 1; deltaR++) {
        for (int deltaC = -1; deltaC <= 1; deltaC++) {
            if (deltaR == 0 && deltaC == 0) continue;
            int nRow = rowS + deltaR;
            int nCol = colS + deltaC;
            if (isValid(nRow, nCol))
                cells[nRow * col + nCol].flagsAround++;
        }
    }
}

// -------------------------------------------------------
// Board::decreaseNeighCountFlags
// -------------------------------------------------------
void Board::decreaseNeighCountFlags(int rowS, int colS)
{
    for (int deltaR = -1; deltaR <= 1; deltaR++) {
        for (int deltaC = -1; deltaC <= 1; deltaC++) {
            if (deltaR == 0 && deltaC == 0) continue;
            int nRow = rowS + deltaR;
            int nCol = colS + deltaC;
            if (isValid(nRow, nCol))
                cells[nRow * col + nCol].flagsAround--;
        }
    }
}

// -------------------------------------------------------
// Board::revealAllMines
// -------------------------------------------------------
void Board::revealAllMines()
{
    for (size_t i = 0; i < cells.size(); ++i) {
        if (cells[i].isMine)
            cells[i].isUncovered = true;
    }
}

// -------------------------------------------------------
// Board::revealAllTiles
// -------------------------------------------------------
void Board::revealAllTiles()
{
    for (size_t i = 0; i < cells.size(); ++i)
        cells[i].isUncovered = true;
}

// -------------------------------------------------------
// Board::openUp
// -------------------------------------------------------
void Board::openUp(int r, int c)
{
    // 1. Basic checks
    if (!isValid(r, c) || getCell(r, c).isUncovered || getCell(r, c).isFlagged || gameOver != 0)
        return;

    cell& current = getCell(r, c);
    current.isUncovered = true;

    // 2. Mine hit
    if (current.isMine) {
        gameOver = -1;
        revealAllMines();
        current.isBoom = true;
        myGameTimer.Stop();
        if (g_VibrationsEnabled) {
            if (m_pRumble) m_pRumble->Pulse(vibrationStrength, vibrationStrength, 250);
        }
        return;
    }

    // 3. Safe cell
    uncoveredSafeCells++;

    // 4. Recursive flood-fill for empty tiles
    if (current.minesAround == 0) {
        for (int deltaR = -1; deltaR <= 1; deltaR++) {
            for (int deltaC = -1; deltaC <= 1; deltaC++) {
                if (deltaR == 0 && deltaC == 0) continue;
                openUp(r + deltaR, c + deltaC);
            }
        }
    }
}

// -------------------------------------------------------
// Board::genMines
// -------------------------------------------------------
void Board::genMines(int mineCount, int startR, int startC)
{
    if (mineCount > row * col) mineCount = row * col;

    for (int i = 0; i < mineCount; i++) {
        int mineColReq = rand() % col;
        int mineRowReq = rand() % row;

        if ((mineRowReq == startR && mineColReq == startC)
            || cells[mineRowReq * col + mineColReq].isMine) {
            i--;
        }
        else {
            cells[mineRowReq * col + mineColReq].isMine = true;
            increaseNeighCount(mineRowReq, mineColReq);
        }
    }
}

// -------------------------------------------------------
// Board::reset
// -------------------------------------------------------
void Board::reset()
{
    for (size_t i = 0; i < cells.size(); ++i) {
        cells[i].isMine      = false;
        cells[i].isUncovered = false;
        cells[i].isFlagged   = false;
        cells[i].isBoom      = false;
        cells[i].minesAround = 0;
        cells[i].flagsAround = 0;
    }
}

// -------------------------------------------------------
// Board::chord
// -------------------------------------------------------
void Board::chord(int rowS, int colS)
{
    // Exit early if not enough flags placed
    if (cells[rowS * col + colS].flagsAround != cells[rowS * col + colS].minesAround)
        return;

    for (int deltaR = -1; deltaR <= 1; deltaR++) {
        for (int deltaC = -1; deltaC <= 1; deltaC++) {
            if (deltaR == 0 && deltaC == 0) continue;

            int nRow = rowS + deltaR;
            int nCol = colS + deltaC;

            if (isValid(nRow, nCol)) {
                cell& neighbor = cells[nRow * col + nCol];
                if (!neighbor.isUncovered && !neighbor.isFlagged)
                    openUp(nRow, nCol);
            }
        }
    }
}

// -------------------------------------------------------
// vibrateGamepad
// -------------------------------------------------------
void vibrateGamepad(RumbleManager& rumble)
{
    if (g_VibrationsEnabled)
        rumble.Pulse(vibrationStrength, vibrationStrength, 250);
}

// -------------------------------------------------------
// InitNewGame
// -------------------------------------------------------
int InitNewGame(Board& boardToReset, int tileCountRow, int tileCountCol, int mineCount, RumbleManager& rumble)
{
    gameOver           = 0;
    uncoveredSafeCells = 0;
    boardGenerated     = false;
    boardToReset.reset();
    myGameTimer.Reset();
    return (tileCountRow * tileCountCol) - mineCount;
}

// -------------------------------------------------------
// checkWin
// -------------------------------------------------------
bool checkWin(int totalSafeCells)
{
    return uncoveredSafeCells == totalSafeCells;
}