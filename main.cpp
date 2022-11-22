#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>

enum class State {kEmpty, kObstacle};

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

std::vector<std::vector<State>> Search(
        std::vector<std::vector<State>> board,
        std::array<int,2> start,
        std::array<int,2> goal) {

    std::cout << "No path found!" << std::endl;
    return {};
}

std::string CellString(State cell) {
    switch(cell) {
        case State::kObstacle: return "⛰️   ";
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


int main() {
    std::array<int, 2> init {0,0};
    std::array<int, 2> goal {4,5};

    auto board = ReadBoardFile("1.1.board");

    auto solution = Search(board, init, goal);

    PrintBoard(solution);
}
