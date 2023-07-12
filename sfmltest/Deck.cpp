#include "Deck.h"
#include <array>
#include <random>
#include <algorithm>
#include <iostream>

using namespace std;

Deck::Deck() {
    array<string, 4> suits = { "hearts", "diamonds", "clubs", "spades" };
    array<string, 13> ranks = { "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "jack", "queen", "king" };
    array<int, 13> values = { 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 };
    for (const auto& suit : suits) {
        for (int j = 0; j < 13; j++) {
            Card card;
            card.suit = suit;
            card.rank = ranks[j];
            card.value = values[j];

            string textureKey = card.rank + "_of_" + card.suit;
            textures[textureKey].loadFromFile("images/deck/" + textureKey + ".png");

            cout << "Loading card: " << textureKey << endl; //debug

            card.sprite.setTexture(textures[textureKey]);
            cards.push_back(card);
        }
        for (int i = 0; i < 13; i++) {
            Card card;
            card.suit = suit;
            card.rank = ranks[i];
            card.value = -values[i];


            string textureKey = card.rank + "_of_" + card.suit + "_neg";
            textures[textureKey].loadFromFile("images/deck/negCards/" + textureKey + ".png");

            cout << "Loading card: " << textureKey << endl;// debug

            card.sprite.setTexture(textures[textureKey]);
            cards.push_back(card);
        }
    }

    cout << "Deck created with " << getSize() << " cards." << endl;
}

void Deck::shuffle() {
    static std::default_random_engine generator(std::random_device{}());
    std::shuffle(cards.begin(), cards.end(), generator);
}
Card Deck::draw() {
    Card card = cards.back();
    cards.pop_back();
    return card;
}

int Deck::getSize() const {
    return cards.size();
}