#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <gtkmm.h>
#include "View.h"
#include "Card.h"
#include "Player.h"
#include "Computer.h"
#include "Human.h"
#include "StartMenu.h"
#include "Controller.h"

class StartMenu;

class Controller;

class GameView : public Gtk::Window, public Observer {
    Glib::RefPtr<Gtk::Application> & app_;
    Glib::RefPtr<Gtk::Builder> & builder_;

    Gtk::Button * play_button_;
    Gtk::Button * quit_button_;

    Gtk::Label * title_label_;
    Gtk::Label * player_labels_[4];
    Gtk::Label * player_scores_[4];
    Gtk::Label * player_discards_[4];
    Gtk::Button * player_ragequit_buttons_[4];
    Gtk::Label * player_plays_records_[4];

    Gtk::Button * hand_buttons_[13];
    Gtk::Image * hand_images_[13];
    Gtk::Image * table_images_[52];

    void buildMenu();
    void buildUI();

    StartMenu* startMenu_;
    Controller* controller_;
    Model* model_;

  protected:
    void clearTableCards();
    void clearHandCards();

    // signal handlers
    void button_play();
    void button_quit();

    std::string getImageFilePathForCard(Card card);
    void showRoundEndDialog();
    void showGameOverDialog();
    void showIllegalPlayDialog();

  public:
    void update();
    void startRound(int seed, bool isFirstRound);  // a next round is required, so play it
    void startGameWithSettings(int seed, bool isComputer[4]);   // starts the game with settings -- makes new Players
    
    //Handler for when player clicks a card from 'Your hand' cards
    void chooseCardFromHand(int index);
    void playerRageQuitted(int index);

    //This will handle all gameplay in a round (including making turns for computer players)
    void playNextTurn();

    GameView( Model*, Controller*, Glib::RefPtr<Gtk::Application> &, Glib::RefPtr<Gtk::Builder> & );
    // GameView ();
    ~GameView();
};

#endif
