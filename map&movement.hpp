#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <conio.h>
#include <ctime>
#include "struct.hpp"
#include "json.hpp"

const int w = 20;
const int h = 20;

using namespace std;
bool movement(char map[w][h], int& play_x, int& play_y,char input){

    int nx = play_x;
    int ny = play_y;

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
    
    if (map[nx][ny] == '-') {
        map[play_x][play_y] = '-';
        play_x = nx;
        play_y = ny;
        map[play_x][play_y] = 'p';
    }
    if (input == 'q') {
        return false;
    }
    return true;
}

void fulldisplay(char map[w][h]){
    system("cls");
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            cout << map[j][i];
            cout << " ";
        }
        cout << endl;
    }
    
}

void charr_ui(Character& player, bool clear){
    if (clear){
        system("cls");
    }
    cout <<"---------------------------------------------------------------------------------------------------------------"<<endl;
    cout <<"Name: " <<player.name << "   HP: "<< player.health<<"   Status: " <<player.status<< "   level: "<<player.level<< "   exp: "<<player.exp<< endl;
    cout <<"Mana: "<<player.mana << "   Stamina: "<< player.stamina<<"   Weapon: " <<player.weapon<< "   Armor: "<<player.armor<< "   Defense: "<<player.defense<< endl;
    cout <<"Inventory: ";
    for (int i = 0; i<25;i++){
        if (i == 10 || i == 20){
            cout <<endl;
        }
        cout << player.inventory[i]<<" ";
    }
    cout << endl;
    cout <<"Magic/Spells: ";
    for (int i = 0; i < 5;i++){
        cout << player.magic[i]<<" ";
    }
    cout << endl;
    cout <<"Skills: ";
    for (int i = 0; i < 6;i++){
        cout << player.skills[i]<<" ";
    }
    cout <<endl<<"---------------------------------------------------------------------------------------------------------------"<<endl;
}


void setmap (char map[w][h], int& xx, int& yy){
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            map[i][j] = '|';
    
    srand(time(0));
    int y = rand() % w;
    int x = rand() % h;
    int totalcells = (w+h)*5;
    int currentcells = 0;

    int direct[] = {0, 0, 1, -1};

    while (totalcells > currentcells){
        if (map[y][x] == '|'){
            map[y][x] = '-';
            currentcells++;
        }
        int dir1 = rand() % 4;
        int dir2 = (rand()+1) % 4;
        x = max(1, min(w-2, x+ direct[dir1]));
        y = max(1, min(h-2, y+ direct[dir2]));
    }
    map[w/2][h/2] = 'p';
    
}