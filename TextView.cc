#include "TextView.h"
#include <algorithm>

using namespace std;

TextView::TextView() {};

TextView::~TextView() {};

void TextView::showGetCommandSign() {
    cout << ">";
}

void TextView::showGetPlayerTypeMessage(int playerIndex) {
	cout << "Is player " << (playerIndex+1) << " a human(h) or a computer(c)?\n";
	this->showGetCommandSign();
}

void TextView::showIllegalPlayMessage() {
	cout << "This is not a legal play.\n";
}

void TextView::showCannotDiscardMessage() {
	cout << "You have a legal play. You may not discard.\n";
}

void TextView::showWinners(vector<Player*> &winners) {
	// the first winner, all other winners will start with a \n character
	cout << "Player " << winners.at(0)->player_id() << " wins!";

	// if there's multiple winners
	for (int i=1; i<winners.size(); i++) {
		cout << "\nPlayer " << winners.at(i)->player_id() << " wins!";
	}
}

void TextView::showRagequitMessage(Player* player) {
	cout << "Player " << player->player_id() << " ragequits. A computer will now take over.\n";
	// tell controller to change this player into a computer player
}

void TextView::showPlayedCard(Player* player, Card * card) {
	cout << "Player " << player->player_id() << " plays " << card->rank() << card->suit() << "." << '\n';
}

void TextView::showDiscardedCard(Player* player, Card* card) {
	cout << "Player " << player->player_id() << " discards " << card->rank() << card->suit() << "." << '\n';
}

void TextView::showCurrentStateOfTable(std::vector<Card> &cards_on_table, std::vector<Card> &hand, std::vector<Card> &legalplays) {
	//divide the cards_on_table into four vectors, one for each suit where Suit {Club, Diamond, Heart, Spade}
	vector<Card *> clubs;
	vector<Card *> diamonds;
	vector<Card *> hearts;
	vector<Card *> spades;
	for (int i=0; i<cards_on_table.size(); i++) {
		int currentCardSuit = (cards_on_table.at(i)).suit().suit();
		if (currentCardSuit == 0) {		// Clubs
			clubs.push_back(&cards_on_table.at(i));
		}
		else if (currentCardSuit == 1) {	// Diamonds
			diamonds.push_back(&cards_on_table.at(i));
		}
		else if (currentCardSuit == 2) {	// Hearts
			hearts.push_back(&cards_on_table.at(i));
		}
		else if (currentCardSuit == 3) {		// Spades
			spades.push_back(&cards_on_table.at(i));
		}
	}

	std::sort(clubs.begin(), clubs.end(), ([] (Card* a, Card* b) { return a->rank().rank() < b->rank().rank(); }) );
	std::sort(diamonds.begin(), diamonds.end(), ([] (Card* a, Card* b) { return a->rank().rank() < b->rank().rank(); }) );
	std::sort(hearts.begin(), hearts.end(), ([] (Card* a, Card* b) { return a->rank().rank() < b->rank().rank(); }) );
	std::sort(spades.begin(), spades.end(), ([] (Card* a, Card* b) { return a->rank().rank() < b->rank().rank(); }) );

	cout << "Cards on the table:\n";
	cout << "Clubs:";
	for (int i=0; i<clubs.size(); i++) {
		cout << " " << clubs.at(i)->rank();
	}
	cout << "\nDiamonds:";
	for (int i=0; i<diamonds.size(); i++) {
		cout << " " << diamonds.at(i)->rank();
	}
	cout << "\nHearts:";
	for (int i=0; i<hearts.size(); i++) {
		cout << " " << hearts.at(i)->rank();
	}
	cout << "\nSpades:";
	for (int i=0; i<spades.size(); i++) {
		cout << " " << spades.at(i)->rank();
	}
	cout << "\nYour hand:";
	for (int i=0; i<hand.size(); i++) {
		cout << " " << hand.at(i).rank() << hand.at(i).suit();
	}
	cout << "\nLegal plays:";
	for (int i=0; i<legalplays.size(); i++) {
		cout << " " << legalplays.at(i).rank() << legalplays.at(i).suit();
	}
	cout << '\n';

	clubs.clear();
	diamonds.clear();
	hearts.clear();
	spades.clear();
	legalplays.clear();
}

void TextView::showDiscardsAndScores(Player* player) {
	std::cout << "Player " << player->player_id() << "'s discards:";

	for (int i=0; i<player->getDiscardedSize(); i++) {
		std::cout << " " << player->discardedAt(i).rank() << player->discardedAt(i).suit();
	}

	int score_gained = player->getScoreForCurrentRound();

	std::cout << '\n';
	std::cout << "Player " << player->player_id() << "'s score: " << player->score() << " + " 
		<< score_gained << " = " << (player->score() + score_gained) << '\n';
	player->updateScore();
}

void TextView::showDeck(Deck deck) {
	for (int i=0; i<4; i++) {	// we display four rows
        for (int j=0; j<13; j++) {		// each row that is displayed shows 13 cards 
            cout << deck.cards().at((i*13)+j) << " ";
        }
        cout << '\n';
    }
}

void TextView::showRoundBeginMessage(Player* player) {
	cout << "A new round begins. It's player " << player->player_id() << "'s turn to play.\n";
}

void TextView::update() {
}
