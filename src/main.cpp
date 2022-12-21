#include <iostream>
#include <csignal>

#include "eventloop.h"

void signal_handler([[maybe_unused]] int signal) {
    mipha::event e = {
            .event_info = 0xf
    };
    mipha::event_loop::get_loop()->send_event(e);
}

int main() {
    std::cout << "mipha start\n";

    signal(SIGTERM, signal_handler);

    mipha::event_loop::get_loop()->dispatch();

    std::cout << "mipha end\n";
    return 0;
}