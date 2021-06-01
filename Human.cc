#include "Human.h"

using namespace std;

Human::Human(std::vector<Card> hand, std::vector<Card> * table_cards, unsigned int player_id, unsigned int score = 0) : Player::Player(hand, table_cards, player_id, score) { }

Command Human::makeRageQuitMove() {
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

Command Human::makeMove() {
	return this->makeRageQuitMove();
}

void Human::rageQuit() {
	has_rage_quitted_ = true;
}

void Human::playCard(Card card) {
	Player::playCard(card);
}

void Human::discardCard(Card card) {
	Player::discardCard(card);
}

//overrided methods for after rageQuit:
void Human::playCard() {
	Player::playCard(legal_cards_.at(0));
}

void Human::discardCard() {
	Player::discardCard(hand_.at(0));
}

bool Human::hasRageQuitted() {
	return has_rage_quitted_;
}

Player::PlayerType Human::getPlayerType() const {
	return Player::PlayerType::human;
}

bool Human::isHuman() const {
	if (this->has_rage_quitted_) {
		return false;
	}
	else {
		return true;
	}
}
