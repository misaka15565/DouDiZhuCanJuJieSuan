#include <map>
#include <string>
#include "TypeDefines.cpp"

using std::string;
using std::map;

map<moveType, string> MOVE_TYPES_STR({{TYPE_0_PASS, "Pass"},
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
bool operator<(const cards &a,const cards &b) {
    for (cardVal i:{3,4,5,6,7,8,9,10,11,12,13,14,18,20,30}) {
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
        type = TYPE_99_WRONG;
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
