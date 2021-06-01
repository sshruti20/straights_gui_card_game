#ifndef OBSERVER_H
#define OBSERVER_H

#include "Card.h"
class Subject;


class Observer {
public:
	virtual void update () = 0;
};

#endif