#include "Statistics.h"

Statistics::Statistics()
{
    exploredNodes = 0;
    prunedNodes = 0;
}

void Statistics::addExploredNode()
{
    exploredNodes++;
}

void Statistics::addPrunedNode()
{
    prunedNodes++;
}

long long Statistics::getExploredNodes() const
{
    return exploredNodes;
}

long long Statistics::getPrunedNodes() const
{
    return prunedNodes;
}

void Statistics::reset()
{
    exploredNodes = 0;
    prunedNodes = 0;
}
