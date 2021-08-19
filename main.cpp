#include "board.h"
#include <stdlib.h>

using namespace std;

int main() {
    Board b;
    cout << b << endl;

    for (int sq = 21; sq < 99; sq++) {
        if (b.get(sq) > 0) {
            
            cout << piece2letter[b.get(sq)] << '\t';

            vector<int> moves = b.possible_moves(sq, -1);
            for (int i = 0; i < moves.size(); i++) {
                cout << moves[i] << ' ';
            }

            cout << endl;
        }
    }
}