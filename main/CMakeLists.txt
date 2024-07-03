cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(gamer C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Dependencies
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

find_package(glfw3 3.4 REQUIRED)
find_package(OpenGL REQUIRED)

# Our Project

add_executable(${PROJECT_NAME} main.c)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()
if(WIN32)
    target_link_libraries(${PROJECT_NAME} "-lopengl32")
    target_link_libraries(${PROJECT_NAME} "-lgdi32")
    target_link_libraries(${PROJECT_NAME} "-lwinmm")
endif()
if(LINUX)

    target_link_libraries(${PROJECT_NAME} "-lGL")
    target_link_libraries(${PROJECT_NAME} "-lglfw")
    target_link_libraries(${PROJECT_NAME} "-lm")
    target_link_libraries(${PROJECT_NAME} "-lpthread")
    target_link_libraries(${PROJECT_NAME} "-ldl")
    target_link_libraries(${PROJECT_NAME} "-lrt")
    set(GLFW_BUILD_X11)
    target_link_libraries(${PROJECT_NAME} "-lX11")

endif()
# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()