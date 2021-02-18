#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <memory>
#include <string>

using namespace std;

/*
function general-search(problem, QUEUEING-FUNCTION)
nodes = MAKE-QUEUE(MAKE-NODE(problem.INITIAL-STATE))
loop do
if EMPTY(nodes) then return "failure"
 node = REMOVE-FRONT(nodes)
if problem.GOAL-TEST(node.STATE) succeeds then return node
 nodes = QUEUEING-FUNCTION(nodes, EXPAND(node, problem.OPERATORS))
end
*/

struct puzzle_state
{
    vector<vector<int>> game_state;
    int empty_slot_x = 0;
    int empty_slot_y = 0;
    shared_ptr<puzzle_state> move = nullptr;
    int depth = 0;
};

struct uniform_cost_comparator{
    bool operator()(const shared_ptr<puzzle_state> &a, const shared_ptr<puzzle_state> &b) const {
        return true;
    }
};

vector<shared_ptr<puzzle_state>> expand(const shared_ptr<puzzle_state> &current_state)
{
    vector<shared_ptr<puzzle_state>> return_value;

    //forward x-coordinate
    for (int i = current_state->empty_slot_x + 1; i < 3; i++)
    {
        auto puzzle_copy = current_state->game_state;
        rotate(puzzle_copy[current_state->empty_slot_y].begin() + current_state->empty_slot_x, 
            puzzle_copy[current_state->empty_slot_y].begin() + current_state->empty_slot_x + 1, 
            puzzle_copy[current_state->empty_slot_y].begin() + i);
        return_value.push_back(make_shared<puzzle_state>(puzzle_state{move(puzzle_copy), i, current_state->empty_slot_y, current_state}));
    }

    //backwards x-coordinate
    for (int j = 0; j < current_state->empty_slot_x; j++)
    {
        auto puzzle_copy = current_state->game_state;
        rotate(puzzle_copy[current_state->empty_slot_y].begin() + j, 
            puzzle_copy[current_state->empty_slot_y].begin() + current_state->empty_slot_x, 
            puzzle_copy[current_state->empty_slot_y].begin() + current_state->empty_slot_x + 1);
        return_value.push_back(make_shared<puzzle_state>(puzzle_state{move(puzzle_copy), j, current_state->empty_slot_y, current_state}));
    }

    //forwards y-coordinate
    for (int k = current_state->empty_slot_y + 1; k < 3; k++) {
        auto puzzle_copy = current_state->game_state;
        for (int i = current_state->empty_slot_y; i < k - 1; i++) {
            swap(puzzle_copy[i][current_state->empty_slot_x], puzzle_copy[i+1][current_state->empty_slot_x]);
        }
        return_value.push_back(make_shared<puzzle_state>(puzzle_state{move(puzzle_copy), current_state->empty_slot_x, k, current_state}));

    }

    //backwards y-coordinate
    for (int l = 0; l < current_state->empty_slot_y; l++) {
        auto puzzle_copy = current_state->game_state;
          for (int i = current_state->empty_slot_y; i > l; i--) {
            swap(puzzle_copy[i][current_state->empty_slot_x], puzzle_copy[i-1][current_state->empty_slot_x]);
        }
        return_value.push_back(make_shared<puzzle_state>(puzzle_state{move(puzzle_copy), current_state->empty_slot_x, l, current_state}));
    } 
    return return_value;
}




/*
function general-search(problem, QUEUEING-FUNCTION)
nodes = MAKE-QUEUE(MAKE-NODE(problem.INITIAL-STATE))
loop do
if EMPTY(nodes) then return "failure"
 node = REMOVE-FRONT(nodes)
if problem.GOAL-TEST(node.STATE) succeeds then return node
 nodes = QUEUEING-FUNCTION(nodes, EXPAND(node, problem.OPERATORS))
end
*/

// void generalSearch(int choice, vector<vector<int>> a)
void generalSearch(int choice, const shared_ptr<puzzle_state> &a)
{
    shared_ptr<puzzle_state> xd;

    /**
1. Uniform Cost Search
2. A* with the Misplaced Tile heuristic.
3. A* with the Manhattan distance heuristic.
     */
    switch (choice)
    {
    case 1:
    priority_queue<shared_ptr<puzzle_state>, uniform_cost_comparator> p_q;
    xd = a;
    p_q.push(xd);
    while (true) {
        if (p_q.empty()) {
            return;
        }
        auto current = p_q.top();
        p_q.pop();   
    }
        break;
    case 2:
        break;
    case 3:
        break;
    }
}

void displayPuzzle(vector<vector<int>> puzzle)
{
    for (int i = 0; i < puzzle.size(); i++)
    {
        for (int j = 0; j < puzzle[i].size(); j++)
        {
            cout << puzzle[i][j] << ' ';
        }
        cout << endl;
    }
}

bool checkComplete(vector<vector<int>> a)
{
    vector<vector<int>> completed;
    completed.resize(3, vector<int>(3, 0));
    completed[0][0] = 1;
    completed[0][1] = 2;
    completed[0][2] = 3;
    completed[1][0] = 4;
    completed[1][1] = 5;
    completed[1][2] = 6;
    completed[2][0] = 7;
    completed[2][1] = 8;
    completed[2][2] = 0;
    // displayPuzzle(completed);
    if (completed == a)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{

    vector<vector<int>> puzzle;

    cout << "8-puzzle solver" << endl;
    cout << endl;
    cout << "Enter the puzzle" << endl;
    int count = 3, value_1, value_2, value_3;

    int currRow = 0;
    int colZero = 0;

    while (count > 0)
    {
        cout << "Enter values for row " << 4 - count << endl;
        vector<int> rowInputs;
        cin >> value_1;
        if (value_1 == 0)
        {
            colZero = 0;
            currRow = 3 - count;
        }
        cin >> value_2;
        if (value_2 == 0)
        {
            colZero = 1;
            currRow = 3 - count;
        }
        cin >> value_3;
        if (value_3 == 0)
        {
            colZero = 2;
            currRow = 3 - count;
        }
        rowInputs.push_back(value_1);
        rowInputs.push_back(value_2);
        rowInputs.push_back(value_3);
        puzzle.push_back(rowInputs);
        count--;
        cout << endl;
    }

    auto init_state = make_shared<puzzle_state>();
    init_state->game_state = move(puzzle);
    init_state->empty_slot_x = currRow;
    init_state->empty_slot_y = colZero;

    displayPuzzle(init_state->game_state);

    int algoChoice;
    cout << "select algorithm" << endl;
    cout << "1. Uniform Cost Search" << endl;
    cout << "2. A* with the Misplaced Tile Heuristic" << endl;
    cout << "3. A* with the Manhattan Distance Heuristic" << endl;

    // if (checkComplete(puzzle) == true) {
    //     cout << "completed" << endl;
    // } else {
    //     cout << "not completed" << endl;
    // }

    cin >> algoChoice;

    bool chosen = false;
    while (chosen == false)
    {
        switch (algoChoice)
        {
        case 1:
            generalSearch(1, init_state);
            chosen = true;
            break;
        case 2:
            generalSearch(2, init_state);
            chosen = true;
            break;
        case 3:
            generalSearch(3, init_state);
            chosen = true;
            break;
        default:
            cout << "Error: no choice provided" << endl;
            cin >> algoChoice;
            break;
        }
    }
}
