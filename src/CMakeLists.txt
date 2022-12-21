add_subdirectory(backend)
add_subdirectory(tests)
add_subdirectory(utils)

include_directories(3rd)

set(mipha_SRC main.cpp
              eventloop.cpp
              eventloop.h
              event.cpp
              event.h)

add_executable(mipha ${mipha_SRC})