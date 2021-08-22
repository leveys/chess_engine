#include "Board.h"
#include <assert.h>

using namespace std;

// set up board with given FEN string
Board::Board(string fen) {

    // split FEN string in 6 fields

    string fensplit[6];

    int start = 0;
    int end = fen.find(' ');

    for (int i = 0; i < 6; i++) {

        fensplit[i] = fen.substr(start, end-start);

        start = end + 1;
        end = fen.find(' ', start);
    }

    // set up board

    for (int i = 0; i < 21; i++) {      // set first 2 sentinel ranks out of bounds
        board[i] = OUT_OF_BOUNDS;
    }

    int pos = 21;

    for (char c : fensplit[0]) {

        if (c == '/') {        // set sentinel files out of bounds
            board[pos] = OUT_OF_BOUNDS;
            board[pos + 1] = OUT_OF_BOUNDS;
            pos += 2;
        } else if (isdigit(c)) {   // leave squares empty
            pos += c - '0';
        } else {
            board[pos] = letter2piece.at(c);
            pos++;
        }
    }

    for (int i = pos; i < 120; i++) {   // set last 2 sentinel ranks out of bounds
        board[i] = OUT_OF_BOUNDS;
    }

    // set active color
    turn = fensplit[1] == "b";

    // set castling rights
    if (fensplit[2] != "-") {

        for (char c : fensplit[2]) {

            int p = letter2piece.at(c);
            castle_rights[(p & 8)/4 + (p & 1)] = true;
        }
    }

    // set en passant target square
    if (fensplit[3] != "-") {
        en_pass_sq = algebraic2int(fensplit[3][0], fensplit[3][1]);
    }

    // set halfmove clock
    halfmove = stoi(fensplit[4]);

    //set fullmove number
    fullmove = stoi(fensplit[5]);
}

// get piece on given square
int Board::get(int square) {
    return board[square];
}

// move piece from start square to end square
void Board::make_move(Move m) { 
    assert(m.start >= 0 && m.start < 120 && m.end >= 0 && m.end < 120);
    assert(board[m.start] != OUT_OF_BOUNDS && board[m.end] != OUT_OF_BOUNDS);

    int piece = board[m.start];
    en_pass_sq = -1;
    
    switch (m.flag) {

        case EN_PASSANT: {
            int en_pass_capt = m.start > m.end ? m.end + 10 : m.end - 10;
            board[en_pass_capt] = EMPTY;
            break;
        }
        
        case KNIGHT_PROMO:
        case BISHOP_PROMO:
        case ROOK_PROMO:
        case QUEEN_PROMO: {
            piece = m.flag + (piece & 8);
            break;
        }

        case CASTLES: {
            int rook_start = 21 + 70*(!(piece & 8)) + 7*(m.start < m.end);
            int rook_end = 24 + 70*(!(piece & 8)) + 2*(m.start < m.end);

            board[rook_end] = board[rook_start];
            board[rook_start] = EMPTY;
            break;
        }

        case TWO_FORWARD: {
            en_pass_sq = (m.start + m.end)/2;
            break;
        }
    }

    // move piece
    board[m.end] = piece;
    board[m.start] = EMPTY;

    // remove castling rights
    if (m.start == 25) {            // black king starting square
        castle_rights[3] = false;
        castle_rights[2] = false;

    } else if (m.start == 95) {     // white king starting square
        castle_rights[1] = false;
        castle_rights[0] = false;

    } else if (m.start == 21 || m.end == 21) {  // black rook queenside starting square
        castle_rights[3] = false;

    } else if (m.start == 28 || m.end == 28) {  // black rook kingside starting square
        castle_rights[2] = false;

    } else if (m.start == 91 || m.end == 91) {  // white rook queenside starting square
        castle_rights[1] = false;
        
    } else if (m.start == 98 || m.end == 98) {  // white rook kingside starting square
        castle_rights[0] = false;
        
    }
}

// returns all possible moves of piece on given square
vector<Move> Board::possible_moves(int square) {
    assert(square >= 0 && square < 120 && en_pass_sq >= -1 && en_pass_sq < 120);

    int piece = board[square];

    vector<Move> moves;

    if (piece & 7 == PAWN_WHITE) {      // piece is pawn
        
        if (square > 30 && square < 89) {   // pawn is not on last (or first) rank (should be impossible because of promotion)

            int coloroffset = (piece - 5) / 4;  // PAWN_WHITE: 1 -> -1     PAWN_BLACK: 9 -> 1

            int infront = square + 10 * coloroffset;
            int infrontl = square + 11 * coloroffset;
            int infrontr = square + 9 * coloroffset;
            int infront2 = square + 20 * coloroffset;

            // pawn promotion
            if ((piece == PAWN_WHITE && square < 39) || (piece == PAWN_BLACK && square > 80)) {
                
                // square in front is empty
                if (board[infront] == 0) {
                    for (int p = 2; p < 6; p++) {
                        moves.push_back(Move(square, infront, (Flag)p));    // pawn moves forward 1 square
                    }
                }

                // square in front left is occupied by opponent piece
                if (board[infrontl] > 0 && (board[infrontl] ^ piece) & 8) {
                    for (int p = 2; p < 6; p++) {
                        moves.push_back(Move(square, infrontl, (Flag)p));   // pawn captures in front left
                    }
                }

                // square in front right is occupied by opponent piece
                if (board[infrontr] > 0 && (board[infrontr] ^ piece) & 8) {
                    for (int p = 2; p < 6; p++) {
                        moves.push_back(Move(square, infrontr, (Flag)p));    // pawn captures in front right
                    }
                }

            } else {    // no promotion

                // square in front is empty
                if (board[infront] == 0) {
                    moves.push_back(Move(square, infront)); // pawn moves forward 1 square
                }

                // square in front left is occupied by opponent piece OR en passant is possible here
                if (infrontl == en_pass_sq || (board[infrontl] > 0 && (board[infrontl] ^ piece) & 8)) {
                    moves.push_back(Move(square, infrontl, infrontl == en_pass_sq ? EN_PASSANT : NO_FLAG)); // pawn captures in front left
                }

                // square in front right is occupied by opponent piece OR en passant is possible here
                if (infrontr == en_pass_sq || (board[infrontr] > 0 && (board[infrontr] ^ piece) & 8)) {
                    moves.push_back(Move(square, infrontr, infrontr == en_pass_sq ? EN_PASSANT : NO_FLAG)); // pawn captures in front right
                }

                // pawn is on starting rank and 2 squares in front are empty
                if (((piece == PAWN_WHITE && square > 80) || (piece == PAWN_BLACK && square < 39)) && board[infront] == 0 && board[infront2] == 0) {
                    moves.push_back(Move(square, infront2)); // pawn moves forward 2 squares
                }
            }
        }

    } else if (piece > 0) {     // piece is not a pawn 
        
        int piecetype = (piece & 7) - 2;
            
        for (int i = 0; i < offsetnum[piecetype]; i++) {

            int offset = offsets[piecetype][i];
            
            int target_index = square + offset;
            int target = board[target_index];

            while (sliding[piecetype] && target == 0) {    // piece is sliding type (bishop, rook or queen) AND target square is empty
                moves.push_back(Move(square, target_index));

                target_index += offset;
                target = board[target_index];
            }

            if (target != OUT_OF_BOUNDS && (target == 0 || (target ^ piece) & 8)) {    // target square is in bounds AND is empty OR occupied by opponent piece
                moves.push_back(Move(square, target_index));
            }
        }

        
        // TODO: check if squares are not attacked
        // castles

        if (piece & 7 == KING_WHITE && castle_rights[(piece & 8)/4]
            && board[square + 1] == 0 && board[square + 2] == 0) {                                  // kingside castles
            
            moves.push_back(Move(square, square + 2, CASTLES));
        
        } else if (piece & 7 == KING_WHITE && castle_rights[(piece & 8)/4 + 1] 
                   && board[square - 1] == 0 && board[square - 2] == 0 && board[square - 3] == 0) { // queenside castles
            
            moves.push_back(Move(square, square - 2, CASTLES));
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
    os << "   a b c d e f g h " << endl << endl;

    bool nl = false;

    for (int i=0; i<4; i++) {
        if (b.castle_rights[i]) {
            os << piece2letter[6 + (i/2)*8 - i%2];
            nl = true;
        }
    }

    if (b.en_pass_sq != -1) {
        os << '\t' << int2algebraic(b.en_pass_sq);
        nl = true;
    }

    if (nl) {
        os << endl;
    }

    os << "move " << b.fullmove << ", " << color[b.turn] << " to play" << endl;

	return os;
}


int algebraic2int(char file, char rank) {

    int r = 7 - (rank - '1');
    int f = file - 'a';

    return board64[r*8 + f];
}

string int2algebraic(int square) {

    int s = board120[square];

    char rank = 8 - s/8 + '0';
    char file = s%8 + 'a';

    return string({file, rank});
}