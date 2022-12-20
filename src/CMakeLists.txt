add_subdirectory(backend)
add_subdirectory(tests)
add_subdirectory(utils)

set(mipha_SRC main.cpp
              eventloop.cpp)

add_executable(mipha ${mipha_SRC})