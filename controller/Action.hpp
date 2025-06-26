#ifndef ACTION_HPP
#define ACTION_HPP
#include <memory>

class Interface;
class Menu;

class Action {
public:
    virtual ~Action() = default;
    virtual void execute(Interface& app) = 0;
};

// navigation Actions
class NavigateAction final : public Action {
    std::shared_ptr<Menu> targetMenu;
public:
    explicit NavigateAction(std::shared_ptr<Menu> target);
    void execute(Interface& app) override;
};

class BackAction final : public Action {
public:
    void execute(Interface& app) override;
};

// core functionality Actions
class QuitAction final : public Action {
public:
    void execute(Interface& app) override;
};

class EnterMessageAction final : public Action {
public:
    void execute(Interface& app) override;
};

class SetGridSizeAction final : public Action {
    bool automatic;
public:
    explicit SetGridSizeAction(bool autoSize);
    void execute(Interface& app) override;
};

class SetRoundsAction final : public Action {
public:
    void execute(Interface& app) override;
};

class OneRoundPrintAction final : public Action {
public:
    void execute(Interface& app) override;
};

class MultiRoundPrintAction final : public Action {
public:
    void execute(Interface& app) override;
};

class DecryptionPrintAction final : public Action {
public:
    void execute(Interface& app) override;
};


class ExecuteEncryptionAction final : public Action {
    bool multiRound;
public:
    explicit ExecuteEncryptionAction(bool multi);
    void execute(Interface& app) override;
};

class ExecuteDecryptionAction final : public Action {
public:
    void execute(Interface& app) override;
};

#endif