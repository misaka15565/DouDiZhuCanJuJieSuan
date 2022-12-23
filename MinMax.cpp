#include "MoveGener.cpp"
#include <string>
#include <unordered_map>
#include <thread>
#include <vector>
#include "easyMap.hpp"
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
    for (int8 i = 0; i < N; i++) {
        for (int8 j = 0; j < x.ourCards.cardCount[i]; j++) {
            tmp[k] = i + 'A';
            k++;
        }
    }
    tmp[k] = 'A' + x.lastMove.type;
    k++;
    for (int8 i = 0; i < N; i++) {
        for (int8 j = 0; j < x.enemyCards.cardCount[i]; j++) {
            tmp[k] = i + 'A';
            k++;
        }
    }
    tmp[k] = x.currentPlayer == our ? 'o' : 'e';
    k++;
    for (int8 i = 0; i < N; i++) {
        for (int8 j = 0; j < x.lastMove.mainCard.cardCount[i]; j++) {
            tmp[k] = i + 'A';
            k++;
        }
    }
    tmp[k] = '|';
    k++;
    for (int8 i = 0; i < N; i++) {
        for (int8 j = 0; j < x.lastMove.subCard.cardCount[i]; j++) {
            tmp[k] = i + 'A';
            k++;
        }
    }

    tmp[k] = 0;
    id = tmp;
    return id;
}
struct returned_result {
    int score;
    move bestMove;
};

struct task {
    status first;
    status second;
};

easyMAP<string, returned_result> caldCache;
void buildMap(status x, returned_result &result) {
    string id = status2id(x);
    returned_result res2;
    auto flag = caldCache.find(id, res2);
    if (flag == true) {
        result = res2;
        return;
    }
    move tmp;
    tmp.type = TYPE_0_PASS;
    returned_result minEmptyResult = {minScore, tmp};
    returned_result maxEmptyResult = {maxScore, tmp};

    if (x.currentPlayer == enemy && x.ourCards.cardNum() == 0) {
        caldCache.insert(id, maxEmptyResult);
        result = maxEmptyResult;
        return;
    }
    if (x.currentPlayer == our && x.enemyCards.cardNum() == 0) {
        caldCache.insert(id, minEmptyResult);
        result = minEmptyResult;
        return;
    }

    possibleMoveSet possibleMoves(x.currentPlayer == our ? x.ourCards : x.enemyCards, x.lastMove);
    for (auto i = possibleMoves.moveSet.rbegin(); i != possibleMoves.moveSet.rend(); i++) {
        status y = x;
        if (x.currentPlayer == our) {
            y.ourCards.remove(i->mainCard);
            y.ourCards.remove(i->subCard);
            y.currentPlayer = enemy;
        } else {
            y.enemyCards.remove(i->mainCard);
            y.enemyCards.remove(i->subCard);
            y.currentPlayer = our;
        }
        y.lastMove = *i;

        buildMap(y, res2);
        if (x.currentPlayer == our) {
            if (res2.score == maxScore) {
                caldCache.insert(id, {maxScore, *i});
                result = {maxScore, *i};
                return;
            }
        } else {
            if (res2.score == minScore) {
                caldCache.insert(id, {minScore, *i});
                result = {minScore, *i};
                return;
            }
        }
    }
    if (x.currentPlayer == our) {
        caldCache.insert(id, minEmptyResult);
        result = minEmptyResult;
        return;
    } else {
        caldCache.insert(id, maxEmptyResult);
        result = maxEmptyResult;
        return;
    }
}

returned_result minMaxSearch(const status &x) {
    returned_result result;
    if (caldCache.isEmpty()) buildMap(x, result);
    string id = status2id(x);
    caldCache.find(id, result);
    return result;
}
