// characters.cpp
#include "characters.h"
#include <iostream>

// Enum to string helper implementations
std::string rarityToString(CharacterRarity rarity) {
    switch (rarity) {
    case CharacterRarity::FiveStar: return "5-Star";
    case CharacterRarity::FourStar: return "4-Star";
    default: return "Unknown";
    }
}

std::string pathToString(CharacterPath path) {
    switch (path) {
    case CharacterPath::Hunt: return "Hunt";
    case CharacterPath::Erudition: return "Erudition";
    case CharacterPath::Destruction: return "Destruction";
    case CharacterPath::Harmony: return "Harmony";
    case CharacterPath::Nihility: return "Nihility";
    case CharacterPath::Preservation: return "Preservation";
    case CharacterPath::Abundance: return "Abundance";
    case CharacterPath::Remembrance: return "Remembrance";
    case CharacterPath::Elation: return "Elation";
    default: return "Unknown";
    }
}

std::string elementToString(CharacterElement element) {
    switch (element) {
    case CharacterElement::Physical: return "Physical";
    case CharacterElement::Fire: return "Fire";
    case CharacterElement::Ice: return "Ice";
    case CharacterElement::Lightning: return "Lightning";
    case CharacterElement::Wind: return "Wind";
    case CharacterElement::Quantum: return "Quantum";
    case CharacterElement::Imaginary: return "Imaginary";
    default: return "Unknown";
    }
}

// Character class implementation
Character::Character(std::string name, CharacterRarity rarity, CharacterPath path, CharacterElement element)
    : m_name(std::move(name)), m_rarity(rarity), m_path(path), m_element(element) {
}

const std::string& Character::getName() const {
    return m_name;
}

CharacterRarity Character::getRarity() const {
    return m_rarity;
}

CharacterPath Character::getPath() const {
    return m_path;
}

CharacterElement Character::getElement() const {
    return m_element;
}

void Character::display() const {
    std::cout << "Character: " << m_name << "\n"
        << "  Rarity: " << rarityToString(m_rarity) << "\n"
        << "  Path: " << pathToString(m_path) << "\n"
        << "  Element: " << elementToString(m_element) << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Character& character) {
    os << character.m_name << " (" << rarityToString(character.m_rarity) << ", "
        << pathToString(character.m_path) << ", " << elementToString(character.m_element) << ")";
    return os;
}