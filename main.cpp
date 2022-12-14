#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>

enum class State {kStart, kFinish, kEmpty, kObstacle, kClosed, kPath};

void SetStartAndFinish(std::array<int,2> const & init,
                       std::array<int,2> const & goal,
                       std::vector<std::vector<State>> & board) {
    board[init[0]][init[1]] = State::kStart;
    board[goal[0]][goal[1]] = State::kFinish;
}

std::vector<State> ParseLine(std::string const& line) {
    std::istringstream sline(line);
    int n;
    char c;
    std::vector<State> row;
    while (sline >> n >> c && c == ',') {
        if (n == 0) {
            row.push_back(State::kEmpty);
        } else {
            row.push_back(State::kObstacle);
        }
    }
    return row;
}

bool Compare (std::vector<int> first, std::vector<int> second) {
    auto f_first = first[2]+first[3];
    auto f_second = second[2]+second[3];
    return f_first > f_second;
}

void CellSort(std::vector<std::vector<int>> *v) {
    sort(v->begin(), v->end(), Compare);
}

int Heuristic (int x1, int y1, int x2, int y2) {
    // Manhattan distance formula
    return (abs(x2-x1) + abs(y2-y1));
}

std::vector<std::vector<State>> ReadBoardFile(std::string const& path) {
    std::ifstream myfile (path);
    std::vector<std::vector<State>> board{};
    if (myfile) {
        std::string line;
        while (getline(myfile, line)) {
            std::vector<State> row = ParseLine(line);
            board.push_back(row);
        }
    }
    return board;
}

bool CheckValidCell(int x, int y, std::vector<std::vector<State>> const& grid) {
    if (x >= grid.size() || x < 0) {
        return false;
    }

    if (y >= grid[0].size() || y < 0) {
        return false;
    }

    if (grid[x][y] == State::kEmpty) {
        return true;
    }
    return false;
}

void AddToOpen (int x, int y, int g, int h,
               std::vector<std::vector<int>> & open_nodes,
               std::vector<std::vector<State>> & grid) {

    std::vector<int> node {x,y,g,h};
    open_nodes.push_back(node);

    grid[x][y] = State::kClosed;
}

void ExpandNeighbors (std::vector<int> const & current_node,
                      std::array<int,2> const & goal,
                      std::vector<std::vector<int>> & open_nodes,
                      std::vector<std::vector<State>> & grid) {

    int const delta[4][2] {
            {-1, 0}, {0, -1}, {1, 0}, {0, 1}
    };

    int x = current_node[0];
    int y = current_node[1];
    int g = current_node[2];

    int x2 = -1;
    int y2 = -1;
    int g2 = g + 1;

    for (auto i : delta) {
        x2 = x + i[0];
        y2 = y + i[1];

        if (CheckValidCell(x2, y2, grid)) {
            auto h2 = Heuristic(x2, y2, goal[0], goal[1]);
            AddToOpen(x2, y2, g2, h2, open_nodes, grid);
        }
    }
}

std::vector<std::vector<State>> Search(
        std::vector<std::vector<State>> & board,
        std::array<int,2> start,
        std::array<int,2> goal) {

    std::vector<std::vector<int>> open {};

    int cost = 0;
    int heuristic = Heuristic(start[0], start[1], goal[0], goal[1]);

    AddToOpen(start[0], start[1], cost, heuristic, open, board);

    std::vector<int> current_node {-1,-1,-1,-1};

    while (!open.empty()) {
        CellSort(&open);
        current_node = open.back();
        open.pop_back();

        int x = current_node[0];
        int y = current_node[1];

        board[x][y] = State::kPath;

        if (x == goal[0] && y == goal[1]) {
            SetStartAndFinish(start, goal, board);
            return board;
        }

        ExpandNeighbors(current_node, goal, open, board);
    }


    std::cout << "No path found!" << std::endl;
    return std::vector<std::vector<State>>{};
}

std::string CellString(State cell) {
    switch(cell) {
        case State::kObstacle: return "??????   ";
        case State::kPath:  return "????   ";
        case State::kStart: return "????   ";
        case State::kFinish: return "????   ";

        default: return "0   ";
    }
}

void PrintBoard(std::vector<std::vector<State>> const& board) {
    for (const auto & i : board) {
        for (auto j : i) {
            std::cout << CellString(j);
        }
        std::cout << std::endl;
    }
}

#include "test.cpp"

int main() {
    std::array<int, 2> init {0,0};
    std::array<int, 2> goal {4,5};

    auto board = ReadBoardFile("1.board");

    auto solution = Search(board, init, goal);

    PrintBoard(solution);

    //Tests
    TestHeuristic();
    TestAddToOpen();
    TestCompare();
    TestSearch();
    TestCheckValidCell();
    TestExpandNeighbors();
}
