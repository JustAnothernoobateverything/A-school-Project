#include <string>
using namespace std;

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
    string inventory[25] = {"","","","","","","","","","","","","","","","","","","","","","","","",""};
    string skills[6] = {"","","","","",""};
    string magic[5] = {"","","","",""};
};
typedef struct Armor{
    string name;
    int durability;
    double dmg_reduction;
    double heal_per_move;
};
typedef struct Weapon{
    string name;
    int durability;
    double damage;
    double damage_buff;
    string status_effect;
};
typedef struct Skill{
    string name;
    double buffmult;
};
typedef struct Spell{
    string name;
    string status_eff = "";
    int tier;
    double damage;
    double cost;
};
typedef struct item{
    string name;
    string type;
    double addbuff;
};