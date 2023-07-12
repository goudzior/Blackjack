#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct Card {
    std::string suit;
    std::string rank;
    int value;
    sf::Texture texture;
    sf::Sprite sprite;
};