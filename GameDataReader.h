#ifndef GAMEDATAREADER_H
#define GAMEDATAREADER_H

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

#include "GameState/Board.h"


class GameDataReader {
public:
  static std::string readFile(std::string path) {
    std::ifstream in {path};
    std::string text {std::istreambuf_iterator<char>(in), {}};
    return text;
  }

  static std::vector<std::pair<char, std::vector<std::string>>> parse_boards(std::string input) {
    std::istringstream iss(input);
    std::string line;

    std::vector<std::pair<char, std::vector<std::string>>> boards;
    while (std::getline(iss, line)) {
      if (line.empty()) {
        continue;
      }
      if (line.size() == 1) {
        char name = line[0];
        std::vector<std::string> rows;
        while (std::getline(iss, line) && !line.empty()) {
          rows.push_back(line);
        }
        boards.emplace_back(name, rows);
      }
    }
    std::reverse(boards.begin(), boards.end());
    return boards;
  }

  static Board* create_board(std::string file_path) {
    auto boards = parse_boards(readFile(file_path));
    Board* board = nullptr;
    std::map<char, Board* > boards_map;
    for (auto board_rep : boards) {
      board = new Board(board_rep, Vector2{0, 0}, 600, boards_map);
    }
    return board;
  }

  static std::vector<Board*> create_boards(std::string file_path) {
    std::vector<Board*> boards;

    std::filesystem::path dir{file_path};
    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(dir)) {
      if (entry.is_regular_file()) {
        boards.push_back(create_board(entry.path().string()));
      }
    }
    return boards;
  }

};



#endif //GAMEDATAREADER_H
