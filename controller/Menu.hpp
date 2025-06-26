#ifndef MENU_HPP
#define MENU_HPP

#include <vector>
#include <string>
#include <memory>
#include "Action.hpp"

class Interface;

class Menu {
public:
    Menu(std::string  title, int level, const std::shared_ptr<Menu> &parent = nullptr);
    void addOption(const std::string& description, std::unique_ptr<Action> action);  // add option to menu
    void display() const; // display the menu (title + options)
    void executeAction(int choice, Interface& app) const; // execute action for a selected option

    [[nodiscard]] std::shared_ptr<Menu> getParent() const; // get parent menu (for "Back" navigation)
    [[nodiscard]] int getLevel() const { return level; }

private:
    std::string title;
    int level;
    std::vector<std::pair<std::string, std::unique_ptr<Action>>> options;
    std::shared_ptr<Menu> parent;
};

#endif