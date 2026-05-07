#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "json.hpp"

using namespace std;

//standard json shortcut so i dont have to type nlohmann 20 times
using json = nlohmann::json;


/// vectors that can allow for as many elements that come from the json files
vector<Weapon> weapons;
vector<Armor> armors;
vector<Skill> skills;
vector<Spell> spells;
vector<item>  items;
vector<Character> enemies;


//Big headache when reading api
//allows for file saving
void file_save(Character player, string path) {
    // label the json object
    json j;
    // plug in all of the data we have about the player to the json file (Single values)
    j["status"]  = player.status;
    j["name"]    = player.name;
    j["exp"]     = player.exp;
    j["level"]   = player.level;
    j["dmg"]     = player.dmg;
    j["defense"] = player.defense;
    j["health"]  = player.health;
    j["stamina"] = player.stamina;
    j["mana"]    = player.mana;
    j["weapon"]  = player.weapon;
    j["armor"]   = player.armor;

    //Since I know what data type is going through i can just label it string
    //used a range for loop so i dont have to deal with an index and not have any out of bounds error since i got like 20 of them when writing this with a normal loop.
    j["inventory"] = json::array();
    for (string item : player.inventory)
        //an overload of the vector push_back() function by nlohmann
        j["inventory"].push_back(item);
    //copied and pasted from above but just changed the type of data being inputed.
    j["skills"] = json::array();
    for (string skill : player.skills)
        j["skills"].push_back(skill);
    //copied and pasted from above but just changed the type of data being inputed.
    j["magic"] = json::array();
    for (string spell : player.magic)
        j["magic"].push_back(spell);

    //open the file and dump all the data into the file
    fstream file;
    file.open(path);
    file << j.dump(4);
    cout << "Player data saved to " << path << "\n";
}
// open/create a json file
json loadJson(string path) {
    //opens the file
    ifstream file(path);
    //creates a json object
    json data;
    //set file we are editing to that object
    file >> data;
    return data;
}
// function that parses the json file
Weapon weaponFromJson(json j) {
    //create a empty weapon object
    Weapon w;
    //fill up the object with all data from the json file using get() another overload from nlohmann
    w.name          = j.at("name").get<string>();
    w.durability    = j.at("durability").get<int>();
    w.damage        = j.at("damage").get<double>();
    w.damage_buff   = j.at("damage_buff").get<double>();
    w.status_effect = j.at("status_effect").get<string>();
    //return the object
    return w;
}
//copied and pasted from above but just changed to fit this class
Armor armorFromJson(json j) {
    Armor a;
    a.name    = j.at("name").get<string>();
    a.durability    = j.at("durability").get<int>();
    a.dmg_reduction = j.at("dmg_reduction").get<double>();
    a.heal_per_move = j.at("heal_per_move").get<double>();
    return a;
}
//copied and pasted from above but just changed to fit this class
Skill skillFromJson(json j) {
    Skill s;
    s.name     = j.at("name").get<string>();
    s.buffmult = j.at("buffmult").get<double>();
    return s;
}
//copied and pasted from above but just changed to fit this class
Spell spellFromJson(json j) {
    Spell s;
    s.name       = j.at("name").get<string>();
    s.status_eff = j.at("status_eff").get<string>();
    s.tier       = j.at("tier").get<int>();
    s.damage     = j.at("damage").get<double>();
    s.cost       = j.at("cost").get<double>();
    return s;
}
//copied and pasted from above but just changed to fit this class
item itemFromJson(json j) {
    item i;
    i.name    = j.at("name").get<string>();
    i.type    = j.at("type").get<string>();
    i.addbuff = j.at("addbuff").get<double>();
    return i;
}
//mostly copied and pasted for non array values
Character characterFromJson(json j) {
    Character c;
    c.status  = j.at("status").get<string>();
    c.name    = j.at("name").get<string>();
    c.exp     = j.at("exp").get<double>();
    c.level   = j.at("level").get<int>();
    c.dmg     = j.at("dmg").get<double>();
    c.defense = j.at("defense").get<double>();
    c.health  = j.at("health").get<double>();
    c.stamina = j.at("stamina").get<double>();
    c.mana    = j.at("mana").get<double>();
    c.weapon  = j.at("weapon").get<string>();
    c.armor   = j.at("armor").get<string>();

    //set auto since we dont know what data could be in the json
    auto inv = j.at("inventory");
    //since we know the length of the array we can use a normal for loop
    //idk why i need (int)inv.size() && the program doesnt work without it, could mean that it is somehow going out of bounds without it since its an and condition (I searched this part up on how to do it if thats fine)
    for (int i = 0; i < (int)inv.size() && i < 25; i++)
        c.inventory[i] = inv[i].get<string>();
    //copied and pasted from above but to fit skills
    auto sk = j.at("skills");
    for (int i = 0; i < (int)sk.size() && i < 6; i++)
        c.skills[i] = sk[i].get<string>();
    //copied and pasted from above but to fit magic
    auto mg = j.at("magic");
    for (int i = 0; i < (int)mg.size() && i < 5; i++)
        c.magic[i] = mg[i].get<string>();
    //return the object
    return c;
}

//basically just calling the read character from json function but with a certian file seperate from enemies.json
void read_save(Character& player, string path) {
    //same thing, create/open file 
    ifstream file(path);
    //create an empty json for the data
    json j;
    //push json object to the file for it to be read by the parser
    file >> j;
    player = characterFromJson(j);
    //say where data is loaded from
    cout << "Player data loaded from " << path << "\n";
}

void load_EVERYTHING() {
    //since i dont know the length of the json file i will use a ranged for loop again
    //load the data into the holding object
    json weaponData = loadJson("Weap.json");
    //use auto because i dont know what data could be inputted in the json file
    for (auto entry : weaponData.at("weapons"))  
        //push it to the vector array for storage. this push_back() function is another overload from nlohmann
        weapons.push_back(weaponFromJson(entry));
    //copy and pasted from above but modified to contain this type of data
    json spellData = loadJson("Spell.json");
    for (auto entry : spellData.at("spells"))
        spells.push_back(spellFromJson(entry));
    //copy and pasted from above but modified to contain this type of data
    json armorData = loadJson("Armor.json");
    for (auto entry : armorData.at("Armor"))
        armors.push_back(armorFromJson(entry));
    //copy and pasted from above but modified to contain this type of data
    json skillData = loadJson("skills.json");
    for (auto entry : skillData.at("skills"))
        skills.push_back(skillFromJson(entry));
    //copy and pasted from above but modified to contain this type of data
    json itemData = loadJson("items.json");
    for (auto entry : itemData.at("items"))
        items.push_back(itemFromJson(entry));
    //copy and pasted from above but modified to contain this type of data
    json enemyData = loadJson("enemies.json");
    for (auto entry : enemyData.at("enemies"))
        enemies.push_back(characterFromJson(entry));
}