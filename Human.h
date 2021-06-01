#ifndef _HUMAN_H
#define _HUMAN_H

#include <iostream>
#include <vector>
#include "Command.h"
#include "Player.h"

class Human: public Player {
public:
  Human(std::vector<Card> hand, std::vector<Card> * table_cards, unsigned int player_id, unsigned int score);
  virtual void rageQuit() override;
  void playCard(Card) override;
  void discardCard(Card) override;
  
  void playCard();
  void discardCard();
  
  Command makeRageQuitMove();

  virtual Command makeMove() override;
  virtual bool isHuman() const override;

  bool hasRageQuitted() override;

  virtual PlayerType getPlayerType() const override;
};

#endif
