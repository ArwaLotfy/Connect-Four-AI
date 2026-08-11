#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H

#include <unordered_map>
#include "Board.h"


struct TTEntry
{
    int score;
    int depth;
};

class TranspositionTable
{
private:
    std::unordered_map<unsigned long long, TTEntry> table;

public:
    unsigned long long hashBoard(const Board& board);

    bool contains(unsigned long long hash) const;

    TTEntry get(unsigned long long hash) const;

    void store(unsigned long long hash, int score, int depth);

    void clear();
};

#endif // TRANSPOSITIONTABLE_H
