#include "board.hpp"
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <algorithm>


using namespace std;

string fens[64];
const string fen_empty = "8/8/8/8/8/8/8/8 w - - 0 1";

char corners[] = {21, 28, 91, 98};
char borders[] = {
    21, 22, 23, 24, 25, 26, 27, 28, 
    31, 38, 41, 48, 51, 58, 61, 68, 71, 78, 81, 88,
    91, 92, 93, 94, 95, 96, 97, 98
};


void create_fenstrings(char piece) {

    for (int i = 0; i < 8; i++) {

        char before = '0' + i;
        char after = '7' - i;

        string rank = string({before, piece, after});

        for (int j = 0; j < 8; j++) {
            
            fens[j*8 + i] = fen_empty.substr(0, j*2) + rank + fen_empty.substr(j*2 + 1, 26);
        }
    }
}


// test if number of moves a rook can make on an empty board is correct
bool rookmoves() {

    create_fenstrings('R');

    for (int i= 0; i < 64; i++) {
        Board b(fens[i]);

        int moves = b.possible_moves(board64[i]).size();
        
        if (moves != 14) {    // a rook can always move 14 squares on an empty board
            return false;
        }
    }

    return true;
}

bool kingmoves() { 
    
    create_fenstrings('k');

    for (int i= 0; i < 64; i++) {
        Board b(fens[i]);

        char square = board64[i];

        int moves = b.possible_moves(square).size();

        if (find(begin(corners), end(corners), square) != end(corners)) {   // king in corner has 3 moves

            if (moves != 3) {
                return false;
            }

        } else if (find(begin(borders), end(borders), square) != end(borders)) {    // king on border has 5 moves 
            
            if (moves != 5) {
                return false;
            }

        } else if (moves != 8) {    // a king in middle of the board has 8 moves

            return false;
        }
    }

    return true;
}


int main() {

    //cout << "rook moves correct: " << (rookmoves() ? "yes" : "no") << endl;
    //cout << "king moves correct: " << (kingmoves() ? "yes" : "no") << endl;

    cout << sizeof(int) << endl;
    cout << sizeof(bool) << endl;
    cout << sizeof(Board) << endl;
}