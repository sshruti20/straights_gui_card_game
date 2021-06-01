#include <iostream>
#include "Subject.h"
#include "Observer.h"

using namespace std;

void Subject::subscribe (Observer *newView) {
    observers_.push_back(newView);
}


void Subject::unsubscribe (Observer *formerView) {
     // observers_.erase(formerView);
}


void Subject::notify() {
  Observers::iterator i;
  for (i = observers_.begin(); i != observers_.end(); ++i)
    (*i)->update();
}
