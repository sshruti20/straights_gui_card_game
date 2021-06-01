#include "Model.h"

using namespace std;

void Model::clearTableCards() {
  cards_on_table_.clear();
}

Model::Model() {
    max_num_cards_ = 52;
    max_num_cards_per_hand_ = 13;
    num_players_ = 4;
    is_game_over_ = false;
}

std::vector<Card> Model::getCardsOnTable() {
    vector<Card> table;
    vector<Card>::iterator it = cards_on_table_.begin();
    for(;it != cards_on_table_.end(); it++) {
        table.push_back(*it);
    }
    return table;
}

void Model::notifyObservers() {
    this->notify();
}

void Model::resetModelForNewGame() {
    this->resetModelForNewRound();
    this->player_list_.clear();
}

void Model::resetModelForNewRound() {
    // clear all the players' hands
    for (int i=0; i<this->player_list_.size(); i++) {
        (player_list_.at(i))->clearEntirePlayerHand();      // clear old hand and replace with newHand
    }
    // clear the cards_on_table
    this->cards_on_table_.clear();
}
