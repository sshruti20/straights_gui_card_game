#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <iostream>
#include <vector>
#include "Card.h"
#include "Command.h"

class Player {
public:

    enum PlayerType {human, computer};

    Player(std::vector<Card> hand, std::vector<Card> * table_cards, unsigned int player_id, unsigned int score);
    virtual void playCard (Card);
    virtual void discardCard (Card);
    void calculateCurrentLegalMoves();

    unsigned int score() const;
    unsigned int player_id() const;
    // Clears all current card piles and sets the hand parameter as the player's new hand
    void clearHand(std::vector<Card> hand);
    bool containsSevenOfSpades() const;

    virtual PlayerType getPlayerType() const = 0;
    int getScoreForCurrentRound();
    void updateScore();
    
    bool isPlayingCardLegal(Card) const;
    
    bool isDiscardAllowed();
    
    virtual void rageQuit() = 0;

    virtual bool isHuman() const = 0;
    virtual Command makeMove() = 0;
    virtual bool hasRageQuitted();
    
    int getDiscardedSize() const;
    int getLegalCardsSize() const;
    
    Card discardedAt(int i) const;
    Card playedAt(int i) const;
    
    std::vector<Card> getLegalCards();
    std::vector<Card> getHandCards();
    std::vector<Card> getPlayedCards();

    void clearEntirePlayerHand();

protected:
    std::vector<Card> legal_cards_;
    std::vector<Card> hand_;
    std::vector<Card> played_;
    std::vector<Card> discarded_;
    std::vector<Card> * model_table_cards_;
    bool has_rage_quitted_;
    
private:
    unsigned int player_id_;
    unsigned int score_;
};

#endif
