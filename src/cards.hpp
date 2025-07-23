#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <format>

struct cards {
    using int8 = int8_t;
    using cardVal = int8_t;
    static constexpr int8 N = 20; // Number of card values, including 3-9, 10, J, Q, K, A, 2, X (small king), D (big king)
    inline static const std::map<char, cardVal> c2v = {
        {'3', 1},
        {'4', 2},
        {'5', 3},
        {'6', 4},
        {'7', 5},
        {'8', 6},
        {'9', 7},
        {'0', 8}, // 用0代表10
        {'J', 9},
        {'j', 9},
        {'Q', 10},
        {'q', 10},
        {'K', 11},
        {'k', 11},
        {'A', 12},
        {'a', 12},
        {'2', 14},
        {'X', 16},
        {'x', 16},
        {'D', 18},
        {'d', 18}};
    inline static const std::map<cardVal, std::string> v2s = {
        {1, "3"},
        {2, "4"},
        {3, "5"},
        {4, "6"},
        {5, "7"},
        {6, "8"},
        {7, "9"},
        {8, "10"},
        {9, "J"},
        {10, "Q"},
        {11, "K"},
        {12, "A"},
        {14, "2"},
        {16, "小王"},
        {18, "大王"}};
    // cardCount[i] = j means there are j cards of value i
    std::array<int8, N> cardCount;
    inline cards(std::string s) :
        cardCount{} {
        for (char c : s) {
            if (c2v.find(c) != c2v.end()) {
                cardCount[c2v.at(c)]++;
            }
        }
    }
    inline cards() :
        cardCount{} {
    }
    // b是否包含在this中
    inline bool isInclude(const cards &b) const {
        for (int8 i = 0; i < N; i++)
            if (b.cardCount[i] > cardCount[i]) return false;
        return true;
    }
    inline bool remove(const cards &b) {
        if (!isInclude(b)) return false;
        for (int8 i = 0; i < N; i++)
            cardCount[i] -= b.cardCount[i];
        return true;
    }

    inline cards operator-(const cards &b) const {
        cards res = *this;
        if (!res.remove(b)) throw std::invalid_argument("Cannot remove cards that are not included");
        return res;
    }

    inline int8 cardNum() const {
        int8 sum = 0;
        for (int8 i = 0; i < N; i++) {
            sum += cardCount[i];
        }
        return sum;
    }
    inline cardVal getMaxCard() const {
        for (int8 i = N - 1; i >= 0; --i) {
            if (cardCount[i] > 0) return i;
        }
        return -1; // No cards
    }
    inline int8 getCardCount() const {
        int8 sum = 0;
        for (int8 i = 0; i < N; i++) {
            sum += cardCount[i];
        }
        return sum;
    }
    inline bool operator==(const cards &b) const {
        return cardCount == b.cardCount;
    }
    inline cards operator+(const cards &b) const {
        cards res;
        for (int8 i = 0; i < N; i++) {
            res.cardCount[i] = cardCount[i] + b.cardCount[i];
        }
        return res;
    }
};

template <>
struct std::formatter<cards> {
    using int8 = int8_t;
    inline constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }
    // formatter的性能不重要，反正用不了几次
    inline auto format(const cards &c, format_context &ctx) const {
        string tmp = " ";
        for (cards::cardVal i = 0; i < cards::N; ++i) {
            if (c.cardCount[i] > 0) {
                for (int8 j = 0; j < c.cardCount[i]; ++j) {
                    tmp += std::format("{} ", cards::v2s.at(i));
                }
            }
        }
        return std::format_to(ctx.out(), "{}", tmp);
    }
};