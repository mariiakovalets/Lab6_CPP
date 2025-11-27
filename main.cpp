 #include "player.h"
#include "game.h"
#include <iostream>
#include <memory>

int main() {
    
    try {
        auto game = std::make_shared<Game>();
        
        Player player1 = CreatePlayer(game, 1);
        Player player2 = CreatePlayer(game, 2);
        
        while (player1.in_progress() && player2.in_progress()) {
            player1.move_next();
            if (!player1.in_progress()) break;  
            
            player2.move_next();
        }
    
        std::cout << "\nФінальні числа: " << game->GetNumber1() 
                  << ", " << game->GetNumber2() << std::endl;
        std::cout << "Фінальна сума: " << game->GetSum() 
     /*              << " (мета була: " << game->GetTarget() << ")"*/ << std::endl; 
        std::cout << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Помилка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 