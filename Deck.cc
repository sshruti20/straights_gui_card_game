/* 
 * To ensure that the your cards are ordered the same way as
 * the given program, use this shuffling algorithm.
 *
 * CARD_COUNT is the constant 52
 * cards_ is an array of pointers to cards
*/

#include <iostream>
#include <vector>
#include <random>
#include "Deck.h"
#include "Card.h"

using namespace std;

Deck::Deck() {
	// make the deck with initial order of cards
	//Represents a Card with a Suit {Club, Diamond, Heart, Spade} and a Rank 
	// * {Ace, 2, 3, 4, 5, 6, 7, 8, 9, T, J, Q, K}.
	for (int i=0; i<4; i++) {		// for each Suit
		for (int j=0; j<13; j++) {	// for each Rank
			Card card = Card( j, i );
			cards_.push_back(card);
		}
	}
}

Deck::~Deck() {		// destructor
	for (auto it=cards_.begin(); it!=cards_.end(); it++) {
		cards_.erase(it);
	}
}

vector<Card> Deck::dealCards(int startingIndex) {		// returns a vector of vectors that have the players hands
	// split the cards up into 4 'hands' -- this method will be called four times
	std::vector<Card> hand;
	for (int j=startingIndex; j<(startingIndex+13); j++) {
		//Model's player's cards are "created" here
		hand.push_back(cards_.at(j));
	}
	return hand;
}

void Deck::initializeSeed(const int seed) {
	this->seed = seed;
}

void Deck::shuffle() {
    static mt19937 rng(seed);

	int n = 52;

	while ( n > 1 ) {
		int k = (int) (rng() % n);
		--n;
		Card c = cards_[n];
		cards_[n] = cards_[k];
		cards_[k] = c;
	} // while
} // shuffle


vector<Card> Deck::cards() {
	return cards_;
}

int Deck::getSeed() const {
	return this->seed;
}