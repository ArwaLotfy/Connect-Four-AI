#include "Board.h"
#include <iostream>
using namespace std;

Board::Board()
{
    initializeBoard();
}

void Board::initializeBoard()
{
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            board[row][col] = 0;
        }
    }
}

void Board::printBoard()const
{
    cout << "\n";

    for (int row = ROWS - 1; row >= 0; row--)
    {
        for (int col = 0; col < COLS; col++)
        {
            if (board[row][col] == 0)
                cout << ". ";

            else if (board[row][col] == 1)
                cout << "X ";

            else
                cout << "O ";
        }
        cout << endl;
    }

    cout << "0 1 2 3 4 5 6\n\n";
}

bool Board::isValidMove(int col) const
{
    if (col < 0 || col >= COLS)
        return false;

    return board[ROWS - 1][col] == 0;
}

int Board::getNextOpenRow(int col) const
{
    for (int row = 0; row < ROWS; row++)
    {
        if (board[row][col] == 0)
        {
            return row;
        }
    }
    return -1;
}

void Board::dropPiece(int row, int col, int piece)
{
    board[row][col] = piece;
}

void Board::undoMove(int row, int col)
{
    board[row][col] = 0;
}

bool Board::checkWin(int piece) const
{
    //Horizontal
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS - 3; col++)
        {
            if (board[row][col] == piece &&
                board[row][col + 1] == piece &&
                board[row][col + 2] == piece &&
                board[row][col + 3] == piece)
            {
                return true;
            }
        }
    }

    //Vertical
    for (int col = 0; col < COLS; col++)
    {
        for (int row = 0; row < ROWS - 3; row++)
        {
            if (board[row][col] == piece &&
                board[row + 1][col] == piece &&
                board[row + 2][col] == piece &&
                board[row + 3][col] == piece)
            {
                return true;
            }
        }
    }

    //Pos Diagonal
    for (int row = 0; row < ROWS - 3; row++)
    {
        for (int col = 0; col < COLS - 3; col++)
        {
            if (board[row][col] == piece &&
                board[row + 1][col + 1] == piece &&
                board[row + 2][col + 2] == piece &&
                board[row + 3][col + 3] == piece)
            {
                return true;
            }
        }
    }

    //Neg Diagonal
    for (int row = 3; row < ROWS; row++)
    {
        for (int col = 0; col < COLS - 3; col++)
        {
            if (board[row][col] == piece &&
                board[row - 1][col + 1] == piece &&
                board[row - 2][col + 2] == piece &&
                board[row - 3][col + 3] == piece)
            {
                return true;
            }
        }
    }
    return false;
}

bool Board::isBoardFull() const
{
    for (int col = 0; col < COLS; col++)
    {
        if (board[ROWS - 1][col] == 0)
        {
            return false;
        }
    }
    return true;
}

int Board::getCell(int row, int col) const
{
    return board[row][col];
}
