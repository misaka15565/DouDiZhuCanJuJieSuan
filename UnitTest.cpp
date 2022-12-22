#include <iostream>
#include "MinMax.cpp"
using std::cout;
using std::endl;
void ptmove(move x) {
    cout << MOVE_TYPES_STR[x.type];
    cout << "|| ";
    for (int8 i=0;i<N;i++) {
        for (int8 j = 0; j < x.mainCard.cardCount[i]; j++) {
            cout << v2s[i] << " ";
        }
    }
    cout << "|| ";
    for (int8 i=0;i<N;i++) {
        for (int8 j = 0; j < x.subCard.cardCount[i]; j++) {
            cout << v2s[i] << " ";
        }
    }
    cout << endl;
}
int main(int argc, char **argv) {
    move lastMove;
    lastMove.type = TYPE_0_PASS;
    cards ours;
    ours.cardCount[c2v['d']] = 1;
    ours.cardCount[c2v['x']] = 1;
    cards enemy;
    enemy.cardCount[c2v['5']] = 1;
    enemy.cardCount[c2v['3']] = 0;
    enemy.cardCount[c2v['4']] = 0;
    status x={ours,enemy,lastMove,our};
    returned_result result=minMaxSearch(x);
    possibleMoveSet t(ours,lastMove);
    for(auto i:t.moveSet)ptmove(i);
    cout<<result.score<<endl;
    ptmove(result.bestMove);
    return 0;
}