#include "Encryptor.hpp"
#include "Grid.hpp"
#include "Cycle.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <windows.h> // For SetConsoleTextAttribute

Encryptor::Encryptor(const int gridSize, const int rounds)
    : gridSize(gridSize), rounds(rounds) {}

std::string Encryptor::encrypt(std::string message) {
    message = prepareMessage(message);
    std::cout << "Prepared message: " << message << std::endl;

    std::string encrypted = message;
    for (int round = 0; round < rounds; ++round) {
        displayRoundHeader(round + 1, encrypted);
        encrypted = encryptCore(encrypted, true);
        displayEncryptionResult(encrypted);
    }
    return encrypted; // main encryption function that can handle multi rounds
}

std::string Encryptor::prepareMessage(const std::string& message) {
    std::string prepared;
    for (const char c : message) {
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '.') {
            prepared += c;
        }
    }
    std::ranges::transform(prepared, prepared.begin(), toupper);
    if (prepared.empty() || prepared.back() != '.') {
        prepared += '.';
    }
    return prepared;
}

int Encryptor::calculateGridSize(const std::string& message) {
    const int need = static_cast<int>(message.length()); // calculate length of input
    // need represents min num of cells required in grid to hold all chars of message
    int C = 0; // C is variable used to deriv grid formula
    // helps find the right layer of diamond pattern
    while (true) {
        if (const int capacity = 1 + 2 * C * (C + 1); capacity >= need) return 2 * C + 1;
        // capacity calculates how many cells a grid of specific size  can hold
        // formula shows how the diamonds fill up.
        // if grid can hold whole message, return
        // grid size is always odd so we return it as 2*c + 1 to get grid length.
        ++C; // if grid too small, we c inrease c to check next size
    }
}

std::string Encryptor::encryptCore(const std::string& message, const bool verbose) {
    const int size = gridSize <= 0 ? calculateGridSize(message) : gridSize;
    if (verbose) {
        usedGridSizes.push_back(size);
        std::cout << "Grid size used: " << size << std::endl;
    }

    Grid grid(size);
    int msgIndex = 0;
    const int layers = (size + 1) / 2;
    // make grid object with determiend layer
    std::string allDiamondLetters, allOriginalLetters;

    for (int layer = 0; layer < layers; ++layer) {
        Cycle cycle(&grid, layer);
        cycle.fillWithMessage(message, msgIndex);
        // create cycle objects and fill grid with message
        if (verbose) {
            allDiamondLetters += cycle.getFullDiamondLetters();
            allOriginalLetters += cycle.getOriginalMessageLetters();
        }
    }

    const Cycle finalCycle(&grid, 0);
    finalCycle.fillEmptyCells();

    if (verbose) {
        displayGridConstruction(grid, allOriginalLetters, allDiamondLetters);
    }

    return grid.getEncryptedMessage();
}

std::string Encryptor::encryptSingleRound(const std::string& message) {
    return encryptCore(message, true);
}

std::string Encryptor::encryptWithDisplay(std::string message) {
    message = prepareMessage(message);
    std::cout << "\n=== STARTING ENCRYPTION PROCESS ===" << std::endl;
    std::cout << "Initial message: " << message << std::endl << std::endl;

    std::string encrypted = message;
    for (int round = 0; round < rounds; ++round) {
        std::cout << "\n=== ROUND " << round + 1 << " ===" << std::endl;
        encrypted = encryptCore(encrypted, true);
        std::cout << "\nRound " << round + 1 << " complete!" << std::endl;
    }

    std::cout << "\n=== FINAL RESULT ===" << std::endl;
    // std::cout << "Full encrypted message: " << encrypted << std::endl;
    return encrypted;
}

std::string Encryptor::multiRoundEncryptWithDisplay(const std::string& message) {
    std::string current = prepareMessage(message);
    std::cout << "Starting multi-round encryption (" << rounds << " rounds)\n";
    std::cout << "Initial message: " << current << "\n";

    for (int round = 1; round <= rounds; ++round) {
        std::cout << "\n=== ROUND " << round << "/" << rounds << " ===\n";
        current = encryptSingleRound(current);
        std::cout << "Round " << round << " result: " << current << "\n";
    }

    std::cout << "\n=== FINAL RESULT ===\n"
              << "Total rounds: " << rounds << "\n"
              << "Final length: " << current.size() << " chars\n";

    return current;
}

void Encryptor::displayGridConstruction(const Grid& grid, const std::string& originalLetters, const std::string& allDiamondLetters) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    std::cout << "=== Original Message in Diamond Path ===\n"
              << originalLetters << "\n"
              << "Message Length: " << originalLetters.size() << "\n\n";

    std::cout << "=== Full Diamond Path Letters (Message + Random) ===\n"
              << allDiamondLetters << "\n"
              << "Total Length: " << allDiamondLetters.size() << "\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    std::cout << "\nFilled Grid:" << std::endl;
    grid.display();
}

void Encryptor::displayRoundHeader(const int round, const std::string& message) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    std::cout << "\nEncryption Round " << round << ":" << std::endl;
    std::cout << "Processing message: " << message << std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void Encryptor::displayEncryptionResult(const std::string& encrypted) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    std::cout << "Encrypted result:\n" << encrypted << std::endl
              << "Length: " << encrypted.size() << std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}