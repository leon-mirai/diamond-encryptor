#include "Decryptor.hpp"
#include <cmath>
#include <iostream>
#include <windows.h>

Decryptor::Decryptor(const int rounds, const bool verbose)
    : rounds(rounds), verbose(verbose) {}
// 'rounds' is number of decryption rounds to perform
// verbose controls whether to display detailed output or not
std::string Decryptor::decryptSingleRound(const std::string& encrypted) const {
    const int gridSize = static_cast<int>(std::sqrt(encrypted.size()));
    // calculates grid size based on encrypted message length
    // assume encrypted message can form square grid
    if(verbose) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        std::cout << "\nGrid size: " << gridSize << "x" << gridSize
                  << " | Message length: " << encrypted.size() << "\n";
    }
    Grid grid(gridSize);
    grid.fillColumnByColumn(encrypted);
    // create grid object and fill ti with encrypted message, column by column
    if(verbose) {
        displayGridState(grid);
    }

    std::string message;
    int msgIndex = 0;
    const int layers = (gridSize + 1) / 2;
    // initialises empty message string, message index, and calculates number of layers
    for(int layer = 0; layer < layers; ++layer) {
        Cycle cycle(&grid, layer);
        auto path = cycle.getDiamondPath();
        if(verbose) {
            displayLayerExtraction(layer, path); // show extraction path for current layer
        }

        cycle.extractToMessage(message, msgIndex); // extracts message from grid using cycle path
    }
    if(verbose) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        std::cout << "\nExtracted message segment: " << message << "\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
    return message;
}

std::string Decryptor::decrypt(const std::string& encryptedMessage) const {
    std::string current = encryptedMessage;
    // initialises current with encrypted message. modified in each round
    for(int i = 0; i < rounds; ++i) {
        if(verbose) {
            displayDecryptionHeader(i+1, rounds);
            std::cout << "Processing: " << current << "\n"; // displays decryption round header and message being processed
        }
        current = decryptSingleRound(current); // decrypt message for single round
        if(verbose && i < rounds - 1) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            std::cout << "\nPreparing for next round...\n";
            std::cout << "Trimmed message: " << current << "\n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // display message indicating prep for next round and trim
        }
        if(i < rounds - 1) {
            current = prepareForNextRound(current);
        }
    }
    if(const size_t dot = current.find('.'); dot != std::string::npos) {     // trim at first period
        current = current.substr(0, dot + 1);
    }
    if(verbose) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        std::cout << "\n==================== FINAL RESULT ====================\n";
        std::cout << "Decrypted message: " << current << "\n";
        std::cout << "Message length: " << current.size() << "\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
    return current;
}

std::string Decryptor::prepareForNextRound(const std::string& message) {
    const int len = static_cast<int>(message.size());// gets length of message

    int raw = static_cast<int>(std::floor(std::sqrt(len))); // largest odd sqrt ≤ len
    if (raw % 2 == 0) --raw;
    raw = std::max(raw, 1);          // at least 1×1
    const int prevSquare = raw * raw;      // full-grid size
    return message.substr(0, prevSquare);
    // calculates largest odd sqrt less than or equal to message length
}

std::string Decryptor::decryptWithDisplay(const std::string& encryptedMessage) const {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    std::cout << "\n======================================\n";
    std::cout << "\n    STARTING DECRYPTION PROCESS       ";
    std::cout << "\n======================================\n";
    std::cout << "  Input length: " << encryptedMessage.size() << " characters\n";
    std::cout << "  Rounds configured: " << rounds << "\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    std::string current = encryptedMessage;

    for (int round = 1; round <= rounds; ++round) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        std::cout << "\n-----ROUND " << round << "/" << rounds << " -----\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

        current = decryptSingleRound(current);

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        std::cout << "After round " << round << ": "
                 << (current.size() > 40 ? current.substr(0, 40) + "..." : current)
                 << " (" << current.size() << " chars)\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

        if (round < rounds) {
            current = prepareForNextRound(current);
            std::cout << "  (Trimmed for next round)\n";
        }
    }
    displayFinalResult(current);
    return current;
}

void Decryptor::displayFinalResult(const std::string& result) {
    const size_t pos = result.find('.'); // position of dot

    std::string message;

    if (pos != std::string::npos) {
        message = result.substr(0, pos + 1);
    }
    else message = result;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    std::cout << "\n======================================\n";
    std::cout << "       DECRYPTION COMPLETE              ";
    std::cout << "\n======================================\n";
    std::cout << "  Final message: " << message << "\n";
    std::cout << "  Message length: " << message.size() << " characters\n";

    // Validate the message
    if (message.empty() || message.back() != '.') {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        std::cout << "  WARNING: Message may be incomplete (missing termination)\n";
    } else {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
        std::cout << "  Message properly terminated with '.'\n";
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void Decryptor::displayDecryptionHeader(const int pass, const int total) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    std::cout << "\n============================================================";
    std::cout << "\n       DECRYPTION PASS " << pass << "/" << total << "       ";
    std::cout << "\n============================================================\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void Decryptor::displayGridState(const Grid& grid) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    std::cout << "=== Reconstructed Grid ===" << std::endl;
    grid.display();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void Decryptor::displayLayerExtraction(const int layer, const std::vector<std::pair<int, int>>& path) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    std::cout << "\nLayer " << layer << " extraction path: ";
    for(const auto& [row, col] : path) {
        std::cout << "(" << row << "," << col << ") ";
    }
    std::cout << "\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}