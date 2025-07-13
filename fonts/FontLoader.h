#ifndef FONTLOADER_H
#define FONTLOADER_H

#include <fstream>
#include <nlohmann/json.hpp>

class FontLoader {
public:
  std::map<char, std::vector<std::vector<bool>>> characters;

  void load(std::string filename) {
    std::ifstream file(filename);

    nlohmann::json j;
    file >> j;

    for (auto [key, value] : j.items()) {
      char character = key[0];
      std::vector<std::vector<bool>> grid;

      for (auto row : value) {
        std::vector<bool> grid_row;
        for (auto pixel : row) {
          grid_row.push_back(pixel.get<bool>());
        }
        grid.push_back(grid_row);
      }

      characters[character] = grid;
    }
  }

  int get_character_width(char c) {
    return characters[c].front().size();
  }
};



#endif //FONTLOADER_H
