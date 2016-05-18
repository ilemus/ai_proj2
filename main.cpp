#include <iostream>

using namespace std;

struct node {
    node *parent;
    int x, y; //current x, y coordinates
    int f; //current distance from epoch
    bool color; //true: red, false: blue
};

/*    0 1 2
    0 R - R
    1 - - -
    2 B - B
*/

void setBoard(char board[3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = 0;
        }
    }
    
}

int main() {
    char board[3][3];
    node *solution;
    
    cout << "A-star search algorithmn applied to knight problem" << endl;
    
    setBoard(board);
    solution = solve(board);
}