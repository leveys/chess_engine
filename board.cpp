#include "Board.h"
#include <assert.h>

using namespace std;

// set up board with starting position
// the board is seen from whites perspective
Board::Board() : Board(FEN_START) {}

// set up board with given FEN string
Board::Board(string fen) {

    for (int i = 0; i < 21; i++) {
        board[i] = -1;
    }

    int pos = 21;

    for (int i = 0; i < fen.length(); i++) {

        if (fen[i] == '/') {
            board[pos] = -1;
            board[pos + 1] = -1;
            pos += 2;
        } else if (isdigit(fen[i])) {
            pos += fen[i] - '0';
        } else {
            board[pos] = letter2piece.at(fen[i]);
            pos++;
        }
    }

    for (int i = pos; i < 120; i++) {
        board[i] = -1;
    }
}

int Board::get(int square) {
    return board[square];
}

// moves piece from start square to end square
// both squares must be correct!
void Board::move(int start, int end) { 
    assert(start >= 0 && start < 120 && end >= 0 && end < 120);
    board[end] = board[start];
    board[start] = EMPTY;
}

// returns all possible moves of piece on given square
vector<int> Board::possible_moves(int square, int en_pass_sq) {
    assert(square >= 0 && square < 120 && en_pass_sq >= -1 && en_pass_sq < 120);

    int piece = board[square];

    vector<int> moves;

    switch (piece) {

        case PAWN_WHITE:
            
            if (square > 30) {   // pawn is not on last rank (should be impossible because of promotion)

                int infront = board[square - 10];
                int infront2 = board[square - 20];
                int infrontl = board[square - 11];
                int infrontr = board[square - 9];

                if (infront == 0) {   // square in front is empty
                    moves.push_back(square - 10); // pawn moves forward 1 square
                }

                if ((infrontl > 0 && infrontl & 8) || square - 11 == en_pass_sq) {    // square in front left is occupied by black piece OR en passant is possible here
                    moves.push_back(square - 11); // pawn captures in front left
                }

                if ((infrontr > 0 && infrontr & 8) || square - 9 == en_pass_sq) {    // square in front right is occupied by black piece OR en passant is possible here
                    moves.push_back(square - 9); // pawn captures in front right
                }

                if (square > 80 && infront == 0 && infront2 == 0) { // pawn is on starting rank and 2 squares in front are empty
                    moves.push_back(square - 20); // pawn moves forward 2 squares
                }
            }
            
            break;

        case PAWN_BLACK:

            if (square < 89) {   // pawn is not on last rank (should be impossible because of promotion)

                int infront = board[square + 10];
                int infront2 = board[square + 20];
                int infrontl = board[square + 11];
                int infrontr = board[square + 9];

                if (infront == 0) {   // square in front is empty
                    moves.push_back(square + 10); // pawn moves forward 1 square
                }

                if ((infrontl > 0 && !(infrontl & 8)) || square + 11 == en_pass_sq) {    // square in front left is occupied by white piece OR en passant is possible here
                    moves.push_back(square + 11); // pawn captures in front left
                }

                if ((infrontr > 0 && !(infrontr & 8)) || square + 9 == en_pass_sq) {    // square in front right is occupied by white piece OR en passant is possible here
                    moves.push_back(square + 9); // pawn captures in front right
                }

                if (square < 39 && infront == 0 && infront2 == 0) { // pawn is on starting rank and 2 squares in front are empty
                    moves.push_back(square + 20); // pawn moves forward 2 squares
                }
            }
            
            break;

        case KNIGHT_WHITE:
        case KNIGHT_BLACK:
        
            for (int offset : knightoffsets) {
                
                int target_index = square + offset;
                int target = board[target_index];

                if (target == 0 || (target != -1 && (target ^ piece) & 8 )) {       // target square is in bounds and empty OR target square is occupied by opponent piece
                    moves.push_back(target_index);
                }
            }

            break;
        
        case BISHOP_WHITE:
        case BISHOP_BLACK:
            //TODO
            break;

        case ROOK_WHITE:
        case ROOK_BLACK:
            //TODO
            break;
        
        case QUEEN_WHITE:
        case QUEEN_BLACK:
            //TODO
            break;
        
        case KING_WHITE:
        case KING_BLACK:
            //TODO
            break;
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