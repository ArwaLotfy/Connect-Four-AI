#ifndef BOARD_H
#define BOARD_H

#include <iostream>


static const int ROWS = 6;
static const int COLS = 7;


class Board
{
private:
    int board[ROWS][COLS];

public:
    Board();

    void initializeBoard();
    void printBoard() const;

    bool isValidMove(int col) const;
    int getNextOpenRow(int col) const;
    void dropPiece(int row, int col, int piece);

    void undoMove(int row, int col);

    bool checkWin(int piece) const;

    bool isBoardFull() const;

    int getCell(int row, int col) const;
};

#endif // BOARD_H
