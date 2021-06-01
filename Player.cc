#include <iostream>
#include "Player.h"
#include "Card.h"

using namespace std;

Player::Player(std::vector<Card> hand, std::vector<Card> * table_cards, unsigned int player_id, unsigned int score = 0){
    
	hand_ = hand;
	model_table_cards_ = table_cards;
	has_rage_quitted_ = false;
	player_id_ = player_id;
	score_ = score;
	calculateCurrentLegalMoves();
}

bool Player::isPlayingCardLegal(Card card) const {
	int rank = card.rank().rank();
	int suit = card.suit().suit();

	// IF it is the start of a new round, then ONLY 7S is legal:
	if (model_table_cards_->size() == 0) {
		if (rank == 6 && suit == 3) {
			return true;
		}
		return false;
	}

	// IF the card has already been played, it shouldn't show up in legal_cards
	auto i = played_.begin();
	while (i != played_.end()) {
		if ( ((*i).rank().rank() == rank) && ((*i).suit().suit() == suit) ) {
			return false;
		}
		i++;
	}
	

	// A card of rank 7 is always legal
	if( rank == 6 ) {
		return true;
	}

	vector<Card>::iterator it;
	for (it = model_table_cards_->begin(); it != model_table_cards_->end(); it++) {
		Card temp = (*it);
		// Card is legal play if same suit + adjacent rank
		if ( ((rank == (temp.rank().rank() - 1)) || (rank == (temp.rank().rank() + 1))) && suit == temp.suit().suit() ) {
			return true;
		}
	}
	return false;
}

unsigned int Player::score() const {
	return score_;
}

unsigned int Player::player_id() const {
	return player_id_ + 1; //player_id_;
}

//Precondition: card belongs to players hand
void Player::playCard(Card card) {

	//Add card to played pile for this player object
	played_.push_back(card);

	//Add card to cards on table for Model
	model_table_cards_->push_back(card);
    
    //Remove card from this player object's hand
    vector<Card>::iterator it;
    for ( it = hand_.begin(); it != hand_.end(); it++) {
        Card c = *it;
        if (c == card) {
            hand_.erase(it);
            break;
        }
    }
    
	//Remove card from this player's legal_cards_
	it = legal_cards_.begin();
	while (it != legal_cards_.end()) {
		if ((*it) == card) {
			legal_cards_.erase(it);
			break;
		}
		it++;
	}
    
    cout<<"Player "<<player_id_ + 1<<" has played card "<<card<<endl;
}

//Assuming it is legal to discard parameter 'card'
void Player::discardCard (Card card) {

	//Add card to discarded pile
	discarded_.push_back(card);

	std::vector<Card> newHand;
	// the new hand is the old hand minus the card that was just discarded
    vector<Card>::iterator it = hand_.begin();
	while (it != hand_.end()) {
		if ( ((*it).rank().rank() != card.rank().rank()) || ((*it).suit().suit() != card.suit().suit()) ) {
			newHand.push_back(*it);
		}
		it++;
	}
	hand_.clear();
	hand_ = newHand;
    
    cout<<"Player "<<player_id_ + 1<<" has discarded card "<<card<<endl;
}

void Player::calculateCurrentLegalMoves() {

	//Iterate through all cards in hand, and add to legal_cards_ if they are legal
	//Cards will be pushed in the same order as they are in hand
    vector<Card> temp;
	vector<Card>::iterator it = hand_.begin();
	for(;it != hand_.end(); it++) {
		if (isPlayingCardLegal(*it)) {
			temp.push_back(*it);
		}
	}
    
    legal_cards_ = temp;
}

void Player::updateScore() {
		int score = 0;
		vector<Card>::iterator it;
		for (it = discarded_.begin(); it != discarded_.end(); it++) {
			score += ((*it).rank().rank()+1);
		}
		score_ += score;
}

bool Player::containsSevenOfSpades() const {
	for(auto it = hand_.begin(); it != hand_.end(); it++ ) {
		Card c = *it;
		if (c.rank().rank() == 6 && c.suit().suit() == 3) {
			return true;
		}
	}
	return false;
}

void Player::clearHand(std::vector<Card> hand) {
	legal_cards_.clear();
	hand_.clear();
	played_.clear();
	discarded_.clear();

	//Assigning the new hand to hand_
	hand_ = hand;
}

void Player::clearEntirePlayerHand() {
	legal_cards_.clear();
	hand_.clear();
	played_.clear();
	discarded_.clear();
}

int Player::getScoreForCurrentRound() {
	int sum = 0;
	vector<Card> ::iterator it;
	for(it = discarded_.begin(); it != discarded_.end(); it++) {
        sum += ((*it).rank().rank()+1) ;
	}
	return sum;
}

int Player::getDiscardedSize() const {
    return discarded_.size();
}

int Player::getLegalCardsSize() const {
    return legal_cards_.size();
}

Card Player::discardedAt(int i) const {
    return discarded_.at(i);
}

Card Player::playedAt(int i) const {
    return played_.at(i);
}

bool Player::hasRageQuitted() {
    return has_rage_quitted_;
}

std::vector<Card> Player::getLegalCards() {
    vector<Card> legal;
    vector<Card>::iterator it = legal_cards_.begin();
    for(;it != legal_cards_.end(); it++) {
        legal.push_back((*it));
    }
    return legal;
}

std::vector<Card> Player::getHandCards() {
    vector<Card> hand;
    vector<Card>::iterator it = hand_.begin();
    for(;it != hand_.end(); it++) {
        hand.push_back(*it);
    }
    return hand;
}

std::vector<Card> Player::getPlayedCards() {
	return this->played_;
}

bool Player::isDiscardAllowed() {
    calculateCurrentLegalMoves();
    if (legal_cards_.size() > 0 ) {
        return false;
    }
    else {
        return true;
    }
}
