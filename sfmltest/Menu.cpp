#include "Menu.h"
#include "Game.h"

Menu::Menu() {
    menuWindow.create(sf::VideoMode(menuWidth, menuHeight), "Blackjack Menu");
    font.loadFromFile("fonts/CasinoFlat.ttf");
    titleFont.loadFromFile("fonts/Casino3DFilledMarquee.ttf");
    backgroundTexture.loadFromFile("images/menuBackground.jpg");
    backgroundSprite.setTexture(backgroundTexture);


    //komen
    sf::Text newGameButton, highScoresButton, howToPlayButton, exitButton, title;

    //tytul
    title.setFont(titleFont);
    title.setCharacterSize(144);
    title.setString("BLACKJACK");
    title.setFillColor(sf::Color(85, 0, 17));
    title.setOutlineThickness(2);
    title.setOutlineColor(sf::Color::White);
    title.setPosition(menuWidth / 2 - 350, menuHeight / 2 - 300);

    //nowa gra
    newGameButton.setFont(font);
    newGameButton.setCharacterSize(72);
    newGameButton.setString("New Game");
    newGameButton.setOutlineThickness(2);
    newGameButton.setOutlineColor(sf::Color::White);
    newGameButton.setPosition(menuWidth / 2 - 350, menuHeight / 2 - 150);

    //highscory
    highScoresButton.setFont(font);
    highScoresButton.setCharacterSize(72);
    highScoresButton.setString("High Scores");
    highScoresButton.setOutlineThickness(2);
    highScoresButton.setOutlineColor(sf::Color::White);
    highScoresButton.setPosition(menuWidth / 2 - 350, menuHeight / 2 - 80);

    //jak grac
    howToPlayButton.setFont(font);
    howToPlayButton.setCharacterSize(72);
    howToPlayButton.setString("How to Play");
    howToPlayButton.setOutlineThickness(2);
    howToPlayButton.setOutlineColor(sf::Color::White);
    howToPlayButton.setPosition(menuWidth / 2 - 350, menuHeight / 2 - 10);

    //wyjscie
    exitButton.setFont(font);
    exitButton.setCharacterSize(72);
    exitButton.setString("Exit");
    exitButton.setOutlineThickness(2);
    exitButton.setOutlineColor(sf::Color::White);
    exitButton.setPosition(menuWidth / 2 - 350, menuHeight / 2 + 60);

    buttons.push_back(newGameButton);
    buttons.push_back(highScoresButton);
    buttons.push_back(howToPlayButton);
    buttons.push_back(exitButton);
    buttons.push_back(title);

    
}

bool Menu::isMouseOnText(const sf::Text& text, const sf::RenderWindow& window) {
    sf::FloatRect textBounds = text.getGlobalBounds();
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    return textBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
}

int Menu::display() {
    while (menuWindow.isOpen()) {
        sf::Event event;
        while (menuWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                menuWindow.close();
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(menuWindow);
                for (size_t i = 0; i < buttons.size(); i++) {
                    sf::FloatRect buttonBounds = buttons[i].getGlobalBounds();
                    if (buttonBounds.contains(sf::Vector2f(mousePos))) {
                        if (i == 0) { //nowa gra
                            Game game;
                            game.start();
                        }
                        else if (i == 1) { //high scory
                            system("start Wyniki.txt");
                        }
                        else if (i == 2) { // zasady
                            system("start Zasady.txt");
                        }
                        else if (i == 3) { // wyjscie
                            menuWindow.close();
                        }
                    }
                }
            }
        }

        for (auto& button : buttons) {
            if (button.getString() != "BLACKJACK") { // ignorowanie "buttona" tytulowego
                if (isMouseOnText(button, menuWindow)) {//zmiana koloru na hoverze
                    button.setFillColor(sf::Color(150, 150, 150));
                }
                else {
                    button.setFillColor(sf::Color::Black);
                }
            }
        }
        menuWindow.draw(backgroundSprite);
        for (const auto& button : buttons) {
            menuWindow.draw(button);
        }
        menuWindow.display();
    }
    return 0;
}

