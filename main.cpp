#include <iostream>

using namespace std;

#define INFINITY INT_MAX
#define DEBUG false

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
static bool display = false;

char solution[][3] = {
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

void printBoard(char board[][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << (int)board[i][j] << " ";
        }
        cout << endl;
    }
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
    
    if (DEBUG && true) {
        cout << "Lowest Node's f: " << lowest->f << endl;
    }
    return lowest;
}

int getHeuristic(node *x) {
    int total = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (x->board[i][j] == solution[i][j]) {
                total++;
            }
        }
    }
    return 9 - total;
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
                if (tempp[i][j] == 2 && tempp[x][y] == 0) {
                    if ((toPositive(i - x) == 2 && toPositive(j - y) == 1)
                        || (toPositive(i - x) == 1 && toPositive(j - y) == 2)) {
                        p->board[i][j] = 0;
                        p->board[x][y] = 2;
                        if (DEBUG && display)
                            cout << "Turn" << p->turn << "Valid x: " << x << " y: " << y << endl;
                        return true;
                    }
                }
            } else {
                if (tempp[i][j] == 1 && tempp[x][y] == 0) {
                    if ((toPositive(i - x) == 2 && toPositive(j - y) == 1)
                        || (toPositive(i - x) == 1 && toPositive(j - y) == 2)) {
                        p->board[i][j] = 0;
                        p->board[x][y] = 1;
                        if (DEBUG && display)
                            cout << "Turn" << p->turn << "Valid x: " << x << " y: " << y << endl;
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
    list *temp = open_list;
    if (open_list != 0) {
        while (temp->parent != 0) {
            temp = temp->parent;
        }
        while (temp->child != 0) {
            if (isEqual(q->board, temp->q->board) && q->f >= temp->q->f) {
            	return true;
            } else {
                temp = temp->child;
            }
        }
        
        if (isEqual(q->board, temp->q->board) && q->f >= temp->q->f) {
        	return true;
        }
    } else {
        return false;
    }
    return false;
}

bool alreadyClosed(node *q) {
    list *temp = closed_list;
    if (closed_list != 0) {
        while (temp->parent != 0) {
            temp = temp->parent;
        }
        while (temp->child != 0) {
            if (isEqual(q->board, temp->q->board) && q->f >= temp->q->f) {
            	return true;
            } else {
                temp = temp->child;
            }
        }
        
        if (isEqual(q->board, temp->q->board) && q->f >= temp->q->f) {
        	return true;
        }
    } else {
        return false;
    }
    return false;
}

void printClosedList() {
	list *p = closed_list;
	cout << "_______________LIST_____________" << endl;
	while (p != 0) {
		if (DEBUG && true)
		cout << "[" << p->child << " | " << p << " | " << p->parent << "], ";
		p = p->parent;
	}
	if (DEBUG && true) cout << endl;
}

void printOpenList() {
	list *p = open_list;
	cout << "_______________LIST_____________" << endl;
	while (p != 0) {
		if (DEBUG && true)
			cout << "[" << p->child << " | " << p << " | " << p->parent << "]" << endl;
		// printBoard(p->q->board);
		p = p->parent;
	}
}

void deleteNode(node *p) {
    list *l = open_list;
    list *del = NULL;

    // resets the list to the beginning for searching
    while (l->parent != 0) {
        l = l->parent;
    }
    
    if (DEBUG && true) printOpenList();
    // if (DEBUG && true) printClosedList();

    // if q is the only node left in the open_list then delete it
    if (l->q == p && l->child == 0) {
        del = open_list;
        open_list = 0;
        if (DEBUG && true)
            cout << "q was the only node in open list" << endl;
    } else {
        while (l->child != 0) {
            if (l->q == p) {
                if (DEBUG && true)
                    cout << "____Deleting node: " << l << "____" << endl;
                del = l;
                list *temp = l->child;
                if (l->parent != 0) {
                    l = l->parent;
                    temp->parent = l;
                    l->child = temp;
                } else {
                    l = l->child;
                    l->parent = 0;
                }
                break;
            }
            l = l->child;
        }
        
        if (l->child == 0 && del == 0) {
            if (l->q == p) {
                del = l;
                l = l->parent;
                l->child = 0;
            } else {
                return;
            }
        }
    }
    
    // Add deleted node to the closed list
    del->child = 0;
    del->parent = closed_list;
    if (closed_list != 0) closed_list->child = del;
    closed_list = del;
}

node* solve(char board[][3]) {
    node *q;
    list *temp;
    bool solved = false;
    // Setup queue and initial node
    temp = new list;
    q = new node;
    q->parent = NULL;
    setto(q->board, board);
    q->f = 0;
    q->turn = true;
    
    temp->child = 0;
    temp->parent = 0;
    temp->q = q;
    
    open_list = temp;
    closed_list = 0;
    
    // A* Algorithm
    while (open_list != 0) {
        node *p;
        if (DEBUG && display)
            cout << " open_list:" << open_list << " c:" << closed_list << endl;
        p = getLowestNode();
        
        // Search entire board for valid moves and evaluate them
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                node *c;
                c = new node;
                c->turn = !p->turn;
                setto(c->board, p->board);
                if (valid(c, i, j)) {
                    if (DEBUG && display)
                        printBoard(c->board);
                    c->parent = p;
                    if (isSolution(c->board)) {
                        q = c;
                        solved = true;
                        return q;
                    } else {
                        if (DEBUG && display)
                            cout << "NOT SOLUTION" << endl;
                        c->f = c->parent->f + 1 + getHeuristic(c);
                        if (!alreadyOpen(c)) {
                            if (!alreadyClosed(c)) {
                                if (DEBUG && display)
                                    cout << "ADD to LIST" << endl;
                                temp = new list;
                                temp->child = 0;
                                temp->parent = open_list;
                                temp->q = c;
                                open_list->child = temp;
                                open_list = temp;
                                if (DEBUG && display)
                                    cout << " o:" << open_list << " c:" << closed_list << endl;
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

void printSolution(node *p) {
    node *x = p;
    int i = 0;
    if (DEBUG && display)
        cout << "------------------------------printing solution------------------------------" << endl;
    while (x != 0) {
    	i++;
    	cout << "--" << i << "--" << endl;
        printBoard(x->board);
    	cout << "-----" << endl;
        x = x-> parent;
    }
}

int main() {
    char board[3][3];
    node *solution;
    
    cout << "----------------A-star search algorithm applied to knight problem--------------" << endl;
    
    setBoard(board);
    solution = solve(board);
    printSolution(solution);
    if (DEBUG && display)
        cout << "Returns success " << solution << endl;
    
}
