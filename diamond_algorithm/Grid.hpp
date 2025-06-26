#ifndef GRID_HPP
#define GRID_HPP
#include <vector>
#include <string>
#include <iostream>

class Grid {

public:
  explicit Grid(int size);
  void display() const;
  void fillColumnByColumn(const std::string& encrypted);
  void fillCell(int row, int col, char ch);

  [[nodiscard]] std::string getEncryptedMessage() const;
  [[nodiscard]] char getCell(int row, int col) const;
  [[nodiscard]] int getSize() const;

private:
  int size;
  std::vector<std::vector<char>> cells;
};
#endif //GRID_HPP
