#ifndef INTERFACE_HPP
#define INTERFACE_HPP
#include <memory>
#include <stack>
#include <map>
#include "Menu.hpp"

class Interface {
public:
    Interface();
    void run();
    void processInput(int choice);

    // for Actions to modify menus/session data
    void setCurrentMenu(const std::shared_ptr<Menu> &menu);
    void navigateBack();
    void exit() { isRunning = false; }

    struct SessionData {
        std::string message;
        int gridSize = 0;
        int rounds = 1;
        bool autoGridSize = false;
    } sessionData;

private:
    std::shared_ptr<Menu> currentMenu;
    std::stack<std::shared_ptr<Menu>> menuHistory;
    std::map<std::string, std::shared_ptr<Menu>> menus;
    bool isRunning = true;
    void initialiseMenus();
};

#endif