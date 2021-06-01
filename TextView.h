#ifndef _TEXTVIEW_
#define _TEXTVIEW_

#include <iostream>
#include "View.h"
#include "Command.h"
#include "Deck.h"
#include "Player.h"

class Controller;
class Model;

class TextView : public Observer {
public:
    TextView();
	virtual ~TextView();

	void showGetCommandSign();

	void showGetPlayerTypeMessage(int playerIndex);

	void showIllegalPlayMessage();

	void showCannotDiscardMessage();

	void showWinners(std::vector<Player*> &winners);

	void showRagequitMessage(Player* player);

	void showPlayedCard(Player* player, Card* card);

	void showDiscardedCard(Player* player, Card* card);

	void showCurrentStateOfTable(std::vector<Card> &cards_on_table, std::vector<Card> &hand, std::vector<Card> &legalplays);

	void showDiscardsAndScores(Player* player);

	void showDeck(Deck deck);

	void showRoundBeginMessage(Player* player);

	void update();

};

#endif
