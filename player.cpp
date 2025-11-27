#include "player.h"
#include "game.h"
#include <iostream>


bool Player::in_progress() const {
    return coro && !coro.done();
}

bool Player::move_next() {
    if (!coro) {
        throw std::runtime_error("coroutine is destroyed");
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


Player CreatePlayer(std::shared_ptr<Game> game, int player_id) {
    while (true) {
        game->MakeMove(player_id);
        
        if (game->CheckWinner()) {
            std::cout << "\n Гравець " << player_id << " переміг!" << std::endl;
            co_return;
        }
        
        co_await 1;
    }
}