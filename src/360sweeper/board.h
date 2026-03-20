#ifndef BOARD_H
#define BOARD_H

#include "StdAfx.h"
#include "globals.h"

// -------------------------------------------------------
// CELL STRUCT
// -------------------------------------------------------
struct cell {
    bool isMine;
    bool isUncovered;
    bool isFlagged;
    bool isBoom;
    int  minesAround;
    int  flagsAround;	
};

// -------------------------------------------------------
// BOARD CLASS
// -------------------------------------------------------
class Board {
    int row;
    int col;
    std::vector<cell> cells;
    RumbleManager* m_pRumble;
public:
    Board(int r, int c, RumbleManager* pRumble = NULL);

    void ReInit(int newRows, int newCols);
    void SetRumbleManager(RumbleManager* pRumble);

    void setMine(int r, int c);
    void setUncovered(int r, int c);

    cell& getCell(int r, int c);
    bool  isValid(int r, int c);

    void increaseNeighCount(int rowS, int colS);
    void increaseNeighCountFlags(int rowS, int colS);
    void decreaseNeighCountFlags(int rowS, int colS);

    void revealAllMines();
    void revealAllTiles();
    void openUp(int r, int c);
    void genMines(int mineCount, int startR, int startC);
    void reset();
    void chord(int rowS, int colS);
};

// -------------------------------------------------------
// GAME LOGIC FUNCTIONS
// -------------------------------------------------------
void vibrateGamepad(RumbleManager& rumble);
int  InitNewGame(Board& boardToReset, int tileCountRow, int tileCountCol, int mineCount, RumbleManager& rumble);
bool checkWin(int totalSafeCells);

#endif // BOARD_H