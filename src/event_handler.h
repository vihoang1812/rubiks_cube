#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <atomic>
#include "common_alias.h"
#include "observer.h"

class EventHandler {
private:
    EventHandler() = default;
    bool running = true;
    vec<Observer*> subscribers;
    queue<Event> eventQueue;
public:
    EventHandler(const EventHandler&) = delete;
    static EventHandler& getInstance();
    void checkEvents();
    void run();
    void mainLoop();
    void subscribe(Observer* subscriber);
    void sendEvent(Event e);
};

#endif