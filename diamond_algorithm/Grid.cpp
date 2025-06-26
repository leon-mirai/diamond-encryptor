#include "Grid.hpp"
#include <iomanip>
#include <thread>
#include <chrono>
#include <windows.h>

Grid::Grid(int size)
    : size(size), cells(size, std::vector(size, ' ')) {} //2d vector

void Grid::fillCell(const int row, const int col, const char ch) {
  if (row >= 0 && row < size && col >= 0 && col < size) {
    cells[row][col] = ch;
    // display the grid after each cell is filled
     std::cout << "Filling cell (" << row << "," << col << ") with '" << ch << "'" << std::endl;
     display();
     std::cout << std::endl;
  }
}

char Grid::getCell(const int row, const int col) const {
  if (row >= 0 && row < size && col >= 0 && col < size)
    return cells[row][col];
  return ' ';
}

void Grid::display() const {
  // print column indices at the top
  std::cout << "  ";
  for (int i = 0; i < size; ++i) {
    std::cout << std::setw(2) << i;
  }
  std::cout << std::endl;

  std::cout << "  ";
  for (int i = 0; i < size; ++i) {
    std::cout << "--";
  }
  std::cout << std::endl;

  // print rows with row indices
  for (int i = 0; i < size; ++i) {
    std::cout << i << "| ";
    for (int j = 0; j < size; ++j) {
      std::cout << std::setw(1) << cells[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

std::string Grid::getEncryptedMessage() const {
  std::string encrypted;
  // read column by column
  for (int col = 0; col < size; ++col) {
    for (int row = 0; row < size; ++row) {
      encrypted += cells[row][col];
    }
  }
  return encrypted;
}

// for decryption. items need to be filled in column by column
void Grid::fillColumnByColumn(const std::string& encrypted) {
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
  std::cout << "Filling grid from encrypted message:\n" << encrypted << std::endl << "\n";
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
  int idx = 0;
  for (int col = 0; col < size; ++col) {
    for (int row = 0; row < size; ++row) {
      if (idx < encrypted.size()) {
        cells[row][col] = encrypted[idx++];
      } else {
        cells[row][col] = ' ';
      }
      // std::cout << "Filled (" << row << "," << col << ") with '"
      //          << cells[row][col] << "'" << std::endl;
    }
  }
  std::cout << "Final grid after reconstruction:" << std::endl;
  this->display(); // fill grid column by column with encrypted message
  // if encrypted message is shorter than gridsize, remaining cells are empty.
}

int Grid::getSize() const {
  return size;
}
