#include "Controller.h"
#include "Model.h"

using namespace std;

Controller::Controller(Model *m, TextView *v) : model_(m), view_(v) {}

// upon recieving a command from the View (the user's command) we will call the appropriate method in Model
bool Controller::shouldCommandBeActedUpon(Command & cmd, int playerIndex) {

    Player * current_player = this->model_->player_list_.at(playerIndex);

    //This will populate the legal cards deck of the player
    current_player->calculateCurrentLegalMoves();

	if (cmd.type == Command::Type::PLAY) {
		// if this is a valid card being played for the current player, then make the change and tell model to update the view with a "played card" setting
		if (current_player->isPlayingCardLegal( (cmd.card) )) {
			// actually play the card (remove from player's hand, add to played_cards_on_table)
			current_player->playCard( (cmd.card) );
			// tell model to update the view with a successful card played message
			this->view_->showPlayedCard(current_player, &(cmd.card));
		}
		// if this is not legal for the current player, then don't make a change in player, and update the view with a "illegal command - get input again" setting
		else {
			this->view_->showIllegalPlayMessage();
			return false;
		}
	}
	else if (cmd.type == Command::Type::DISCARD) {
		// do something else
		if (this->model_->player_list_.at(playerIndex)->getLegalCardsSize() == 0) {	// discard is only legal if no legal plays are possible
			this->model_->player_list_.at(playerIndex)->discardCard( (cmd.card) );
			this->view_->showDiscardedCard(this->model_->player_list_.at(playerIndex), &(cmd.card));
		}
		else {		// should not be discarding
			this->view_->showCannotDiscardMessage();
			return false;
		}
	}
	else if (cmd.type == Command::Type::DECK) {
		this->view_->showDeck((this->model_->deck_));
		return false;
	}
	else if (cmd.type == Command::Type::QUIT) {
		// this->model_->notify("quit");
		exit(0);
	}
	else if (cmd.type == Command::Type::RAGEQUIT) {
		this->model_->player_list_.at(playerIndex)->rageQuit();
		this->view_->showRagequitMessage(this->model_->player_list_.at(playerIndex));
		// return true;
	}
	else {		// cmd.type == BAD_COMMAND
		// something went wrong
		return false;
	}
	return true;
}

void Controller::automaticMoveHandler(Command move, int playerIndex) {
	// can handle this logic here since Computer player can only make these two legal moves (two cases)
	if (move.type == Command::Type::PLAY) {
		// tell model to update the view with a successful card played message
		this->view_->showPlayedCard(this->model_->player_list_.at(playerIndex), &(move.card));
	}
	else {
		this->view_->showDiscardedCard(this->model_->player_list_.at(playerIndex), &(move.card));
	}
}

// Assuming this function is called to execute one round in the Text-Based game
void Controller::gameLogic(bool firstTime) {
	int startingPlayerIndex = 0;

	// find the first player
	for (int i=0; i<4; i++) {
		if (this->model_->player_list_.at(i)->containsSevenOfSpades()) {
			startingPlayerIndex = i;
			break;
		}
	}

	this->view_->showRoundBeginMessage(this->model_->player_list_.at(startingPlayerIndex));

	// there are at most 13 turns per round, depending on whether the user uses the Quit command or not
	for (int i=0; i<13; i++) {
        int k;
		for (int j=startingPlayerIndex, k=0; k<4; j++, k++) {
            this->model_->player_list_.at(j%4)->calculateCurrentLegalMoves();

            // Human + non rage quit player
            if ( (this->model_->player_list_.at( (j%4) )->isHuman()) && (this->model_->player_list_.at( (j%4) )->hasRageQuitted() == false) ) {
				Command* cmd = new Command();
                vector<Card> legc = this->model_->player_list_.at(j%4)->getLegalCards();
                vector<Card> hand = this->model_->player_list_.at(j%4)->getHandCards();

                vector<Card> table = this->model_->getCardsOnTable();
				this->view_->showCurrentStateOfTable(table, hand, legc);

				// weird issue for the first user input, to fix it we use a flag:
				if (firstTime) {
					firstTime = false;
					std::cin.ignore();
				}

				do {
					this->view_->showGetCommandSign();
					std::cin >> *cmd;
				}
                // Fix for when trying to make illegal move
				while (!shouldCommandBeActedUpon( *cmd, (j%4) ));

				// special case for first ragequit move
				if (this->model_->player_list_.at( (j%4) )->hasRageQuitted() == true) {
					this->model_->player_list_.at(j%4)->calculateCurrentLegalMoves();
					Command move = this->model_->player_list_.at( (j%4) )->makeMove();
					automaticMoveHandler(move, (j%4));
				}
			}

			else {	// computer
				Command move = this->model_->player_list_.at( (j%4) )->makeMove();
				automaticMoveHandler(move, (j%4));
			}
		}
	}

	// update scores of all the players
	for (int i=0; i<4; i++) {
		// tell view to show each player's scores which will also update each player's score
		this->view_->showDiscardsAndScores(this->model_->player_list_.at(i));
	}
}

void Controller::runGame(int input_seed) {
	if (input_seed != 0) {		// if user entered a specific seed for shuffle
		this->model_->deck_.initializeSeed(input_seed);
	}

	// shuffle and then deal cards
	this->model_->deck_.shuffle();

	for (int i=0; i<4; i++) {		// initialize the players
		// deal cards for the hand of this player
		std::vector<Card> hand = this->model_->deck_.dealCards(i*13);

		std::string input;
		this->view_->showGetPlayerTypeMessage(i);		// to show the View's get player types message
		std::cin >> input;

		if (input == "h") {
			this->model_->player_list_.push_back( new Human(hand, &(this->model_->cards_on_table_), i, 0) );
		}
		else {		// "c"
			this->model_->player_list_.push_back( new Computer(hand, &(this->model_->cards_on_table_), i, 0) );
		}
	}

	gameLogic(true);

	// if more than one round is needed:
	bool gameIsRunning = true;

	// check if game should continue
	for (int i=0; i<4; i++) {
		if (this->model_->player_list_.at(i)->score() >= 80) {
			gameIsRunning = false;
			break;
		}
	}

	while (gameIsRunning) {		// will run for as many rounds are necessary
		this->model_->clearTableCards();		// reset the table

		// shuffle and then deal cards
		this->model_->deck_.shuffle();

		for (int i=0; i<4; i++) {		// initialize the players
			// deal cards for the hand of this player
			vector<Card> hand = this->model_->deck_.dealCards(i*13);
			this->model_->player_list_.at(i)->clearHand(hand);		// clears old hand and sets hand to the new hand
		}

		gameLogic(false);

		// check if game should continue
		for (int i=0; i<4; i++) {
			if (this->model_->player_list_.at(i)->score() >= 80) {
				gameIsRunning = false;
				break;
			}
		}
	}

	// game is over

	// find winner(s)
	int bestScore = this->model_->player_list_.at(0)->score();		// the lowest score is the best
	for (int i=1; i<4; i++) {
		// find the best score
	    if (this->model_->player_list_.at(i)->score() < bestScore) {
	        bestScore = this->model_->player_list_.at(i)->score();
	    }
	}

	// make a vector of all the winners, multiple winners if multiple lowest scores
	std::vector<Player *> winners;
	for (int i=0; i<4; i++) {
	    if (this->model_->player_list_.at(i)->score() == bestScore) {
	        winners.push_back(this->model_->player_list_.at(i));
	    }
	}

	this->view_->showWinners(winners);
}

void Controller::shuffleDeck(int input_seed) {
	if (input_seed != 0) {		// if user entered a specific seed for shuffle
		this->model_->deck_.initializeSeed(input_seed);
	}

	// shuffle and then deal cards
	this->model_->deck_.shuffle();
}

void Controller::dealCardsAndInitializePlayers(bool isComputer[4]) {
	this->model_->resetModelForNewGame();

	for (int i=0; i<4; i++) {	// initialize the players
		// deal cards for the hand of this player
		std::vector<Card> hand = this->model_->deck_.dealCards(i*13);

		if (isComputer[i] == false) {
			this->model_->player_list_.push_back( new Human(hand, &(this->model_->cards_on_table_), i, 0) );
		}
		else {		// "c"
			this->model_->player_list_.push_back( new Computer(hand, &(this->model_->cards_on_table_), i, 0) );
		}
	}
}

void Controller::dealCardsAndClearPlayersHands() {
	this->model_->resetModelForNewRound();
	for (int i=0; i<4; i++) {
		// deal cards for the new hand of this player
		std::vector<Card> hand = this->model_->deck_.dealCards(i*13);
		(this->model_->player_list_.at(i))->clearHand(hand);
	}
}

void Controller::startNewRound() {
	// find the first player
	for (int i=0; i<this->model_->player_list_.size(); i++) {
		// print the hand
		cout << "Player " << i << ": ";
		std::vector<Card> hand = (this->model_->player_list_.at(i))->getHandCards();
		for (int j=0; j<hand.size(); j++) {
			cout << hand.at(j) << " ";
		}
		cout << "\n";
		if (this->model_->player_list_.at(i)->containsSevenOfSpades()) {
			this->model_->current_player_index_ = i;			// sets the current_player_index_ in Model
			cout << "current_player_index_: " << this->model_->current_player_index_ << endl;
			break;
		}
	}

	this->model_->notifyObservers();
	// tell Model to notify GameView that it should display the current player's hand
		// IF that player is a human then it will wait for input
		// ELSE GameView will find out the move based on Controller's logic for Computer moves and Model will be told that it needs to notify again
	// After that, GameView will ask Controller to increment the current_player_index_ and then notify GameView again
}

bool Controller::isRoundOver() const {
	// Round is over when all players have no more cards in their hands:
    for (int i=0; i<this->model_->player_list_.size(); i++) {
        if ( this->model_->player_list_.at(i)->getHandCards().size() > 0 ) {
            return false;
        }
    }
    return true;
}

bool Controller::isGameOver() const {
	// Game is over when a player has more than 80 points
	for (int i=0; i<this->model_->player_list_.size(); i++) {
		if (this->model_->player_list_.at(i)->score() >= 80) {
			return true;
		}
	}
	return false;
}

std::vector<Player *> Controller::getWinners() {
	// find winner(s)
	int bestScore = this->model_->player_list_.at(0)->score();		// the lowest score is the best
	for (int i=1; i<4; i++) {
		// find the best score
	    if (this->model_->player_list_.at(i)->score() < bestScore) {
	        bestScore = this->model_->player_list_.at(i)->score();
	    }
	}

	// make a vector of all the winners, multiple winners if multiple lowest scores
	std::vector<Player *> winners;
	for (int i=0; i<4; i++) {
	    if (this->model_->player_list_.at(i)->score() == bestScore) {
	        winners.push_back(this->model_->player_list_.at(i));
	    }
	}

	return winners;	
}

void Controller::quitGame() const {		// Quits the game
	std::exit(0);
}

void Controller::updatePlayersScores() {	// Updates the scores of the players
	for (int i=0; i<this->model_->player_list_.size(); i++) {
		(this->model_->player_list_.at(i))->updateScore();
	}
}