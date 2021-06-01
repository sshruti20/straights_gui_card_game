#ifndef _STARTMENU_
#define _STARTMENU_

#include <iostream>
#include <gtkmm.h>
#include "Controller.h"
#include "GameView.h"
#include "Subject.h"

class GameView;

class StartMenu {
private:
  Gtk::Dialog* dialog_;
  Gtk::CheckButton* checkButtons_[4];
  Gtk::Entry* seed_;
  Gtk::Button* cancel_;
  Gtk::Button* start_;

  // GUI GameView
  GameView* view_;

  // "Start" start menu button handler
  void startGame();

  // "Cancel" start menu button handler
  void hideStartMenu();

public:
    // constructor
    StartMenu(GameView *);

    // show the start menu
    void showStartMenu();
};

#endif
