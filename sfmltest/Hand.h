#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Card.h"

class Hand {
private:
    std::vector<Card> cards;
    sf::Texture cardBackTexture;
    sf::Sprite cardBackSprite;
public:
    Hand();
    int getValue();
    int getAbsoluteValue();
    bool draw(Card card);
    void clear();
    void render(sf::RenderWindow& window, const sf::Vector2f position, bool isDealer = false, bool revealDealerCard = true);
    int getFirstCardValue() const;
    int size() const;
};