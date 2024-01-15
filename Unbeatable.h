#ifndef UNBEATABLE_H
#define UNBEATABLE_H

#include <iostream>
#include <cstdlib>
#include <system_error>
#include "GameState.h"
#include "HumanValid.h"
#include "Vertex.h"
// #include "Settings.h"
#include "Graph.h"

using namespace std;

// int evaluateGameState(Vertex<GameState>* start, int player) { // Kyrilov Code
//     cin.ignore();
//     if (start->neighbors.size() == 0){
//         int currPlayer = start->data.currentTurn;
//         if (start->data.hasWon(player)){
//             return 100;
//         }
//         else if (start->data.hasWon(!player)){
//             return -100;
//         }
//         else{
//             return 50;
//         }
//     }
//     else{
//         int reward = evaluateGameState(start->neighbors[0]->location, player);
//         for (int i = 1; i < start->neighbors.size(); i++){
//             int curr = evaluateGameState(start->neighbors[i]->location, player);
//             if (start->data.currentTurn == player){
//                 if (curr > reward){
//                     reward = curr;
//                 }
//             }
//             else{
//                 if (curr < reward){
//                     reward = curr;
//                 }
//             }
//         }
//         return reward;
//     }
// } // test

int evaluateGameState(GameState& game) {
    if (game.hasWon(0)) {
        return 100; // Player X has won
    } else if (game.hasWon(1)) {
        return -100;  // Player O has won
    } else {
        return 0;  // Game is Draw
    }
}

int minimax(GameState& board, int depth, bool maximizingPlayer) {
    int score = evaluateGameState(board);

    if (depth == 0)
        return score;

    // If the game is over, return the score
    if (score != 0) return score;

    // If the game is done and nobody won, it's a draw
    if (board.done && !board.hasWon(0) && !board.hasWon(1)) return 0;

    // If it's the maximizing player's turn
    if (maximizingPlayer) {
        int maxEval = -100;
        for (int i = 0; i < board.size; ++i) {
            for (int j = 0; j < board.size; ++j) {
                if (board.grid[i][j] == -1) {
                    board.play(i,j);
                    int eval = minimax(board, depth - 1, false);
                    // cout << eval << "row " << i << " col " << j << endl;
                    board.turnCount--;
                    board.grid[i][j] = -1; // Undo the move
                    maxEval = std::max(maxEval, eval);
                }
            }
        }
        return maxEval;
    } else { // If it's the minimizing player's turn
        int minEval = 100;
        for (int i = 0; i < board.size; ++i) {
            for (int j = 0; j < board.size; ++j) {
                if (board.grid[i][j] == -1) {
                    board.play(i,j);
                    int eval = minimax(board, depth - 1, true);
                    board.turnCount--;
                    board.grid[i][j] = -1; // Undo the move
                    minEval = std::min(minEval, eval);
                }
            }
        }
        return minEval;
    }
}

// Recursive function for minimax algorithm because depth of 5 required
// int minimax(GameState& game, int depth, bool maxPlayers) {
//     // Vertex<GameState>* whatgoeshere = new Vertex<GameState>(game);
//     // int score = evaluateGameState(whatgoeshere, 0); // modify this section
//     int score = evaluateGameState(game);
//     // cin.ignore();
//     // cout << "Reached " << depth << " " << score << endl;

//     if (score != 0) { // base case
//         return score;
//     }

//     if (depth == 0) { // base case
//         return 0;
//     }

//     if (maxPlayers) { // highest technically can be set to any -/+ large value, 
//         int highest = -1000;

//         for (int row = 0; row < game.size; row++) {
//             for (int col = 0; col < game.size; col++) {
//                 if (game.grid[row][col] == -1) {
//                     game.play(row, col);
//                     highest = max(highest, minimax(game, depth - 1, false));
//                     game.grid[row][col] = -1; // Undos the move
//                 }
//             }
//         }
//         return highest;
//     } 
//     else {
//         int highest = 1000;

//         for (int row = 0; row < game.size; row++) {
//             for (int col = 0; col < game.size; col++) {
//                 if (game.grid[row][col] == -1) {
//                     game.play(row, col);
//                     highest = min(highest, minimax(game, depth - 1, true));
//                     game.grid[row][col] = -1;
//                 }
//             }
//         }
//         return highest;
//     }
// }

int getReward(Vertex<GameState>* start, int player){
    if (start->neighbors.size() == 0){
        int currPlayer = start->data.currentTurn;
        if (start->data.hasWon(player)){
            return 100;
        }
        else if (start->data.hasWon(!player)){
            return -100;
        }
        else{
            return 0;
        }
    }
    else{
        int reward = getReward(start->neighbors[0]->location, player);
        for (int i = 1; i < start->neighbors.size(); i++){
            int curr = getReward(start->neighbors[i]->location, player);
            if (start->data.currentTurn == player){
                if (curr > reward){
                    reward = curr;
                }
            }
            else{
                if (curr < reward){
                    reward = curr;
                }
            }
        }
        return reward;
    }
}

Vec findBestMove(GameState game){
    if(game.grid[1][1] == -1){
        return Vec(1, 1);
    }

    bool otherPlayer = !(game.currentTurn);

    if(game.grid[1][1] == otherPlayer && game.grid[2][2] == -1){
        return Vec(2, 2);
    }

    Graph<GameState> stateSpace;
    Vertex<GameState>* start = new Vertex<GameState>(game);

    stateSpace.addVertex(start);

    Queue<Vertex<GameState>*> frontier;
    frontier.enqueue(start);
    
    int lowestScore = -1000;
    int lowestX = -1;
    int lowestY = -1;

    while (!frontier.isEmpty()){
        Vertex<GameState>* curr = frontier.dequeue();
        if (!curr->data.done){
            for(int i = 0; i < game.size; i++){
                for (int j = 0; j < game.size; j++){
                    if(curr->data.grid[i][j] == -1){
                        GameState next = curr->data;
                        next.play(i, j);
                        Vertex<GameState>* successor = new Vertex<GameState>(next);

                        int tempScore = getReward(successor, 0);
                        if(tempScore > lowestScore){
                            lowestScore = tempScore;
                            lowestX = i;
                            lowestY = j;
                        }

                        stateSpace.addVertex(successor);
                        stateSpace.addDirectedEdge(curr, successor);
                        if (!successor->data.done){
                            frontier.enqueue(successor);
                        }
                    }
                }
            }
        }
    }
    // cout << "lowest score: " << lowestScore << endl;
    // cout << "lowest x at: " << lowestX << endl;
    // cout << "lowest y at: " << lowestY << endl;

    // for every successor of start, call getReward(successor, player1)


    return Vec(lowestX, lowestY);
}

// Bug spotted somewhere in this function, where finding best move mutilates gamestate and forces game to draw
// Pass by value, so we can mutilate gamestate to play all potential values
Vec findBestMove1(GameState game) {
    int depth = 5;
    bool maxPlayers = false;
    int highest = maxPlayers ? -1000 : 1000;
    Vec move(-1, -1); // Value should not be needed

    for (int row = 0; row < game.size; row++) {
        for (int col = 0; col < game.size; col++) {
            // if (game.grid[row][col] == -1) {
            //     game.play(row, col); // Grid,play() is taking up all possible turns and forcing a draw
            // } // Not sure the point of this logic tho

            if (game.grid[row][col] == -1) { // -1 means the square is available
                game.play(row, col); // Grid,play() was taking up all possible turns and forcing a draw
                // cin.ignore();
                // cout << "Reached " << depth << " " << score << " "endl;
                int highScore = minimax(game, depth, maxPlayers); // a new score is created to determine if minimax is better than normal score
                game.grid[row][col] = -1; // Set to -1 so we can use the square

                if ((maxPlayers && highScore > highest) || (!maxPlayers && highScore < highest))  {
                    highest = highScore;
                    move = Vec(row, col); // Square with highest evaluation is kept
                    // cout << highScore << " Row: " << row << " Col:" << col << endl;
                    // cin.ignore();
                    // cin.ignore();
                }
            }
            

            // move = Vec(row, col); can't do it because it will cause an error. 
        }
    }
    return move;
    // return Vec(1, 1);
}

// Conditions for passing function in function, needs same paramaters and return type
// * and & are part of syntax, getPlayerMove is just a name for the function passed through
// Data type is in parenthesis.

// Valid move function is level 1 computer

// Function to play between Player and Advanced Computer

Vec humanMove1(GameState game) {
    int row, col;

    std::cout << "Enter your move (row and column): ";
    std::cin >> row >> col;

    return Vec(row, col); // connected to GameState file, where upon choosing x and y, grid will place on board.
}

Vec validMove1(GameState game) {
    // Randomly choose a valid move
    int row, col;
    
    do {
        row = rand() % game.size; // valid move will access rand() and because random can't be bigger than the size of the grid,
        col = rand() % game.size; // it will produce a certain depth of field 0 1 2, and if grid is 4x4, 0 1 2 3
    } 
    while (game.grid[row][col] != -1); // shortens while loop in lines referring back to do statement. 

    return Vec(row, col);
}

void playHardGame(GameState game, Vec (*getPlayerMove)(GameState), Vec (*getComputerMove)(GameState)) {
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
            cout << "Game is over" << endl;
            // int x;
            // cin >> x;
            cin.ignore();
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

// Vec unbeatableMove(GameState& game) {

// }

// File change occurs here

#endif