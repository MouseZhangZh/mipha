#include "eventloop.h"

#include <iostream>
#include <memory>

namespace mipha {

int event_loop::dispatch() {
    event e;
    bool run = true;
    while (run) {
        q.wait_dequeue(e);
        std::cout << (e.event_info & 0x000f) << std::endl;
        switch (e.event_info & 0x000f) {
            case 0x0:
                std::cout << "Received null event, do nothing.\n";
                break;
            case 0xf:
                run = false;
                std::cout << "Received quit event, quit.\n";
                break;
            default:
                continue;
        }
    }
    return 0;
}

void event_loop::send_event(event e) {
    q.enqueue(std::move(e));
}

void event_loop::send_event(event&& e) {
    q.enqueue(std::move(e));
}

}