#ifndef _MODEL_H_
#define _MODEL_H_

#include <iostream>
#include <vector>
#include "View.h"
#include "Subject.h"
#include "Player.h"
#include "Deck.h"

class Model : public Subject {

public:
  Model();
  void shuffleDeck();
  void quit();
  void startRound();
  bool shouldNextRoundBePlayed();
  void returnDeckToInitialState();
  void clearTableCards();

  // special for GUI option to start a new game at any time
  void resetModelForNewGame();
  void resetModelForNewRound();

  void notifyObservers();
    
  std::vector<Card> getCardsOnTable();
    
  friend class Controller;
  friend class GameView;

private:

  unsigned int max_num_cards_;
  unsigned int max_num_cards_per_hand_;
  unsigned int num_players_;
  int current_player_index_;

  std::vector<Card> cards_on_table_;
  std::vector<Player *> player_list_;
  Deck deck_;
    
  bool is_game_over_;
  std::vector<View *> view_list_;

};

#endif
