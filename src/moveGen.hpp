#include <cstdint>
#include <print>
#include <vector>
#include <algorithm>
#include "cardMove.hpp"

namespace moveGen {

using std::vector;
using int8 = int8_t;
inline vector<cards> gen1234(const cards &hand, const int8 num) {
    vector<cards> result;
    for (int8 i = 0; i < cards::N; i++) {
        if (hand.cardCount[i] >= num) {
            cards c;
            c.cardCount[i] = num;
            result.push_back(c);
        }
    }
    return result;
}

inline cards makeNewSerial(int from, int to, const int8 num) {
    if (from < 0 || to >= cards::N || from > to || num < 1 || num > 4) {
        throw std::invalid_argument("Invalid range or num");
    }
    cards res;
    for (int i = from; i <= to; ++i) {
        res.cardCount[i] = num;
    }
    return res;
}

inline vector<cards> genSerial(const cards &hand, const int8 num, const int8 minlen) {
    vector<cards> result;
    if (num < 1 || num > 4) {
        throw std::invalid_argument("num must be between 1 and 4");
    }
    int last_pos = -1;
    for (int i = 0; i < cards::N; ++i) {
        if (hand.cardCount[i] < num) {
            last_pos = -1;
            continue;
        } else if (last_pos < 0) {
            last_pos = i;
        }
        if (i - last_pos + 1 >= minlen) {
            for (int j = i; j >= last_pos + minlen; --j) {
                result.push_back(makeNewSerial(last_pos, j, num));
            }
        }
    }
    return result;
}

// 单张生成
inline vector<cards> genSingle(const cards &hand) {
    return gen1234(hand, 1);
}

inline vector<cards> genPair(const cards &hand) {
    return gen1234(hand, 2);
}

inline vector<cards> genTriple(const cards &hand) {
    return gen1234(hand, 3);
}

inline vector<cards> genBomb(const cards &hand) {
    return gen1234(hand, 4);
}

// 顺子、连对、连续三张生成
inline vector<cards> genSerialSingle(const cards &hand) {
    return genSerial(hand, 1, cardMove::minSerialSingle);
}
inline vector<cards> genSerialPair(const cards &hand) {
    return genSerial(hand, 2, cardMove::minSerialPair);
}
inline vector<cards> genSerialTriple(const cards &hand) {
    return genSerial(hand, 3, cardMove::minSerialTriple);
}
using MoveType = cardMove::MoveType;
// 三带一、三带二、四带二、四带二对
inline vector<cardMove> genAwithB(const cards &hand, const MoveType type) {
    // A卡的数量
    const int8 aNum = (type == MoveType::THREE_ONE || type == MoveType::THREE_TWO) ? 3 : 4;
    // B卡一组的数量
    const int8 bNum = (type == MoveType::THREE_ONE || type == MoveType::FOUR_TWO) ? 1 : 2;
    // B卡的组数
    const int8 gNum = (type == MoveType::FOUR_TWO || type == MoveType::FOUR_TWO_TWO) ? 2 : 1;
    vector<cardMove> moves;
    vector<cards> aCards = gen1234(hand, aNum);
    for (const auto &a : aCards) {
        cards remainingCard = hand - a; // 剩余的牌
        vector<cards> bCards = gen1234(remainingCard, bNum);
        if (gNum == 1) {
            for (const auto &b : bCards) {
                moves.push_back(cardMove(a, b, type));
            }
        } else if (gNum == 2) {
            for (const auto &b1 : bCards) {
                for (const auto &b2 : bCards) {
                    if (b1 != b2) {
                        moves.push_back(cardMove(a, b1 + b2, type));
                    }
                }
            }
            // 现在处理两组牌是同点数牌的情况
            auto bcardsSame = gen1234(remainingCard, bNum * gNum);
            for (const auto &b : bcardsSame) {
                moves.push_back(cardMove(a, b, type));
            }
        } else {
            throw std::invalid_argument("Invalid group number for AwithB move generation");
        }
    }
    return moves;
}

// 三带一、三带二
inline vector<cardMove> genThreeOne(const cards &hand) {
    return genAwithB(hand, MoveType::THREE_ONE);
}
inline vector<cardMove> genThreeTwo(const cards &hand) {
    return genAwithB(hand, MoveType::THREE_TWO);
}
// 四带二、四带两对
inline vector<cardMove> genFourTwo(const cards &hand) {
    return genAwithB(hand, MoveType::FOUR_TWO);
}
inline vector<cardMove> genFourTwoTwo(const cards &hand) {
    return genAwithB(hand, MoveType::FOUR_TWO_TWO);
}
//飞机
inline vector<cardMove> genSerialThreeX(const cards &hand,const int8 bNum) {
    vector<cardMove> res;
    auto serialTriple = genSerialTriple(hand);
    for (const auto &triple : serialTriple) {
        auto remainingCard = hand - triple; // 剩余的牌
        const int8 gNum = triple.cardNum() / 3;
        if(remainingCard.cardNum() < gNum * bNum) continue; // 没有足够的牌组成飞机
        
    }
    return res;
}



inline vector<cardMove> genAllMoves(const cards &hand, const cards &lastMove) {
    vector<cardMove> moves;
    return moves;
}
} // namespace moveGen