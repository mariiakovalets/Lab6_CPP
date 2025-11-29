#include "player.h"
#include "game.h"
#include <iostream>
#include <memory>

int main() {
    try {
        auto game = std::make_shared<Game>();
        
        Player player1 = create_player(game, 1);
        Player player2 = create_player(game, 2);

        while (player1.in_progress() && player2.in_progress()) {
            player1.move_next();
            if (!player1.in_progress()) break;

            player2.move_next();
        }
        
        std::cout << "\nFinal numbers: [" << game->get_number1() 
                  << ", " << game->get_number2() << "]" << std::endl;
        std::cout << "Final sum: " << game->get_sum() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}