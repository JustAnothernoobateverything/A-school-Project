#include <string>
using namespace std;

//struct for character
typedef struct Character{
    string status;
    string name;
    double exp;
    int level;
    double dmg;
    double defense;
    double health;
    double stamina;
    double mana;
    string weapon;
    string armor;
    //arrays that are annoying.
    string inventory[25] = {"","","","","","","","","","","","","","","","","","","","","","","","",""};
    string skills[6] = {"","","","","",""};
    string magic[5] = {"","","","",""};
};
//struct for armor
typedef struct Armor{
    string name;
    int durability;
    double dmg_reduction;
    double heal_per_move;
};
//struct for weapon
typedef struct Weapon{
    string name;
    int durability;
    double damage;
    double damage_buff;
    string status_effect;
};
//struct for skill
typedef struct Skill{
    string name;
    double buffmult;
};
//struct for spell
typedef struct Spell{
    string name;
    string status_eff = "";
    int tier;
    double damage;
    double cost;
};
//struct for item
typedef struct item{
    string name;
    string type;
    double addbuff;
};