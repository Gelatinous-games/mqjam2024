cmake_minimum_required(VERSION 3.11)
project(gamer C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RAYLIB_VERSION 5.0)

find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

# Only look for GLFW and OpenGL if not building for Web
if(NOT ${PLATFORM} STREQUAL "Web")
  if(LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
  else()
    find_package(glfw3 REQUIRED)
  endif()
  find_package(OpenGL REQUIRED)
endif()

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} raylib)

# Copy assets to build directory
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

# Web Configurations
# Web Configurations
if (${PLATFORM} STREQUAL "Web")
  # Preload assets
  set(ASSET_DIR "${CMAKE_BINARY_DIR}/resources")
  file(GLOB_RECURSE ASSET_FILES "${ASSET_DIR}/*")
  set(PRELOAD_FILES "")
  foreach(ASSET ${ASSET_FILES})
    file(RELATIVE_PATH ASSET_RELATIVE ${CMAKE_BINARY_DIR} ${ASSET})
    set(PRELOAD_FILES "${PRELOAD_FILES} --preload-file ${ASSET_RELATIVE}")
  endforeach()

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1 -s INITIAL_MEMORY=32MB -s ALLOW_MEMORY_GROWTH=1")
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${PRELOAD_FILES}")
else()
  # Only link GLFW and OpenGL for non-Web platforms
  target_link_libraries(${PROJECT_NAME} glfw)
  target_link_libraries(${PROJECT_NAME} OpenGL::GL)
endif()

if(WIN32)
  target_link_libraries(${PROJECT_NAME} "-lopengl32" "-lgdi32" "-lwinmm")
endif()

if(APPLE)
  target_link_libraries(${PROJECT_NAME} "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()