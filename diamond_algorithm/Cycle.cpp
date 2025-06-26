#include "Cycle.hpp"
#include <algorithm>
#include <random>

Cycle::Cycle(Grid* grid, const int layer)
    : layer{layer}, grid{grid} {}
// uses initialisation list for efficiency
// layer and grid are intialised directly

// determines diamond path's coordinates within grid
std::vector<std::pair<int, int>> Cycle::getDiamondPath() const {
  std::vector<std::pair<int, int>> path;
  const int size = grid->getSize();
  const int center = size / 2;

  // starting point (middle of left column for this layer)
  const int startRow = center; // 3 for a 7x7
  const int startCol = layer; // 0 for outermost layer

  //Phase 1: Upward-right diagonal traversal.
  int row = startRow; // 3
  int col = startCol; // 0
  while (col <= center) { // continue to center column
    path.emplace_back(row, col); // track path
    --row; // move one row up
    ++col; // move one column right
  }
  // after loop: row = -1, col = 4 (reached diamond's top center)

  //Phase 2: Downward-right diagonal traversal.
  row += 2; // adjust row position to 1. adjust row for next diagonal
  // Now at position: row = 1, col = 4
  while (col < size - layer) { // continue to right edge
    path.emplace_back(row, col);
    ++row;
    ++col;
  }
  // after loop: row = 3, col = 6 (diamond's right center)

  // Phase 3: Down-left diagonal (from right-center edge to bottom-center)
  col -= 2; // move col left by 2 (from 6 to 4)
  // Now at position: row = 3, col = 4
  while (row < size - layer) { // go to bottom center
    path.emplace_back(row, col);
    ++row;
    --col;
  }
  // Aafter loop: row = 6, col = 1 (bottom center)

  // Phase 4: Up-left diagonal (from bottom-center back to starting point)
  row -= 2; // adjust row upward (from 6 to 4)
  // Now at position: row = 4, col = 1
  while (col > /* used to be >= */layer && row > startRow) { // traverse till starting point
    path.emplace_back(row, col);
    --row;
    --col;
  }
  return path;
}

void Cycle::fillWithMessage(const std::string& message, int& msgIndex) {
  auto path = getDiamondPath();
  fullDiamondLetters.clear();
  originalMessageLetters.clear();

  for (const auto& [row, col] : path) { // iterate through each coordinate
    char ch;
    bool isMessageChar = false;

    if (msgIndex < message.size()) { // check for remaining message chars
      ch = message[msgIndex++]; // get next char
      isMessageChar = true; // flag as message char
    } else {
      // fill with random letter if message is exhausted
      std::random_device dev;
      std::mt19937 rng(dev());
      std::uniform_int_distribution<std::mt19937::result_type> dist26(0, 25);
      ch = static_cast<char>('A' + dist26(rng));
    }

    grid->fillCell(row, col, ch); // places cchar in the grid
    fullDiamondLetters += ch; // append to full list

    if (isMessageChar) {
      originalMessageLetters += ch;  // append to original message letters
    }
  }
}
// fill remaining empty cells with random letters
void Cycle::fillEmptyCells() const {
  const int size = grid->getSize();

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist26(0, 25);

  for (int row = 0; row < size; ++row) {
    for (int col = 0; col < size; ++col) {
      if (grid->getCell(row, col) == ' ') { // check empty cells
        const char randomChar = static_cast<char>('A' + dist26(rng));
        grid->fillCell(row, col, randomChar);
      }
    }
  }
}

void Cycle::extractToMessage(std::string& message, int& msgIndex) {
  auto path = getDiamondPath();
  fullDiamondLetters.clear();
  originalMessageLetters.clear();

  for (const auto& [row, col] : path) {
    if (const char c = grid->getCell(row, col); c != ' ') { // checks for non empty cells
      message += c; // append char to emssage
      fullDiamondLetters += c;
      originalMessageLetters += c;
      ++msgIndex;
    }
  }
}
