#include <set>
#include <vector>
#include "DataStructure.cpp"
using std::set;
using std::vector;
class possibleMoveSet {
public:
    vector<move> moveSet;
    void genTYPE_0_PASS(cards x);
    void genTYPE_1_SINGLE(cards x);
    void genTYPE_2_PAIR(cards x);
    void genTYPE_3_TRIPLE(cards x);
    void genTYPE_4_BOMB(cards x);
    void genTYPE_5_KING_BOMB(cards x);
    void genTYPE_6_3_1(cards x);
    void genTYPE_7_3_2(cards x);
    void genTYPE_8_SERIAL_SINGLE(cards x);
    void genTYPE_9_SERIAL_PAIR(cards x);
    void genTYPE_10_SERIAL_TRIPLE(cards x);
    void genTYPE_11_SERIAL_3_1(cards x);
    void genTYPE_12_SERIAL_3_2(cards x);
    void genTYPE_13_4_2(cards x);
    void genTYPE_14_4_2_2(cards x);
    
    possibleMoveSet(cards x, move lastMove);
};
possibleMoveSet::possibleMoveSet(cards x, move lastMove) {
    set<cardVal> singles;
    set<cardVal> pairs;
    set<cardVal> triples;
    set<cardVal> bombs;
    {
        move tmp;
        tmp.type = TYPE_0_PASS;
        moveSet.push_back(tmp);
    }
    // SINGLE PAIR TRIPLE BOMB
    for (const auto &i : x.cardCount) {
        if (i.second >= 1) {
            move tmp;
            tmp.type = TYPE_1_SINGLE;
            tmp.mainCard.cardCount[i.first] = 1;
            moveSet.push_back(tmp);
            singles.insert(i.first);
        }
        if (i.second >= 2) {
            move tmp;
            tmp.type = TYPE_2_PAIR;
            tmp.mainCard.cardCount[i.first] = 2;
            moveSet.push_back(tmp);
            pairs.insert(i.first);
        }
        if (i.second >= 3) {
            move tmp;
            tmp.type = TYPE_3_TRIPLE;
            tmp.mainCard.cardCount[i.first] = 3;
            moveSet.push_back(tmp);
            triples.insert(i.first);
        }
        if (i.second >= 4) {
            move tmp;
            tmp.type = TYPE_4_BOMB;
            tmp.mainCard.cardCount[i.first] = 4;
            moveSet.push_back(tmp);
            triples.insert(i.first);
        }
    }
    // KING_BOMB
    if (x.cardCount[20] >= 1 && x.cardCount[30] >= 1) {
        move tmp;
        tmp.type = TYPE_5_KING_BOMB;
        tmp.mainCard.cardCount[20] = 1;
        tmp.mainCard.cardCount[30] = 1;
        moveSet.push_back(tmp);
    }
    // 3+1
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
    // 3+2
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
    const int minSerialSingle = 5; // 最小顺子长度
    const int minSerialPair = 3;   // 最小对子长度
    const int minSerialTriple = 2; // 最小飞机长度
    for (cardVal i = c2v['3']; i <= c2v['A'] - minSerialSingle + 1; i++) {
        if (singles.count(i) == 1) {
            for (cardVal j = i + 1; j <= c2v['A'] - minSerialSingle + 1; j++) {
                if (singles.count(j) == 0) break;
                if (j - i + 1 >= minSerialSingle) {
                    move tmp;
                    tmp.type = TYPE_8_SERIAL_SINGLE;
                    for (cardVal k = i; k <= j; k++) {
                        tmp.mainCard.cardCount[k] = 1;
                    }
                    moveSet.push_back(tmp);
                }
            }
        }
    }

    for (cardVal i = c2v['3']; i <= c2v['A'] - minSerialTriple + 1; i++) {
        if (triples.count(i) == 1) {
            for (cardVal j = i + 1; j <= c2v['A'] - minSerialTriple + 1; j++) {
                if (triples.count(j) == 0) break;
                if (j - i + 1 >= minSerialTriple) {
                    move tmp;
                    tmp.type = TYPE_10_SERIAL_TRIPLE;
                    for (cardVal k = i; k <= j; k++) {
                        tmp.mainCard.cardCount[k] = 3;
                    }
                    moveSet.push_back(tmp);
                }
            }
        }
    }
    vector<move> serial_triples;
    for (cardVal i = c2v['3']; i <= c2v['A'] - minSerialPair + 1; i++) {
        if (pairs.count(i) == 1) {
            for (cardVal j = i + 1; j <= c2v['A'] - minSerialPair + 1; j++) {
                if (pairs.count(j) == 0) break;
                if (j - i + 1 >= minSerialPair) {
                    move tmp;
                    tmp.type = TYPE_9_SERIAL_PAIR;
                    for (cardVal k = i; k <= j; k++) {
                        tmp.mainCard.cardCount[k] = 2;
                    }
                    moveSet.push_back(tmp);
                    serial_triples.push_back(tmp);
                }
            }
        }
    }

    // serial 3_1 and 3_2
    for (const auto &i : serial_triples) {
        cards tmpA = x;
        set<cardVal> pairs2;
        tmpA.remove(i.mainCard);
        const int8 serialNum = tmpA.cardNum() / 3;
        // 应该写函数生成不同type的可能出牌
        // 写函数枚举Cmn
    }
}