#ifndef _VIEW_
#define _VIEW_
#include <string>
#include "Command.h"
#include "Observer.h"

class Controller;
class Model;

class View : public Observer {
public:
    View();
	virtual ~View();
	//virtual void update(std::string changedcontent, int playerIndex, Card* card);	// Observer Pattern: concrete update() method

	//Model* model() const;
	//Controller* controller() const;

private:
	// Observer Pattern: to access Model accessors without having to downcast subject
	//Model* model_;
	
	// Strategy Pattern member (plus signal handlers)
	//Controller* controller_;

	// // Signal handlers, gets input:
	// void getCommand();

}; // View

#endif
