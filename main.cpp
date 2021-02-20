#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <memory>
#include <string>
#include <chrono>

using namespace std;

struct puzzle_state
{
    vector<vector<int>> game_state;
    int empty_slot_x = 0;
    int empty_slot_y = 0;
    shared_ptr<puzzle_state> pre_state = nullptr;
    int depth = 0;
};

int countMisplacedTiles(const vector<vector<int>> &a) {
    int sum = 0;
    if (a[0][0] != 1) {
        sum++;
    } 
    if (a[0][1] != 2) {
        sum++;
    }
    if (a[0][2] != 3) {
        sum++;
    }
    if (a[1][0] != 4) {
        sum++;
    } 
    if (a[1][1] != 5) {
        sum++;
    }
    if (a[1][2] != 6) {
        sum++;
    }
     if (a[2][0] != 7) {
        sum++;
    } 
    if (a[2][1] != 8) {
        sum++;
    }
    if (a[2][2] != 0) {
        sum++;
    }
    return sum;    
}

int evaluateManhattanDistance(const vector<vector<int>> &a) {
    //find x,y pos of 1.
    //distance += abs(x - actual_x) + abs(y - real_y)
    
    int distance = 0;

    int pos_0_x, pos_0_y, pos_1_x, pos_1_y, pos_2_x, pos_2_y, pos_3_x, pos_3_y,
        pos_4_x, pos_4_y, pos_5_x, pos_5_y, pos_6_x, pos_6_y, pos_7_x, pos_7_y, 
        pos_8_x, pos_8_y = 0;

    const int real_1_x = 0;
    const int real_1_y = 0;
    const int real_2_x = 1;
    const int real_2_y = 0; 
    const int real_3_x = 2;
    const int real_3_y = 0;
    const int real_4_x = 0;
    const int real_4_y = 1; 
    const int real_5_x = 1;
    const int real_5_y = 1;
    const int real_6_x = 2;
    const int real_6_y = 1; 
    const int real_7_x = 0;
    const int real_7_y = 2;
    const int real_8_x = 1;
    const int real_8_y = 2;     
    const int real_0_x = 2;     
    const int real_0_y = 2;     

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (a[i][j] == 1) {
                pos_1_x = j;
                pos_1_y = i;
            }
            if (a[i][j] == 2) {
                pos_2_x = j;
                pos_2_y = i;
            }
            if (a[i][j] == 3) {
                pos_3_x = j;
                pos_3_y = i;
            }
            if (a[i][j] == 4) {
                pos_4_x = j;
                pos_4_y = i;
            }
            if (a[i][j] == 5) {
                pos_5_x = j;
                pos_5_y = i;
            }
            if (a[i][j] == 6) {
                pos_6_x = j;
                pos_6_y = i;
            }
            if (a[i][j] == 7) {
                pos_7_x = j;
                pos_7_y = i;
            }
            if (a[i][j] == 8) {
                pos_8_x = j;
                pos_8_y = i;
            }
            if (a[i][j] == 0) {
                pos_0_x = j;
                pos_0_y = i;
            }          
        }
    }

    //distance += abs(x - actual_x) + abs(y - real_y)
    distance += abs(pos_0_x - real_0_x) + abs(pos_0_y - real_0_y) + abs(pos_1_x - real_1_x) + abs(pos_1_y - real_1_y) 
        + abs(pos_2_x - real_2_x) + abs(pos_2_y - real_2_y) + abs(pos_3_x - real_3_x) + abs(pos_3_y - real_3_y)
        + abs(pos_4_x - real_4_x) + abs(pos_4_y - real_4_y) + abs(pos_5_x - real_5_x) + abs(pos_5_y - real_5_y)
        + abs(pos_6_x - real_6_x) + abs(pos_6_y - real_6_y) + abs(pos_7_x - real_7_x) + abs(pos_7_y - real_7_y)
        + abs(pos_8_x - real_8_x) + abs(pos_8_y - real_8_y);

    return distance;
    
}

struct uniform_cost_comparator {
    bool operator()(const shared_ptr<puzzle_state> &a, const shared_ptr<puzzle_state> &b) const {
        if (a->depth > b->depth) {
        return true;
        } else {
            return false;
        }
    }
};

struct misplaced_tile_comparator {
    bool operator()(const shared_ptr<puzzle_state> &a, const shared_ptr<puzzle_state> &b) const {
        int misplacedA = countMisplacedTiles(a->game_state);
        int misplacedB = countMisplacedTiles(b->game_state);
        if (misplacedA > misplacedB) {
            return true;
        } 
        else if (misplacedA == misplacedB) {
            if (a->depth > b->depth) {
                return true; 
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
};

struct manhattan_distance_comparator {
    bool operator()(const shared_ptr<puzzle_state> &a, const shared_ptr<puzzle_state> &b) const {
        int manhattanA = evaluateManhattanDistance(a->game_state);
        int manhattanB = evaluateManhattanDistance(b->game_state);
        if (manhattanA + a->depth > manhattanB + b->depth) {
            return true;
        } else if (manhattanA == manhattanB) {
            if (a->depth > b->depth) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
};


bool checkComplete(const vector<vector<int>> &a)
{
    static vector<vector<int>> completed = { {1,2,3}, {4,5,6}, {7,8,0} };

    if (completed == a)
    {
        return true;
    }
    else
    {
        return false;
    }
}

vector<shared_ptr<puzzle_state>> expand(const shared_ptr<puzzle_state> &current_state)
{
    vector<shared_ptr<puzzle_state>> node_children;

    /**

    //forward x-coordinate
    for (int i = current_state->empty_slot_x + 1; i < 3; i++)
    {
        auto puzzle_copy = current_state->game_state;
        rotate(puzzle_copy[current_state->empty_slot_y].begin() + current_state->empty_slot_x, 
            puzzle_copy[current_state->empty_slot_y].begin() + current_state->empty_slot_x + 1, 
            puzzle_copy[current_state->empty_slot_y].begin() + i + 1);
        node_children.push_back(make_shared<puzzle_state>(puzzle_state{
            move(puzzle_copy), i, current_state->empty_slot_y, current_state, current_state->depth+1
            }));
    }

    //backwards x-coordinate
    for (int j = 0; j < current_state->empty_slot_x; j++)
    {
        auto puzzle_copy = current_state->game_state;
        // param 1 - beginn
        // param 2 - middle
        // param 3 - end
        rotate(puzzle_copy[current_state->empty_slot_y].begin() + j, 
            puzzle_copy[current_state->empty_slot_y].begin() + current_state->empty_slot_x, 
            puzzle_copy[current_state->empty_slot_y].begin() + current_state->empty_slot_x + 1);
        node_children.push_back(make_shared<puzzle_state>(puzzle_state{
            move(puzzle_copy), j, current_state->empty_slot_y, current_state, current_state->depth+1
            }));
    }

    //forwards y-coordinate
    for (int k = current_state->empty_slot_y + 1; k < 3; k++) {
        auto puzzle_copy = current_state->game_state;
        for (int i = current_state->empty_slot_y; i < k; i++) {
            swap(puzzle_copy[i][current_state->empty_slot_x], puzzle_copy[i+1][current_state->empty_slot_x]);
        }
        node_children.push_back(make_shared<puzzle_state>(puzzle_state{
            move(puzzle_copy), current_state->empty_slot_x, k, current_state, current_state->depth+1
            }));

    }

    //backwards y-coordinate
    for (int l = 0; l < current_state->empty_slot_y; l++) {
        auto puzzle_copy = current_state->game_state;
          for (int i = current_state->empty_slot_y; i > l; i--) {
            swap(puzzle_copy[i][current_state->empty_slot_x], puzzle_copy[i-1][current_state->empty_slot_x]);
        }
        node_children.push_back(make_shared<puzzle_state>(puzzle_state{
            move(puzzle_copy), current_state->empty_slot_x, l, current_state, current_state->depth+1
            }));
    } 

    **/

    if(current_state->empty_slot_x != 0) {
        auto puzzle_copy = current_state->game_state;
        swap(puzzle_copy[current_state->empty_slot_y][current_state->empty_slot_x - 1], puzzle_copy[current_state->empty_slot_y][current_state->empty_slot_x]);
        node_children.push_back(make_shared<puzzle_state>(puzzle_state{
            move(puzzle_copy), current_state->empty_slot_x -1,current_state->empty_slot_y ,  current_state, current_state->depth + 1
            }));
    }

    if (current_state->empty_slot_y != 0) {
        auto puzzle_copy = current_state->game_state;
        swap(puzzle_copy[current_state->empty_slot_y -1 ][current_state->empty_slot_x], puzzle_copy[current_state->empty_slot_y][current_state->empty_slot_x]);
        node_children.push_back(make_shared<puzzle_state>(puzzle_state{
            move(puzzle_copy), current_state->empty_slot_x,current_state->empty_slot_y  -1 ,  current_state, current_state->depth + 1
            }));
    }

    if (current_state->empty_slot_x != 2) {
        auto puzzle_copy = current_state->game_state;
        swap(puzzle_copy[current_state->empty_slot_y][current_state->empty_slot_x + 1], puzzle_copy[current_state->empty_slot_y][current_state->empty_slot_x]);
        node_children.push_back(make_shared<puzzle_state>(puzzle_state{
            move(puzzle_copy), current_state->empty_slot_x +1,current_state->empty_slot_y ,  current_state, current_state->depth + 1
            }));
    }

    if (current_state->empty_slot_y != 2) {
        auto puzzle_copy = current_state->game_state;
        swap(puzzle_copy[current_state->empty_slot_y+1][current_state->empty_slot_x], puzzle_copy[current_state->empty_slot_y][current_state->empty_slot_x]);
        node_children.push_back(make_shared<puzzle_state>(puzzle_state{
            move(puzzle_copy), current_state->empty_slot_x,current_state->empty_slot_y+1 ,  current_state, current_state->depth + 1
            }));
    }
    
    return node_children;
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
shared_ptr<puzzle_state> generalSearch(int choice, const shared_ptr<puzzle_state> &a)
{
    // shared_ptr<puzzle_state> xd;

    /**
1. Uniform Cost Search
2. A* with the Misplaced Tile heuristic.
3. A* with the Manhattan distance heuristic.
     */
    auto start = std::chrono::steady_clock::now();

    // Uniform Cost Search
    if (choice == 1) {
        //type, container, comparison function
        priority_queue<shared_ptr<puzzle_state>, vector<shared_ptr<puzzle_state>>, uniform_cost_comparator> p_q;
    //  xd = a;
     p_q.push(a);
        int num_times_expanded = 0;
        // int max_q_size = 1;     
     while (true) {
        //  max_q_size = max(max_q_size,(int)p_q.size());
         if (p_q.empty()) {
               auto end = std::chrono::steady_clock::now();
               cout << "Algorithm took " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms." << endl;
             return nullptr;
        }
         auto current = p_q.top();
         p_q.pop();   
         if (checkComplete(current->game_state) == true) {
               auto end = std::chrono::steady_clock::now();
               cout << "Algorithm took " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms." << endl;
               cout << "Nodes expanded: " << num_times_expanded << endl;
             return current;
         } else {
             vector<shared_ptr<puzzle_state>> next_states = expand(current);
             num_times_expanded++;
             for (const auto& next_state : next_states) {
                 p_q.push(next_state);
             }
         }
     }

    // A* with the Misplaced Tile heuristic.
    } else if (choice == 2) {\
        priority_queue<shared_ptr<puzzle_state>, vector<shared_ptr<puzzle_state>>, misplaced_tile_comparator> p_q;
     p_q.push(a);
          int num_times_expanded = 0;
     while (true) {
        //  max_q_size = max(max_q_size,(int)p_q.size());
         if (p_q.empty()) {
               auto end = std::chrono::steady_clock::now();
               cout << "Algorithm took " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms." << endl;
             return nullptr;
        }
         auto current = p_q.top();
         p_q.pop();   
         if (checkComplete(current->game_state) == true) {
               auto end = std::chrono::steady_clock::now();
               cout << "Algorithm took " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms." << endl;
               cout << "Nodes expanded: " << num_times_expanded << endl;
             return current;
         } else {
             vector<shared_ptr<puzzle_state>> next_states = expand(current);
                 num_times_expanded++;
             for (const auto& next_state : next_states) {
                 p_q.push(next_state);
             }
         }
     }

    // A* with the Manhattan distance heuristic.
    } else if (choice ==3) {
        priority_queue<shared_ptr<puzzle_state>, vector<shared_ptr<puzzle_state>>, manhattan_distance_comparator> p_q;
    p_q.push(a);
        int num_times_expanded = 0;
        //  int max_q_size = 1;
     while (true) {
        //  max_q_size = max(max_q_size,(int)p_q.size());
         if (p_q.empty()) {
               auto end = std::chrono::steady_clock::now();
               cout << "Algorithm took " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms." << endl;
             return nullptr;
        }
         auto current = p_q.top();
         p_q.pop();   
         if (checkComplete(current->game_state) == true) {
               auto end = std::chrono::steady_clock::now();
               cout << "Algorithm took " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms." << endl;
               cout << "Nodes expanded: " << num_times_expanded << endl;
             return current;
         } else {
             vector<shared_ptr<puzzle_state>> next_states = expand(current);
                 num_times_expanded++;
             for (const auto& next_state : next_states) {
                 p_q.push(next_state);
             }
         }

    }
    }

    // switch (choice)
    // {
    // case 1:
    // priority_queue<shared_ptr<puzzle_state>, uniform_cost_comparator> p_q;
    // xd = a;
    // // p_q.push(xd);
    // // while (true) {
    // //     if (p_q.empty()) {
    // //         return;
    // //     }
    // //     auto current = p_q.top();
    // //     p_q.pop();   
    // // }
    //     break;
    // case 2:
    //     break;
    // case 3:
    //     break;
    // }
        return nullptr;
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

void display_full_puzzle(const std::shared_ptr<puzzle_state>& state){
    if(state == nullptr) {
        return;
    }
    displayPuzzle(state->game_state);
    std::cout << std::endl;
    display_full_puzzle(state->pre_state);
    
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
    init_state->empty_slot_y = currRow;
    init_state->empty_slot_x = colZero;

    displayPuzzle(init_state->game_state);

    cout << endl;

    int algoChoice;
    cout << "Choose algorithm" << endl;
    cout << "1. Uniform Cost Search" << endl;
    cout << "2. A* with the Misplaced Tile Heuristic" << endl;
    cout << "3. A* with the Manhattan Distance Heuristic" << endl;
    cout << endl;

    // if (checkComplete(puzzle) == true) {
    //     cout << "completed" << endl;
    // } else {
    //     cout << "not completed" << endl;
    // }

    cin >> algoChoice;

    bool chosen = false;
    while (chosen == false)
    {
        if (algoChoice == 1) {
            // cout << "loading" << endl;
            shared_ptr<puzzle_state> a = generalSearch(1, init_state);
            if (a == nullptr) {
                cout << "The matrix is unsolvable." << endl; 
                return 0;
            } else {
            cout << "The depth for this algorithm is " << a->depth << endl;
            }
            chosen = true;
            // cout << "done" << endl;
        } else if (algoChoice == 2) {
            shared_ptr<puzzle_state> a = generalSearch(2, init_state);
            if (a == nullptr) {
                cout << "The matrix is unsolvable." << endl;
                return 0;
            }
            cout << "The depth for this algorithm is " << a->depth << endl;
            chosen = true;
        } else if (algoChoice == 3) {
            shared_ptr<puzzle_state> a = generalSearch(3, init_state);
             if (a == nullptr) {
                cout << "The matrix is unsolvable." << endl;
                return 0;
            }
            cout << "The depth for this algorithm is " << a->depth << endl;
            chosen = true;
        }
        // switch (algoChoice)
        // {
        // case 1:
        //     shared_ptr<puzzle_state> a = generalSearch(1, init_state);
        //     cout << "The depth for this algorithm is " << a->depth << endl;
        //     chosen = true;
        //     break;
        // case 2:
        //     generalSearch(2, init_state);
        //     chosen = true;
        //     break;
        // case 3:
        //     generalSearch(3, init_state);
        //     chosen = true;
        //     break;
        // default:
        //     cout << "Error: no choice provided. Re-enter choice or quit (ctrl + c)" << endl;
        //     cin >> algoChoice;
        //     break;
        // }
    }
}
