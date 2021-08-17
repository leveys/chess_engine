#include "Board.h"

using namespace std;

// set up board with starting position
// the board is seen from whites perspective
Board::Board() : Board(FEN_START) {}

Board::Board(string fen) {

    int pos = 0;

    for (int i = 0; i < fen.length(); i++) {

        if (isdigit(fen[i])) {
            pos += fen[i] - '0';
        } else if (fen[i] != '/') {
            board[pos] = letter_piece.at(fen[i]);
            pos++;
        }
    }
}

int Board::get(int square) {
    return board[square];
}

void Board::move(int start, int end) {
    board[end] = board[start];
    board[start] = EMPTY;
}

vector<int> Board::possible_moves(int square) {
    int piece = board[square];

    switch (piece) {
        case ROOK_WHITE || ROOK_BLACK:
            //TODO
            break;
    }
}

std::ostream& operator<<(std::ostream& os, Board& b) {

    for (int rank = 0; rank < 8; rank++) {
        os << 8-rank << "| ";
        for (int file = 0; file < 8; file++) {
            os << piece_letter[b.board[8*rank + file]] << " ";
        }
        os << endl;
    }

    os << " +-----------------" << endl;
    os << "   a b c d e f g h " << endl;

	return os;
}