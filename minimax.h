#ifndef MINIMAX_H
#define MINIMAX_H


class Minimax
{
private:
    int m_depth; // глубина n-дерева
    bool m_whoMove;


public:
    Minimax(bool whoMove, int depth);
};

#endif // MINIMAX_H
