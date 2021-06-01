#include "Computer.h"

using namespace std;

Computer::Computer(std::vector<Card> hand, std::vector<Card> * table_cards, unsigned int player_id, unsigned int score = 0) : Player::Player(hand, table_cards, player_id, score) {}

Command Computer::makeComputerMove() {			// Can make a smarter AIs
	Player::calculateCurrentLegalMoves();
	Command c;
	if (legal_cards_.size() > 0 ) {
		c.type = Command::Type::PLAY;
		c.card = legal_cards_.at(0);
        this->playCard();
	}
	else {
		c.type = Command::Type::DISCARD;
		c.card = hand_.at(0);
        this->discardCard();
	}

	return c;
}


Command Computer::makeMove() {
	return this->makeComputerMove();
}

void Computer::playCard() {
    // Make smarter AI
    // Choose the card with highest rank
    Card highestRankCard = legal_cards_.at(0);
    vector<Card>::iterator it = legal_cards_.begin();
    for(; it != legal_cards_.end(); it++) {
        if ( (*it).rank().rank() > highestRankCard.rank().rank() ) {
            highestRankCard = *it;
        }
    }
    Player::playCard(highestRankCard);
}

void Computer::discardCard() {
    //Choose the card with lowest rank and play it
    Card lowestRankCard = hand_.at(0);
    vector<Card>::iterator it = hand_.begin();
    for(; it != hand_.end(); it++) {
        if ( (*it).rank().rank() < lowestRankCard.rank().rank() ) {
            lowestRankCard = *it;
        }
    }
    Player::discardCard(lowestRankCard);
}

Player::PlayerType Computer::getPlayerType() const {
	return Player::PlayerType::computer;
}

bool Computer::isHuman() const {
	return false;
}

void Computer::rageQuit() {
}
