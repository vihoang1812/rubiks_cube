#ifndef OBSERVER_H
#define OBSERVER_H

//Interface Class

#include "event.h"

class Observer {
private:
public:
    virtual void onEvent(Event event) = 0;
};

#endif 