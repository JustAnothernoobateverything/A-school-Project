#include <iostream>//for screen
#include <string>//it is carrying the program, allows for strings
#include <fstream>//for file saving and loading
#include <algorithm>//for max and min for the map
#include <vector>// for modifiable lists
#include <conio.h>//for movement controls
#include <map>//for the status effects list
#include <windows.h>//for waiting timers for combat
//#include "struct.hpp"//all of the structs (can't add here because of r)
#include "json.hpp"//the json parsing and writing libary (blessing)
#include "map&movement.hpp"// holds the functions for the map and movement of the program
#include "saving&loading.hpp"//has saving and loading functions

using namespace std;


//const map<string, double> status_effects = {{"burn", 5},{"bleed",3},{"corrosion", 10}}; Cut content

/*double status_return(string status){ // Cut content 
    for (auto stat : status_effects){
        if ((stat.first).compare(status)==0){
            return stat.second;
        }
    }
    return 1;
}*/

double dmg_output(double dmg, string skils, string dweapon, int level){
    bool flag1 = false;
    bool flag2 = false;
    double dmgweap;
    double dweapbuff;
    double skilmult;
    for (int i = 0; i < skills.size(); i++){
        if (skils.compare(skills[i].name)==0){
            skilmult = skills[i].buffmult;
            flag1 = true;
        }
    }
    if (!flag1){
        skilmult =1;
    }
    for (int i = 0; i < weapons.size(); i++){
        if (dweapon.compare(weapons[i].name)==0){
            dmgweap = weapons[i].damage;
            dweapbuff = weapons[i].damage_buff;
            flag1 = true;
        }
    }
    if(!flag2){
        dmgweap = 0;
        dweapbuff = 1;
    }
    return ((dmg +((dmgweap/dmg))*dweapbuff)/(skilmult))+level;
}

double Damage_taken(double defense, string armor, double damage){
    bool flag = false;
    double parmor;
    double dmgweap;
    double dweapbuff;
    for (int i = 0; i < armors.size(); i++){
        if (armor.compare(armors[i].name)==0){
            parmor = armors[i].dmg_reduction;
            flag = true;
        }
    }
    if (!flag){
        parmor = 1;
    }

    return ((damage-(parmor))/(defense));
}


void item_buff(item it, Character& P){
    if((it.type).compare("Magic") == 0){
        P.mana += it.addbuff;
        cout <<"The item gives you some mana!"<<endl;
    }
    else if((it.type).compare("Stamina") == 0){
        P.stamina += it.addbuff;
        cout <<"The item gives you some Stamina!"<<endl;
    }
    else if((it.type).compare("Health") == 0){
        P.health += it.addbuff;
        cout <<"The item heals you!"<<endl;
    }
    else{
        cout <<"The item did nothing!"<<endl;
    }
    return;
}

void use_spell(Spell s, Character& p, Character& e){
    if (s.cost > p.mana){
        cout << "Your mana is too low to use this spell!";
        cout << " Press any key to continue..."<<endl;
        getch();
    }
    else{
        p.mana -= s.cost;
        e.health -= Damage_taken(e.defense,e.armor,s.damage);
        cout << "Spell "<< s.name <<" has done "<< Damage_taken(e.defense,e.armor,s.damage) << " damage."<<endl;
    }
}

void level_up(Character& p){
    srand(time(0));
    double exp = (20 * ((p.level/10)+1));
    char response;
    cout << "Would you like to level up by losing " << exp<<" exp? (y/n)"<<endl;
    cin >> response;
    if (response = 'y'){
        if (p.exp < exp){
            cout << "You dont have enough exp to level up."<< endl;
        }
        else{
            p.exp -= exp;
            p.level++;
            if (p.level%3==0){
                int rr = rand()%(spells.size()-1);
                Spell spek = spells[rr];
                cout<<"You gain a spell called "<< spek.name <<"!"<< endl;
                int choice;
                cout << "What slot would you like to put it in (1-5): ";
                cin >> choice;
                p.magic[choice-1] = spek.name;
            }
            if (p.level%2==0){
                int rr = rand()%(skills.size()-1);
                Skill spek = skills[rr];
                cout<<"You gain a skill called "<< spek.name <<"!"<< endl;
                int choice;
                cout << "What slot would you like to put it in (1-6): ";
                cin >> choice;
                p.skills[choice-1] = spek.name;
            }
        }
    }
}




void combatmode(Character& p){
    srand(time(0));
    Character currentenemy = enemies[rand()%(enemies.size())];
    system("cls");
    cout << "You have encountered a " <<currentenemy.name << "!"<< endl;
    while (true){
        charr_ui(currentenemy,true);
        charr_ui(p,false);
        char input = getch();
        if (input == 'f'){
            cout << "You have fled!" << endl;
            cout << "Press any key to continue..."<<endl;
            getch();
            break;
        }
        if (input == '1'){
            double dmg = Damage_taken(currentenemy.defense,currentenemy.armor,p.dmg);
            currentenemy.health -= dmg;
            cout << "basic attack has done "<< dmg<< " damage!";
            cout << "Press any key to continue..."<<endl;
            getch();
        }
        if (input == 'e'){
            int pick = 0;
            cout << "Please pick what spell you would like to use (1-5)";
            cin >> pick;
            pick--;
            Spell sp;
            for (int i = 0; i < (spells.size()); i++){
                if ((p.magic[pick].compare((spells[i].name)))==0)
                {
                    sp = spells[i];
                }
            }
            use_spell(sp,p,currentenemy);
            cout << "Press any key to continue..."<<endl;
            getch();
        }
        if (input == 'r'){
            int pick = 0;
            cout << "Please pick what skill you would like to use (1-6)";
            cin >> pick;
            pick--;
            currentenemy.health -= dmg_output(p.dmg,p.skills[pick],p.weapon,p.level);
            cout << "Skill "<< p.skills[pick]<<" has done "<< dmg_output(p.dmg,p.skills[pick],p.weapon,p.level) << " damage!"<<endl;
            cout << " Press any key to continue..."<<endl;
            getch();
        }
        if (input == 'i'){
            int pick = 0;
            cout << "Please pick what item you would like to use (1-25)";
            cin >> pick;
            pick--;
            item it;
            for (int i = 0; i < (items.size()); i++){
                if ((p.inventory[pick].compare((items[i].name)))==0)
                {
                    it = items[i];
                }
            }
            item_buff(it,p);
            cout << "Press any key to continue..."<<endl;
            getch();
        }
        if (currentenemy.health <= 0){
            cout <<"You have beaten the encounter and gained "<< currentenemy.exp<<" Experience!"<<endl;
            if (rand()%2 == 1){
                int rr = rand()%(items.size()-1);
                item it = items[rr];
                cout<<"You gain an item called "<< it.name <<"!"<< endl;
                int choice;
                cout << "What slot would you like to put it in (1-25): ";
                cin >> choice;
                p.inventory[choice-1] = it.name;
            }
            p.exp += currentenemy.exp;
            cout << " Press any key to continue..."<<endl;
            getch();
            break;
        }
        charr_ui(currentenemy, true);
        p.health -= Damage_taken(p.defense,p.armor,dmg_output(currentenemy.dmg,"",currentenemy.weapon,currentenemy.level));
        if(p.health <= 0){
            cout << "You have died!";
            exit(0);
        }
        charr_ui(p,false);
        
    }
}


void set_player(Character& p){
    cout << "Please enter a name for this character: ";
    cin>>p.name;
    p.health = 100;
    p.mana = 50;
    p.stamina = 50;
    p.defense = 1;
    p.level = 1;
    p.exp = 0;
    p.dmg = 5;
    p.weapon = "";
    p.armor = "";
    p.status = "";
}

void main(){
    load_EVERYTHING();
    cout << "Welcome to cheap dungeon crawl (Press c for controls)"<<endl;
    char map[w][h];
    Character player;
    char save;
    cout << "Would you like to import the last save file?(y/n)"<<endl;
    cin >> save;
    if (save=='y'){
        read_save(player, "save.json");
    }
    else{
        set_player(player);
    }
    int playerposx=w/2;
    int playerposy=h/2;
    setmap(map,playerposx,playerposy);
    int purpose = 1;
    int encounters = 0;
    while (true){
        char input;
        srand(time(0));
        if (!movement(map,playerposx, playerposy,input)){
            break;
        }
        if (input == 'm'){
            if (encounters >= 5){
                encounters = 0;
                setmap(map,playerposx,playerposy);
            }
            else{
                cout << "Encounter at least 5 enemies before changing the map."<<endl;
            }
        }
        if ((rand()%4)==1){
            encounters++;
            cout << "You've been attacked!"<< endl;
            combatmode(player);
        }
        if (input == 'c'){
            while(true){
                cout<< "To move use w a s d. Use m to change maps. Use o to save your character. Use P to level up. And q to quit. (Press c again to close this menu!)"<< endl;
                cout<<"Combat: press 1 for basic attack, e for skills, r for magic, i to use an item ,and f to flee"<<endl;
                input = getch();
                if (input=='c'){
                    break;
                }
            }
        }
        if (input == 'o'){
            file_save(player, "save.json");
            getch();
        }
        if (input == 'p'){
            level_up(player);
            getch();
        }
        fulldisplay(map);
        charr_ui(player, false);
        input = getch();

    }
    return;
}