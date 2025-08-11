#include <cstdint>
#include <map>
#include <string>
#include <format>
#include "cards.hpp"

struct cardMove {
    static const int8_t moveTypeOffset = 12; // cards::c2v['t'];
    using int8 = int8_t;
    enum class MoveType : uint8_t {
        PASS = 0,
        SINGLE = 1,
        PAIR = 2,
        TRIPLE = 3,
        BOMB = 4,
        KING_BOMB = 5,
        THREE_ONE = 6,
        THREE_TWO = 7,
        SERIAL_SINGLE = 8,
        SERIAL_PAIR = 9,
        SERIAL_TRIPLE = 10,
        SERIAL_THREE_ONE = 11, // 三带一的飞机
        SERIAL_THREE_TWO = 12,
        FOUR_TWO = 13,
        FOUR_TWO_TWO = 14,
        INVALID = 15, // 无效
    };
    static constexpr int minSerialSingle = 5; // 最小顺子长度
    static constexpr int minSerialPair = 3;   // 最小对子长度
    static constexpr int minSerialTriple = 2; // 最小飞机长度

    decltype(cards::cardCount) attach_main; // 低4位是主牌，高4位是附牌

    enum class moveCompareResult : int8_t {
        INVALID = -1, // 无效
        EQUAL = 0,    // 相等
        LESS = 1,     // 小于
        GREATER = 2   // 大于
    };
    // MoveType type; // 牌型
    inline MoveType getType() const {
        return static_cast<MoveType>(attach_main[moveTypeOffset]);
    }
    inline cards getMainCards() const {
        cards main;
        for (int8 i = 0; i < cards::N; i++) {
            main.cardCount[i] = (attach_main[i] & 0x0F);
        }
        main.cardCount[moveTypeOffset] = 0; // 清除类型位
        return main;
    }
    inline cards getAttachCards() const {
        cards attach;
        for (int8 i = 0; i < cards::N; i++) {
            attach.cardCount[i] = (attach_main[i] >> 4) & 0x0F;
        }
        // 因为attach存在高4位所以无需清除类型位
        return attach;
    }

    inline cards::cardVal getMainCardMaxVal() const {
        for (int8 i = cards::N - 1; i >= 0; --i) {
            if ((attach_main[i] & 0x0F) > 0 && i != moveTypeOffset) return i;
        }
        return -1; // No cards
    }

    inline cards getAllCards() const {
        cards all;
        for (int8 i = 0; i < cards::N; i++) {
            all.cardCount[i] = (attach_main[i] & 0x0F) | ((attach_main[i] & 0xF0) >> 4);
        }
        all.cardCount[moveTypeOffset] = 0; // 清除类型位
        return all;
    }

    inline int8 getAllCardNum() const {
        int8 count = 0;
        for (int8 i = 0; i < cards::N; i++) {
            count += (attach_main[i] & 0x0F) + ((attach_main[i] & 0xF0) >> 4);
        }
        return count - attach_main[moveTypeOffset];
    }

    // 比较this和b的大小
    //  返回值：INVALID表示无效，EQUAL表示相等，LESS表示this < b，GREATER表示this > b
    inline moveCompareResult compare(const cardMove &b) const {
        if (getType() == MoveType::PASS && b.getType() == MoveType::PASS) return moveCompareResult::EQUAL;
        if (getType() == MoveType::PASS || getType() == MoveType::INVALID) return moveCompareResult::LESS;
        if (b.getType() == MoveType::PASS || b.getType() == MoveType::INVALID) return moveCompareResult::GREATER;

        if (getType() != b.getType()) {
            if (getType() == MoveType::KING_BOMB) return moveCompareResult::GREATER;
            if (b.getType() == MoveType::KING_BOMB) return moveCompareResult::LESS;
            if (getType() == MoveType::BOMB) return moveCompareResult::GREATER;
            if (b.getType() == MoveType::BOMB) return moveCompareResult::LESS;
            return moveCompareResult::INVALID;
        }

        // 同类型比较
        switch (getType()) {
        case MoveType::SINGLE:
        case MoveType::PAIR:
        case MoveType::TRIPLE:
        case MoveType::BOMB:
        case MoveType::THREE_ONE:
        case MoveType::THREE_TWO:
        case MoveType::FOUR_TWO:
        case MoveType::FOUR_TWO_TWO:
            return getMainCardMaxVal() > b.getMainCardMaxVal() ?
                       moveCompareResult::GREATER :
                       (getMainCardMaxVal() < b.getMainCardMaxVal() ?
                            moveCompareResult::LESS :
                            moveCompareResult::EQUAL);

        case MoveType::SERIAL_SINGLE:
        case MoveType::SERIAL_PAIR:
        case MoveType::SERIAL_TRIPLE:
        case MoveType::SERIAL_THREE_ONE:
        case MoveType::SERIAL_THREE_TWO:
            if (getAllCardNum() != b.getAllCardNum()) return moveCompareResult::INVALID;
            if (getMainCardMaxVal() > b.getMainCardMaxVal()) {
                return moveCompareResult::GREATER;
            } else if (getMainCardMaxVal() < b.getMainCardMaxVal()) {
                return moveCompareResult::LESS;
            } else {
                return moveCompareResult::EQUAL;
            }
        default:
            throw std::runtime_error("Should not reach here, invalid move type");
            return moveCompareResult::INVALID;
        }
    }
    inline cardMove(cards main, cards attach, MoveType t) :
        attach_main() {
        for (int8 i = 0; i < cards::N; i++) {
            attach_main[i] = (main.cardCount[i] & 0x0F) | ((attach.cardCount[i] & 0x0F) << 4);
        }
        attach_main[moveTypeOffset] = static_cast<uint8_t>(t);
    }
    inline cardMove(MoveType t) :
        attach_main() {
        if (t != MoveType::PASS && t != MoveType::INVALID) {
            throw std::invalid_argument("Cannot create a cardMove which is not PASS type without cards");
        }
        attach_main[moveTypeOffset] = static_cast<uint8_t>(t);
    }
    inline cardMove() :
        attach_main() {
        attach_main[moveTypeOffset] = static_cast<uint8_t>(MoveType::PASS);
    }
    inline cardMove(cards main, MoveType t) :
        attach_main() {
        for (int8 i = 0; i < cards::N; i++) {
            attach_main[i] = (main.cardCount[i] & 0x0F);
        }
        attach_main[moveTypeOffset] = static_cast<uint8_t>(t);
    }
    inline bool operator==(const cardMove &b) const {
        return attach_main == b.attach_main;
    }
};
template <>
struct std::hash<cardMove> {
    inline std::size_t operator()(const cardMove &m) const noexcept {
        return std::hash<std::string_view>()(
            std::string_view(reinterpret_cast<const char *>(&m), sizeof(m)));
    }
};

template <>
struct std::formatter<cardMove> {
    using MoveType = cardMove::MoveType;
    inline static const map<MoveType, string>
        MOVE_TYPES_STR{{MoveType::INVALID, "无效"},
                       {MoveType::PASS, "过"},
                       {MoveType::SINGLE, "单张"},
                       {MoveType::PAIR, "对子"},
                       {MoveType::TRIPLE, "三张"},
                       {MoveType::BOMB, "炸弹"},
                       {MoveType::KING_BOMB, "王炸"},
                       {MoveType::THREE_ONE, "三带一"},
                       {MoveType::THREE_TWO, "三带二"},
                       {MoveType::SERIAL_SINGLE, "顺子"},
                       {MoveType::SERIAL_PAIR, "连对"},
                       {MoveType::SERIAL_TRIPLE, "飞机"},
                       {MoveType::SERIAL_THREE_ONE, "三带一组成的飞机"},
                       {MoveType::SERIAL_THREE_TWO, "三带二组成的飞机"},
                       {MoveType::FOUR_TWO, "四带二"},
                       {MoveType::FOUR_TWO_TWO, "四带两对"}};
    inline constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }
    inline auto format(const cardMove &m, format_context &ctx) const {
        return std::format_to(ctx.out(), "{} {} {}", MOVE_TYPES_STR.at(m.getType()), m.getMainCards(), m.getAttachCards());
    }
};