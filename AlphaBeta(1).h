#ifndef ALPHABETA_H
#define ALPHABETA_H

#include "Board.h"
#include "Heuristic.h"
#include "Statistics.h"
#include "TranspositionTable.h"


class AlphaBeta
{
private:
    int aiPiece;
    int playerPiece;

    Heuristic heuristic;

    TranspositionTable tt;

    const int moveOrder[7] = {3, 2, 4, 1, 5, 0, 6};

    int alphaBeta(Board& board, int depth, int alpha, int beta,
                  bool maximizingPlayer, Statistics& stats);

public:
    AlphaBeta(int ai, int player);

    int getBestMove(Board& board, int depth, Statistics& stats);
};

#endif // ALPHABETA_H
