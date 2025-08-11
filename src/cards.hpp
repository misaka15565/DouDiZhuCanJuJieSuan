#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <format>
#define DDZ_SPECIAL
struct cards {
    using int8 = int8_t;
    using cardVal = int8_t;
// 你知道吗，如果是一副牌的斗地主，完全不需要让2 小王 大王的值隔开，因为小王大王只有一张的说，不会组成2xd的连对的说
#ifndef DDZ_SPECIAL
    static constexpr int8 N = 18; // Number of card values, including 3-9, 10, J, Q, K, A, 2, X (small king), D (big king)
    inline static const std::map<char, cardVal> c2v = {
        {'3', 0},
        {'4', 1},
        {'5', 2},
        {'6', 3},
        {'7', 4},
        {'8', 5},
        {'9', 6},
        {'0', 7}, // 用0代表10
        {'J', 8},
        {'j', 8},
        {'Q', 9},
        {'q', 9},
        {'K', 10},
        {'k', 10},
        {'A', 11},
        {'a', 11},
        {'2', 13},
        {'X', 15},
        {'x', 15},
        {'D', 17},
        {'d', 17}};
    inline static const std::map<cardVal, std::string> v2s = {
        {0, "3"},
        {1, "4"},
        {2, "5"},
        {3, "6"},
        {4, "7"},
        {5, "8"},
        {6, "9"},
        {7, "10"},
        {8, "J"},
        {9, "Q"},
        {10, "K"},
        {11, "A"},
        {13, "2"},
        {15, "小王"},
        {17, "大王"}};
#else
    static constexpr int8 N = 16; // Number of card values, including 3-9, 10, J, Q, K, A, 2, X (small king), D (big king)
    inline static const std::map<char, cardVal> c2v = {
        {'3', 0},
        {'4', 1},
        {'5', 2},
        {'6', 3},
        {'7', 4},
        {'8', 5},
        {'9', 6},
        {'0', 7}, // 用0代表10
        {'J', 8},
        {'j', 8},
        {'Q', 9},
        {'q', 9},
        {'K', 10},
        {'k', 10},
        {'A', 11},
        {'a', 11},
        {'t', 12},//t代表这个牌不存在，这个位置给cardmove存出牌种类
        {'2', 13},
        {'X', 14},
        {'x', 14},
        {'D', 15},
        {'d', 15}};
    inline static const std::map<cardVal, std::string> v2s = {
        {0, "3"},
        {1, "4"},
        {2, "5"},
        {3, "6"},
        {4, "7"},
        {5, "8"},
        {6, "9"},
        {7, "10"},
        {8, "J"},
        {9, "Q"},
        {10, "K"},
        {11, "A"},
        {13, "2"},
        {14, "小王"},
        {15, "大王"}};
#endif

    // cardCount[i] = j means there are j cards of value i
    std::array<uint8_t, N> cardCount;
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
struct std::hash<cards> {
    inline std::size_t operator()(const cards &c) const noexcept {
        return std::hash<std::string_view>()(
            std::string_view(reinterpret_cast<const char *>(c.cardCount.data()), sizeof(c.cardCount)));
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