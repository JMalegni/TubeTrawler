cmake_minimum_required(VERSION 3.26)
project(TubeTrawler)

set(CMAKE_CXX_STANDARD 17)

add_executable(TubeTrawler main.cpp
        Maxheap.h
        RBT.h
        Timer.h
)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/SFML/include/SFML)
target_link_libraries(TubeTrawler sfml-system sfml-window sfml-graphics sfml-audio)