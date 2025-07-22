#include <array>
#include <map>
#include <string>
#include <format>

using int8 = int8_t;
using std::string;
class cards {
public:
    using cardVal = int8_t;
    static constexpr int8 N = 21; // Number of card values, including 3-9, 0, J, Q, K, A, 2, X (small king), D (big king)
    inline static const std::map<char, cardVal> c2v = {
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
        {'2', 16},
        {'X', 18},
        {'x', 18},
        {'D', 20},
        {'d', 20}};
    inline static const std::map<cardVal, std::string> v2s = {
        {3, "3"},
        {4, "4"},
        {5, "5"},
        {6, "6"},
        {7, "7"},
        {8, "8"},
        {9, "9"},
        {10, "10"},
        {11, "J"},
        {12, "Q"},
        {13, "K"},
        {14, "A"},
        {16, "2"},
        {18, "小王"},
        {20, "大王"}};
    // cardCount[i] = j means there are j cards of value i
    std::array<int8, N> cardCount;
    inline cards(std::string s): cardCount{} {
        for (char c : s) {
            if (c2v.find(c) != c2v.end()) {
                cardCount[c2v.at(c)]++;
            }
        }
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
};

template <>
struct std::formatter<cards> {
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