#include "board.h"
#include <stdlib.h>

using namespace std;

int main() {
    Board b;
    cout << b << endl;

     vector<int> moves = b.possible_moves(97, -1);
     for (int i = 0; i < moves.size(); i++) {
         cout << moves[i] << endl;
     }
}