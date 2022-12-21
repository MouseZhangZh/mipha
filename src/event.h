#ifndef MIPHA_EVENT_H
#define MIPHA_EVENT_H

#include <cstdio>

namespace mipha {

struct event {
    // 24-32  16-23  8-15  0-7
    // hold   hold   hold  type
    int32_t event_info = 0;
    int32_t data[31] = { 0 };
};

}



#endif //MIPHA_EVENT_H
