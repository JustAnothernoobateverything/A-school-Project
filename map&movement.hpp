#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <conio.h>
#include <ctime>
#include "struct.hpp"
#include "json.hpp"

//map width and height as constants
const int w = 20;
const int h = 20;

using namespace std;
//the movement system
bool movement(char map[w][h], int& play_x, int& play_y,char input){
    //find player position
    int nx = play_x;
    int ny = play_y;
    //what the user inputs to move
    if (input == 'w') {
        ny--;
    }
    if (input == 's') {
        ny++;
    }
    if (input == 'a') {
        nx--;
    }
    if (input == 'd') {
        nx++;
    }
    //change the char in the array to show that the player moved
    if (map[nx][ny] == '-') {
        map[play_x][play_y] = '-';
        play_x = nx;
        play_y = ny;
        //place new player posistion after clearing the old posistion
        map[play_x][play_y] = 'p';
    }
    //if the user presses q quits the program
    if (input == 'q') {
        //for closing the program
        return false;
    }
    //if q isnt press don't close the program
    return true;
}

void fulldisplay(char map[w][h]){
    //clear the console from any text.
    system("cls");
    //prints out the 2d array to the console
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            cout << map[j][i];
            cout << " ";
        }
        cout << endl;
    }
    
}
//displayer user and enemy ui to the console
void charr_ui(Character& player, bool clear){
    //if true clear the console 
    if (clear){
        system("cls");
    }
    //display all stats to the user
    cout <<"---------------------------------------------------------------------------------------------------------------"<<endl;
    cout <<"Name: " <<player.name << "   HP: "<< player.health<<"   Status: " <<player.status<< "   level: "<<player.level<< "   exp: "<<player.exp<< endl;
    cout <<"Mana: "<<player.mana << "   Stamina: "<< player.stamina<<"   Weapon: " <<player.weapon<< "   Armor: "<<player.armor<< "   Defense: "<<player.defense<< endl;
    //display the inventory array to the user
    cout <<"Inventory: ";
    for (int i = 0; i<25;i++){
        if (i == 10 || i == 20){
            cout <<endl;
        }
        cout << player.inventory[i]<<" ";
    }
    //display the magic/spells available to the user
    cout << endl;
    cout <<"Magic/Spells: ";
    for (int i = 0; i < 5;i++){
        cout << player.magic[i]<<" ";
    }
    //then do the same for skills
    cout << endl;
    cout <<"Skills: ";
    for (int i = 0; i < 6;i++){
        cout << player.skills[i]<<" ";
    }
    cout <<endl<<"---------------------------------------------------------------------------------------------------------------"<<endl;
}

//set the map up using drunkards walk algorthim
void setmap (char map[w][h], int& xx, int& yy){
    //set map to all |
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            map[i][j] = '|';
    //randomize using system time
    srand(time(0));
    //randomize the point of where the walk will start
    int y = rand() % w;
    int x = rand() % h;
    //calculates the total amount of cells that should be created
    int totalcells = (w+h)*5;
    //initialize the current cells var to tell when the program to stop
    int currentcells = 0;
    //Array that is used for the direction of the walk
    int direct[] = {0, 0, 1, -1};

    while (totalcells > currentcells){
        //set it to a playable char '-' instead of a wall '|'
        if (map[y][x] == '|'){
            map[y][x] = '-';
            currentcells++;
        }
        //randomize the index that will give us the direction, for both the x and y coords for direction
        int dir1 = rand() % 4;
        int dir2 = (rand()+1) % 4;
        //use max a min so we dont go out of bounds of the array. also add the current x and y posistion by the value of array of direction from the randomized index
        x = max(1, min(w-2, x+ direct[dir1]));
        y = max(1, min(h-2, y+ direct[dir2]));
    }
    //set player to center of the map (Less than %5 chance that user gets stuck)
    map[w/2][h/2] = 'p';
    
}