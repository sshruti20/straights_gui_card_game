#ifndef _SUBJECT_
#define _SUBJECT_

#include <vector>
#include <string>

class Observer;

class Subject {
public:
       void subscribe( Observer* );
       void unsubscribe( Observer* );

protected:
       void notify();

private:
       typedef std::vector< Observer* > Observers;
       Observers observers_;
}; // Subject

#endif
