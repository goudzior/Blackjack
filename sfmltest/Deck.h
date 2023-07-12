#pragma once
#include <vector>
#include "Card.h"

class Deck {
private:
    std::vector<Card> cards;
    std::map<std::string, sf::Texture> textures;
public:
    Deck();
    void shuffle();
    Card draw();
    int getSize() const;
};