#include "Heuristic.h"
using namespace std;

Heuristic::Heuristic(int aiPiece, int playerPiece)
{
    ai = aiPiece;
    player = playerPiece;
}

int Heuristic::evaluateWindow(int window[4])
{
    int aiCount = 0;
    int playerCount = 0;
    int emptyCount = 0;

    for (int i = 0; i < 4; i++)
    {
        if (window[i] == ai)
            aiCount++;

        else if (window[i] == player)
            playerCount++;

        else
            emptyCount++;
    }

    int score = 0;

    if (aiCount == 4)
        score += 100000;

    else if (aiCount == 3 && emptyCount == 1)
        score += 100;

    else if (aiCount == 2 && emptyCount == 2)
        score +=10;

    if (playerCount == 3 && emptyCount == 1)
        score -= 150;

    if (playerCount == 4)
        score -= 100000;

    return score;
}

int Heuristic::evaluateBoard(const Board& board)
{
    int score = 0;
    const int colWeights[COLS] = {1, 2, 3, 5, 3, 2, 1};

    for (int col = 0; col < COLS; col++)
    {
        for (int row = 0; row < ROWS; row++)
        {
            if (board.getCell(row, col) == ai)
                score += colWeights[col];
        }
    }

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS - 3; col++)
        {
            int window[4] =
            {
                board.getCell(row, col),
                board.getCell(row, col + 1),
                board.getCell(row, col + 2),
                board.getCell(row, col + 3)
            };

            score += evaluateWindow(window);
        }
    }

    for (int col = 0; col < COLS; col++)
    {
        for (int row = 0; row < ROWS - 3; row++)
        {
            int window[4] =
            {
                board.getCell(row, col),
                board.getCell(row + 1, col),
                board.getCell(row + 2, col),
                board.getCell(row + 3, col)
            };

            score += evaluateWindow(window);
        }
    }

    for (int row = 0; row < ROWS - 3; row++)
    {
        for (int col = 0; col < COLS - 3; col++)
        {
            int window[4] =
            {
                board.getCell(row, col),
                board.getCell(row + 1, col + 1),
                board.getCell(row + 2, col + 2),
                board.getCell(row + 3, col + 3)
            };

            score += evaluateWindow(window);
        }
    }

    for (int row = 3; row < ROWS; row++)
    {
        for (int col = 0; col < COLS - 3; col++)
        {
            int window[4] =
            {
                board.getCell(row, col),
                board.getCell(row - 1, col + 1),
                board.getCell(row - 2, col + 2),
                board.getCell(row - 3, col + 3)
            };

            score += evaluateWindow(window);
        }
    }

    return score;
}
