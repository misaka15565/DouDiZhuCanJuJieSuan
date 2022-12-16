#include <map>
#include <string>
#include <vector>
#include <set>
typedef char int8;
typedef char cardVal;
typedef char cardType;
typedef char statusCode;
using std::string;
using std::map;
using std::vector;
using std::set;

const cardType TYPE_0_PASS = 0;
const cardType TYPE_1_SINGLE = 1;
const cardType TYPE_2_PAIR = 2;
const cardType TYPE_3_TRIPLE = 3;
const cardType TYPE_4_BOMB = 4;
const cardType TYPE_5_KING_BOMB = 5;
const cardType TYPE_6_3_1 = 6;
const cardType TYPE_7_3_2 = 7;
const cardType TYPE_8_SERIAL_SINGLE = 8;
const cardType TYPE_9_SERIAL_PAIR = 9;
const cardType TYPE_10_SERIAL_TRIPLE = 10;
const cardType TYPE_11_SERIAL_3_1 = 11;
const cardType TYPE_12_SERIAL_3_2 = 12;
const cardType TYPE_13_4_2 = 13;
const cardType TYPE_14_4_2_2 = 14;
const cardType TYPE_99_WRONG = 99;

map<cardType, string> MOVE_TYPES_STR({{TYPE_0_PASS, "Pass"},
                                      {TYPE_1_SINGLE, "Single"},
                                      {TYPE_2_PAIR, "Pair"},
                                      {TYPE_3_TRIPLE, "Triple"},
                                      {TYPE_4_BOMB, "Bomb!"},
                                      {TYPE_5_KING_BOMB, "King Bomb!!!"},
                                      {TYPE_6_3_1, "3 + 1"},
                                      {TYPE_7_3_2, "3 + 2"},
                                      {TYPE_8_SERIAL_SINGLE, "Serial Single"},
                                      {TYPE_9_SERIAL_PAIR, "Serial Pair"},
                                      {TYPE_10_SERIAL_TRIPLE, "Serial Triple"},
                                      {TYPE_11_SERIAL_3_1, "Serial 3 + 1"},
                                      {TYPE_12_SERIAL_3_2, "Serial 3 + 2"},
                                      {TYPE_13_4_2, "4 + 2"},
                                      {TYPE_14_4_2_2, "4 + 2 Pairs"},
                                      {TYPE_99_WRONG, "Wrong Type!"}});

map<string, cardVal> s2v = {
    {"3", 3},
    {"4", 4},
    {"5", 5},
    {"6", 6},
    {"7", 7},
    {"8", 8},
    {"9", 9},
    {"0", 10},
    {"J", 11},
    {"j", 11},
    {"Q", 12},
    {"q", 12},
    {"K", 13},
    {"k", 13},
    {"A", 14},
    {"a", 14},
    {"2", 18},
    {"X", 20},
    {"x", 20},
    {"D", 30},
    {"d", 30},
};

map<char, cardVal> c2v = {
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'9', 9},
    {'0', 10},
    {'J', 11},
    {'j', 11},
    {'Q', 12},
    {'q', 12},
    {'K', 13},
    {'k', 13},
    {'A', 14},
    {'a', 14},
    {'2', 18},
    {'X', 20},
    {'x', 20},
    {'D', 30},
    {'d', 30},
};

map<cardVal, string> v2s = {{3, "3"}, {4, "4"}, {5, "5"}, {6, "6"}, {7, "7"}, {8, "8"}, {9, "9"}, {10, "10"}, {11, "J"}, {12, "Q"}, {13, "K"}, {14, "A"}, {18, "2"}, {20, "小王"}, {30, "大王"}};

class cards {
public:
    // CardValue,CardCount
    map<cardVal, int8> cardCount;
    cards() {
        for (const auto &i : v2s) {
            cardCount[i.first] = 0;
        }
    }
    bool isIncludedIn(cards b) {
        for (const auto &i : b.cardCount)
            if (i.second > cardCount[i.first]) return false;
        return true;
    }
    bool remove(cards b) {
        if (isIncludedIn(b) != true) return false;
        for (const auto &i : b.cardCount)
            cardCount[i.first] -= i.second;
        return true;
    }
    cardVal biggestCard() {
        cardVal k = 0;
        for (const auto &i : cardCount) {
            if (i.second > 0 && i.first > k) k = i.first;
        }
        return k;
    }
    int8 cardNum() {
        int8 sum = 0;
        for (const auto &i : cardCount) {
            sum += i.second;
        }
        return sum;
    };
};

#define bigger 1
#define notbigger 0
#define illegal 2

class move {
public:
    cards mainCard;
    cards subCard;
    cardType type;
    move() {
        type = TYPE_99_WRONG;
    }
    cards totalCards() {
        cards sum = mainCard;
        for (const auto &i : subCard.cardCount) {
            sum.cardCount[i.first] += i.second;
        }
        return sum;
    };
    int8 cardNum() {
        return mainCard.cardNum()+subCard.cardNum();
    };
    statusCode isBiggerThan(move b) {
        if (type == TYPE_0_PASS) return bigger;
        if (type == TYPE_5_KING_BOMB) return bigger;
        if (type == TYPE_4_BOMB && b.type != TYPE_4_BOMB) return bigger;
        if (b.type == TYPE_5_KING_BOMB) return notbigger;
        if (b.type == TYPE_4_BOMB && type != TYPE_4_BOMB) return notbigger;
        if (type == b.type && cardNum() == b.cardNum()) {
            cardVal va = mainCard.biggestCard(), vb = b.mainCard.biggestCard();
            return va > vb ? bigger : notbigger;
        }
        return illegal;
    };
};

class possibleMoveSet {
public:
    vector<move> moveSet;
    possibleMoveSet(cards x);
};
possibleMoveSet::possibleMoveSet(cards x) {
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
        const int8 serialNum=tmpA.cardNum()/3;
        //应该写函数生成不同type的可能出牌
        //写函数枚举Cmn
        
    }
}