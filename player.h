#pragma once
#include <coroutine>
#include <exception>
#include <stdexcept>
#include <memory>

class Game;

class Player {
public:
    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;
    
    bool in_progress() const;
    bool move_next();
    
    Player(Player const&) = delete;
    Player& operator=(Player const&) = delete;
    Player(Player&& other) noexcept;
    ~Player();

    struct promise_type {
        auto initial_suspend() { return std::suspend_never{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void unhandled_exception() { std::terminate(); }
        void return_void() {}
        
        auto get_return_object() {
            return Player{handle::from_promise(*this)};
        }
    
        struct Awaiter {
            promise_type& p;
            
            bool await_ready() const { return false; }
            bool await_suspend(std::coroutine_handle<>) noexcept {
                return true;
            }
            void await_resume() noexcept {}
        };
        
        Awaiter await_transform(int) { return {*this}; }
    };
    
private:
    Player(handle h) : coro(h) {}
    handle coro;
};

Player CreatePlayer(std::shared_ptr<Game> game, int player_id); 