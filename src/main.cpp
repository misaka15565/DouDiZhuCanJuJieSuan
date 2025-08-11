#include <format>
#include <print>
#include <iostream>
#include <chrono>
#include "gameCore.hpp"
using namespace std;
using moveGen::MoveType;

cardMove getCardMove(cards hand, cardMove lastMove) {
    // 获取可能的出牌
    auto allpossibleMoves = moveGen::genAllMoves(hand, lastMove);
    if (allpossibleMoves.empty()) {
        throw std::runtime_error("没有可能的出牌，should not reach here");
    }

    while (true) {
        print("手牌为{}，上次出牌为 {}，请输入出牌，如果是过请直接回车：\n", hand, lastMove);
        string moveInput;
        // 清除cin缓冲区
        cin.clear();
        getline(std::cin, moveInput);
        for (auto c : moveInput) {
            if (c == 'e') {
                throw std::runtime_error("用户选择退出计算");
            }
        }
        cards cardinput(moveInput);

        vector<cardMove> matchMoves;
        for (const auto &move : allpossibleMoves) {
            if (move.getAllCards() == cardinput) {
                matchMoves.push_back(move);
            }
        }
        if (matchMoves.size() == 0) {
            print("没有匹配的出牌，请重新输入\n");
            continue;
        } else if (matchMoves.size() > 1) {
            print("匹配到多个出牌，请选择：\n");
            for (size_t i = 0; i < matchMoves.size(); ++i) {
                print("{}: {}\n", i + 1, matchMoves[i]);
            }
            int choice;
            cin >> choice;
            cin.ignore(); // 清除换行符
            if (choice < 1 || choice > matchMoves.size()) {
                print("无效的选择，请重新输入\n");
                continue;
            }
            return matchMoves[choice - 1];
        } else {
            println("匹配到出牌：{}", matchMoves[0]);
            return matchMoves[0];
        }
    }
}

int main(int argc, char **argv, char **envp) {
    if(argc>1 && argv[1][0]=='b'){
        //benchmark
        auto start = chrono::high_resolution_clock::now();
        for(int i=0;i<50;i++){
            actionCache.clear(); // 清除缓存
            cards ourcard{"3335688jqk22"};
            cards enemycard{"44556700kaaa"};
            gameCore game{ourcard, enemycard};
            calculateBestAction(game.getCurrentStatus());
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = end - start;
        println("基准测试耗时：{} ms", duration.count());
        exit(0);
    }


    try {
        println("欢迎使用斗地主残局计算器！");
        println("请使用0代表10，x代表小王，d代表大王输入手牌：\n-------------------------------------------");
        println("请输入我方手牌：");
        string ourHandInput;
        getline(std::cin, ourHandInput);
        auto ourcards = cards(ourHandInput);

        println("请输入敌方手牌（如果是对方先出请输入其出牌前的所有牌）：");
        string enemyHandInput;
        getline(std::cin, enemyHandInput);
        auto enemycards = cards(enemyHandInput);

        println("如果是敌方先出，请输入敌方出牌（如果是我方先出请直接回车）：");
        auto lastMoveInput = getCardMove(enemycards, MoveType::INVALID);
        enemycards.remove(lastMoveInput.getAllCards()); // 移除敌方出牌后的手牌

        gameCore game{ourcards, enemycards, lastMoveInput};
        while (!game.isGameOver()) {
            // 为我方计算最佳出牌
            // 记录开始时间
            auto start = chrono::high_resolution_clock::now();
            bestAction action = calculateBestAction(game.getCurrentStatus());
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> duration = end - start;
            if (duration.count() > 1) {
                println("计算最佳出牌耗时：{} ms", duration.count());
                println("计算局面数：{}", actionCache.size());
            }

            if (action.score == min_score) {
                println("对方有必胜策略");
                return 0;
            }
            println("***我方应当出牌：{}***", action.move);
            game.playCard(action.move);
            println("我方剩余的牌：{}", game.getOurHand());
            if (game.isGameOver()) {
                break; // 如果我方出完牌，游戏结束
            }

            println("请输入对家出牌：");
            cardMove enemyMove = getCardMove(game.getEnemyHand(), action.move);
            game.playCard(enemyMove);
        }
        if (game.getOurHand().cardNum() == 0) {
            println("我方胜利！");
        } else {
            println("敌方胜利！");
        }

    } catch (const std::exception &e) {
        println("发生异常: {}", e.what());
    } catch (...) {
        println("发生未知异常");
    }
    return 0;
}