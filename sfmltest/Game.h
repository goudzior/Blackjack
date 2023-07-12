#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <ctime>
#include <fstream>
#include "CashoutRecord.h"
#include "Deck.h"
#include "Hand.h"
#include "constants.h"

class Game {
public:
    // konstruktor i destruktor
    Game();
    ~Game();

    // metody przebiegu gry
    void start();
    void reset();
    void waitForInput();
    void delay(float seconds);

    // metoda sprawdzajaca czy myszka na przycisku
    bool isMouseOnButton(const sf::RenderWindow& window, size_t buttonIndex) const;

    // cashout i sprawdzanie wyniku rundy
    void evaluateGameOutcome();
    void cashOut();

    // metoda pomocnicza do cashout
    std::string getCurrentDate();

private:
    // SFML grafika
    sf::RenderWindow window;
    sf::Font font;
    sf::Text message;
    sf::Text playerScore;
    sf::Text dealerScore;
    sf::Text cardCount;
    sf::Clock delayClock;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::vector<sf::Sprite> gameButtons;
    std::map<std::string, sf::Texture> buttonTextures;
    std::vector<std::string> buttonNames = { "bet", "deal", "hit", "stand", "cashout" };

    // obiekty gry
    Deck* deck;
    Hand playerHand;
    Hand dealerHand;

    // zmienne gracza
    bool revealDealerCard;
    sf::Text playerMoneyDisplay;
    sf::Text currentBetDisplay;
    bool bettingPhase;
    bool bettingInput;
    std::string betInputString;
    int playerMoney;
    int currentBet;
    bool usernameInput;
    std::string usernameString;

    // prywatne metody do przebiegu gry
    void playerTurn();
    void dealerTurn();
    void dealInitialCards();

    // update wynikow
    void updateScores();

    // renderowanie grafik
    void render();

    // ladowanie stanu poczatkowego gry
    void loadFonts();

    // zmiany betów
    void adjustBet();
};
