#ifndef BOARD_H
#define BOARD_H

#define FEN_START "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

//  PIECE TYPES

#define OUTSIDE_BOARD   Piece{-1}
#define EMPTY           Piece{0}

#define PAWN        1   // 0001
#define KNIGHT      2   // 0010
#define BISHOP      3   // 0011
#define ROOK        4   // 0100
#define QUEEN       5   // 0101
#define KING        6   // 0110

#define WHITE       0   // 0000
#define BLACK       8   // 1000

// piece = PIECE_TYPE | COLOR

// piece & 8 -> color
// piece & 7 -> piece type

#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct Piece {

    signed char val;   // val = type | color

    char type() { return val & 7; }
    char color() { return val & 8; }

    bool operator==(const Piece other) { return val == other.val; }
    bool operator!=(const Piece other) { return val != other.val; }
};

const char piece2letter[] = {' ', 'P', 'N', 'B', 'R', 'Q', 'K', '?', '?', 'p', 'n', 'b', 'r', 'q', 'k'};
const map<char, Piece> letter2piece = { 
    {'P', Piece{PAWN    | WHITE}}, 
    {'N', Piece{KNIGHT  | WHITE}}, 
    {'B', Piece{BISHOP  | WHITE}}, 
    {'R', Piece{ROOK    | WHITE}}, 
    {'Q', Piece{QUEEN   | WHITE}}, 
    {'K', Piece{KING    | WHITE}}, 
    {'p', Piece{PAWN    | BLACK}}, 
    {'n', Piece{KNIGHT  | BLACK}}, 
    {'b', Piece{BISHOP  | BLACK}}, 
    {'r', Piece{ROOK    | BLACK}}, 
    {'q', Piece{QUEEN   | BLACK}}, 
    {'k', Piece{KING    | BLACK}}
};

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

enum Flag : char {
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

    Move(char start, char end, Flag flag) : start(start), end(end), flag(flag) {}
    Move(char start, char end) : Move(start, end, NO_FLAG) {}

    char start;
    char end;
    Flag flag;
};


class Board {
    private:
        Piece board[120] = { EMPTY };
        char en_pass_sq = -1;
        bool castle_rights[4] = {false};     // white kingside, white queenside, black kingside, black queenside
        bool turn;      // 0 -> white to play    1 -> black to play
        int halfmove;
        int fullmove;
    public:
        explicit Board(string fen = FEN_START);
        Piece get(char square);
        void make_move(Move move);
        vector<Move> possible_moves(char square);
        friend std::ostream& operator<<(std::ostream& os, Board& b);
};

#endif //BOARD_H