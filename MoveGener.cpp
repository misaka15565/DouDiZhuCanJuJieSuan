#include <set>
#include <vector>
#include "DataStructure.cpp"
using std::set;
using std::vector;
class possibleMoveSet {
private:
    set<cardVal> singles;
    set<cardVal> pairs;
    set<cardVal> triples;
    set<cardVal> bombs;
    void findSingles(const cards &x);
    void findPairs(const cards &x);
    void findTriples(const cards &x);
    void findBombs(const cards &x);
    void findSerial(const set<cardVal> &x, const moveType type,
                    const int minLength);
    void genSerial(const cardVal begin, const cardVal end,
                   const moveType type, const int minLength);
    vector<move> findSerialTriples(const cards &x);

    void genTYPE_0_PASS(const cards &x);
    void genTYPE_1_SINGLE(const cards &x);
    void genTYPE_2_PAIR(const cards &x);
    void genTYPE_3_TRIPLE(const cards &x);
    void genTYPE_4_BOMB(const cards &x);
    void genTYPE_5_KING_BOMB(const cards &x);
    void genTYPE_6_3_1(const cards &x);
    void genTYPE_7_3_2(const cards &x);
    void genTYPE_8_SERIAL_SINGLE(const cards &x);
    void genTYPE_9_SERIAL_PAIR(const cards &x);
    void genTYPE_10_SERIAL_TRIPLE(const cards &x);
    void genTYPE_11_SERIAL_3_1(const cards &x);
    void genTYPE_12_SERIAL_3_2(const cards &x);
    void genTYPE_13_4_2(const cards &x);
    void genTYPE_14_4_2_2(const cards &x);

public:
    vector<move> moveSet;
    possibleMoveSet(const cards &x, move lastMove);
};

void possibleMoveSet::findSingles(const cards &x) {
    if (singles.empty() == false) return;
    for (const auto &i : x.cardCount) {
        if (i.second >= 1) singles.insert(i.first);
    }
}

void possibleMoveSet::findPairs(const cards &x) {
    if (pairs.empty() == false) return;
    for (const auto &i : x.cardCount) {
        if (i.second >= 2) pairs.insert(i.first);
    }
}

void possibleMoveSet::findTriples(const cards &x) {
    if (triples.empty() == false) return;
    for (const auto &i : x.cardCount) {
        if (i.second >= 3) triples.insert(i.first);
    }
}

void possibleMoveSet::findBombs(const cards &x) {
    if (bombs.empty() == false) return;
    for (const auto &i : x.cardCount) {
        if (i.second >= 4) bombs.insert(i.first);
    }
}

void possibleMoveSet::genTYPE_0_PASS(const cards &x) {
    move tmp;
    tmp.type = TYPE_0_PASS;
    moveSet.push_back(tmp);
}

void possibleMoveSet::genTYPE_1_SINGLE(const cards &x) {
    findSingles(x);
    for (const auto &i : singles) {
        move tmp;
        tmp.type = TYPE_1_SINGLE;
        tmp.mainCard.cardCount[i] = 1;
        moveSet.push_back(tmp);
    }
}

void possibleMoveSet::genTYPE_2_PAIR(const cards &x) {
    findPairs(x);
    for (const auto &i : pairs) {
        move tmp;
        tmp.type = TYPE_2_PAIR;
        tmp.mainCard.cardCount[i] = 2;
        moveSet.push_back(tmp);
    }
}

void possibleMoveSet::genTYPE_3_TRIPLE(const cards &x) {
    findTriples(x);
    for (const auto &i : triples) {
        move tmp;
        tmp.type = TYPE_3_TRIPLE;
        tmp.mainCard.cardCount[i] = 3;
        moveSet.push_back(tmp);
    }
}

void possibleMoveSet::genTYPE_4_BOMB(const cards &x) {
    findBombs(x);
    for (const auto &i : bombs) {
        move tmp;
        tmp.type = TYPE_4_BOMB;
        tmp.mainCard.cardCount[i] = 4;
        moveSet.push_back(tmp);
    }
}

void possibleMoveSet::genTYPE_5_KING_BOMB(const cards &x) {
    if (x.cardCount.find(c2v['X'])->second >= 1 && x.cardCount.find(c2v['D'])->second >= 1) {
        move tmp;
        tmp.type = TYPE_5_KING_BOMB;
        tmp.mainCard.cardCount[20] = 1;
        tmp.mainCard.cardCount[30] = 1;
        moveSet.push_back(tmp);
    }
}

void possibleMoveSet::genTYPE_6_3_1(const cards &x) {
    findTriples(x);
    findSingles(x);
    for (const auto &i : triples) {
        for (const auto &j : singles) {
            if (j != i) {
                move tmp;
                tmp.type = TYPE_6_3_1;
                tmp.mainCard.cardCount[i] = 3;
                tmp.subCard.cardCount[j] = 1;
                moveSet.push_back(tmp);
            }
        }
    }
}

void possibleMoveSet::genTYPE_7_3_2(const cards &x) {
    findTriples(x);
    findPairs(x);
    for (const auto &i : triples) {
        for (const auto &j : pairs) {
            if (j != i) {
                move tmp;
                tmp.type = TYPE_7_3_2;
                tmp.mainCard.cardCount[i] = 3;
                tmp.subCard.cardCount[j] = 2;
                moveSet.push_back(tmp);
            }
        }
    }
}

void possibleMoveSet::genSerial(const cardVal begin,
                                const cardVal end,
                                const moveType type,
                                const int minLength) {
    int8 size;
    switch (type) {
    case TYPE_8_SERIAL_SINGLE: size = 1; break;
    case TYPE_9_SERIAL_PAIR: size = 2; break;
    case TYPE_10_SERIAL_TRIPLE: size = 3; break;
    }
    for (cardVal i = begin; i + minLength - 1 <= end; i++) {
        for (cardVal j = i + minLength - 1; j <= end; j++) {
            move tmp;
            tmp.type = type;
            for (cardVal k = i; k <= j; k++)
                tmp.mainCard.cardCount[k] = size;
            moveSet.push_back(tmp);
        }
    }
}

void possibleMoveSet::findSerial(const set<cardVal> &x,
                                 const moveType type,
                                 const int minLength) {
    for (cardVal i = c2v['3']; i <= c2v['A']; i++) {
        if (x.count(i) == 0) continue;
        cardVal begin = i;
        while (x.count(i) != 0 && i <= c2v['A']) i++;
        cardVal end = i - 1;
        if (end - begin + 1 >= minLength)
            genSerial(begin, end, type, minLength);
    }
}

void possibleMoveSet::genTYPE_8_SERIAL_SINGLE(const cards &x) {
    findSingles(x);
    findSerial(singles, TYPE_8_SERIAL_SINGLE, minSerialSingle);
}

void possibleMoveSet::genTYPE_9_SERIAL_PAIR(const cards &x) {
    findPairs(x);
    findSerial(pairs, TYPE_9_SERIAL_PAIR, minSerialPair);
}

void possibleMoveSet::genTYPE_10_SERIAL_TRIPLE(const cards &x) {
    findTriples(x);
    findSerial(triples, TYPE_10_SERIAL_TRIPLE, minSerialTriple);
}

vector<move> possibleMoveSet::findSerialTriples(const cards &x) {
    findTriples(x);
    vector<move> result;
    for (cardVal i = c2v['3']; i <= c2v['A']; i++) {
        if (triples.count(i) == 0) continue;
        cardVal begin = i;
        while (triples.count(i) != 0 && i <= c2v['A']) i++;
        cardVal end = i - 1;
        if (end - begin + 1 >= minSerialTriple) {
            for (cardVal k = begin; k + minSerialTriple - 1 <= end; k++) {
                for (cardVal j = i + minSerialTriple - 1; j <= end; j++) {
                    move tmp;
                    tmp.type = TYPE_10_SERIAL_TRIPLE;
                    for (cardVal k = i; k <= j; k++)
                        tmp.mainCard.cardCount[k] = 3;
                    result.push_back(tmp);
                }
            }
        }
    }
    return result;
}

void possibleMoveSet::genTYPE_11_SERIAL_3_1(const cards &x) {
    vector<move> result = findSerialTriples(x);
    for (const auto &i : result) {
        cards tmpA = x;
        tmpA.remove(i.mainCard);
        tmpA.remove(i.subCard);
        // 写函数枚举Cmn
    }
}

possibleMoveSet::possibleMoveSet(const cards &x, move lastMove) {
    genTYPE_0_PASS(x);
    if (lastMove.type == TYPE_5_KING_BOMB) return;
    switch (lastMove.type) {
    case TYPE_1_SINGLE: genTYPE_1_SINGLE(x); break;
    case TYPE_2_PAIR: genTYPE_2_PAIR(x); break;
    case TYPE_3_TRIPLE: genTYPE_3_TRIPLE(x); break;
    case TYPE_6_3_1: genTYPE_6_3_1(x); break;
    case TYPE_7_3_2: genTYPE_7_3_2(x); break;
    case TYPE_8_SERIAL_SINGLE: genTYPE_8_SERIAL_SINGLE(x); break;
    case TYPE_9_SERIAL_PAIR: genTYPE_9_SERIAL_PAIR(x); break;
    case TYPE_10_SERIAL_TRIPLE: genTYPE_10_SERIAL_TRIPLE(x); break;
    case TYPE_11_SERIAL_3_1: genTYPE_11_SERIAL_3_1(x); break;
    case TYPE_12_SERIAL_3_2: genTYPE_12_SERIAL_3_2(x); break;
    case TYPE_13_4_2: genTYPE_13_4_2(x); break;
    case TYPE_14_4_2_2: genTYPE_14_4_2_2(x); break;
    }
    genTYPE_4_BOMB(x);
    genTYPE_5_KING_BOMB(x);
    for (auto it = moveSet.begin(); it != moveSet.end();) {
        if (it->isBiggerThan(lastMove) == false) {
            it = moveSet.erase(it);
        } else {
            it++;
        }
    }
}