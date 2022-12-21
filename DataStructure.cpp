#include <map>
#include <string>
#include "TypeDefines.cpp"

using std::string;
using std::map;

map<moveType, string> MOVE_TYPES_STR({{TYPE_0_PASS, "过"},
                                      {TYPE_1_SINGLE, "单张"},
                                      {TYPE_2_PAIR, "对子"},
                                      {TYPE_3_TRIPLE, "三张"},
                                      {TYPE_4_BOMB, "炸弹!"},
                                      {TYPE_5_KING_BOMB, "王炸"},
                                      {TYPE_6_3_1, "三带一"},
                                      {TYPE_7_3_2, "三带二"},
                                      {TYPE_8_SERIAL_SINGLE, "顺子"},
                                      {TYPE_9_SERIAL_PAIR, "连对"},
                                      {TYPE_10_SERIAL_TRIPLE, "飞机"},
                                      {TYPE_11_SERIAL_3_1, "3带1组成的飞机"},
                                      {TYPE_12_SERIAL_3_2, "3带2组成的飞机"},
                                      {TYPE_13_4_2, "四带二"},
                                      {TYPE_14_4_2_2, "四带两对"}});

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
    bool isIncludedIn(cards b) const {
        for (const auto &i : b.cardCount)
            if (i.second > cardCount.find(i.first)->second) return false;
        return true;
    }
    bool remove(cards b) {
        if (isIncludedIn(b) != true) return false;
        for (const auto &i : b.cardCount)
            cardCount[i.first] -= i.second;
        return true;
    }
    cardVal biggestCard() const {
        cardVal k = 0;
        for (const auto &i : cardCount) {
            if (i.second > 0 && i.first > k) k = i.first;
        }
        return k;
    }
    int8 cardNum() const {
        int8 sum = 0;
        for (const auto &i : cardCount) {
            sum += i.second;
        }
        return sum;
    };
};
bool operator==(const cards &a, const cards &b) {
    return a.cardCount == b.cardCount;
}
bool operator<(const cards &a, const cards &b) {
    for (cardVal i : {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 18, 20, 30}) {
        if (a.cardCount.find(i)->second < b.cardCount.find(i)->second) return true;
    }
    return false;
}
class move {
public:
    cards mainCard;
    cards subCard;
    moveType type;
    move() {
        type = TYPE_0_PASS;
    }
    cards totalCards() const {
        cards sum = mainCard;
        for (const auto &i : subCard.cardCount) {
            sum.cardCount[i.first] += i.second;
        }
        return sum;
    };
    int8 cardNum() const {
        return mainCard.cardNum() + subCard.cardNum();
    };
    statusCode isBiggerThan(move b) const {
        if (type == TYPE_0_PASS && b.type != TYPE_0_PASS) return bigger;
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

bool operator==(const move &a, const move &b) {
    return a.mainCard == b.mainCard && a.subCard == b.subCard && a.type == b.type;
}