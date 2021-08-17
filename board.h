#ifndef BOARD_H
#define BOARD_H

#define EMPTY 0

#define PAWN_WHITE 1
#define KNIGHT_WHITE 2
#define BISHOP_WHITE 3
#define ROOK_WHITE 4
#define QUEEN_WHITE 5
#define KING_WHITE 6

#define PAWN_BLACK 7
#define KNIGHT_BLACK 8
#define BISHOP_BLACK 9
#define ROOK_BLACK 10
#define QUEEN_BLACK 11
#define KING_BLACK 12

#define FEN_START "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

const char piece_letter[] = {' ', 'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};
const map<char, int> letter_piece = { {'P', 1}, {'N', 2}, {'B', 3}, {'R', 4}, {'Q', 5}, {'K', 6}, {'p', 7}, {'n', 8}, {'b', 9}, {'r', 10}, {'q', 11}, {'k', 12} };

class Board {
    private:
        int board[64] = {EMPTY};
    public:
        explicit Board();
        explicit Board(string fen);
        int get(int square);
        void move(int start, int end);
        vector<int> possible_moves(int square);
        friend std::ostream& operator<<(std::ostream& os, Board& b);
        //TODO
};

#endif //BOARD_H