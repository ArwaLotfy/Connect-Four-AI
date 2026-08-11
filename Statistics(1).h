#ifndef STATISTICS_H
#define STATISTICS_H


class Statistics
{
private:
    long long exploredNodes;
    long long prunedNodes;

public:
    Statistics();

    void addExploredNode();
    void addPrunedNode();

    long long getExploredNodes() const;
    long long getPrunedNodes() const;

    void reset();
};

#endif // STATISTICS_H
