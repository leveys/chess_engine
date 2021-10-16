#include "Board.hpp"
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
        board[i] = OUTSIDE_BOARD;
    }

    int pos = 21;

    for (char c : fensplit[0]) {

        if (c == '/') {        // set sentinel files out of bounds
            board[pos] = OUTSIDE_BOARD;
            board[pos + 1] = OUTSIDE_BOARD;
            pos += 2;
        } else if (isdigit(c)) {   // leave squares empty
            pos += c - '0';
        } else {
            board[pos] = letter2piece.at(c);

            if (c == 'K') {
                kings[0] = pos;
            } else if (c == 'k') {
                kings[1] = pos;
            }

            pos++;
        }
    }

    for (int i = pos; i < 120; i++) {   // set last 2 sentinel ranks out of bounds
        board[i] = OUTSIDE_BOARD;
    }

    // set active color
    turn = fensplit[1] == "b";

    // set castling rights
    if (fensplit[2] != "-") {

        for (char c : fensplit[2]) {

            Piece p = letter2piece.at(c);
            castle_rights[p.color()/4 + (p.val & 1)] = true;
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
Piece Board::get(Byte square) {
    return board[square];
}

// move piece from start square to end square
// if the move is legal, returns true
bool Board::make_move(Move m) { 
    assert(m.start >= 0 && m.start < 120 && m.end >= 0 && m.end < 120);
    assert(board[m.start] != OUTSIDE_BOARD && board[m.end] != OUTSIDE_BOARD);
    assert(halfmove <= 50);

    Piece piece = board[m.start];
    en_pass_sq = 0;
    
    // check flag
    switch (m.flag) {

        case EN_PASSANT: {
            Byte en_pass_capt = m.start > m.end ? m.end + 10 : m.end - 10;
            board[en_pass_capt] = EMPTY;
            break;
        }
        
        case KNIGHT_PROMO:
        case BISHOP_PROMO:
        case ROOK_PROMO:
        case QUEEN_PROMO: {
            piece = Piece{s_Byte(m.flag | piece.color())};
            break;
        }

        case CASTLES: {
            Byte rook_start = 21 + 70*(!piece.color()) + 7*(m.start < m.end);
            Byte rook_end = 24 + 70*(!piece.color()) + 2*(m.start < m.end);

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

    // update king position if changed
    if (piece.type() == KING) {
        kings[piece.color()/8] = m.end;
    }

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

    // update fullmove, the counter updates after black moves
    fullmove += turn;

    // update halfmove, the counter is reset after captures or pawn moves, and incremented otherwise
    if (m.captured != EMPTY || piece.type() == PAWN) {
        halfmove = 0;
    } else {
        halfmove++;
    }

    // update turn
    turn = !turn;

    // check if move was legal
    int castled = (m.flag == CASTLES) * (m.start - m.end)/2;
    return move_was_legal(piece.color(), castled);
}

// unmake last move made
void Board::unmake_move(Move m) {
    assert(m.start >= 0 && m.start < 120 && m.end >= 0 && m.end < 120);
    assert(board[m.start] != OUTSIDE_BOARD && board[m.end] != OUTSIDE_BOARD);

    Piece piece = board[m.end];

    // check flag
    switch (m.flag) {

        case EN_PASSANT: {
            Byte en_pass_capt = m.start > m.end ? m.end + 10 : m.end - 10;
            board[en_pass_capt] = Piece{s_Byte(PAWN | !piece.color() * 8)};
            break;
        }
        
        case KNIGHT_PROMO:
        case BISHOP_PROMO:
        case ROOK_PROMO:
        case QUEEN_PROMO: {
            piece = Piece{s_Byte(PAWN | piece.color())};
            break;
        }

        case CASTLES: {
            Byte rook_start = 21 + 70*(!piece.color()) + 7*(m.start < m.end);
            Byte rook_end = 24 + 70*(!piece.color()) + 2*(m.start < m.end);

            board[rook_start] = board[rook_end];
            board[rook_end] = EMPTY;
            break;
        }
    }

    // move piece
    board[m.start] = board[m.end];
    board[m.end] = m.captured;

    // update king position if changed
    if (piece.type() == KING) {
        kings[piece.color()/8] = m.start;
    }

    // update turn
    turn = !turn;

    // update fullmove, the counter updates after black moves
    fullmove -= turn;

    // TODO: update en_pass_sq, halfmove clock, castling rights (stack)
}


// check if last move made was legal
bool Board::move_was_legal(Byte color, int castled) {   // castled = 0 -> didnt castle, castled = 1 -> castled kingside, castled = -1 -> castled queenside

    Byte kingpos = kings[color/8];

    if (castled) {  // last move was castling
        return !is_attacked(kingpos, color) && !is_attacked(kingpos + castled, color) && !is_attacked(kingpos + castled*2, color);
    } else {
        return !is_attacked(kingpos, color);
    }
}

// check if square is attacked
bool Board::is_attacked(Byte square, Byte color) {

    // knight/king attacks
    for (int piece = KNIGHT; piece <= KING; piece += 4) {

        for (int offset : offsets[piece-2]) {

            Piece target = board[square + offset];
            if (target.is_piece() && target.color() != color && target.type() == piece) {
                return true;
            }
        }
    }

    // sliding attacks (bishop/rook/queen)
    for (int piece = BISHOP; piece <= ROOK; piece++) {

        for (int j = 0; j < 4; j++) {

            Byte offset = offsets[piece-2][j];
            Byte target_index = square + offset;
            Piece target = board[target_index];

            while (target == EMPTY) {
                target_index += offset;
                target = board[target_index];
            }

            if (target.is_piece() && target.color() != color && (target.type() == piece || target.type() == QUEEN)) {
                return true;
            }
        }
    }

    // pawn attacks
    int coloroffset = color / 4 - 1;  // WHITE: 0 -> -1     BLACK: 8 -> 1
    Piece infrontl = board[square + 11 * coloroffset];
    Piece infrontr = board[square + 9 * coloroffset];

    if ((infrontl.is_piece() && infrontl.color() != color && infrontl.type() == PAWN) || 
        (infrontr.is_piece() && infrontr.color() != color && infrontr.type() == PAWN)) {
        return true;
    }

    // square is not under attack
    return false;
}

// returns all possible moves of piece on given square
vector<Move> Board::possible_moves(Byte square) {
    assert(square >= 0 && square < 120 && en_pass_sq >= 0 && en_pass_sq < 120);

    Piece piece = board[square];

    vector<Move> moves;

    if (piece.type() == PAWN) {      // piece is pawn
        
        if (square > 30 && square < 89) {   // pawn is not on last (or first) rank (should be impossible because of promotion)

            int coloroffset = piece.color() / 4 - 1;  // WHITE: 0 -> -1     BLACK: 8 -> 1

            int infront = square + 10 * coloroffset;
            int infrontl = square + 11 * coloroffset;
            int infrontr = square + 9 * coloroffset;
            int infront2 = square + 20 * coloroffset;


            // pawn promotion
            if ((piece.color() == WHITE && square < 39) || (piece.color() == BLACK && square > 80)) {
                
                // square in front is empty
                if (board[infront] == EMPTY) {
                    for (int p = 2; p < 6; p++) {
                        moves.push_back(Move(square, infront, EMPTY, (Flag)p));    // pawn moves forward 1 square
                    }
                }

                // square in front left is occupied by opponent piece
                if (board[infrontl].val > 0 && board[infrontl].color() != piece.color()) {
                    for (int p = 2; p < 6; p++) {
                        moves.push_back(Move(square, infrontl, board[infrontl], (Flag)p));   // pawn captures in front left
                    }
                }

                // square in front right is occupied by opponent piece
                if (board[infrontr].val > 0 && board[infrontr].color() != piece.color()) {
                    for (int p = 2; p < 6; p++) {
                        moves.push_back(Move(square, infrontr, board[infrontr], (Flag)p));    // pawn captures in front right
                    }
                }

            } else {    // no promotion

                // square in front is empty
                if (board[infront] == EMPTY) {
                    moves.push_back(Move(square, infront, EMPTY)); // pawn moves forward 1 square
                }

                // square in front left is occupied by opponent piece OR en passant is possible here
                if (infrontl == en_pass_sq || (board[infrontl].val > 0 && board[infrontl].color() != piece.color())) {
                    moves.push_back(Move(square, infrontl, board[infrontl], infrontl == en_pass_sq ? EN_PASSANT : NO_FLAG)); // pawn captures in front left
                }

                // square in front right is occupied by opponent piece OR en passant is possible here
                if (infrontr == en_pass_sq || (board[infrontr].val > 0 && board[infrontr].color() != piece.color())) {
                    moves.push_back(Move(square, infrontr, board[infrontr], infrontr == en_pass_sq ? EN_PASSANT : NO_FLAG)); // pawn captures in front right
                }

                // pawn is on starting rank and 2 squares in front are empty
                if (((piece.color() == WHITE && square > 80) || (piece.color() == BLACK && square < 39)) 
                    && board[infront] == EMPTY && board[infront2] == EMPTY) {

                    moves.push_back(Move(square, infront2, EMPTY, TWO_FORWARD)); // pawn moves forward 2 squares
                }
            }
        }

    } else if (piece.val > 0) {     // piece is not a pawn 
        
        int piecetype = piece.type() - 2;
            
        for (int i = 0; i < offsetnum[piecetype]; i++) {

            Byte offset = offsets[piecetype][i];
            
            Byte target_index = square + offset;
            Piece target = board[target_index];

            while (sliding[piecetype] && target == EMPTY) {    // piece is sliding type (bishop, rook or queen) AND target square is empty
                moves.push_back(Move(square, target_index, EMPTY));

                target_index += offset;
                target = board[target_index];
            }

            if (target != OUTSIDE_BOARD && (target == EMPTY || (target.color() != piece.color()))) {    // target square is in bounds AND is empty OR occupied by opponent piece
                moves.push_back(Move(square, target_index, target));
            }
        }

        
        // TODO: check if squares are not attacked
        // castles

        if (piece.type() == KING && castle_rights[piece.color()/4]
            && board[square + 1] == EMPTY && board[square + 2] == EMPTY) {                                  // kingside castles
            
            moves.push_back(Move(square, square + 2, EMPTY, CASTLES));
        
        } else if (piece.type() == KING && castle_rights[piece.color()/4 + 1] 
            && board[square - 1] == EMPTY && board[square - 2] == EMPTY && board[square - 3] == EMPTY) {    // queenside castles
            
            moves.push_back(Move(square, square - 2, EMPTY, CASTLES));
        }
    }

    return moves;
}

std::ostream& operator<<(std::ostream& os, Board& b) {

    for (int rank = 2; rank < 10; rank++) {
        os << 10-rank << "| ";
        for (int file = 1; file < 9; file++) {
            os << piece2letter[b.board[10*rank + file].val] << " ";
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

    if (b.en_pass_sq) {
        os << '\t' << int2algebraic(b.en_pass_sq);
        nl = true;
    }

    if (nl) {
        os << endl;
    }

    os << "move " << b.fullmove << ", " << color_str[b.turn] << " to play" << endl;

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