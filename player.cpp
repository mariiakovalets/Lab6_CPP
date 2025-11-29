#include "player.h"
#include "game.h"
#include <iostream>

bool Player::in_progress() const {
    return coro && !coro.done();
}

bool Player::move_next() {
    if (!coro) {
        throw std::runtime_error("Corutine is destroyed");
    }
    coro.resume();
    return !coro.done();
}

Player::Player(Player&& other) noexcept : coro(other.coro) {
    other.coro = nullptr;
}

Player::~Player() {
    if (coro) {
        coro.destroy();
    }
}

Player create_player(std::shared_ptr<Game> game, int player_id) {
    while (true) {
        game->make_move(player_id);
        
        if (game->check_winner()) {
            std::cout << "\nPlayer " << player_id << " won!" << std::endl;
            co_return;
        }
        
        co_await 1;
    }
}