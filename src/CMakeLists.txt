add_subdirectory(backend)
add_subdirectory(tests)
add_subdirectory(utils)

#find_package(glfw3 REQUIRED)
#include_directories(${GLFW3_INCLUDE_DIR})

include_directories(3rd/imgui)

file(GLOB mipha_SRC
        main.cpp
        utils/tgaimage.cpp
        utils/model.cpp
        utils/geometry.cpp)

add_executable(mipha ${mipha_SRC})
#target_link_libraries(mipha glfw)

#if (APPLE)
#    target_link_libraries(mipha "-framework OpenGL")
#endif()