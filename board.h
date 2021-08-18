#ifndef BOARD_H
#define BOARD_H

#define OUT_OF_BOUNDS   -1
#define EMPTY           0

#define PAWN_WHITE      1   // 0 001
#define KNIGHT_WHITE    2   // 0 010
#define BISHOP_WHITE    3   // 0 011
#define ROOK_WHITE      4   // 0 100
#define QUEEN_WHITE     5   // 0 101
#define KING_WHITE      6   // 0 110

#define PAWN_BLACK      9   // 1 001
#define KNIGHT_BLACK    10  // 1 010
#define BISHOP_BLACK    11  // 1 011
#define ROOK_BLACK      12  // 1 100
#define QUEEN_BLACK     13  // 1 101
#define KING_BLACK      14  // 1 110

#define FEN_START "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

const char piece2letter[] = {' ', 'P', 'N', 'B', 'R', 'Q', 'K', '/', '/', 'p', 'n', 'b', 'r', 'q', 'k'};
const map<char, int> letter2piece = { {'P', 1}, {'N', 2}, {'B', 3}, {'R', 4}, {'Q', 5}, {'K', 6}, {'p', 9}, {'n', 10}, {'b', 11}, {'r', 12}, {'q', 13}, {'k', 14} };

const int knightoffsets[] = {-21, -19, -12, -8, 8, 12, 19, 21};


/*  the chess board is represented by a 10x12 array of integers,
    this is a 8x8 board surrounded by 2 sentinel ranks and 1 file on each side.
    see https://www.chessprogramming.org/10x12_Board for more info.

    110	111	112	113	114	115	116	117	118	119	
    100	101	102	103	104	105	106	107	108	109	
        ______________________________
    90｜91	92	93	94	95	96	97	98｜ 99	
    80｜81	82	83	84	85	86	87	88｜ 89	
    70｜71	72	73	74	75	76	77	78｜ 79	
    60｜61	62	63	64	65	66	67	68｜ 69	
    50｜51	52	53	54	55	56	57	58｜ 59	
    40｜41	42	43	44	45	46	47	48｜ 49	
    30｜31	32	33	34	35	36	37	38｜ 39	
    20｜21	22	23	24	25	26	27	28｜ 29	
    10｜11	12	13	14	15	16	17	18｜ 19	
        ⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻
    0   1   2   3   4   5   6   7   8    9

*/




class Board {
    private:
        int board[120] = {EMPTY};
    public:
        explicit Board();
        explicit Board(string fen);
        int get(int square);
        void move(int start, int end);
        vector<int> possible_moves(int square, int en_pass_sq);
        friend std::ostream& operator<<(std::ostream& os, Board& b);
};

#endif //BOARD_H