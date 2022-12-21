#include <cstdint>
#include <set>
#include <vector>
#include <algorithm>
#include "DataStructure.cpp"
using std::set;
using std::vector;
using std::sort;
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
    vector<cards> findSerialTriples() const;
    vector<cards> genSerialTriples(const cardVal begin,
                                   const cardVal end) const;
    vector<cards> genSubcards(
        const cards &x, int8 n) const;

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
    for (int8 i=0;i<N;i++) {
        if (x.cardCount[i] >= 1) singles.insert(i);
    }
}

void possibleMoveSet::findPairs(const cards &x) {
    if (pairs.empty() == false) return;
    for (int8 i=0;i<N;i++) {
        if (x.cardCount[i] >= 2) pairs.insert(i);
    }
}

void possibleMoveSet::findTriples(const cards &x) {
    if (triples.empty() == false) return;
    for (int8 i=0;i<N;i++) {
        if (x.cardCount[i] >= 3) triples.insert(i);
    }
}

void possibleMoveSet::findBombs(const cards &x) {
    if (bombs.empty() == false) return;
    for (int8 i=0;i<N;i++) {
        if (x.cardCount[i] >= 4) bombs.insert(i);
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
    if (x.cardCount[c2v['X']] >= 1
        && x.cardCount[c2v['D']] >= 1) {
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

vector<cards> possibleMoveSet::genSerialTriples(const cardVal begin,
                                                const cardVal end) const {
    vector<cards> result;
    for (cardVal i = begin; i + minSerialTriple - 1 <= end; i++) {
        for (cardVal j = i + minSerialTriple - 1; j <= end; j++) {
            cards tmp;
            for (cardVal k = i; k <= j; k++)
                tmp.cardCount[k] = 3;
            result.push_back(tmp);
        }
    }
    return result;
}

vector<cards> possibleMoveSet::findSerialTriples() const {
    vector<cards> result;
    for (cardVal i = c2v['3']; i <= c2v['A']; i++) {
        if (triples.count(i) == 0) continue;
        cardVal begin = i;
        while (triples.count(i) != 0 && i <= c2v['A']) i++;
        cardVal end = i - 1;
        if (end - begin + 1 >= minSerialTriple) {
            vector<cards> tmp = genSerialTriples(begin, end);
            result.insert(result.end(), tmp.begin(), tmp.end());
        }
    }
    return result;
}

void getAnyNinM(const vector<cardVal> &x,
                vector<vector<cardVal>::const_iterator> tmp,
                vector<cards> &result, int8 n) {
    if (x.size() < n) return;
    vector<cardVal>::const_iterator b; // 指向第一个处理的元素
    for (b = (tmp.empty() ? x.begin() : *(tmp.end() - 1) + 1);
         b < x.end() - n + 1; b++) {
        vector<vector<cardVal>::const_iterator> tmpB = tmp;
        tmpB.push_back(b);
        if (n == 1) {
            cards t;
            for (const auto &j : tmpB) {
                t.cardCount[*j]++;
            }
            result.push_back(t);
        } else {
            getAnyNinM(x, tmpB, result, n - 1);
        }
    }
}

vector<cards> possibleMoveSet::genSubcards(
    const cards &x, int8 n) const {
    vector<cards> result;
    vector<cardVal> tmpA;
    for (int8 i=0;i<N;i++) {
        for (int8 j = 0; j < x.cardCount[i]; j++) {
            tmpA.push_back(i);
        }
    }
    vector<vector<cardVal>::const_iterator> tmp;
    getAnyNinM(tmpA, tmp, result, n);
    sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}

void possibleMoveSet::genTYPE_11_SERIAL_3_1(const cards &x) {
    findTriples(x);
    vector<cards> result = findSerialTriples();
    for (const auto &i : result) {
        cards tmpA = x;
        tmpA.remove(i);
        int8 n = i.cardNum() / 3;
        vector<cards> tmpB = genSubcards(tmpA, n);
        for (const auto &j : tmpB) {
            move tmp;
            tmp.type = TYPE_11_SERIAL_3_1;
            tmp.mainCard = i;
            tmp.subCard = j;
            moveSet.push_back(tmp);
        }
    }
}

void possibleMoveSet::genTYPE_12_SERIAL_3_2(const cards &x) {
    findTriples(x);
    vector<cards> result = findSerialTriples();
    for (const auto &i : result) {
        cards tmpA = x;
        tmpA.remove(i);
        int8 n = i.cardNum() / 3;
        for (auto &k : tmpA.cardCount) {
            k /= 2;
        }
        vector<cards> tmpB = genSubcards(tmpA, n);
        for (const auto &j : tmpB) {
            move tmp;
            tmp.type = TYPE_12_SERIAL_3_2;
            tmp.mainCard = i;
            tmp.subCard = j;
            for (auto &k : tmp.subCard.cardCount) {
                k *= 2;
            }
            moveSet.push_back(tmp);
        }
    }
}

void possibleMoveSet::genTYPE_13_4_2(const cards &x) {
    findBombs(x);
    vector<cards> tmpZ;
    for (const auto &i : bombs) {
        cards tmp;
        tmp.cardCount[i] = 4;
        tmpZ.push_back(tmp);
    }
    for (const auto &i : tmpZ) {
        cards tmpA = x;
        tmpA.remove(i);
        vector<cards> tmpB = genSubcards(tmpA, 2);
        for (const auto &j : tmpB) {
            move tmp;
            tmp.type = TYPE_13_4_2;
            tmp.mainCard = i;
            tmp.subCard = j;
            moveSet.push_back(tmp);
        }
    }
}

void possibleMoveSet::genTYPE_14_4_2_2(const cards &x) {
    findBombs(x);
    vector<cards> tmpZ;
    for (const auto &i : bombs) {
        cards tmp;
        tmp.cardCount[i] = 4;
        tmpZ.push_back(tmp);
    }
    for (const auto &i : tmpZ) {
        cards tmpA = x;
        tmpA.remove(i);
        for (auto &k : tmpA.cardCount) {
            k /= 2;
        }
        vector<cards> tmpB = genSubcards(tmpA, 2);
        for (const auto &j : tmpB) {
            move tmp;
            tmp.type = TYPE_14_4_2_2;
            tmp.mainCard = i;
            tmp.subCard = j;
            for (auto &k : tmp.subCard.cardCount) {
                k *= 2;
            }
            moveSet.push_back(tmp);
        }
    }
}

possibleMoveSet::possibleMoveSet(const cards &x, move lastMove) {
    if (lastMove.type != TYPE_0_PASS) genTYPE_0_PASS(x);
    if (lastMove.type == TYPE_5_KING_BOMB) return;
    switch (lastMove.type) {
    case TYPE_0_PASS:
        genTYPE_1_SINGLE(x);
        genTYPE_2_PAIR(x);
        genTYPE_3_TRIPLE(x);
        genTYPE_6_3_1(x);
        genTYPE_7_3_2(x);
        genTYPE_8_SERIAL_SINGLE(x);
        genTYPE_9_SERIAL_PAIR(x);
        genTYPE_10_SERIAL_TRIPLE(x);
        genTYPE_11_SERIAL_3_1(x);
        genTYPE_12_SERIAL_3_2(x);
        genTYPE_13_4_2(x);
        genTYPE_14_4_2_2(x);
        break;
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
    if (lastMove.type != TYPE_0_PASS) {
        for (auto it = moveSet.begin(); it != moveSet.end();) {
            if (it->isBiggerThan(lastMove) != bigger) {
                it = moveSet.erase(it);
            } else {
                it++;
            }
        }
    }
}