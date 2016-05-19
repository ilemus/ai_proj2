#include <iostream>

using namespace std;

#define INFINITY INT_MAX
#define DEBUG true

struct node {
    node *parent;
    char board[3][3];
    int f; //current distance from epoch
    bool color; //true: red, false: blue
};

struct list {
    list *parent;
    list *child;
    node *q;
};

static list *open_list;
static list *closed_list;

char solution[][] {
    {1, 0, 1},
    {0, 0, 0},
    {2, 0, 2}
}

/*  R = 2, B = 1
      0 1 2 j
    0 R - R |
    1 - - - |
    2 B - B |
    i_______|
*/
void setBoard(char board[][3]) {
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

node* getLowestNode() {
    node *lowest = open_list->q;
    list *temp;
    temp = open_list;
    while (temp->parent != 0) {
        if (temp->q->f < lowest->f) {
            lowest = q;
        }
        
        temp = temp->parent;
    }
    
    if (DEBUG) {
        cout << "Lowest Node's f: " << lowest->f << endl;
    }
    return lowest;
}

node* solve(char board[][3]) {
    node *q;
    list *temp;
    bool solved = false;
    bool turn = true;
    
    // Setup queue and initial node
    q->f = INFINITY;
    setBoard(q->board);
    q->board[0][1] = 9;
    open_list->q = q;
    open_list->parent = 0;
    open_list->child = 0;
    closed_list->q = 0;
    closed_list->parent = 0;
    closed_list->child = 0;
    
    q->parent = q;
    q->board = board;
    q->f = 0;
    q->color = true;
    
    temp->child = 0;
    temp->parent = open_list;
    temp->q = q;
    
    open_list->child = temp;
    open_list->parent = open_list;
    
    // Main loop
    while (open_list->parent != 0) {
        node *p;
        *p = getLowestNode();
        popNode(p);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (valid(p, i, j)) {
                    node *c->parent = p;
                    setValidMove(c);
                    if (isSolution(c)) {
                        q = c;
                        solved = true;
                        break;
                    } else {
                        c->f = c->parent->f + 1;
                        if (!alreadyOpen(c)) {
                            if (!alreadyClosed(c)) {
                                temp->child = 0;
                                temp->parent = open_list;
                                temp->q = c;
                                open_list = temp;
                            }
                        }
                    }
                }
            }
        }
        deleteNode(p);
        if (solved) {
            break;
        }
    }
    return q;
}

int main() {
    char board[3][3];
    node *solution;
    
    cout << "A-star search algorithmn applied to knight problem" << endl;
    
    setBoard(board);
    solution = solve(board);
}