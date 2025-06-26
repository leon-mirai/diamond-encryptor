#include "Interface.hpp"
#include <iostream>
#include <memory>
#include <thread>
#include <sstream>

Interface::Interface() {
    initialiseMenus();
}

void Interface::processInput(const int choice) {
    try {
        if (choice == 0 && currentMenu->getParent()) {
            navigateBack();
        } else {
            currentMenu->executeAction(choice - 1, *this); // Adjust for 0-based index
        }
    } catch (const std::out_of_range&) {
        std::cout << "Invalid choice. Try again.\n";
    }
}

void Interface::run() {
    while (isRunning) {
        currentMenu->display();
        std::cout << "Enter choice: ";

        std::string input;
        std::getline(std::cin, input);

        try {
            if (input.empty()) {
                throw std::invalid_argument("Input cannot be empty.");
            }

            std::istringstream iss(input);
            int choice;
            if (!(iss >> choice)) {
                throw std::invalid_argument("Input must be a valid number.");
            }

            if (choice == 0 && currentMenu->getParent()) {
                navigateBack();
            } else {
                try {
                    currentMenu->executeAction(choice - 1, *this);
                } catch (const std::out_of_range&) {
                    std::cout << "\nInvalid choice. Please try again.\n";
                }
            }
        } catch (const std::invalid_argument& e) {
            std::cout << "\n" << e.what() << "\n";
        }
    }
}


void Interface::setCurrentMenu(const std::shared_ptr<Menu> &menu) {
    if (currentMenu) menuHistory.push(currentMenu);
    currentMenu = menu;
}

void Interface::navigateBack() {
    if (!menuHistory.empty()) {
        currentMenu = menuHistory.top();
        menuHistory.pop();
    }
}

void Interface::initialiseMenus() {
    // Level 1: Main Menu
    auto mainMenu = std::make_shared<Menu>("Menu - Level 1", 1);

    // Level 2 Menus
    auto encryptMenu = std::make_shared<Menu>("Menu - Level 2: Encryption", 2, mainMenu);
    auto decryptMenu = std::make_shared<Menu>("Menu - Level 2: Decryption", 2, mainMenu);

    // Level 1 Options
    mainMenu->addOption("Encrypt a message", std::make_unique<NavigateAction>(encryptMenu));
    mainMenu->addOption("Decrypt a message", std::make_unique<NavigateAction>(decryptMenu));
    mainMenu->addOption("Quit", std::make_unique<QuitAction>());

    // Level 2: Encryption Options
    auto oneRoundMenu = std::make_shared<Menu>("Menu - Level 3: Encryption", 3, encryptMenu);
    auto multiRoundMenu = std::make_shared<Menu>("Menu - Level 3: Encryption", 3, encryptMenu);

    encryptMenu->addOption("Enter a message", std::make_unique<EnterMessageAction>());
    encryptMenu->addOption("One-round encryption", std::make_unique<NavigateAction>(oneRoundMenu));
    encryptMenu->addOption("Automatic multi-round encryption", std::make_unique<NavigateAction>(multiRoundMenu));
    encryptMenu->addOption("Back", std::make_unique<BackAction>());

    // Level 3: One-Round Options
    oneRoundMenu->addOption("Enter grid size", std::make_unique<SetGridSizeAction>(false));
    oneRoundMenu->addOption("Automatic grid size", std::make_unique<SetGridSizeAction>(true));
    oneRoundMenu->addOption("Print the grid and the encoded message", std::make_unique<OneRoundPrintAction>());
    oneRoundMenu->addOption("Back", std::make_unique<BackAction>());

    // Level 3: Multi-Round Options
    multiRoundMenu->addOption("Enter the round number", std::make_unique<SetRoundsAction>());
    multiRoundMenu->addOption("For each round, print the grid and the corresponding encoded message", std::make_unique<MultiRoundPrintAction>());
    multiRoundMenu->addOption("Back", std::make_unique<BackAction>());

    // Level 2: Decryption Options
    decryptMenu->addOption("Enter a message", std::make_unique<EnterMessageAction>());
    decryptMenu->addOption("Enter the round number", std::make_unique<SetRoundsAction>());
    decryptMenu->addOption("For each round, print the grid and the corresponding decoded message",
                         std::make_unique<DecryptionPrintAction>());
    decryptMenu->addOption("Back", std::make_unique<BackAction>());

    // Register menus
    menus["main"] = mainMenu;
    menus["encrypt"] = encryptMenu;
    menus["decrypt"] = decryptMenu;
    menus["one_round"] = oneRoundMenu;
    menus["multi_round"] = multiRoundMenu;

    currentMenu = mainMenu;
}