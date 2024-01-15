#ifndef HUMAN_VALID_H
#define HUMAN_VALID_H

#include <iostream>
#include <cstdlib>
#include <system_error>
#include "GameState.h"

using namespace std;

bool inputValid(int x, int y, int s){
    if(x < s && y < s){
        return true;
    }
    return false;
}

// Function to play the game
void playGame(GameState& game, Vec (*getPlayerMove)(GameState&)) {
    while (!game.done) {
        clearScreen(); // to keep terminal clean of extra boards
        cout << game;
        
        // Get player move
        Vec move = getPlayerMove(game);
        
        // Validate and make the move
        while (!game.play(move.x, move.y)) {
            cout << "Invalid move. Try again." << endl;
            move = getPlayerMove(game);
        }
    }

    clearScreen();
    std::cout << game;
    // lines below: 0 determines winner X, 1 = O, and any other case remaining = draw
    if (game.hasWon(0)) {
        std::cout << "Player X wins!\n";
    } 
    else if (game.hasWon(1)) {
        std::cout << "Player O wins!\n";
    } 
    else {
        std::cout << "It's a draw!\n";
    }
    
}

// Function to play between Player and Computer Move
void playGame(GameState& game, Vec (*getPlayerMove)(GameState&), Vec (*getComputerMove)(GameState&)) {
    while (!game.done) {
        clearScreen();
        cout << game;
        
        // Get player move
        Vec move = getPlayerMove(game);
        
        // Validate and make the move
        while (!game.play(move.x, move.y)) {
            cout << "Invalid move. Try again." << endl;
            move = getPlayerMove(game);
        }

        // Check if the game is done after the player's move
        if (game.done) {
            break;
        }

        // Get computer move
        Vec computerMove = getComputerMove(game);
        
        // Validate and make the computer's move
        while (!game.play(computerMove.x, computerMove.y)) {
            computerMove = getComputerMove(game);
        }
    }

    std::cout << game;

    if (game.hasWon(0)) {
        std::cout << "Player X wins!\n";
    } 
    else if (game.hasWon(1)) {
        std::cout << "Player O wins!\n";
    } 
    else {
        std::cout << "It's a draw!\n";
    }
}

// Function for a human player move
Vec humanMove(GameState& game) {
    int row, col;
    
    bool inputIsValid = false;
    while(!inputIsValid){
        std::cout << "Enter your move (row and column): ";
        std::cin >> row >> col;
        inputIsValid = inputValid(row, col, game.size);
        if(!inputIsValid){
            cout << "Cannot enter a coordinate which is not on the board" << endl;
        }
    }

    return Vec(row, col); // connected to GameState file, where upon choosing x and y, grid will place on board.
}

// Function for a simple computer player move
Vec validMove(GameState& game) {
    // Randomly choose a valid move
    int row, col;
    
    do {
        row = rand() % game.size; // valid move will access rand() and because random can't be bigger than the size of the grid,
        col = rand() % game.size; // it will produce a certain depth of field 0 1 2, and if grid is 4x4, 0 1 2 3
    } 
    while (game.grid[row][col] != -1); // shortens while loop in lines referring back to do statement. 

    return Vec(row, col);
}

#endif