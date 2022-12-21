#include <cstdint>
#define TYPE_0_PASS (0)
#define TYPE_1_SINGLE (1)
#define TYPE_2_PAIR (2)
#define TYPE_3_TRIPLE (3)
#define TYPE_4_BOMB (4)
#define TYPE_5_KING_BOMB (5)
#define TYPE_6_3_1 (6)
#define TYPE_7_3_2 (7)
#define TYPE_8_SERIAL_SINGLE (8)
#define TYPE_9_SERIAL_PAIR (9)
#define TYPE_10_SERIAL_TRIPLE (10)
#define TYPE_11_SERIAL_3_1 (11)
#define TYPE_12_SERIAL_3_2 (12)
#define TYPE_13_4_2 (13)
#define TYPE_14_4_2_2 (14)

const int minSerialSingle = 5; // 最小顺子长度
const int minSerialPair = 3;   // 最小对子长度
const int minSerialTriple = 2; // 最小飞机长度
typedef int8_t int8;
typedef int8_t cardVal;
typedef int8_t moveType;
typedef int8_t statusCode;
#define bigger 1
#define notbigger 0
#define illegal 2
