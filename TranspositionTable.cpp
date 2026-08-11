#include "TranspositionTable.h"
#include <random>

using namespace std;

static unsigned long long zobristTable[ROWS][COLS][3];
static bool initialized = false;

void initializeZobrist()
{
    random_device rd;
    mt19937_64 gen(rd());

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            for (int piece = 0; piece < 3; piece++)
            {
                zobristTable[row][col][piece] = gen();
            }
        }
    }
}

unsigned long long TranspositionTable::hashBoard(const Board& board)
{
    if (!initialized)
    {
        initializeZobrist();
        initialized = true;
    }

    unsigned long long hash = 0;

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            int piece = board.getCell(row, col);

            if (piece != 0)
            {
                hash ^= zobristTable[row][col][piece];
            }
        }
    }

    return hash;
}

bool TranspositionTable::contains(unsigned long long hash) const
{
    return table.find(hash) != table.end();
}

TTEntry TranspositionTable::get(unsigned long long hash) const
{
    return table.at(hash);
}

void TranspositionTable::store(unsigned long long hash, int score, int depth)
{
    if (!contains(hash) || depth >= table[hash].depth)
    {
        table[hash] = {score, depth};
    }
}

void TranspositionTable::clear()
{
    table.clear();
}
