add_subdirectory(backend)
add_subdirectory(tests)
add_subdirectory(utils)

find_package(glfw3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIR})

include_directories(3rd/imgui)

file(GLOB mipha_SRC
        main.cpp
        3rd/imgui/*.h
        3rd/imgui/*.cpp
        3rd/imgui/backends/imgui_impl_glfw.h
        3rd/imgui/backends/imgui_impl_glfw.cpp
        3rd/imgui/backends/imgui_impl_opengl3.h
        3rd/imgui/backends/imgui_impl_opengl3.cpp)

add_executable(mipha ${mipha_SRC})
target_link_libraries(mipha glfw)

if (APPLE)
    target_link_libraries(mipha "-framework OpenGL")
endif()