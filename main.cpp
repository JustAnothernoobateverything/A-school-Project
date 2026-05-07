#include <iostream>//for screen
#include <string>//it is carrying the program, allows for strings
#include <fstream>//for file saving and loading
#include <algorithm>//for max and min for the map
#include <vector>// for modifiable lists
#include <conio.h>//for movement controls
#include <map>//for the status effects list
#include <windows.h>//for waiting timers for combat
//#include "struct.hpp"//all of the structs (can't add here because of redefinition error)
#include "json.hpp"//the json parsing and writing libary (blessing)
#include "map&movement.hpp"// holds the functions for the map and movement of the program
#include "saving&loading.hpp"//has saving and loading functions

// for me to not type std every line
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
//calculates damage output
double dmg_output(double dmg, string skils, string dweapon, int level){
    //flags to check if a skill like that exists
    bool flag1 = false;
    bool flag2 = false;
    //vars to determine the mult of certian items and skils
    double dmgweap;
    double dweapbuff;
    double skilmult;
    for (int i = 0; i < skills.size(); i++){
        if (skils.compare(skills[i].name)==0){
            skilmult = skills[i].buffmult;
            //if found set to true.
            flag1 = true;
        }
    }
    if (!flag1){
        //if still false set mult to 1
        skilmult =1;
    }
    //basically same config but copied and pasted to fit weapons
    for (int i = 0; i < weapons.size(); i++){
        if (dweapon.compare(weapons[i].name)==0){
            dmgweap = weapons[i].damage;
            dweapbuff = weapons[i].damage_buff;
            flag1 = true;
        }
    }
    //same
    if(!flag2){
        dmgweap = 0;
        dweapbuff = 1;
    }
    //return the damage by dividing it and multiplying it by different multipliers
    return ((dmg +((dmgweap/dmg))*dweapbuff)/(skilmult))+level;
}
//Calculates how much damage an enitiy is going to take
double Damage_taken(double defense, string armor, double damage){
    //same like damage output where we need to find the right armor that the character has from the array.
    bool flag = false;
    //multipliers var
    double parmor;
    double dmgweap;
    double dweapbuff;
    //same thing as damage output
    for (int i = 0; i < armors.size(); i++){
        if (armor.compare(armors[i].name)==0){
            parmor = armors[i].dmg_reduction;
            flag = true;
        }
    }
    //check if it exsist if not set multiplier to zero
    if (!flag){
        parmor = 1;
    }
    //return the damage taken
    return ((damage-(parmor))/(defense));
}

//called if user wants to use an item
void item_buff(item it, Character& P){
    //check the item type from the vector data
    //if it has the right type buff that aspect of the character
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
    //if there is no type that matches the data item does nothing
    else{
        cout <<"The item did nothing!"<<endl;
    }
    return;
}
//called if player wants to use a spell
void use_spell(Spell s, Character& p, Character& e){
    //check if player has enough mana
    if (s.cost > p.mana){
        //if not enough then dont use spell
        cout << "Your mana is too low to use this spell!";
        //wait till user enters an input to continue
        cout << " Press any key to continue..."<<endl;
        getch();
    }
    else{
        //subtract the cost of the spell from player's mana
        p.mana -= s.cost;
        //death damage to the enemy's health
        e.health -= Damage_taken(e.defense,e.armor,s.damage);
        //tell user how much damage has been done
        cout << "Spell "<< s.name <<" has done "<< Damage_taken(e.defense,e.armor,s.damage) << " damage."<<endl;
    }
}
//if player wants to level up function is called
void level_up(Character& p){
    //to randomize drops
    srand(time(0));
    // the cost calculations
    double exp = (20 * ((p.level/10)+1));
    //if the user wants to level up after seeing exp cost
    char response;
    cout << "Would you like to level up by losing " << exp<<" exp? (y/n)"<<endl;
    cin >> response;
    //if yes then level up
    if (response = 'y'){
        if (p.exp < exp){
            //if not enough exp then dont level up
            cout << "You dont have enough exp to level up."<< endl;
        }
        else{
            //if enough subtract the exp from user exp and increase level
            p.exp -= exp;
            p.level++;
            //every 3 levels gain a random spell
            if (p.level%3==0){
                int rr = rand()%(spells.size()-1);
                Spell spek = spells[rr];
                cout<<"You gain a spell called "<< spek.name <<"!"<< endl;
                int choice;
                cout << "What slot would you like to put it in (1-5): ";
                cin >> choice;
                p.magic[choice-1] = spek.name;
            }
            //every 2 levels gain a random skill
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



//combat loop 
void combatmode(Character& p){
    //seeding time for enemies
    srand(time(0));
    //get a random enemy from the vector array
    Character currentenemy = enemies[rand()%(enemies.size())];
    //clear the old map and go into combat mode
    system("cls");
    //tell user what they have encountered
    cout << "You have encountered a " <<currentenemy.name << "!"<< endl;
    while (true){
        //display the enemy and user ui
        charr_ui(currentenemy,true);
        charr_ui(p,false);
        //get the input of the user to determine what they want to do
        char input = getch();
        //to flee
        if (input == 'f'){
            cout << "You have fled!" << endl;
            cout << "Press any key to continue..."<<endl;
            getch();
            break;
        }
        //to use a basic attack
        if (input == '1'){
            //determine damage of basic attack
            double dmg = Damage_taken(currentenemy.defense,currentenemy.armor,p.dmg);
            //subtract the health from enemy that was hit by attack
            currentenemy.health -= dmg;
            //tell user how much dmg they did
            cout << "basic attack has done "<< dmg<< " damage!";
            //wait for user to know what the attack did
            cout << "Press any key to continue..."<<endl;
            getch();
        }
        //press e to use spells
        if (input == 'e'){
            //the slot that the user picks
            int pick = 0;
            cout << "Please pick what spell you would like to use (1-5)";
            cin >> pick;
            pick--;
            Spell sp;
            //find the spell that the user picked against the vector array
            for (int i = 0; i < (spells.size()); i++){
                if ((p.magic[pick].compare((spells[i].name)))==0)
                {
                    sp = spells[i];
                }  
            }
            //then call and use the use_spell function
            use_spell(sp,p,currentenemy);
            //wait until user knows that their spell was cast
            cout << "Press any key to continue..."<<endl;
            getch();
        }
        //press r to use a skill
        if (input == 'r'){
            //ask what skill the user wants to use based on slot posistion
            int pick = 0;
            cout << "Please pick what skill you would like to use (1-6)";
            cin >> pick;
            pick--;
            //deal damage to enemy from using the skill
            currentenemy.health -= dmg_output(p.dmg,p.skills[pick],p.weapon,p.level);
            //tell user how much damage was done
            cout << "Skill "<< p.skills[pick]<<" has done "<< dmg_output(p.dmg,p.skills[pick],p.weapon,p.level) << " damage!"<<endl;
            //wait for user to know that their skill activated
            cout << " Press any key to continue..."<<endl;
            getch();
        }
        //press i to use item
        if (input == 'i'){
            //the user picks what to use by slot id
            int pick = 0;
            cout << "Please pick what item you would like to use (1-25)";
            cin >> pick;
            pick--;
            item it;
            //check against the vector item data to find the object
            for (int i = 0; i < (items.size()); i++){
                if ((p.inventory[pick].compare((items[i].name)))==0)
                {
                    it = items[i];
                }
            }
            //put it into the item_buff function
            item_buff(it,p);
            //wait for user to know their item was used
            cout << "Press any key to continue..."<<endl;
            getch();
        }
        //if enemy has no health
        if (currentenemy.health <= 0){
            //tell player has beated encounter
            cout <<"You have beaten the encounter and gained "<< currentenemy.exp<<" Experience!"<<endl;
            //randomize drops
            if (rand()%2 == 1){
                //get a random item
                int rr = rand()%(items.size()-1);
                item it = items[rr];
                //tell user what they have
                cout<<"You gain an item called "<< it.name <<"!"<< endl;
                int choice;
                //ask what user they want to put it in
                cout << "What slot would you like to put it in (1-25): ";
                cin >> choice;
                p.inventory[choice-1] = it.name;
            }
            //add exp from the battle
            p.exp += currentenemy.exp;
            cout << " Press any key to continue..."<<endl;
            //wait for user to know that they beat the enemy
            getch();
            break;
        }
        //enemy deals damage to you
        charr_ui(currentenemy, true);
        //deal damage to player
        p.health -= Damage_taken(p.defense,p.armor,dmg_output(currentenemy.dmg,"",currentenemy.weapon,currentenemy.level));
        //if user health less than zero then you die and program stops.
        if(p.health <= 0){
            cout << "You have died!";
            exit(0);
        }
        charr_ui(p,false);
        
    }
}

//set up the player if they are new
void set_player(Character& p){
    //ask user for a name
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
    //load everything from jsons at runtime
    load_EVERYTHING();
    //welcome user
    cout << "Welcome to cheap dungeon crawl (Press c for controls)"<<endl;
    //create map
    char map[w][h];
    //create player
    Character player;
    //ask if player wants to load old save
    char save;
    cout << "Would you like to import the last save file?(y/n)"<<endl;
    cin >> save;
    if (save=='y'){
        read_save(player, "save.json");
    }
    //if not then create a new player
    else{
        set_player(player);
    }
    //set player posistion to the middle of the map
    int playerposx=w/2;
    int playerposy=h/2;
    setmap(map,playerposx,playerposy);
    //i honestly dont know what this var is doing here
    int purpose = 1;
    //encounter checker
    int encounters = 0;
    //game loop
    while (true){
        //set input var
        char input;
        //set randomness from time
        srand(time(0));
        //if movement is false then stop the program because the player pressed q to quit
        if (!movement(map,playerposx, playerposy,input)){
            break;
        }
        //change map but you need at least 5 encounters to change it
        if (input == 'm'){
            if (encounters >= 5){
                encounters = 0;
                setmap(map,playerposx,playerposy);
            }
            else{
                cout << "Encounter at least 5 enemies before changing the map."<<endl;
            }
        }
        // there is a 1/4 chance to encounter an encounter
        if ((rand()%4)==1){
            encounters++;
            cout << "You've been attacked!"<< endl;
            combatmode(player);
        }
        //to display the controls to the user
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
        //to save the user data to save.json or whatever you want
        if (input == 'o'){
            file_save(player, "save.json");
            getch();
        }
        //to level up the player
        if (input == 'p'){
            level_up(player);
            getch();
        }
        //display the map
        fulldisplay(map);
        //display the ui of the player
        charr_ui(player, false);
        //get the user input for what they want to do
        input = getch();

    }
    return;
}