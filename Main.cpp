#include <iostream>
#include "MinMax.cpp"
#include <ctime>
using std::cout;
using std::cin;
using std::endl;
void ptcards(const cards &x) {
    for (int8 i=0;i<N;i++) {
        for (int8 j = 0; j < x.cardCount[i]; j++) {
            cout << v2s[i] << " ";
        }
    }
}
void ptmove(const move &x) {
    cout << MOVE_TYPES_STR[x.type];
    cout << "-- ";
    ptcards(x.mainCard);
    if (x.subCard.cardNum() != 0) {
        cout << "-- ";
        ptcards(x.subCard);
    }
    cout << endl;
}
cards str2cards(const string &x) {
    const char *p = x.c_str();
    cards result;
    for (int i = 0; p[i] != 0; i++) {
        if (p[i] == 'e' || p[i] == 'E') exit(0); // 输入e或E退出程序
        auto pos = c2v.find(p[i]);
        if (pos == c2v.end()) continue; // 若输入不合法字符直接跳过
        result.cardCount[pos->second]++;
    }
    return result;
}
move getMove(const cards &x, move lastMove) {
    while (true) {
        move result;
        cout << "---------------------------\n";
        cout << "牌：";
        ptcards(x);
        cout << "\n上一次出牌：";
        ptmove(lastMove);
        cout << "---------------------------\n";
        for (const auto &i : MOVE_TYPES_STR) {
            cout << (int)i.first << "<=>" << i.second << " ";
        }
        cout << "\n请输入牌型代码：\n";
        int type;
        cin >> type;
        type %= 0x100;
        if (MOVE_TYPES_STR.find(type) == MOVE_TYPES_STR.end()) {
            cout << "输入错误，请从头重新输入\n";
            continue;
        }
        result.type = type;
        cout << "请输入主要牌，若牌型为过请输入p：\n";
        string t;
        cin >> t;
        if (t != "p") result.mainCard = str2cards(t);
        cout << "请输入次要牌，若无请输入p：\n";
        cin >> t;
        if (t != "p") result.subCard = str2cards(t);
        possibleMoveSet p(x, lastMove);
        if (std::find(p.moveSet.begin(), p.moveSet.end(), result) == p.moveSet.end()) {
            cout << "输入错误或输入不可能，请从头重新输入\n";
            continue;
        }
        return result;
    }
}
int main(int argc, char **argv) {
    cout << "---------------------------\n";
    cout << "Ctrl+C - 退出程序" << endl;
    cout << "---------------------------\n";
    cout << "x小王 d大王 0代表10\n";
    cout << "主要牌指三带一、三带二中的三，四带二、四带两对的四，飞机的机体\n";
    cout << "如3334中的333，44445566中的4444,33344456中的333444\n";
    cout << "---------------------------\n";
    string strourcards;
    string strenemycards;
    cards ours;
    cards enemys;
    cout << "请输入我方的牌:\n";
    cin >> strourcards;
    ours = str2cards(strourcards);
    cout << "请输入对方的牌:\n";
    cin >> strenemycards;
    enemys = str2cards(strenemycards);
    cout << "我方的牌：";
    ptcards(ours);
    cout << "对方的牌：";
    ptcards(enemys);
    clock_t start, end;
    status x;
    move tmp;
    x.currentPlayer = our;
    x.enemyCards = enemys;
    x.ourCards = ours;
    x.lastMove = tmp;
    while (x.ourCards.cardNum() > 0 && x.enemyCards.cardNum() > 0) {
        cout << "---------------------------\n";
        cout << "开始计算\n";
        cout << "我方的牌：";
        ptcards(x.ourCards);
        cout<<endl;
        cout << "对方的牌：";
        ptcards(x.enemyCards);
        cout<<endl;
        start = clock();
        returned_result result = minMaxSearch(x);
        end = clock();
        cout << "计算用时：" << (double)(end - start) / CLOCKS_PER_SEC << "秒\n";
        if (result.score == minScore) {
            cout << "对方有必胜策略\n";
            exit(0);
        }
        cout << "我方应该出牌：\n";
        ptmove(result.bestMove);
        x.ourCards.remove(result.bestMove.mainCard);
        x.ourCards.remove(result.bestMove.subCard);
        if(x.ourCards.cardNum()==0){
            cout<<"我方胜利\n";
            exit(0);
        }
        cout << "请输入对方出的牌：\n";
        tmp = getMove(x.enemyCards, result.bestMove);
        x.enemyCards.remove(tmp.mainCard);
        x.enemyCards.remove(tmp.subCard);
        x.lastMove=tmp;
    }
    return 0;
}