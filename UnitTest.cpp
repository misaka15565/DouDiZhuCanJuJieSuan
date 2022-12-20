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
    lastMove.type = TYPE_0_PASS;
    cards a;
    a.cardCount[c2v['3']] = 4;
    a.cardCount[c2v['4']] = 4;
    

    possibleMoveSet moveset(a, lastMove);
    for (auto i : moveset.moveSet) {
        ptmove(i);
    }
    return 0;
}