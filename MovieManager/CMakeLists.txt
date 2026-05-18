cmake_minimum_required(VERSION 3.5)
project(MovieManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# If the user or the environment provided a Qt installation path, use it.
# Previously this was hard-coded to a MinGW Qt install which causes include errors
# when building with a different Qt kit (e.g. MSVC). Set the QT_DIR environment
# variable or configure CMake's CMAKE_PREFIX_PATH in your IDE to point to your
# Qt installation root (the directory that contains "lib/cmake/Qt6").
if(NOT DEFINED CMAKE_PREFIX_PATH AND DEFINED ENV{QT_DIR})
    set(CMAKE_PREFIX_PATH $ENV{QT_DIR})
endif()

if(NOT CMAKE_PREFIX_PATH)
    message(STATUS "CMAKE_PREFIX_PATH is not set. If Qt headers cannot be found, set the QT_DIR environment variable or configure CMAKE_PREFIX_PATH to your Qt installation (e.g. C:/Qt/6.x.x/msvc_64).")
endif()

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(MovieManager
    src/main.cpp
    src/mainWindow.cpp
    src/homeWindow.cpp
    src/movieCollection.cpp
    src/utils.cpp
    src/reviewDialog.cpp
    include/mainWindow.h
    include/homeWindow.h
    include/movie.h
    include/movieCollection.h
    include/utils.h
    include/reviewDialog.h
)

qt_add_resources(MovieManager "posters"
    PREFIX "/"
    FILES
        posters/matrix.jpg
        posters/inception.jpg
        posters/interstellar.jpg
        posters/godfather.jpg
        posters/pulp_fiction.jpg
        posters/dark_knight.jpg
        posters/parasite.jpg
        posters/spirited_away.jpg
        posters/oppenheimer.jpg
        posters/dune2.jpg
        posters/blade_runner.jpg
        posters/get_out.jpg
)

target_include_directories(MovieManager PRIVATE include)
target_link_libraries(MovieManager Qt6::Widgets)