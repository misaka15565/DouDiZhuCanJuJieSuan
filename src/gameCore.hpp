#include <cstdint>
#include <unordered_map>
#include <tuple>
#include "moveGen.hpp"

enum class FACTION : int8_t {
    OUR_SIDE,
    ENEMY_SIDE
};
constexpr int8_t max_score = 99;
constexpr int8_t min_score = -99;
// 可以永远只为一方计算，只要swap两方的牌
struct gameStatus {
    cards curPlayerHand;
    cards curEnemyHand;
    cardMove lastMove;
    inline bool operator==(const gameStatus &other) const {
        return std::tie(curPlayerHand, curEnemyHand, lastMove) == std::tie(other.curPlayerHand, other.curEnemyHand, other.lastMove);
    }
};
template <>
struct std::hash<gameStatus> {
    inline std::size_t operator()(const gameStatus &s) const noexcept {
        return std::hash<std::string_view>()(
            std::string_view(reinterpret_cast<const char *>(&s), sizeof(s)));
    }
};

struct bestAction {
    cardMove move; // 最佳出牌
    int8_t score;  // 分数
    inline bool operator==(const bestAction &other) const {
        return move == other.move && score == other.score;
    }
};
template <>
struct std::hash<bestAction> {
    inline std::size_t operator()(const bestAction &a) const noexcept {
        return std::hash<std::string_view>()(
            std::string_view(reinterpret_cast<const char *>(&a), sizeof(a)));
    }
};
inline std::unordered_map<gameStatus, bestAction> actionCache; // 缓存最佳动作
class gameCore {
    cards ourHand;     // 我方手牌
    cards enemyHand;   // 敌方手牌
    cardMove lastMove; // 上一手牌
    FACTION curPlayer; // 当前玩家

public:
    inline gameCore(cards our, cards enemy) :
        ourHand(our), enemyHand(enemy), lastMove(cardMove::MoveType::PASS), curPlayer(FACTION::OUR_SIDE) {
    }
    inline gameCore(cards our, cards enemy, cardMove last) :
        ourHand(our), enemyHand(enemy), lastMove(last), curPlayer(FACTION::OUR_SIDE) {
    }
    // 获取当前玩家
    inline FACTION getCurrentPlayer() const {
        return curPlayer;
    }
    // 获取我方手牌
    inline cards getOurHand() const {
        return ourHand;
    }
    // 获取敌方手牌
    inline cards getEnemyHand() const {
        return enemyHand;
    }
    // 获取上一手牌
    inline cardMove getLastMove() const {
        return lastMove;
    }
    inline bool isGameOver() const {
        return ourHand.cardNum() == 0 || enemyHand.cardNum() == 0;
    }
    // 获取我方手牌当前可能的出法
    inline std::vector<cardMove> getPossibleMoves() const {
        return moveGen::genAllMoves(ourHand, lastMove);
    }
    // 获取敌方手牌当前可能的出法
    inline std::vector<cardMove> getEnemyPossibleMoves() const {
        return moveGen::genAllMoves(enemyHand, lastMove);
    }

    // 获取当前状态
    inline gameStatus getCurrentStatus() const {
        auto curPlayerHand = (curPlayer == FACTION::OUR_SIDE) ? ourHand : enemyHand;
        auto curEnemyHand = (curPlayer == FACTION::OUR_SIDE) ? enemyHand : ourHand;
        return {curPlayerHand, curEnemyHand, lastMove};
    }

    // 当前玩家出牌
    inline bool playCard(cardMove move) {
        auto &curPlayer_hand = (curPlayer == FACTION::OUR_SIDE) ? ourHand : enemyHand;
        auto allpossibleMoves = moveGen::genAllMoves(curPlayer_hand, lastMove);
        if (std::find(allpossibleMoves.begin(), allpossibleMoves.end(), move) != allpossibleMoves.end()) {
            // 出牌逻辑
            lastMove = move;
            auto res = curPlayer_hand.remove(move.getAllCards());
            if (!res) throw std::invalid_argument("Should not reach here, invalid move");
            curPlayer = (curPlayer == FACTION::OUR_SIDE) ? FACTION::ENEMY_SIDE : FACTION::OUR_SIDE;
            return true;
        }
        return false;
    }
};

// 计算当前状态下的最佳动作
inline bestAction calculateBestAction(const gameStatus &status) {
    // 在缓存中检查
    auto it = actionCache.find(status);
    if (it != actionCache.end()) {
        return it->second; // 如果找到，直接返回缓存的结果
    }
    // 检查当前是否有玩家已经出完
    if (status.curPlayerHand.cardNum() == 0 || status.curEnemyHand.cardNum() == 0) {
        throw std::runtime_error("Game over, one player has no cards left, and should not reach here");
    }

    // 列出curplayer方所有可能的出牌
    auto movelist = moveGen::genAllMoves(status.curPlayerHand, status.lastMove);
    if (movelist.empty()) {
        throw std::runtime_error("No valid moves available, should not reach here");
    }

    for (const auto &move : movelist) {
        // 检查是否出完这个move就获胜了
        if (status.curPlayerHand.cardNum() == move.getAllCards().cardNum()) {
            // 如果出完了，直接返回这个动作
            actionCache[status] = {move, max_score};
            return actionCache[status];
        }
        // 计算新状态
        gameStatus newStatus = status;
        newStatus.curPlayerHand.remove(move.getAllCards());
        newStatus.lastMove = move;
        std::swap(newStatus.curPlayerHand, newStatus.curEnemyHand); // 换玩家
        // 递归计算对方的最佳动作
        bestAction enemyBest = calculateBestAction(newStatus);
        // 如果此动作得到敌方的maxscore，说明动作不可行
        if (enemyBest.score == max_score) {
            continue; // 这个动作不可行，跳过
        } else {
            actionCache[status] = {move, max_score};
            return {move, max_score}; // 返回这个动作
        }
    }
    // 没有合适的动作，敌方必胜
    actionCache[status] = {cardMove::MoveType::INVALID, min_score};
    return {cardMove::MoveType::INVALID, min_score};
}