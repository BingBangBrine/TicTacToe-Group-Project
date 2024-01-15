#include <iostream>
#include <cstdlib>
#include <system_error>
#include "GameState.h"
#include "Unbeatable.h"
#include "Settings.h"
#include "HumanValid.h"

using namespace std;


// Function to display the menu and get user input
int displayMenu() {
    int choice;
    std::cout << "TIC-TAC-TOE" << endl;
    std::cout << "1. Play against computer" << endl;
    std::cout << "2. Play against human" << endl;
    std::cout << "3. Settings" << endl;
    std::cout << "4. Exit" << endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    clearScreen();
    return choice;
}

int main() {
    int choice;

    do {
        GameState game; // game initialized every single loop, if outside the loop, will only execute grid once.
        choice = displayMenu(); // display only outputs at the start before switch command

        switch (choice) {
            case 1:
                if(getDifficulty()){
                    playHardGame(game, humanMove1, findBestMove);
                    break;
                }
                else{
                    playGame(game, humanMove, validMove);
                    break;
                }
            case 2:
                playGame(game, humanMove);
                break;
            case 3:
                settings();
                break;
            case 4:
                std::cout << "Exiting the game.\n";
                break;
            default:
                std::cerr << "Invalid choice. Please try again.\n";
                break;
        }

    } while (choice != 4);
    cin.ignore();
    GameState game;
    
    // playHardGame(game, humanMove1, validMove1);
    return 0;
}
