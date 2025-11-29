#include "game.h"
#include "random_generator.h"
#include <iostream>
#include <vector>

Game::Game() {
    number1_ = random_int(1, 10);
    number2_ = random_int(1, 10);
    
    initial_sum_ = number1_ + number2_;
    target_sum_ = initial_sum_ * target_multiplier;
    
    player_last_actions_[0] = std::nullopt;
    player_last_actions_[1] = std::nullopt;
    
    std::cout << "Initial numbers: [" << number1_ << ", " << number2_ << "]" << std::endl;
    std::cout << "Initial sum: " << initial_sum_ << std::endl;
    std::cout << "Target sum: " << target_sum_ << std::endl;
    std::cout << std::endl;
}

std::optional<int> Game::try_apply_action(int value, Action action) const {
    if (action == Action::divide_number_by3 && value % 3 != 0) {
        return std::nullopt;
    }
    
    switch (action) {
        case Action::increment_number:
            return value + 1;
        case Action::multiply_number_by2:
            return value * 2;
        case Action::divide_number_by3:
            return value / 3;
    }
    
    return std::nullopt;
}

Action Game::choose_action(int player_id) {
    std::vector<Action> available_actions;
    
    const auto& last_action = player_last_actions_[player_id - 1];
    
    for (Action action : {Action::increment_number, 
                          Action::multiply_number_by2, 
                          Action::divide_number_by3}) {
        
        bool is_new_action = !last_action.has_value() || 
                             action != last_action.value();
        
        if (action == Action::divide_number_by3) {
            bool can_divide = (number1_ % 3 == 0) || (number2_ % 3 == 0);
            if (is_new_action && can_divide) {
                available_actions.push_back(action);
            }
        } else {
            if (is_new_action) {
                available_actions.push_back(action);
            }
        }
    }
    
    int random_index = random_int(0, available_actions.size() - 1);
    return available_actions[random_index];
}

void Game::make_move(int player_id) {
    Action action = choose_action(player_id);
    
    bool apply_to_first = (random_int(0, 1) == 0);
    int current_value = apply_to_first ? number1_ : number2_;
    
    std::optional<int> result = try_apply_action(current_value, action);
    
    if (!result.has_value()) {
        apply_to_first = !apply_to_first;
        current_value = apply_to_first ? number1_ : number2_;
        result = try_apply_action(current_value, action);
    }
    
    if (result.has_value()) {
        if (apply_to_first) {
            number1_ = result.value();
        } else {
            number2_ = result.value();
        }
        
        player_last_actions_[player_id - 1] = action;
        
        std::cout << "Player" << player_id << ": " 
                  << action_to_string(action, apply_to_first) << std::endl;
        std::cout << "   Current numbers: [" << number1_ << ", " << number2_ << "]\n";
        std::cout << "   Sum: " << get_sum() << std::endl;
    }
}

bool Game::check_winner() const {
    return get_sum() >= target_sum_;
}

std::string Game::action_to_string(Action action, bool is_first_number) const {
    std::string num = is_first_number ? "the first" : "the second";
    
    switch (action) {
        case Action::increment_number:
            return "Added 1 to " + num;
        case Action::multiply_number_by2:
            return "Multiplied " + num + " by 2";
        case Action::divide_number_by3:
            return "Divided " + num + " by 3";
    }
    return "Unknown action";
}