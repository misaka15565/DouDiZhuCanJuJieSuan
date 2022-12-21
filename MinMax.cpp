#include "MoveGener.cpp"
#include <string>
#include <unordered_map>
const int8 maxScore = 100;
const int8 minScore = 0;
const int8 our = 0;
const int8 enemy = 1;
struct status {
    cards ourCards;
    cards enemyCards;
    move lastMove;
    int8 currentPlayer;
};
string status2id(const status &x) {
    string id;
    char tmp[200] = {0};
    int k = 0;
    for (const auto &i : x.ourCards.cardCount) {
        for (int8 j = 0; j < i.second; j++) {
            tmp[k] = i.first + 'A';
            k++;
        }
    }
    tmp[k] = '|';
    k++;
    for (const auto &i : x.enemyCards.cardCount) {
        for (int8 j = 0; j < i.second; j++) {
            tmp[k] = i.first + 'A';
            k++;
        }
    }
    tmp[k] = '|';
    k++;
    for (const auto &i : x.lastMove.mainCard.cardCount) {
        for (int8 j = 0; j < i.second; j++) {
            tmp[k] = i.first + 'A';
            k++;
        }
    }
    tmp[k] = '|';
    k++;
    for (const auto &i : x.lastMove.subCard.cardCount) {
        for (int8 j = 0; j < i.second; j++) {
            tmp[k] = i.first + 'A';
            k++;
        }
    }
    tmp[k] = 'A' + x.lastMove.type;
    k++;
    tmp[k] = x.currentPlayer == our ? 'o' : 'e';
    k++;
    tmp[k] = 0;
    id = tmp;
    return id;
}
struct returned_result {
    int score;
    move bestMove;
};
std::unordered_map<string, returned_result> caldCache;
returned_result minMaxSearch(const status &x) {
    string id = status2id(x);
    auto pos = caldCache.find(id);
    if (pos != caldCache.end()) return pos->second;
    move tmp;
    tmp.type = TYPE_0_PASS;
    returned_result minEmptyResult = {minScore, tmp};
    returned_result maxEmptyResult = {maxScore, tmp};

    if (x.currentPlayer == enemy && x.ourCards.cardNum() == 0) {
        caldCache.insert(std::pair<string, returned_result>(id, maxEmptyResult));
        return maxEmptyResult;
    }
    if (x.currentPlayer == our && x.enemyCards.cardNum() == 0) {
        caldCache.insert(std::pair<string, returned_result>(id, minEmptyResult));
        return minEmptyResult;
    }

    if (x.currentPlayer == our) {
        possibleMoveSet possibleMoves(x.ourCards, x.lastMove);
        for (const auto &i : possibleMoves.moveSet) {
            status y = x;
            y.ourCards.remove(i.mainCard);
            y.ourCards.remove(i.subCard);
            y.currentPlayer = enemy;
            y.lastMove = i;
            returned_result result = minMaxSearch(y);
            if (result.score == maxScore) {
                caldCache.insert(std::pair<string, returned_result>(id, {maxScore, i}));
                return {maxScore, i};
            }
        }
        caldCache.insert(std::pair<string, returned_result>(id, minEmptyResult));
        return minEmptyResult;
    } else { // currentPlayer==enemy
        possibleMoveSet possibleMoves(x.enemyCards, x.lastMove);
        for (const auto &i : possibleMoves.moveSet) {
            status y = x;
            y.enemyCards.remove(i.mainCard);
            y.enemyCards.remove(i.subCard);
            y.currentPlayer = our;
            y.lastMove = i;
            returned_result result = minMaxSearch(y);
            if (result.score == minScore) {
                caldCache.insert(std::pair<string, returned_result>(id, {minScore, i}));
                return {minScore, i};
            }
        }
        caldCache.insert(std::pair<string, returned_result>(id, maxEmptyResult));
        return maxEmptyResult;
    }
}