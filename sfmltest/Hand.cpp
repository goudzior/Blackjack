#include "Hand.h"
#include "constants.h"

Hand::Hand() {
    cardBackTexture.loadFromFile("images/cardBack.png");
    cardBackSprite.setTexture(cardBackTexture);
}

int Hand::getValue() {
    int value = 0;
    int aceCount = 0;
    int negativeAceCount = 0;
    for (int i = 0; i < cards.size(); i++) {
        value += cards[i].value;
        if (cards[i].rank == "Ace") {
            if (cards[i].value > 0) {
                aceCount++;
            }
            else {
                negativeAceCount++;
            }
        }
    }
    while (aceCount > 0 && value > maxPoints) {
        value -= 10;
        aceCount--;
    }
    while (negativeAceCount > 0 && value < -maxPoints) {
        value += 10;
        negativeAceCount--;
    }
    return value;
}

int Hand::getAbsoluteValue() {
    return abs(getValue());
}

bool Hand::draw(Card card) {
    if (cards.size() >= 10) {
        return false;
    }
    cards.push_back(card);
    return true;
}

void Hand::clear() {
    cards.clear();
}
void Hand::render(sf::RenderWindow& window, const sf::Vector2f position, bool isDealer, bool revealDealerCard) {
    for (int i = 0; i < cards.size(); i++) {
        if (isDealer && i == 0 && !revealDealerCard) {
            cardBackSprite.setPosition(position.x + i * cardWidth, position.y);
            window.draw(cardBackSprite);
        }
        else {
            cards[i].sprite.setPosition(position.x + i * cardWidth, position.y);
            window.draw(cards[i].sprite);
        }
    }
}

int Hand::getFirstCardValue() const {
    if (!cards.empty()) {
        return cards[0].value;
    }
    return 0;
}

int Hand::size() const {
    return cards.size();
}