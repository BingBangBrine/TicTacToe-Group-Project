#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <system_error>
#include "GameState.h"
#include "HumanValid.h"
#include "Unbeatable.h"



bool getDifficulty(){
    // retrieve difficulty from file, 2 is hard, 1 is easy 
    ifstream file;
    int difficulty = 0;
    file.open("settings.txt", std::ios::in);
    
    if(file.is_open()) {
        std::string line;
        while(getline(file, line)) {
            for(char c : line){
                if(isdigit(c)){
                    difficulty = c;
                }
            }
        }
        file.close();
    }

    return (difficulty - '0')-1;
}

void setDifficulty(string choice){
    ofstream file("settings.txt");

    if(file.is_open()) {
        file << choice;
        file.close();
    }
}

void difficultyScreen(){
    clearScreen();
    cout << "Choose Computer Level" << endl;
    cout << "1. Set difficulty to easy" << endl;
    cout << "2. Set difficulty to unbeatable" << endl;
    cout << "3. Cancel" << endl;
    
    string difficultyLevel = "easy";
    if(getDifficulty()){
        difficultyLevel = "unbeatable";
    }
    
    cout << "Current difficulty is set to " << difficultyLevel << endl << endl;
}

void settings() {
    // setting computer level section
    string difficultyInput;
    difficultyScreen();
    while(getline(cin, difficultyInput)){
        if(difficultyInput == "1" || difficultyInput == "2"){
            setDifficulty(difficultyInput);
            break;
        }
        if(difficultyInput == "3"){
            clearScreen();
            return;
        }
        difficultyScreen();
    }

    clearScreen();
    
    if(difficultyInput == "1"){
        cout << "Difficulty set to easy" << endl;
    }
    else{
        cout << "Difficulty set to unbeatable" << endl;
    }
    
    cout << endl << "Press [enter] to continue" << endl;

    string temp;
    getline(cin, temp);
    clearScreen();
}

#endif