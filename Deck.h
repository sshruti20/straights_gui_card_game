#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <random>
#include <vector>
#include <memory>

class Deck {
private:
	int seed = 0;
	std::vector<Card> cards_;

public:
    static const unsigned int CARD_COUNT = 52;
	Deck();
	~Deck();
	std::vector<Card> dealCards(int startingIndex);
	void shuffle();
	void initializeSeed(const int seed);
	std::vector<Card> cards();
	int getSeed() const;
};


#endif
