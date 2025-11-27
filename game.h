#pragma once
#include <array>
#include <string>
#include <optional>
#include <random>

enum class Action {
    IncrementNumber = 1,
    MultiplyNumberBy2 = 2,
    DivideNumberBy3 = 3
};

class Game {
public:
    Game();
    
    void MakeMove(int player_id);
    bool CheckWinner() const;
    
    int GetNumber1() const { return number1_; }
    int GetNumber2() const { return number2_; }
    int GetSum() const { return number1_ + number2_; }
    int GetTarget() const { return target_sum_; }
    int GetInitialSum() const { return initial_sum_; }
    
private:
    int number1_;
    int number2_;
    int initial_sum_;
    int target_sum_;
    std::array<std::optional<Action>, 2> player_last_actions_;

    std::mt19937 gen_;
    
    Action ChooseRandomAction(int player_id);
    std::optional<int> TryApplyAction(int value, Action action) const;
    std::string ActionToString(Action action, bool is_first_number) const;
};