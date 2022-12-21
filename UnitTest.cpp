#include <iostream>
#include "MinMax.cpp"
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
    cards ours;
    ours.cardCount[c2v['3']] = 4;
    ours.cardCount[c2v['4']] = 1;
    cards enemy;
    enemy.cardCount[c2v['5']] = 1;
    status x={ours,enemy,lastMove,our};
    returned_result result=minMaxSearch(x);
    possibleMoveSet t(ours,lastMove);
    for(auto i:t.moveSet)ptmove(i);
    cout<<result.score<<endl;
    ptmove(result.bestMove);
    return 0;
}