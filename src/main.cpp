#include <format>
#include <print>

#include "cardMove.hpp"
using namespace std;

int main(int argc, char **argv, char **envp) {
    cards a("332255667789kq00jjj");

    println("{}", a);
    return 0;
}