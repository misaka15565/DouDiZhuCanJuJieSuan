#include <corecrt.h>
#include <iostream>
#include "MoveGener.cpp"
using std::cout;
using std::endl;
void ptmove(move x) {
    cout << MOVE_TYPES_STR[x.type];
    cout << "|| ";
    for (auto i : x.mainCard.cardCount) {
        for (int8 j = 0; j < i.second; j++) {
            cout << v2s[i.first] << " ";
        }
    }
    cout << "|| ";
    for (auto i : x.subCard.cardCount) {
        for (int8 j = 0; j < i.second; j++) {
            cout << v2s[i.first] << " ";
        }
    }
    cout << endl;
}
int main(int argc, char **argv) {
    move lastMove;
    lastMove.type = TYPE_11_SERIAL_3_1;
    lastMove.mainCard.cardCount[c2v['3']]=3;
    lastMove.mainCard.cardCount[c2v['4']]=3;
    lastMove.subCard.cardCount[c2v['9']]=1;
    lastMove.subCard.cardCount[c2v['0']]=1;
    cards a;
    a.cardCount[c2v['3']] = 1;
    a.cardCount[c2v['4']] = 1;
    a.cardCount[c2v['5']] = 4;
    a.cardCount[c2v['6']] = 4;
    a.cardCount[c2v['7']] = 4;
    a.cardCount[c2v['8']] = 4;
    a.cardCount[c2v['x']] = 1;
    a.cardCount[c2v['d']] = 1;
    possibleMoveSet moveset(a, lastMove);
    for (auto i : moveset.moveSet) {
        ptmove(i);
    }
    return 0;
}