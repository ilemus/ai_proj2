#include <iostream>

using namespace std;

struct node {
    node *parent;
    int x, y; //current x, y coordinates
    int f; //current distance from epoch
    bool color; //true: red, false: blue
};

/*  R = 2, B = 1
      0 1 2 j
    0 R - R |
    1 - - - |
    2 B - B |
    i_______|
*/
void setBoard(char board[3]) {
    // Clear Board
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = 0;
        }
    }
    
    // Initialize pieces onto board
    board[0][0] = 2;
    board[0][2] = 2;
    board[2][0] = 1;
    board[2][2] = 1;
}

node* solve(char board[3]) {
    node *q;
    q->parent = q;
    q->x = 0;
    q->y = 0;
    q->f = 0;
    q->color = true;
    
}

int main() {
    char board[3][3];
    node *solution;
    
    cout << "A-star search algorithmn applied to knight problem" << endl;
    
    setBoard(board);
    solution = solve(board);
}