#pragma once

#include "concurrentqueue/blockingconcurrentqueue.h"
#include "event.h"

namespace mipha {

class event_loop;
static std::shared_ptr<event_loop> g_event_loop;

class event_loop {
public:
    static std::shared_ptr<event_loop> get_loop() {
        if (!g_event_loop) {
            g_event_loop.reset(new event_loop());
        }
        return g_event_loop;
    }
    int dispatch();
    void send_event(event);
    void send_event(event &&);
private:
    event_loop() = default;
    moodycamel::BlockingConcurrentQueue<event> q;

};

}