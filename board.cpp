#include "Board.h"
#include <assert.h>

using namespace std;

// set up board with starting position
Board::Board() : Board(FEN_START) {}

// set up board with given FEN string
Board::Board(string fen) {

    for (int i = 0; i < 21; i++) {      // set first 2 sentinel ranks out of bounds
        board[i] = OUT_OF_BOUNDS;
    }

    int pos = 21;

    for (int i = 0; i < fen.length(); i++) {

        if (fen[i] == '/') {        // set sentinel files out of bounds
            board[pos] = OUT_OF_BOUNDS;
            board[pos + 1] = OUT_OF_BOUNDS;
            pos += 2;
        } else if (isdigit(fen[i])) {   // leave squares empty
            pos += fen[i] - '0';
        } else {
            board[pos] = letter2piece.at(fen[i]);
            pos++;
        }
    }

    for (int i = pos; i < 120; i++) {   // set last 2 sentinel ranks out of bounds
        board[i] = OUT_OF_BOUNDS;
    }
}

// get piece on given square
int Board::get(int square) {
    return board[square];
}

// move piece from start square to end square
void Board::move(int start, int end) { 
    assert(start >= 0 && start < 120 && end >= 0 && end < 120);
    assert(board[start] != OUT_OF_BOUNDS && board[end] != OUT_OF_BOUNDS);

    board[end] = board[start];
    board[start] = EMPTY;
}

// returns all possible moves of piece on given square
vector<int> Board::possible_moves(int square, int en_pass_sq) {
    assert(square >= 0 && square < 120 && en_pass_sq >= -1 && en_pass_sq < 120);

    int piece = board[square];

    vector<int> moves;

    if (piece == PAWN_WHITE || piece == PAWN_BLACK) {
        
        if (square > 30 && square < 89) {   // pawn is not on last (or first) rank (should be impossible because of promotion)

            int coloroffset = (piece - 5) / 4;  // PAWN_WHITE -> -1     PAWN_BLACK -> 1

            int infront = square + 10 * coloroffset;
            int infront2 = square + 20 * coloroffset;
            int infrontl = square + 11 * coloroffset;
            int infrontr = square + 9 * coloroffset;

            // square in front is empty
            if (board[infront] == 0) {
                moves.push_back(infront); // pawn moves forward 1 square
            }

            // square in front left is occupied by opponent piece OR en passant is possible here
            if (infrontl == en_pass_sq || (board[infrontl] > 0 && (board[infrontl] ^ piece) & 8)) {
                moves.push_back(infrontl); // pawn captures in front left
            }

            // square in front right is occupied by opponent piece OR en passant is possible here
            if (infrontr == en_pass_sq || (board[infrontr] > 0 && (board[infrontr] ^ piece) & 8)) {
                moves.push_back(infrontr); // pawn captures in front right
            }

            // pawn is on starting rank and 2 squares in front are empty
            if (((piece == PAWN_WHITE && square > 80) || (piece == PAWN_BLACK && square < 39)) && board[infront] == 0 && board[infront2] == 0) {
                moves.push_back(infront2); // pawn moves forward 2 squares
            }
        }

    } else if (piece > 0) {     // piece is not a pawn 
        
        int piecetype = (piece & 7) - 2;
            
        for (int i = 0; i < offsetnum[piecetype]; i++) {

            int offset = offsets[piecetype][i];
            
            int target_index = square + offset;
            int target = board[target_index];

            while (sliding[piecetype] && target == 0) {    // piece is sliding type (bishop, rook or queen) AND target square is empty
                moves.push_back(target_index);

                target_index += offset;
                target = board[target_index];
            }

            if (target != OUT_OF_BOUNDS && (target == 0 || (target ^ piece) & 8)) {    // target square is in bounds AND is empty OR occupied by opponent piece
                moves.push_back(target_index);
            }
        }
    }

    return moves;
}

std::ostream& operator<<(std::ostream& os, Board& b) {

    for (int rank = 2; rank < 10; rank++) {
        os << 10-rank << "| ";
        for (int file = 1; file < 9; file++) {
            os << piece2letter[b.board[10*rank + file]] << " ";
        }
        os << endl;
    }

    os << " +-----------------" << endl;
    os << "   a b c d e f g h " << endl;

	return os;
}