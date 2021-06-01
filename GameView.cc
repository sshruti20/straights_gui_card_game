#include <iostream>
#include <string>
#include "GameView.h"
using namespace std;

GameView::GameView( Model* m, Controller* c, Glib::RefPtr<Gtk::Application> & app, Glib::RefPtr<Gtk::Builder> & builder) :
    model_(m), controller_(c), Gtk::Window::Window(), app_{app}, builder_{builder}, play_button_{nullptr}, quit_button_{nullptr} {
    set_title( "Straights" );
    set_default_size( 800, 600 );  // Set default size, width and height, in pixels.
    set_border_width( 10 );

    // Add this GameView as an Observer of Model (Subject)
    model_->subscribe(this);

    // Build the UI and then show
    buildUI();
    show_all_children();
}

GameView::~GameView() {
} // GameView::~GameView

void GameView::buildUI() {
    Gtk::Window* window_ = nullptr;
    builder_->get_widget( "window1", window_ );
    if ( window_ == nullptr ) {
    	g_warning("Window extraction error");
    	return;
    } // if
    Gtk::Widget * tmpWidget = window_->get_child();
    window_->remove();
    add( *tmpWidget );

    // get stylesheet
    auto css = Gtk::CssProvider::create();
    // try to load button styles
    if (!css->load_from_path("style.css")) {
        cout << "CSS loading error\n";
    }

    builder_->get_widget( "title", title_label_ );

    // get references to each player's details section on screen
    for (int i=1; i<5; i++) {
        // each player's label so it is easy to tell who is Computer and who is Human
        string player_label = "label-player" + std::to_string(i);
        builder_->get_widget( player_label, player_labels_[i-1] );

        // each player's current score so far (gets updated after each round when a new rounds needs to be played)
        string player_score_label = "score-label" + std::to_string(i);
        builder_->get_widget( player_score_label, player_scores_[i-1] );

        // 
        string player_discards_label = "discards-label" + std::to_string(i);
        builder_->get_widget( player_discards_label, player_discards_[i-1] );

        string player_ragequit_button_label = "rage-button" + std::to_string(i);
        builder_->get_widget( player_ragequit_button_label, player_ragequit_buttons_[i-1] );
        player_ragequit_buttons_[i-1]->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &GameView::playerRageQuitted), i-1));

        string player_plays_record = "plays-label" + std::to_string(i);
        builder_->get_widget( player_plays_record, player_plays_records_[i-1] ); 
    }

    // get the start button reference and hook it up to the corresponding handler
    builder_->get_widget( "start-button", play_button_ );
    play_button_->signal_clicked().connect( sigc::mem_fun(*this, &GameView::button_play) );

    // get the quit button reference and hook it up to the corresponding handler
    builder_->get_widget( "quit-button", quit_button_ );
    quit_button_->signal_clicked().connect( sigc::mem_fun(*this, &GameView::button_quit) );

    // get the image references for the cards on the table
    for (int i = 0; i < 52; i++) {
        string card_on_table_image = "C" + std::to_string(i);
        builder_->get_widget(card_on_table_image, table_images_[i]);
    }

    // needed for initializing the CSS (ie: to know which screen the CSS should be applied to)
    auto current_screen = Gdk::Screen::get_default();

    // get the button and image references for the hand on the screen/GUI
    for (int i = 1; i < 14; i++) {
        string hand_button_name = "button" + std::to_string(i);
        builder_->get_widget(hand_button_name, hand_buttons_[i-1]);
        
        string card_in_hand_image = "image" + std::to_string(i);
        builder_->get_widget(card_in_hand_image, hand_images_[i-1]);
        
        // set up for styling the hand buttons later using CSS
        auto styling = hand_buttons_[i-1]->get_style_context();
        styling->add_provider_for_screen(current_screen, css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        
        // set handler for hand buttons, and pass in card in hand index
        hand_buttons_[i-1]->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &GameView::chooseCardFromHand), i-1));
    }

    // build up the Start Menu, but only show it on a Play button click
    startMenu_ = new StartMenu(this);

} // GameView::buildUI

// Play button handler
void GameView::button_play() {
    startMenu_->showStartMenu();
}

// Quit button handler
void GameView::button_quit() {
    this->controller_->quitGame();
}

std::string GameView::getImageFilePathForCard(Card card) {
    // maps the given card's rank and suit to the corresponding file path/url (filename)
    int rank = card.rank().rank();
    int suit = card.suit().suit();

    string image_file_path;
    if (rank == 10) {
        image_file_path = "img/" + std::to_string(card.suit().suit()) + "_j.png";
    }
    else if (rank == 11) {
        image_file_path = "img/" + std::to_string(card.suit().suit()) + "_q.png";
    }
    else if (rank == 12) {
        image_file_path = "img/" + std::to_string(card.suit().suit()) + "_k.png";
    }
    else {
        image_file_path = "img/" + std::to_string(card.suit().suit()) + '_' + std::to_string(card.rank().rank()) + ".png";
    }
    return image_file_path;
}

void GameView::update() {
    // first clean the table
    this->clearTableCards();
    this->clearHandCards();

    // show the current player's turn in the title label
    title_label_->set_label("STRAIGHTS\n-Welcome-\nPlayer " + std::to_string(this->model_->current_player_index_ +1) + "'s Turn");

    // update the four players' details sections
    for (int i=0; i<4; i++) {
        // show whether the player is human or computer (needed for when a player ragequits)
        // and disable the Computer Ragequit buttons
        if ( ( this->model_->player_list_.at(i) )->isHuman() == false ) {
            player_labels_[i]->set_label("Player " + std::to_string(i+1) + " - Computer");
            player_ragequit_buttons_[i]->set_sensitive(false);
        }
        else {      // the player is human
            // enable ragequit buttons for human players
            player_labels_[i]->set_label("Player " + std::to_string(i+1) + " - Human");
            player_ragequit_buttons_[i]->set_sensitive(true);
        }

        // update the discards piles for all the players
        player_discards_[i]->set_label("Discards: " + std::to_string( (this->model_->player_list_.at(i))->getDiscardedSize()) );

        // ONLY really updates the scores at the END of a round
        player_scores_[i]->set_label("Score: " + std::to_string( (this->model_->player_list_.at(i))->score()) );

        // show the player's current plays
        std::vector<Card> current_player_played_cards = (this->model_->player_list_.at(i))->getPlayedCards();
        string plays_so_far = "Plays: ";
        for (int j=0; j<current_player_played_cards.size(); j++) {
            Card card = current_player_played_cards.at(j);
            int rank = card.rank().rank();
            int suit = card.suit().suit();
            string outputRank;
            string outputSuit;
            // convert Rank to string for output
            if (rank == 10) {
                outputRank = "J";
            }
            else if (rank == 11) {
                outputRank = "Q";
            }
            else if (rank == 12) {
                outputRank = "K";
            }
            else {
                outputRank = std::to_string(rank+1);
            }
            // convert Suit to string for output
            if (suit == 0) {
                outputSuit = "C";
            }
            else if (suit == 1) {
                outputSuit = "D";
            }
            else if (suit == 2) {
                outputSuit = "H";
            }
            else {  // suit == 3
                outputSuit = "S";
            }
            // add newline 5 card plays in to format output (make it look nice)
            if ( j == 5 ) {
                plays_so_far += "\n";
            }
            plays_so_far += (outputRank + outputSuit + " ");
        }
        player_plays_records_[i]->set_label(plays_so_far);
    }

    // show the current state of the table
    std::vector<Card> table_cards = this->model_->cards_on_table_;
    for (int i=0; i < this->model_->cards_on_table_.size(); i++) {
        // map each card in cards_on_table_ to its table_image[index]
        Card card = this->model_->cards_on_table_.at(i);
        string image_file_path = getImageFilePathForCard(card);

        // get rank and quit for correct indexing so that the card on the table shows up in the correct spot
        int rank = card.rank().rank();
        int suit = card.suit().suit();
        table_images_[ (suit*13)+rank ]->set(image_file_path);
    }

    // first de-sensitize all the cards in the hand and remove any styling
    for (int i=0; i<13; i++) {
        hand_buttons_[i]->set_sensitive(false);

        auto style = hand_buttons_[i]->get_style_context();
        hand_buttons_[i]->set_opacity(1);
        style->remove_class("legal");
        // style->remove_class("discard");
    }

    // show the current player's hand
    std::vector<Card> hand = ( this->model_->player_list_.at(this->model_->current_player_index_) )->getHandCards();
    for (int i=0; i<hand.size(); i++) {
        Card card = hand.at(i);
        string image_file_path = getImageFilePathForCard(card);     // gets the filename for the given card
        hand_images_[i]->set(image_file_path);

        // add highlighting for legal cards
        auto style = hand_buttons_[i]->get_style_context();
        if (this->model_->player_list_.at(this->model_->current_player_index_)->isPlayingCardLegal(card)) {
            hand_buttons_[i]->set_opacity(1);
            style->add_class("legal");
        }

        // only let the the hand card buttons be sensitive for Human players
        if ( ( this->model_->player_list_.at(this->model_->current_player_index_) )->isHuman() == true) {
            hand_buttons_[i]->set_sensitive(true);
        }
        else {
            hand_buttons_[i]->set_sensitive(false);
        }
    }

    // If the Round is over, just show the Round Over Dialog and then decide whether a next round needs to be played
    if (this->controller_->isRoundOver()) {
        this->showRoundEndDialog();
    }

    playNextTurn();     // only actually plays a next turn if the next player is a Computer AND the game/round is not yet over
}

// Plays next turn if the next player is a Computer AND the game/round is not yet over
void GameView::playNextTurn() {    
    Player * currentPlayer = this->model_->player_list_.at(this->model_->current_player_index_);
    // Don't make an automatic move if the current player is human, the round is over, or the game is over
    if (currentPlayer->isHuman() || this->controller_->isRoundOver() || this->controller_->isGameOver()) { return; }

    currentPlayer->makeMove();
    this->model_->current_player_index_++;
    this->model_->current_player_index_ = this->model_->current_player_index_ % 4;
    this->model_->notifyObservers();
 }

void GameView::startRound(int seed, bool isFirstRound) {
    // clear GUI (frontend) cards
    this->clearTableCards();
    this->clearHandCards();

    // a next round is required, so play it. Only dealCards and clear players' hands if it is NOT the first round
    if (!isFirstRound) {
        this->controller_->shuffleDeck(seed);
        this->controller_->dealCardsAndClearPlayersHands();
    }
    controller_->startNewRound();
}

void GameView::startGameWithSettings(int seed, bool isComputer[4]) {        // starts the game with settings -- makes new Players
    // first shuffle the deck
    controller_->shuffleDeck(seed);

    // This will also clear any old Model Data (backend) for cards and players
    controller_->dealCardsAndInitializePlayers(isComputer);

    // start round
    this->startRound(seed, true);
}

// hand card button handler
void GameView::chooseCardFromHand(int index) {
    // get the current player
    Player * currentPlayer = this->model_->player_list_.at(this->model_->current_player_index_);
    Card cardSelected = currentPlayer->getHandCards().at(index);
    bool isCardLegal = currentPlayer->isPlayingCardLegal(cardSelected);
    bool canDiscard = currentPlayer->isDiscardAllowed();
    
    cout << "DEBUGGING: clicked card "<< cardSelected << endl;
    
    //Case (idk if this could possibly happen) if it is computer player's turn and user clicks 'your hand' cards
    if ( !currentPlayer->isHuman() ) {
        cout << "DEBUGGING: ***ERROR*** DON'T CLICK CARDS ON COMPUTER PLAYER'S TURN" << endl;
        return;
    }
    
    if ( !canDiscard && isCardLegal ) {
     // Case when legal play is to be made and legal card is clicked
        currentPlayer->playCard(cardSelected);
    }
    else if (canDiscard) {
     // Case when card is to be discarded (any card is legal to discard)
        currentPlayer->discardCard(cardSelected);
    }
    else if (!canDiscard && !isCardLegal) {
        // Case when legal play is to be made and illegal card is clicked
        cout << "DEBUGGING: Can't play card " << cardSelected << endl;
        this->showIllegalPlayDialog();
        return;
    }

    this->model_->notifyObservers();    // show the effect of playing the card out of the current player's hand
        
    // move onto the next player's turn
    this->model_->current_player_index_++;
    this->model_->current_player_index_ = this->model_->current_player_index_ % 4;

    this->model_->notifyObservers();    // show the table/screen for the next player
}

// handler for RageQuit button
void GameView::playerRageQuitted(int index) {    
    this->model_->player_list_.at(index)->rageQuit();
    this->model_->notifyObservers();     // shows effect of ragquit and makes the next move for the newly converted Computer player
}

void GameView::clearTableCards() {
    // sets the table card images to nothing.png
    for (int i=0; i<52; i++) {
        table_images_[i]->set("img/nothing.png");
    }
}

void GameView::clearHandCards() {
    // sets the hand card images to nothing.png
    for (int i=0; i<13; i++) {
        hand_images_[i]->set("img/nothing.png");
    }
}

void GameView::showRoundEndDialog() {       // Shows the round end dialog
    Gtk::MessageDialog dialog( *this, "The Round is Over", false, Gtk::MESSAGE_OTHER, Gtk::BUTTONS_OK);
    // get the score update text for the players
    string score_update_text = "";
    for (int i=0; i<this->model_->player_list_.size(); i++) {
        score_update_text += "\nPlayer "+ std::to_string((this->model_->player_list_.at(i))->player_id()) + "'s score: " + std::to_string((this->model_->player_list_.at(i))->score()) +
                            " + " + std::to_string((this->model_->player_list_.at(i))->getScoreForCurrentRound()) + " = " +
                            std::to_string( (this->model_->player_list_.at(i))->score() + (this->model_->player_list_.at(i))->getScoreForCurrentRound() ) + "\n";
    }
    // actually update the current player's score in Model
    this->controller_->updatePlayersScores();

    dialog.set_secondary_text( score_update_text );     // set score update text

    int result = dialog.run();
    switch(result) {
        case( Gtk::RESPONSE_OK ):
            dialog.hide();
            // decide if next round should be started
            if (this->controller_->isGameOver()) {
                this->showGameOverDialog();
            }
            else {
                // play another round
                this->startRound(this->model_->deck_.getSeed(), false);
            }
            break;
        default:
            cout << "Error: unhandled button clicked" << endl;
            break;
    } // switch
} // showRoundEndDialog()

void GameView::showGameOverDialog() {       // show the game over dialog
    Gtk::MessageDialog gameOverDialog( *this, "The Game is Over", false, Gtk::MESSAGE_OTHER, Gtk::BUTTONS_OK);
    // get the winners
    std::vector<Player *> winners = this->controller_->getWinners();
    // generate winers text
    string winners_list_text = "";
    for (int i=0; i<winners.size(); i++) {
        winners_list_text += "\nPlayer "+ std::to_string( (winners.at(i))->player_id() ) + " Wins!" + "\n";
    }
    // show the winners
    gameOverDialog.set_secondary_text( winners_list_text );

    int result = gameOverDialog.run();
    switch(result) {
        case( Gtk::RESPONSE_OK ):
            gameOverDialog.hide();
            // this->clearHandCards();
            break;
        default:
            cout << "Error: unhandled button clicked" << endl;
            break;
    } // switch
} // showGameOverDialog()

void GameView::showIllegalPlayDialog() {        // show the illegal play dialog
    Gtk::MessageDialog illegalPlayDialog( *this, "Illegal Card Selected", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
    illegalPlayDialog.set_secondary_text( "The card you selected is not legal\nPlease select a different card." );
    int result = illegalPlayDialog.run();
    switch(result) {
        case( Gtk::RESPONSE_OK ):     // resume gameplay
            illegalPlayDialog.hide();
            break;
        default:
            cout << "Error: unhandled button clicked" << endl;
            break;
    } // switch
} // showIllegalPlayDialog()
