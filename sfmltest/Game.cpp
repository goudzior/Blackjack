#include "Game.h"
#include <stdexcept>
#include <iostream>;
using namespace std;

void Game::updateScores() {
        int dealerValue = dealerHand.getValue();
        if (!revealDealerCard) {
            dealerValue -= dealerHand.getFirstCardValue();
        }
        playerScore.setString("Player: " + to_string(playerHand.getValue()));
        dealerScore.setString("Dealer: " + to_string(dealerValue));
}

void Game::render() {
    window.clear(sf::Color::Green);
    window.draw(backgroundSprite);

    //renderowanie reki gracza
    playerHand.render(window, sf::Vector2f(10, windowHeight - cardHeight - 150));

    // renderowania reki krupiera
    dealerHand.render(window, sf::Vector2f(10, 50), true, revealDealerCard);

    window.draw(message);
    window.draw(playerScore);
    window.draw(dealerScore);
    window.draw(cardCount);
    window.draw(playerMoneyDisplay);
    window.draw(currentBetDisplay);

    for (const sf::Sprite& button : gameButtons) {
        window.draw(button);
    }
    
    window.display();
}

void Game::loadFonts() {
    font.loadFromFile("fonts/CasinoFlat2.ttf");
    //wiadomosci
    message.setFont(font);
    message.setCharacterSize(32);
    message.setPosition(cardWidth-25, windowHeight/2-20);
    message.setOutlineThickness(2);
    message.setOutlineColor(sf::Color::Black);

    //wynik gracza
    playerScore.setFont(font);
    playerScore.setCharacterSize(32);
    playerScore.setPosition(cardWidth - 25, windowHeight - cardHeight - 190);
    playerScore.setOutlineThickness(2);
    playerScore.setOutlineColor(sf::Color::Black);

    //wynik krupiera
    dealerScore.setFont(font);
    dealerScore.setCharacterSize(32);
    dealerScore.setPosition(cardWidth - 25, windowHeight - 570);
    dealerScore.setOutlineThickness(2);
    dealerScore.setOutlineColor(sf::Color::Black);
    revealDealerCard = false;

    //licznik kart
    cardCount.setFont(font);
    cardCount.setCharacterSize(24);
    cardCount.setPosition(windowWidth - 200, 10);
    cardCount.setOutlineThickness(2);
    cardCount.setOutlineColor(sf::Color::Black);

    // bank
    playerMoneyDisplay.setFont(font);
    playerMoneyDisplay.setCharacterSize(24);
    playerMoneyDisplay.setPosition(windowWidth - 200, 35);
    playerMoneyDisplay.setOutlineThickness(2);
    playerMoneyDisplay.setOutlineColor(sf::Color::Black);


    // zak³ad
    currentBetDisplay.setFont(font);
    currentBetDisplay.setCharacterSize(24);
    currentBetDisplay.setPosition(windowWidth - 200, 60);
    currentBetDisplay.setOutlineThickness(2);
    currentBetDisplay.setOutlineColor(sf::Color::Black);


}

Game::Game() {
    playerMoney = 1000;
    currentBet = 0;
    bettingPhase = true;
    bettingInput = false;
    betInputString = "";
    usernameInput = false;
    usernameString = "";
    loadFonts();
    deck = new Deck;
    window.create(sf::VideoMode(windowWidth, windowHeight), "Blackjack");
    backgroundTexture.loadFromFile("images/casinoFelt.jpg");
    backgroundSprite.setTexture(backgroundTexture);
    float scaleX = static_cast<float>(windowWidth) / backgroundTexture.getSize().x;
    float scaleY = static_cast<float>(windowHeight) / backgroundTexture.getSize().y;
    backgroundSprite.setScale(scaleX, scaleY);

    cardCount.setString("Cards left: " + to_string(deck->getSize()));
    for (const std::string& buttonName : buttonNames) {
        std::string filePath = "images/buttons/button_" + buttonName + ".png";
        sf::Texture& texture = buttonTextures[buttonName];
        texture.loadFromFile(filePath);
    }

    float xPosition = cardWidth + 255;
    float yPosition = windowHeight / 2 - 30;
    float spacing = 85.0;

    for (const std::string& buttonName : buttonNames) {
        sf::Sprite buttonSprite;
        buttonSprite.setTexture(buttonTextures[buttonName]);
        buttonSprite.setPosition(xPosition, yPosition);

        gameButtons.push_back(buttonSprite);

        xPosition += spacing;
    }

    
}

Game::~Game()=default;

void Game::start() {
    reset();
    bettingPhase = true;  
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
          
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (size_t i = 0; i < gameButtons.size(); ++i) {
                    if (gameButtons[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        switch (i) {
                        case 0:
                            //bet
                            if (bettingPhase && !bettingInput) {
                                message.setString("\n\nA minimum of 1$ is required\nInput your bet and confirm it with Enter");
                                betInputString = "";  
                                bettingInput = true;
                                playerMoney += currentBet;
                                playerMoneyDisplay.setString("Money: " + std::to_string(playerMoney) + "$");
                                currentBetDisplay.setString("Bet: " + betInputString + "$");
                                
                            }
                            break;
                        case 1:
                            // deal
                            if (bettingPhase && currentBet!=0) {
                                dealInitialCards();
                                bettingPhase = false;
                            }
                            break;
                        case 2:
                            //hit
                            if (!bettingPhase) {
                                playerTurn();
                                cardCount.setString("Cards left: " + to_string(deck->getSize()));
                            }
                            break;
                        case 3:
                            //stand
                            if (!bettingPhase) {
                                dealerTurn();
                            }
                            break;
                        case 4:
                            //cashout
                            if (bettingPhase) {
                                message.setString("\n\nPlease input your username\nConfirm with Enter");
                                usernameString = "";
                                usernameInput = true;
                            }
                            break;
                        default:
                            break;
                        }
                        break;
                    }
                }
            }
            if (bettingInput) {
                playerScore.setString("");
                dealerScore.setString("");
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                        betInputString += static_cast<char>(event.text.unicode);
                        currentBetDisplay.setString("Bet: " + betInputString + "$");

                    }
                    else if (event.text.unicode == '\b' && !betInputString.empty()) {
                        betInputString.pop_back();  // backspace
                        currentBetDisplay.setString("Bet: " + betInputString + "$");
                    }
                    else if (event.text.unicode == '\r') {  // Enter
                        adjustBet();
                        betInputString = "";
                        message.setString("");
                        bettingInput = false;
                    }
                }
            }
            else {
                currentBetDisplay.setString("Bet: " + std::to_string(currentBet) + "$");
                window.draw(currentBetDisplay);
            }
            if (usernameInput) {
                playerScore.setString("");
                dealerScore.setString("");
                if (event.type == sf::Event::TextEntered) {
                    if ((event.text.unicode >= 'A' && event.text.unicode <= 'Z') ||
                        (event.text.unicode >= 'a' && event.text.unicode <= 'z') ||
                        (event.text.unicode >= '0' && event.text.unicode <= '9')) {
                        usernameString += static_cast<char>(event.text.unicode);
                    }
                    else if (event.text.unicode == '\b' && !usernameString.empty()) {
                        usernameString.pop_back();  // backspace
                    }
                    message.setString("\n\nPlease input your username:" + usernameString + "\nConfirm with Enter");
                    if (event.text.unicode == '\r') {  // enter
                        if (!usernameString.empty()) {  // sprawdzej czy nie pusty
                            cashOut();
                            usernameString = "";
                            message.setString("");
                            usernameInput = false;
                        }
                        else {
                            message.setString("\n\nUsername cannot be empty. Please input your username.");
                        }
                    }
                }
            }

        }
        
        
        for (size_t i = 0; i < gameButtons.size(); ++i) {
            // domyslny kolor
            sf::Color defaultColor(255, 255, 255);
            sf::Color highlightColor(100, 100, 100);
            sf::Color inactiveColor(128, 128, 128);

            if (!bettingPhase) {
                if (i < 2) {
                    gameButtons[i].setColor(inactiveColor);
                }
                else if (i == 4) {  // case for cashout button
                    gameButtons[i].setColor(inactiveColor);
                }
                else {
                    gameButtons[i].setColor(defaultColor);
                }
            }
            else {
                switch (i) {
                case 0:
                    gameButtons[i].setColor(defaultColor);
                    break;
                case 1:
                    if (currentBet == 0 || bettingInput) {
                        gameButtons[i].setColor(inactiveColor);
                    }
                    else {
                        gameButtons[i].setColor(defaultColor);
                    }
                    break;
                case 2:
                case 3:
                    gameButtons[i].setColor(inactiveColor);
                    break;
                case 4:
                    gameButtons[i].setColor(defaultColor);
                    break;
                }

            }

            if (isMouseOnButton(window, i) && gameButtons[i].getColor() != inactiveColor) {
                gameButtons[i].setColor(highlightColor);
            }
        }

        render();

    }

    

}


void Game::waitForInput() {
    bool inputReceived = false;
    while (!inputReceived) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    inputReceived = true;
                    break;
                }
            }
        }
    }
}

void Game::delay(float seconds) {
    sf::Clock delayClock;
    while (delayClock.getElapsedTime().asSeconds() < seconds) {
        // czekaj x sekund
    }
}

void Game::evaluateGameOutcome() {
    int playerValue = playerHand.getAbsoluteValue();
    int dealerValue = dealerHand.getAbsoluteValue();

    if (playerValue == dealerValue) {
        message.setString("Push.\n\nPress Enter to restart.");
        playerMoney += currentBet;
    }
    else if (playerValue == maxPoints || (playerValue < maxPoints && dealerValue > maxPoints)) {
        message.setString("You win!\n\nPress Enter to restart.");
        playerMoney += currentBet*2;
    }
    else if (dealerValue == maxPoints || (dealerValue < maxPoints && playerValue > maxPoints)) {
        message.setString("You lose.\n\nPress Enter to restart.");

    }
    else if (playerValue < dealerValue) {
        message.setString("You lose.\n\nPress Enter to restart.");

    }
    else {
        message.setString("You win!\n\nPress Enter to restart.");
        playerMoney += currentBet * 2;
    }


    render(); 
    delayClock.restart();
    delay(2);
}

void Game::reset() {
    if (deck->getSize() != 104) {
        delete deck;
        deck = new Deck;
    }
    currentBet = 0;
    revealDealerCard = false;
    bettingPhase = true;
    playerHand.clear();
    dealerHand.clear();
    deck->shuffle();
 
    playerScore.setString("");
    dealerScore.setString("");
    message.setString("");
    cardCount.setString("Cards left: " + std::to_string(deck->getSize()));
    playerMoneyDisplay.setString("Money: " + std::to_string(playerMoney) + "$");
    currentBetDisplay.setString("Bet: " + std::to_string(currentBet) + "$");

    if (playerHand.getAbsoluteValue() == maxPoints) {
        revealDealerCard = true;
        updateScores(); 
        render();
        delay(0.5);
        evaluateGameOutcome();
        delay(1); 
    }
}

void Game::playerTurn() {
    cardCount.setString("Cards left: " + to_string(deck->getSize()));
    bool success = playerHand.draw(deck->draw());
    if (!success) {
        dealerTurn();
    }
    updateScores();
    render();
    if (playerHand.getAbsoluteValue() > maxPoints) {
        delay(0.5);
        revealDealerCard = true;
        updateScores();
        evaluateGameOutcome();
        waitForInput();
        reset();
        render();
    }

    else if (playerHand.getAbsoluteValue() == maxPoints) {
        delay(0.5);
        revealDealerCard = true;
        updateScores();
        evaluateGameOutcome();
        waitForInput();
        reset();
        render();
    }
    
}

void Game::dealerTurn() {
    revealDealerCard = true;
    updateScores();
    gameButtons[2].setColor(sf::Color(128, 128, 128));
    gameButtons[3].setColor(sf::Color(128, 128, 128));
    render();
    delay(0.5); 

    
    while ((dealerHand.getAbsoluteValue() < 17 || (dealerHand.getAbsoluteValue() == 17 && dealerHand.getFirstCardValue() == 11)) && dealerHand.size() < 10) {
        delay(0.5);
        dealerHand.draw(deck->draw());
        cardCount.setString("Cards left: " + to_string(deck->getSize()));
        updateScores();
        render();
        delay(0.5);
    }


    evaluateGameOutcome();
    if (playerMoney <= 0) {
        message.setString("\n\nGame Over!\nPress Enter to exit.");
        render();
        waitForInput();
        window.close();
        return;
    }

    waitForInput();
    reset();
    render();
}

void Game::dealInitialCards(){
    playerHand.draw(deck->draw());
    dealerHand.draw(deck->draw());
    playerHand.draw(deck->draw());
    dealerHand.draw(deck->draw());
    updateScores();
    bettingPhase = false;
    cardCount.setString("Cards left: " + to_string(deck->getSize()));
    render();
}

void Game::adjustBet() {
    int newBet = 0;
    if (!betInputString.empty()) {
        try {
            newBet = std::stoi(betInputString);
        }
        catch (...) {
            newBet = 0;  
        }
    }
    if (newBet >= 1 && newBet <= playerMoney) {
        currentBet = newBet;
        playerMoney -= newBet;  
        playerMoneyDisplay.setString("Money: " + std::to_string(playerMoney) + "$");  
    }
}


bool Game::isMouseOnButton(const sf::RenderWindow& window, size_t buttonIndex) const {
    if (buttonIndex >= gameButtons.size()) {
        return false; //bledny index
    }

    sf::FloatRect buttonBounds = gameButtons[buttonIndex].getGlobalBounds();
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    return buttonBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
}

void Game::cashOut() {
    std::string filename = "Wyniki.txt";

        // odczytaj wyniki w pliku
        std::vector<CashoutRecord> records = readCashoutRecords(filename);

        // dodaj nowy wynik
        records.push_back({ usernameString, playerMoney, getCurrentDate() });

        // posortuj
        std::sort(records.begin(), records.end(), [](const CashoutRecord& a, const CashoutRecord& b) {
            return a.winnings > b.winnings;
            });

        // zapisz to do pliku
        writeCashoutRecords(filename, records);
  
    playerScore.setString("");
    dealerScore.setString("");
    message.setString("\n\nCashout successful!\nThanks for playing!\nPress enter to exit.");
    render();
    waitForInput();
    window.close();
}

std::string Game::getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    char date[100];
    std::tm tm;
    localtime_s(&tm, &now_time); // Use localtime_s instead of localtime
    std::strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", &tm); // Pass pointer to tm
    return std::string(date);
}

