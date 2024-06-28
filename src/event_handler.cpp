#include <iostream>
#include <thread>
#include "event_handler.h"

EventHandler& EventHandler::getInstance() {
    static EventHandler instance;
    return instance;
}

void EventHandler::checkEvents() {
    while(!eventQueue.empty()) {
        Event e = eventQueue.front();

        eventQueue.pop();
        for(Observer* o : subscribers) {
            o->onEvent(e);
        }
    }
}

void EventHandler::subscribe(Observer* subscriber) {
    subscribers.push_back(subscriber);
}

void EventHandler::sendEvent(Event e) {
    eventQueue.push(e);
}