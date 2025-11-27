#include "game.h"
#include <iostream>
#include <vector>


Game::Game() : gen_(std::random_device{}()) {
    std::uniform_int_distribution<int> d(1, 10);
    number1_ = d(gen_);
    number2_ = d(gen_);
    
    initial_sum_ = number1_ + number2_;
    target_sum_ = initial_sum_ * 10;
    
    player_last_actions_[0] = std::nullopt;
    player_last_actions_[1] = std::nullopt;
    
    std::cout << "Початкові числа: " << number1_ << ", " 
              << number2_ << std::endl;
    std::cout << "Початкова сума: " << initial_sum_ << std::endl;
    std::cout << "Мета: " << target_sum_ << std::endl;
    std::cout << std::endl;
}

std::optional<int> Game::TryApplyAction(int value, Action action) const {
    if (action == Action::DivideNumberBy3 && value % 3 != 0) {
        return std::nullopt;
    }
    
    switch (action) {
        case Action::IncrementNumber:
            return value + 1;
        case Action::MultiplyNumberBy2:
            return value * 2;
        case Action::DivideNumberBy3:
            return value / 3;
    }
    
    return std::nullopt;
}

Action Game::ChooseRandomAction(int player_id) {
    constexpr std::array action_candidates{
        Action::IncrementNumber,
        Action::MultiplyNumberBy2,
        Action::DivideNumberBy3
    };
    
    std::vector<double> action_weights;
    action_weights.reserve(action_candidates.size());
    
    for (const Action action : action_candidates) {
        const bool action_is_new = !player_last_actions_[player_id - 1].has_value() ||
                                   action != player_last_actions_[player_id - 1].value();
        
        if (action_is_new) {
            action_weights.push_back(1.0);
        } else {
            action_weights.push_back(0.0);
        }
    }

    std::discrete_distribution<std::size_t> dist(action_weights.begin(), 
                                                 action_weights.end());
    
    return action_candidates[dist(gen_)];
}


void Game::MakeMove(int player_id) {
    std::uniform_int_distribution<int> num_dist(0, 1);
    
    std::optional<int> result;
    Action action;
    bool apply_to_first;
    
    int attempts = 0;
    while (!result.has_value() && attempts < 10) {
        action = ChooseRandomAction(player_id);
        apply_to_first = (num_dist(gen_) == 0);
        
        int current_value = apply_to_first ? number1_ : number2_;
        result = TryApplyAction(current_value, action);
    
        if (!result.has_value()) {
            apply_to_first = !apply_to_first;
            current_value = apply_to_first ? number1_ : number2_;
            result = TryApplyAction(current_value, action);
        }
        
        ++attempts;
    }
    
    if (apply_to_first) {
        number1_ = result.value();
    } else {
        number2_ = result.value();
    }
    
    player_last_actions_[player_id - 1] = action;
    
    std::cout << "Гравець " << player_id  << " " 
              << ActionToString(action, apply_to_first) << std::endl;
    std::cout << "   Числа: [" << number1_ << ", " << number2_  << "]"
              << ", сума: " << GetSum() << std::endl;
}

bool Game::CheckWinner() const {
    return GetSum() >= target_sum_;
}


std::string Game::ActionToString(Action action, bool is_first_number) const {
    std::string num = is_first_number ? "перше" : "друге";
    
    switch (action) {
        case Action::IncrementNumber:
            return "Додав 1 до " + num + " числа";
        case Action::MultiplyNumberBy2:
            return "Помножив " + num + " число на 2";
        case Action::DivideNumberBy3:
            return "Поділив " + num + " число на 3";
    }
    
    return "Невідома дія";
}