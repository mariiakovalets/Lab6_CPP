#pragma once
#include <array>
#include <string>
#include <optional>

enum class Action {
    increment_number = 0,    
    multiply_number_by2 = 1,  
    divide_number_by3 = 2    
};

class Game {
public:
    Game();
    void make_move(int player_id);
    bool check_winner() const;
    
    int get_number1() const { return number1_; }
    int get_number2() const { return number2_; }
    int get_sum() const { return number1_ + number2_; }
    int get_target() const { return target_sum_; }
    int get_initial_sum() const { return initial_sum_; }
    
private:
    static constexpr int target_multiplier = 10;
    
    int number1_;
    int number2_;
    int initial_sum_;
    int target_sum_;

    std::array<std::optional<Action>, 2> player_last_actions_;
    
    Action choose_action(int player_id);
    std::optional<int> try_apply_action(int value, Action action) const;
    std::string action_to_string(Action action, bool is_first_number) const;
};