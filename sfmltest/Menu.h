#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <stdlib.h>
#include "constants.h"
#include <cstdlib>
using std::vector;

class Menu {
private:
    sf::RenderWindow menuWindow;
    sf::Font font, titleFont;
    vector<sf::Text> buttons;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

public:
    Menu();
    bool isMouseOnText(const sf::Text& text, const sf::RenderWindow& window);
    int display();
};
