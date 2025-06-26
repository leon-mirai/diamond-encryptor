#include "Menu.hpp"
#include "Interface.hpp"
#include <iostream>
#include <utility>
#include <windows.h>

Menu::Menu(std::string  title, const int level, const std::shared_ptr<Menu> &parent)
    : title(std::move(title)), parent(parent), level(level) {}

// add menu option
void Menu::addOption(const std::string& description, std::unique_ptr<Action> action) {
    options.emplace_back(description, std::move(action));
}

// display menu
void Menu::display() const {
    const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // title and separators
    SetConsoleTextAttribute(hConsole, 11); // bright cyan
    std::cout << "\n==============================\n";
    std::cout << title << "\n";
    std::cout << "==============================\n";

    // prompt
    SetConsoleTextAttribute(hConsole, 10); // light green
    std::cout << "Select an option:\n\n";

    // options
    SetConsoleTextAttribute(hConsole, 15); // bright white
    for (size_t i = 0; i < options.size(); i++)
        std::cout << "  " << i + 1 << ". " << options[i].first << "\n";


    // input prompt
    SetConsoleTextAttribute(hConsole, 14); // Light Yellow
    std::cout << "\n> ";

    // reset to default color (optional)
    SetConsoleTextAttribute(hConsole, 15);
}


// get parent menu
std::shared_ptr<Menu> Menu::getParent() const {
    return parent;
}

// execute action
void Menu::executeAction(const int choice, Interface& app) const {
    if (choice >= 0 && choice < static_cast<int>(options.size()))
        options[choice].second->execute(app);
    else std::cout << "Invalid choice!\n";

}
