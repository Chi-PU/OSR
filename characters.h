//// characters.h
#pragma once
#include <string>
#include <ostream>

enum class CharacterRarity {
    FiveStar,
    FourStar
};

enum class CharacterPath {
    Hunt,
    Erudition,
    Destruction,
    Harmony,
    Nihility,
    Preservation,
    Abundance,
    Remembrance,
    Elation
};

enum class CharacterElement {
    Physical,
    Fire,
    Ice,
    Lightning,
    Wind,
    Quantum,
    Imaginary
};

// Helper functions for enum to string conversion
std::string rarityToString(CharacterRarity rarity);
std::string pathToString(CharacterPath path);
std::string elementToString(CharacterElement element);

class Character {
public:
    Character(std::string name, CharacterRarity rarity, CharacterPath path, CharacterElement element);

    // Getters
    const std::string& getName() const;
    CharacterRarity getRarity() const;
    CharacterPath getPath() const;
    CharacterElement getElement() const;

    // Display character information
    void display() const;

    // Stream operator for easy printing
    friend std::ostream& operator<<(std::ostream& os, const Character& character);

private:
    std::string m_name;
    CharacterRarity m_rarity;
    CharacterPath m_path;
    CharacterElement m_element;
};
