#include <cstdint>
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
            for (int j = last_pos; j <= i - minlen + 1; ++j) {
                result.push_back(makeNewSerial(j, i, num));
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

// 王炸
inline vector<cards> genKingBomb(const cards &hand) {
    vector<cards> result;
    const int8 smallKingVal = 16; // cards::c2v.at('x');
    const int8 bigKingVal = 18;   // cards::c2v.at('d');
    if (hand.cardCount[smallKingVal] > 0 && hand.cardCount[bigKingVal] > 0) {
        cards c;
        c.cardCount[smallKingVal] = 1; // 小王
        c.cardCount[bigKingVal] = 1;   // 大王
        result.push_back(c);
    }
    return result;
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

// 在剩余牌中选择n张牌，返回所有可能结果
inline vector<cards> selectNfromRemaining(const cards &remaining, const int8 n) {
    vector<cards> result;
    if (n < 1) {
        throw std::invalid_argument("Invalid number of cards to select");
    }
    if (n == 0) {
        result.push_back(cards()); // 返回空牌
        return result;
    }
    if (n > remaining.cardNum())
        return result; // 没有足够的牌
    std::function<void(const cards &, cards, int8, int8)> func;
    func = [&result, n, &func](const cards &current, cards remaining, int8 start, int8 count) {
        if (count == n) {
            result.push_back(current);
            return;
        }
        for (int i = start; i < cards::N; ++i) {
            if (remaining.cardCount[i] > 0) {
                cards next = current;
                next.cardCount[i]++;
                cards newremain = remaining;
                newremain.cardCount[i]--;
                func(next, newremain, i, count + 1);
            }
        }
    };
    func(cards(), remaining, 0, 0);
    return result;
}

// 飞机
inline vector<cardMove> genSerialThreeX(const cards &hand, const int8 bNum) {
    vector<cardMove> res;
    auto serialTriple = genSerialTriple(hand);
    for (const auto &triple : serialTriple) {
        auto remainingCard = hand - triple; // 剩余的牌
        const int8 gNum = triple.cardNum() / 3;
        if (remainingCard.cardNum() < gNum * bNum) continue; // 没有足够的牌组成飞机

        for (auto &i : remainingCard.cardCount) {
            i /= bNum;
        }

        auto attachCardsList = selectNfromRemaining(remainingCard, gNum);
        for (auto &attachCards : attachCardsList) {
            for (auto &i : attachCards.cardCount) {
                i *= bNum;
            }
            res.push_back(cardMove(triple, attachCards, bNum == 1 ? MoveType::SERIAL_THREE_ONE : MoveType::SERIAL_THREE_TWO));
        }
    }
    return res;
}
inline vector<cardMove> genSerialThreeOne(const cards &hand) {
    return genSerialThreeX(hand, 1);
}
inline vector<cardMove> genSerialThreeTwo(const cards &hand) {
    return genSerialThreeX(hand, 2);
}
inline vector<cardMove> makeCardMovefromCards(const vector<cards> &v_cards, const MoveType type) {
    vector<cardMove> moves;
    for (const auto &hand : v_cards) {
        moves.push_back(cardMove(hand, cards(), type));
    }
    return moves;
}

inline vector<cardMove> genAllMoves(const cards &hand, const cardMove &lastMove) {
    vector<cardMove> result;

    const auto addMove2res = [&result](const vector<cardMove> &moves) {
        result.insert(result.end(), moves.begin(), moves.end());
    };
    const auto addCards2res = [&result](const vector<cards> &cardsList, MoveType type) {
        auto moves = makeCardMovefromCards(cardsList, type);
        result.insert(result.end(), moves.begin(), moves.end());
    };

    // 无论上一个是什么牌，都可以出王炸
    addCards2res(genKingBomb(hand), MoveType::KING_BOMB);
    // 如果上一个不是王炸，则可以出炸弹
    if (lastMove.type != MoveType::KING_BOMB) {
        addCards2res(genBomb(hand), MoveType::BOMB);
    }

    switch (lastMove.type) {
    case MoveType::INVALID: // 如果上一个是无效，说明是敌方先出模式，什么都可以出包括过
    case MoveType::PASS:
        // 如果上一个动作是过牌，则不能出过，但可以出其他所有
        addCards2res(genSingle(hand), MoveType::SINGLE);
        addCards2res(genPair(hand), MoveType::PAIR);
        addCards2res(genTriple(hand), MoveType::TRIPLE);
        addCards2res(genSerialSingle(hand), MoveType::SERIAL_SINGLE);
        addCards2res(genSerialPair(hand), MoveType::SERIAL_PAIR);
        addCards2res(genSerialTriple(hand), MoveType::SERIAL_TRIPLE);

        addMove2res(genThreeOne(hand));
        addMove2res(genThreeTwo(hand));
        addMove2res(genFourTwo(hand));
        addMove2res(genFourTwoTwo(hand));
        addMove2res(genSerialThreeOne(hand)); // 飞机带单
        addMove2res(genSerialThreeTwo(hand)); // 飞机带对
        break;
    case MoveType::SINGLE:
        addCards2res(genSingle(hand), MoveType::SINGLE);
        break;
    case MoveType::PAIR:
        addCards2res(genPair(hand), MoveType::PAIR);
        break;
    case MoveType::TRIPLE:
        addCards2res(genTriple(hand), MoveType::TRIPLE);
        break;
    case MoveType::SERIAL_SINGLE:
        addCards2res(genSerialSingle(hand), MoveType::SERIAL_SINGLE);
        break;
    case MoveType::SERIAL_PAIR:
        addCards2res(genSerialPair(hand), MoveType::SERIAL_PAIR);
        break;
    case MoveType::SERIAL_TRIPLE:
        addCards2res(genSerialTriple(hand), MoveType::SERIAL_TRIPLE);
        break;
    case MoveType::THREE_ONE:
        addMove2res(genThreeOne(hand));
        break;
    case MoveType::THREE_TWO:
        addMove2res(genThreeTwo(hand));
        break;
    case MoveType::FOUR_TWO:
        addMove2res(genFourTwo(hand));
        break;
    case MoveType::FOUR_TWO_TWO:
        addMove2res(genFourTwoTwo(hand));
        break;
    case MoveType::SERIAL_THREE_ONE:
        addMove2res(genSerialThreeOne(hand));
        break;
    case MoveType::SERIAL_THREE_TWO:
        addMove2res(genSerialThreeTwo(hand));
        break;
    case MoveType::BOMB:
    case MoveType::KING_BOMB:
    default:
        break;
    }
    // 过滤掉比lastMove小的牌
    result.erase(remove_if(result.begin(), result.end(),
                           [&lastMove](const cardMove &move) {
                               return move.compare(lastMove) != cardMove::moveCompareResult::GREATER;
                           }),
                 result.end());
    // 如果lastmove不为pass，则可以出过
    if (lastMove.type != MoveType::PASS) {
        result.push_back(cardMove(cards(), cards(), MoveType::PASS));
    }
    //倒置
    std::reverse(result.begin(), result.end());
    return result;
}
} // namespace moveGen