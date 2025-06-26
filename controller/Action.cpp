#include "Action.hpp"
#include "Interface.hpp"
#include "Menu.hpp"
#include <iostream>
#include <limits>
#include "../diamond_algorithm/Encryptor.hpp"
#include "../diamond_algorithm/Decryptor.hpp"
#include <utility>

// navigation Actions
NavigateAction::NavigateAction(std::shared_ptr<Menu> target) : targetMenu(std::move(target)) {}

void NavigateAction::execute(Interface& app) {
    app.setCurrentMenu(targetMenu);
}

void BackAction::execute(Interface& app) {
    app.navigateBack();
}

// core Functionality Actions
void QuitAction::execute(Interface& app) {
    app.exit();
}

void EnterMessageAction::execute(Interface& app) {
    while (true) {
        std::cout << "Enter message (only alphabetic letters allowed): ";

        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) {
            std::cout << "Input cannot be empty. Please enter a message\n";
            continue;
        }

        bool valid = true;
        for (const char c : input) {
            if (!std::isalpha(static_cast<unsigned char>(c)) && c != ' ' && c != '.') {
                valid = false;
                break;
            }
        }

        if (valid && !app.sessionData.message.empty()) {
            break; // message is okay
        }

        if (valid) {
            app.sessionData.message = input;
            break;
        }
        std::cout << "Invalid message. Only alphabetic characters, spaces, and periods are allowed.\n";

    }
}

SetGridSizeAction::SetGridSizeAction(const bool autoSize) : automatic(autoSize) {}

void SetGridSizeAction::execute(Interface& app) {
    if (automatic) {
        app.sessionData.autoGridSize = true;
        std::cout << "Grid size will be calculated automatically\n";
    } else {
        int size;
        const std::string preparedMessage = Encryptor::prepareMessage(app.sessionData.message);
        const int minGridSize = Encryptor::calculateGridSize(preparedMessage);

        while (true) {
            std::cout << "Enter grid size (odd number, at least " << minGridSize << "): ";

            std::string input;
            std::getline(std::cin, input);

            if (input.empty()) {
                std::cout << "Input cannot be empty. Please enter a message\n";
                continue;
            }

            try {
                size = std::stoi(input);  // convert string to integer
            } catch (const std::invalid_argument& e) {
                std::cout << "Invalid input. Please enter a valid number.\n";
                continue;
            }

            if (size % 2 == 1 && size >= minGridSize) { // must be odd and big enough
                app.sessionData.gridSize = size;
                break;
            }
            std::cout << "Invalid grid size. Must be an odd number >= " << minGridSize << ".\n";
        }
    }
}


void SetRoundsAction::execute(Interface& app) {
    int rounds;
    while (true) {
        std::cout << "Enter number of rounds: ";

        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) {
            std::cout << "Input cannot be empty. Please enter a number\n";
            continue;
        }

        try {
            rounds = std::stoi(input);  // convert string to integer
        } catch (const std::invalid_argument& e) {
            std::cout << "Invalid input. Please enter a valid number.\n";
            continue;
        }

        if (rounds > 0) {
            app.sessionData.rounds = rounds;
            break;
        }
        std::cout << "Please enter a positive number.\n";
    }
}

void OneRoundPrintAction::execute(Interface& app) {
    if (app.sessionData.message.empty()) {
        std::cout << "No message to encrypt!\n";
        return;
    }
    Encryptor encryptor(app.sessionData.gridSize, 1);
    std::cout << "\n=== One-Round Encryption Grid ===\n";
    const std::string result = encryptor.encryptWithDisplay(app.sessionData.message);
    std::cout << "Final encrypted message: " << result << "\n";
}

void MultiRoundPrintAction::execute(Interface& app) {
    if (app.sessionData.message.empty()) {
        std::cout << "No message to encrypt!\n";
        return;
    }

    Encryptor encryptor(app.sessionData.gridSize, app.sessionData.rounds);
    std::cout << "\n=== Multi-Round Encryption Steps ===\n";
    const std::string result = encryptor.multiRoundEncryptWithDisplay(app.sessionData.message);
    std::cout << "Final encrypted message: " << result << "\n";
}

void DecryptionPrintAction::execute(Interface& app) {
    if (app.sessionData.message.empty()) {
        std::cout << "No message to decrypt!\n";
        return;
    }

    const Decryptor decryptor(app.sessionData.rounds);
    std::cout << "\n=== Decryption Steps ===\n";
    const std::string result = decryptor.decryptWithDisplay(app.sessionData.message);
    std::cout << "Decrypted message before trimming: " << result << "\n";
}

ExecuteEncryptionAction::ExecuteEncryptionAction(const bool multi) : multiRound(multi) {}

void ExecuteEncryptionAction::execute(Interface& app) {
    // check for empty message first
    if (app.sessionData.message.empty()) {
        std::cout << "Please enter a message first!\n";

        // prompt for immediate message input
        std::cout << "Enter message now: ";
        std::cin.ignore();
        std::getline(std::cin, app.sessionData.message);

        if (app.sessionData.message.empty()) {
            std::cout << "No message entered. Aborting.\n";
            return;
        }
    }

    try {
        const int gridSize = app.sessionData.autoGridSize ? 0 : app.sessionData.gridSize;
        Encryptor encryptor(gridSize, multiRound ? app.sessionData.rounds : 1);

        // use the appropriate encryption method
        if (multiRound) app.sessionData.message = encryptor.multiRoundEncryptWithDisplay(app.sessionData.message);
        else app.sessionData.message = encryptor.encryptWithDisplay(app.sessionData.message);


    } catch (const std::exception& e) {
        std::cerr << "Encryption failed: " << e.what() << "\n";
    }
}

void ExecuteDecryptionAction::execute(Interface& app) {
    if (app.sessionData.message.empty()) {
        std::cout << "Error: No message to decrypt!\n";
        return;
    }
    const Decryptor decryptor(app.sessionData.rounds, true);
    app.sessionData.message = decryptor.decryptWithDisplay(app.sessionData.message);
}