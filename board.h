#ifndef BOARD_H
#define BOARD_H

//  PIECE TYPES
//TODO: change to enum/struct   maybe?

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

// piece & 8 -> color
// piece & 7 -> piece type

#define FEN_START "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

const char piece2letter[] = {' ', 'P', 'N', 'B', 'R', 'Q', 'K', '?', '?', 'p', 'n', 'b', 'r', 'q', 'k'};
const map<char, int> letter2piece = { {'P', 1}, {'N', 2}, {'B', 3}, {'R', 4}, {'Q', 5}, {'K', 6}, {'p', 9}, {'n', 10}, {'b', 11}, {'r', 12}, {'q', 13}, {'k', 14} };
const string color[] = {"white", "black"};


const int offsets[5][8] = {             // board index offsets
    {-21, -19, -12, -8, 8, 12, 19, 21}, // knight 
    {-11, -9, 9, 11, 0, 0, 0, 0},       // bishop
    {-10, -1, 1, 10, 0, 0, 0, 0},       // rook
    {-11, -10, -9, -1, 1,  9, 10, 11},  // queen
    {-11, -10, -9, -1, 1,  9, 10, 11}   // king
};

const int offsetnum[] = {8, 4, 4, 8, 8};    // number of offsets in offset table

const bool sliding[] = {false, true, true, true, false};


/*  the chess board is represented by a 10x12 array of integers,
    this is a 8x8 board surrounded by 2 sentinel ranks and 1 file on each side.
    see https://www.chessprogramming.org/10x12_Board for more info.

    0   1   2   3   4   5   6   7   8   9
    10  11	12	13	14	15	16	17	18  19
        ______________________________           BLACK
    20 |21	22	23	24	25	26	27	28| 29
    30 |31	32	33	34	35	36	37	38| 39
    40 |41	42	43	44	45	46	47	48| 49
    50 |51	52	53	54	55	56	57	58| 59
    60 |61	62	63	64	65	66	67	68| 69
    70 |71	72	73	74	75	76	77	78| 79
    80 |81	82	83	84	85	86	87	88| 89
    90 |91	92	93	94	95	96	97	98| 99
        ⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻           WHITE
    100	101	102	103	104	105	106	107	108	109
    110	111	112	113	114	115	116	117	118	119	

*/

const int board64[] = {
    21, 22, 23, 24, 25, 26, 27, 28,
    31, 32, 33, 34, 35, 36, 37, 38,
    41, 42, 43, 44, 45, 46, 47, 48,
    51, 52, 53, 54, 55, 56, 57, 58,
    61, 62, 63, 64, 65, 66, 67, 68,
    71, 72, 73, 74, 75, 76, 77, 78,
    81, 82, 83, 84, 85, 86, 87, 88,
    91, 92, 93, 94, 95, 96, 97, 98
};

const int board120[] = {
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
     -1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
     -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
     -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
     -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
     -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
     -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
     -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

int algebraic2int(char file, char rank);

string int2algebraic(int square);


enum Flag {
    NO_FLAG =       0,
    EN_PASSANT =    1,
    KNIGHT_PROMO =  2,
    BISHOP_PROMO =  3,
    ROOK_PROMO =    4,
    QUEEN_PROMO =   5,
    CASTLES =       6,
    TWO_FORWARD =   7
};

struct Move {

    Move(int start, int end, Flag flag) : start(start), end(end), flag(flag) {}
    Move(int start, int end) : Move(start, end, NO_FLAG) {}

    int start;
    int end;
    Flag flag;
};


class Board {
    private:
        int board[120] = {EMPTY};
        int en_pass_sq = -1;
        bool castle_rights[4] = {false};     // white kingside, white queenside, black kingside, black queenside
        bool turn;      // 0 -> white to play    1 -> black to play
        int halfmove;
        int fullmove;
    public:
        explicit Board(string fen = FEN_START);
        int get(int square);
        void make_move(Move move);
        vector<Move> possible_moves(int square);
        friend std::ostream& operator<<(std::ostream& os, Board& b);
};

#endif //BOARD_H