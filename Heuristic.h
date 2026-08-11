#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "Board.h"

class Heuristic
{
private:
    int ai;
    int player;

public:
    Heuristic(int aiPiece, int playerPiece);
    int evaluateWindow(int window[4]);
    int evaluateBoard(const Board& board);
};

#endif // HEURISTIC_H
