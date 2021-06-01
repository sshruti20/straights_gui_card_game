#ifndef _CONTROLLER_
#define _CONTROLLER_

#include <string>
#include <vector>
#include <iostream>
#include "Model.h"
#include "Command.h"
#include "View.h"
#include "TextView.h"
#include "Player.h"
#include "Human.h"
#include "Computer.h"

class Model;
class View;
class TextView;

class Controller {
public:
   Controller( Model*, TextView* );
   bool shouldCommandBeActedUpon(Command & cmd, int playerIndex);
   void automaticMoveHandler(Command move, int playerIndex);
   void gameLogic(bool firstTime);
   void runGame(int input_seed);

   // for GUI
   void shuffleDeck(int seed);
   void dealCardsAndInitializePlayers(bool isComputer[4]);
   void dealCardsAndClearPlayersHands();
   void startNewRound();
   bool isRoundOver() const;
   bool isGameOver() const;
   std::vector<Player *> getWinners();
   void quitGame() const;
   void updatePlayersScores();

private:
   Model *model_;
   TextView *view_;
}; // Controller

#endif
