#include "board.hpp"
#include <stdlib.h>

using namespace std;

int main() {

    Board b;
    cout << b << endl;

    for (char sq = 21; sq < 99; sq++) {
        if (b.get(sq).val > 0) {
            
            cout << piece2letter[b.get(sq).val] << '\t';

            vector<Move> moves = b.possible_moves(sq);
            for (int i = 0; i < moves.size(); i++) {
                cout << (int)moves[i].end << ' ';
            }

            cout << endl;
        }
    }
}