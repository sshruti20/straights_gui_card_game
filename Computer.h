#ifndef _COMPUTER_H
#define _COMPUTER_H

#include <iostream>
#include "Player.h"
#include "Command.h"

class Computer : public Player {
	public:
	Computer(std::vector<Card> hand, std::vector<Card> * table_cards, unsigned int player_id, unsigned int score);
	void playCard();
	void discardCard();
	Command makeComputerMove();		// makes an AI move

	virtual Command makeMove() override;
	 bool isHuman() const override;
	 void rageQuit() override;

	virtual Player::PlayerType getPlayerType() const override;
};

#endif
