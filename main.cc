#include "Model.h"
#include "Controller.h"
#include "View.h"
#include "GameView.h"
#include <iostream>
#include <cstdlib>
#include <gtkmm.h>

int main( int argc, char * argv[] ) {
	Model model;
	TextView view;
    Controller controller( &model, &view );

	int input_seed = 0;

	if (argc >= 2) {
		input_seed = atoi(argv[1]);
	}

	try {
		auto app = Gtk::Application::create( argc, argv, "straights.cs247.wwelcome" );
		auto builder_ = Gtk::Builder::create_from_file( "GUIwip.glade" );

		GameView game_( &model, &controller, app, builder_ );
		app->run( game_ );
		app->remove_window( game_ );

	}  catch( const Glib::FileError & ex ) {
		std::cerr << "FileError: " << ex.what() << std::endl;
		return 1;
	} catch( const Glib::MarkupError & ex ) {
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return 1;
	} catch( const Gtk::BuilderError & ex ) {
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return 1;
    }
    
    return 0;

}
