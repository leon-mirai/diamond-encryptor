/*
 Cycle class is designed to determine the movement in diamond pattern
 It is used for both message insertion and extraction from grid.
 */

#ifndef CYCLE_HPP
#define CYCLE_HPP
#include "Grid.hpp" // needed for grid operations
#include <string> // for handling the text messagees
#include <vector> // stores diamond path coordinates
#include <utility> // using pairs (row, col)

class Cycle {
public:
  Cycle(Grid* grid, int layer); // constructor that sets up Cycle object
    // grid is a pointer to the Grid I am working on
    // layer tells us which "diamond" i am in. (0- is the outermost layer)
  void fillWithMessage(const std::string& message, int& msgIndex);
    // this function inserts messages into grid,following diamond path
    // message is the text to be inserted. msgIdx is a reference that tracks current position in message
  void fillEmptyCells() const;
    // populates empty grid cells with random letters, it masks the message content
  void extractToMessage(std::string& message, int& msgIndex);
    // retrieves message from grid, following diamond path
    // message is where the extracted text is stored
    // msgIndex tracks progress of message extraction
  [[nodiscard]] std::vector<std::pair<int, int>> getDiamondPath() const;
  // and getter for calculates coordinates of diamond path
  // return vector of (row, col) pairs
  [[nodiscard]] const std::string& getFullDiamondLetters() const { return fullDiamondLetters; }
  // returns all letters along the diamond path (message + random)
  [[nodiscard]] const std::string& getOriginalMessageLetters() const { return originalMessageLetters; }
  // returns only letters from original message
private:
  int layer; // indicates current diamond layer
  Grid* grid; // ppointer to grid
  std::string fullDiamondLetters; // debug: stores all letters along diamond path
  std::string originalMessageLetters; // stores only original message letters
};



#endif //CYCLE_HPP
