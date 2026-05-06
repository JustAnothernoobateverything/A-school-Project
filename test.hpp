#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

void file_save(Character player, string path) {
    json j;

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

    j["inventory"] = json::array();
    for (string item : player.inventory)
        j["inventory"].push_back(item);

    j["skills"] = json::array();
    for (string skill : player.skills)
        j["skills"].push_back(skill);

    j["magic"] = json::array();
    for (string spell : player.magic)
        j["magic"].push_back(spell);

    ofstream file(path);
    file << j.dump(4);
    cout << "Player data saved to " << path << "\n";
}

json loadJson(string path) {
    ifstream file(path);
    json data;
    file >> data;
    return data;
}

Weapon weaponFromJson(json j) {
    Weapon w;
    w.name          = j.at("name").get<string>();
    w.durability    = j.at("durability").get<int>();
    w.damage        = j.at("damage").get<double>();
    w.damage_buff   = j.at("damage_buff").get<double>();
    w.status_effect = j.at("status_effect").get<string>();
    return w;
}

Armor armorFromJson(json j) {
    Armor a;
    a.name    = j.at("name").get<string>();
    a.durability    = j.at("durability").get<int>();
    a.dmg_reduction = j.at("dmg_reduction").get<double>();
    a.heal_per_move = j.at("heal_per_move").get<double>();
    return a;
}

Skill skillFromJson(json j) {
    Skill s;
    s.name     = j.at("name").get<string>();
    s.buffmult = j.at("buffmult").get<double>();
    return s;
}

Spell spellFromJson(json j) {
    Spell s;
    s.name       = j.at("name").get<string>();
    s.status_eff = j.at("status_eff").get<string>();
    s.tier       = j.at("tier").get<int>();
    s.damage     = j.at("damage").get<double>();
    s.cost       = j.at("cost").get<double>();
    return s;
}

item itemFromJson(json j) {
    item i;
    i.name    = j.at("name").get<string>();
    i.type    = j.at("type").get<string>();
    i.addbuff = j.at("addbuff").get<double>();
    return i;
}

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

    auto inv = j.at("inventory");
    for (int i = 0; i < (int)inv.size() && i < 25; i++)
        c.inventory[i] = inv[i].get<string>();

    auto sk = j.at("skills");
    for (int i = 0; i < (int)sk.size() && i < 6; i++)
        c.skills[i] = sk[i].get<string>();

    auto mg = j.at("magic");
    for (int i = 0; i < (int)mg.size() && i < 5; i++)
        c.magic[i] = mg[i].get<string>();

    return c;
}

void read_save(Character& player, string path) {
    ifstream file(path);
    json j;
    file >> j;
    player = characterFromJson(j);
    cout << "Player data loaded from " << path << "\n";
}

void load_EVERYTHING() {
    vector<Weapon>    weapons;
    vector<Armor>   armors;
    vector<Skill>     skills;
    vector<Spell>     spells;
    vector<item>      items;
    vector<Character> enemies;
    json weaponData = loadJson("Weap.json");
    for (auto& entry : weaponData.at("weapons"))  
        weapons.push_back(weaponFromJson(entry));

    json spellData = loadJson("Spell.json");
    for (auto& entry : spellData.at("spells"))
        spells.push_back(spellFromJson(entry));

    json armorData = loadJson("Armor.json");
    for (auto& entry : armorData.at("Armor"))
        armors.push_back(armorFromJson(entry));

    json skillData = loadJson("skills.json");
    for (auto& entry : skillData.at("skills"))
        skills.push_back(skillFromJson(entry));

    json itemData = loadJson("items.json");
    for (auto& entry : itemData.at("items"))
        items.push_back(itemFromJson(entry));

    json enemyData = loadJson("enemies.json");
    for (auto& entry : enemyData.at("enemies"))
        enemies.push_back(characterFromJson(entry));
}