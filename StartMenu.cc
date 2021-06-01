#include "StartMenu.h"
using namespace std;

StartMenu::StartMenu(GameView* view): view_(view) {
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("startmenu.glade");
  builder->get_widget("dialog1", dialog_);

  //Set references for checkboxes
  for (int i=0; i < 4; i++) {
    string name = "checkbutton" + to_string(i+1);
    builder->get_widget(name, checkButtons_[i]);
  }

  builder->get_widget("entry_seed", seed_);

  builder->get_widget( "start-button", start_ );
  builder->get_widget( "cancel-button", cancel_ );

  cancel_->signal_clicked().connect(sigc::mem_fun(*this, &StartMenu::hideStartMenu));
  start_->signal_clicked().connect(sigc::mem_fun(*this, &StartMenu::startGame));
}

void StartMenu::startGame() {
  // in case a game is already running when this is called, we will need to clear all the related data in Model
  bool comp[4];
  for (int i=0; i<4; i++) {
    comp[i] = checkButtons_[i]->get_active();
    cout << "SO COMP[" << i << "] = " << comp[i] << endl;
  }

  string seed = seed_->get_text();
  if (seed == "") { seed = "0"; }
  cout << "SEED: " << seed << endl;

  hideStartMenu();

  view_->startGameWithSettings(std::stoi(seed), comp);
}

void StartMenu::showStartMenu() {
  dialog_->run();
}

void StartMenu::hideStartMenu() {
  dialog_->hide();
}
