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

        if(e == Event::PROGRAM_EXIT) {
            running = false;
            return;
        }

        eventQueue.pop();
        for(Observer* o : subscribers) {
            o->onEvent(e);
        }
    }
}

//On a separate thread
void EventHandler::run() {

    //auto task = [this](){mainLoop();};
    auto t1 = std::thread(&EventHandler::mainLoop, this);
    t1.detach();
}

void EventHandler::mainLoop() {
    std::cout << "Event Loop Running" << std::endl;
    while(running) {
        checkEvents();
    }
}

void EventHandler::subscribe(Observer* subscriber) {
    subscribers.push_back(subscriber);
}

void EventHandler::sendEvent(Event e) {
    eventQueue.push(e);
}