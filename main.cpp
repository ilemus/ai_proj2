#include <iostream>

using namespace std;

#define INFINITY INT_MAX
#define DEBUG true

struct node {
    node *parent;
    char board[3][3];
    int f; //current distance from epoch
    bool turn; //true: red, false: blue
};

struct list {
    list *parent;
    list *child;
    node *q;
};

static list *open_list;
static list *closed_list;

// Red's turn first
static bool turn = true;

char solution[][] {
    {1, 0, 1},
    {0, 0, 0},
    {2, 0, 2}
};

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
            lowest = temp->q;
        }
        
        temp = temp->parent;
    }
    
    if (temp->parent == 0) {
        if (temp->q->f < lowest->f) {
            lowest = temp->q;
        }
    }
    
    if (DEBUG) {
        cout << "Lowest Node's f: " << lowest->f << endl;
    }
    return lowest;
}

int getHeuristic(Node x) {
    int total = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (x->board[i][j] == solution[i][j]) {
                total++;
            }
        }
    }
    return total;
}

bool isEqual(char a[][3], char b[][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (a[i][j] != b[i][j]) return false;
        }
    }
    return true;
}

void setto(char a[][3], char b[][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a[i][j] = b[i][j];
        }
    }
}

int toPositive(int x) {
    if (x >=0) {
        return x;
    } else {
        return -1 * x;
    }
}

bool valid(node *p, int x, int y) {
    char tempp[3][3];
    setto(tempp, p->board);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (p->turn) {
                if (tempp[i][j] == 2) {
                    if ((toPositive(i - x) == 2 && toPositive(j - y) == 1)
                        || (toPositive(i - x) == 1 && toPositive(j - y) == 2)) {
                        p->board[i][j] = 0;
                        p->board[x][y] = 2;
                        return true;
                    }
                }
            } else {
                if (tempp[i][j] == 1) {
                    if ((toPositive(i - x) == 2 && toPositive(j - y) == 1)
                        || (toPositive(i - x) == 1 && toPositive(j - y) == 2)) {
                        p->board[i][j] = 0;
                        p->board[x][y] = 1;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isSolution(char x[][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (x[i][j] == solution[i][j]) {
                continue;
            } else {
                return false;
            }
        }
    }
}

bool alreadyOpen(node *q) {
    node *temp = open_list;
    if (open_list != 0) {
        while (temp->parent != 0) {
            temp = temp->parent;
        }
        while (temp->child != 0) {
            if (isEqual(q->board, temp->q->board) {
                return true;
            } else {
                temp = temp->child;
            }
        }
        
        if (isEqual(q->board, temp->q->board) {
            return true;
        }
    } else {
        return false;
    }
}

bool alreadyClosed(node *q) {
    node *temp = closed_list;
    if (closed_list != 0) {
        while (temp->parent != 0) {
            temp = temp->parent;
        }
        while (temp->child != 0) {
            if (isEqual(q->board, temp->q->board) {
                return true;
            } else {
                temp = temp->child;
            }
        }
        
        if (isEqual(q->board, temp->q->board) {
            return true;
        }
    } else {
        return false;
    }
}

void deleteNode(node *p) {
    list *l = open_list;
    list *del = 0;

    // resets the list to the beginning for searching
    while (l->parent != 0) {
        l = l->parent;
    }
    
    // if q is the only node left in the open_list then delete it
    if (l->q == p) {
        del = open_list;
        open_list = 0;
    } else {
        while (l->child != 0) {
            if (l->q == p) {
                del = l;
                list *temp = l->child;
                l = l->parent;
                l->child = temp;
                break;
            }
        }
        
        if (l->child == 0 && del == 0) {
            if (l->q == p) {
                del = l;
                list *temp = l->child;
                l = l->parent;
                l->child = temp;
            } else {
                return;
            }
        }
    }
    
    // Add deleted node to the closed list
    del->child = 0;
    del->parent = closed_list;
    closed_list = del;
}

node* solve(char board[][3]) {
    node *q;
    list *temp;
    bool solved = false;
    
    // Setup queue and initial node
    q->f = 0;
    
    q->parent = 0;
    setto(q->board, board);
    q->f = 0;
    q->turn = true;
    
    temp->child = 0;
    temp->parent = 0;
    temp->q = q;
    
    open_list = temp;
    closed_list = 0;
    
    // Main loop
    // TODO: open list MUST have a parent with 0 in which it should not be
    // Evaluated, investigate pointer...
    while (open_list != 0) {
        node *p;
        p = getLowestNode();
        
        // Search entire board for valid moves and evaluate them
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (valid(p, i, j)) {
                    node *c;
                    c->parent = p;
                    c->turn = !p->turn;
                    if (isSolution(c->board)) {
                        q = c;
                        solved = true;
                        break;
                    } else {
                        c->f = c->parent->f + 1 + getHeuristic(c);
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