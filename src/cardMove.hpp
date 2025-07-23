#include <cstdint>
#include <map>
#include <string>
#include <format>
#include "cards.hpp"

struct cardMove {
    using int8 = int8_t;
    enum class MoveType : int8_t {
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
        FOUR_TWO_TWO = 14
    };
    static constexpr int minSerialSingle = 5; // 最小顺子长度
    static constexpr int minSerialPair = 3;   // 最小对子长度
    static constexpr int minSerialTriple = 2; // 最小飞机长度

    cards maincards;   // 主牌
    cards attachcards; // 附牌
    enum class moveCompareResult : int8_t {
        INVALID = -1, // 无效
        EQUAL = 0,    // 相等
        LESS = 1,     // 小于
        GREATER = 2   // 大于
    };
    MoveType type; // 牌型
    inline cards getAllCards() const {
        cards all = maincards;
        for (int8 i = 0; i < cards::N; i++) {
            all.cardCount[i] += attachcards.cardCount[i];
        }
        return all;
    }
    // 比较this和b的大小
    //  返回值：INVALID表示无效，EQUAL表示相等，LESS表示this < b，GREATER表示this > b
    inline moveCompareResult compare(const cardMove &b) const {
        if (type == MoveType::PASS && b.type == MoveType::PASS) return moveCompareResult::EQUAL;
        if (type == MoveType::PASS) return moveCompareResult::LESS;
        if (b.type == MoveType::PASS) return moveCompareResult::GREATER;

        if (type != b.type) {
            if (type == MoveType::KING_BOMB) return moveCompareResult::GREATER;
            if (b.type == MoveType::KING_BOMB) return moveCompareResult::LESS;
            if (type == MoveType::BOMB) return moveCompareResult::GREATER;
            if (b.type == MoveType::BOMB) return moveCompareResult::LESS;
            return moveCompareResult::INVALID;
        }

        // 同类型比较
        switch (type) {
        case MoveType::SINGLE:
        case MoveType::PAIR:
        case MoveType::TRIPLE:
        case MoveType::BOMB:
        case MoveType::THREE_ONE:
        case MoveType::THREE_TWO:
        case MoveType::FOUR_TWO:
        case MoveType::FOUR_TWO_TWO:
            return maincards.getMaxCard() > b.maincards.getMaxCard() ?
                       moveCompareResult::GREATER :
                       (maincards.getMaxCard() < b.maincards.getMaxCard() ?
                            moveCompareResult::LESS :
                            moveCompareResult::EQUAL);

        case MoveType::SERIAL_SINGLE:
        case MoveType::SERIAL_PAIR:
        case MoveType::SERIAL_TRIPLE:
        case MoveType::SERIAL_THREE_ONE:
        case MoveType::SERIAL_THREE_TWO:
            if (maincards.cardNum() != b.maincards.cardNum()) return moveCompareResult::INVALID;
            if (maincards.getMaxCard() > b.maincards.getMaxCard()) {
                return moveCompareResult::GREATER;
            } else if (maincards.getMaxCard() < b.maincards.getMaxCard()) {
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
        maincards(main), attachcards(attach), type(t) {
    }
    inline cardMove(MoveType t):maincards(),attachcards(), type(t) {
        if(t!=MoveType::PASS) {
            throw std::invalid_argument("Cannot create a cardMove which is not PASS type without cards");
        }
    }
    inline cardMove(cards main, MoveType t) :
        maincards(main), attachcards(), type(t) {
        if (t != MoveType::SINGLE && t != MoveType::PAIR && t != MoveType::TRIPLE &&
            t != MoveType::BOMB && t != MoveType::KING_BOMB && t!=MoveType::SERIAL_SINGLE &&
            t!=MoveType::SERIAL_PAIR && t!=MoveType::SERIAL_TRIPLE) {
            throw std::invalid_argument("Cannot create a cardMove with only main cards for this type");
        }
    }
};
template<>
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
        MOVE_TYPES_STR{{MoveType::PASS, "过"},
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
        return std::format_to(ctx.out(), "{} {} {}", MOVE_TYPES_STR.at(m.type), m.maincards, m.attachcards);
    }
};