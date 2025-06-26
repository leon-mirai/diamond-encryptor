#ifndef ENCRYPTOR_HPP
#define ENCRYPTOR_HPP

#include <string>
#include <vector>

class Grid;
class Cycle;

class Encryptor {
public:
    Encryptor(int gridSize, int rounds); // gridsize: the size of the grid to use for encryption.
    // rounds:   the number of encryption rounds to perform.

    // core functionality
    std::string encrypt(std::string message);
    std::string encryptSingleRound(const std::string& message);
    std::string encryptWithDisplay(std::string message); // displays step by step process
    std::string multiRoundEncryptWithDisplay(const std::string& message); // displays multi round step by step

    // helper methods
    static std::string prepareMessage(const std::string& message); // cleans encryption
    static int calculateGridSize(const std::string& message);
    std::string encryptCore(const std::string& message, bool verbose); // core encryption logic with verbose for more detail

    // display methods
    static void displayGridConstruction(const Grid& grid,const std::string& originalLetters, const std::string& allDiamondLetters); // grid construction details
    static void displayRoundHeader(int round, const std::string& message);
    static void displayEncryptionResult(const std::string& encrypted);

private:
    int gridSize;
    int rounds;
    std::vector<int> usedGridSizes;
};
#endif
