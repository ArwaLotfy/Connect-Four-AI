#include "AlphaBeta.h"
#include <algorithm>

using namespace std;

static const int WIN_SCORE = 1000000;
static const int LOSE_SCORE = -1000000;
static const int INF = 10000000;

AlphaBeta::AlphaBeta(int ai, int player)
    :heuristic(ai, player)
{
    aiPiece = ai;
    playerPiece = player;
}

int AlphaBeta::alphaBeta(Board& board, int depth, int alpha, int beta,
                  bool maximizingPlayer, Statistics& stats)

{
    stats.addExploredNode();

    if (board.checkWin(aiPiece))
        return WIN_SCORE;

    if (board.checkWin(playerPiece))
        return LOSE_SCORE;

    if (board.isBoardFull())
        return 0;

    if (depth == 0)
        return heuristic.evaluateBoard(board);

    unsigned long long hash = tt.hashBoard(board);

    if (tt.contains(hash))
    {
        TTEntry entry = tt.get(hash);

        if (entry.depth >= depth)
            return entry.score;
    }

    if (maximizingPlayer)
    {
        int bestScore = -INF;

        for (int i = 0; i < 7; i++)
        {
            int col = moveOrder[i];

            if (board.isValidMove(col))
            {
                int row = board.getNextOpenRow(col);
                board.dropPiece(row, col, aiPiece);
                int score = alphaBeta(board, depth - 1, alpha, beta, false, stats);
                board.undoMove(row, col);

                bestScore = max(bestScore, score);
                alpha = max(alpha, score);

                if (alpha >= beta)
                {
                    stats.addPrunedNode();
                    break;
                }
            }
        }

        tt.store(hash, bestScore, depth);

        return bestScore;
    }
    else
    {
        int bestScore = INF;

        for (int i = 0; i < 7; i++)
        {
            int col = moveOrder[i];

            if (board.isValidMove(col))
            {
                int row = board.getNextOpenRow(col);
                board.dropPiece(row, col, playerPiece);
                int score = alphaBeta(board, depth - 1, alpha, beta, true, stats);
                board.undoMove(row, col);

                bestScore = min(bestScore, score);
                beta = min(beta, score);

                if (alpha >= beta)
                {
                    stats.addPrunedNode();
                    break;
                }
            }
        }

        tt.store(hash, bestScore, depth);

        return bestScore;
    }
}

int AlphaBeta::getBestMove(Board& board, int depth, Statistics& stats)
{
    if (board.isBoardFull())
        return -1;

    tt.clear();
    stats.reset();

    int bestScore = -INF;
    int bestColumn = 3;

    for (int i = 0; i < 7; i++)
    {
        int col = moveOrder[i];

        if (board.isValidMove(col))
        {
            int row = board.getNextOpenRow(col);
            board.dropPiece(row, col, aiPiece);
            bool aiWins = board.checkWin(aiPiece);
            board.undoMove(row, col);

            if (aiWins)
                return col;
        }
    }

    for (int i = 0; i < 7; i++)
    {
        int col = moveOrder[i];

        if (board.isValidMove(col))
        {
            int row = board.getNextOpenRow(col);
            board.dropPiece(row, col, playerPiece);
            bool playerWins = board.checkWin(playerPiece);
            board.undoMove(row, col);

            if (playerWins)
                return col;
        }
    }


    for (int i = 0; i < 7; i++)
    {
        int col = moveOrder[i];

        if (board.isValidMove(col))
        {
            int row = board.getNextOpenRow(col);
            board.dropPiece(row, col, aiPiece);
            int score = alphaBeta(board, depth - 1, -INF, INF, false, stats);
            board.undoMove(row, col);

            if (score > bestScore)
            {
                bestScore = score;
                bestColumn = col;
            }
        }
    }
    return bestColumn;
}
