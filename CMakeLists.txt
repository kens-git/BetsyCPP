cmake_minimum_required(VERSION 2.8.11)

project(BetsyCPP)

set(CMAKE_CXX_STANDARD 14)

find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)

file(GLOB sources "source/*.cpp")

add_executable(${PROJECT_NAME} ${sources})

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Network)

if (UNIX AND NOT APPLE)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    target_link_libraries(${PROJECT_NAME} Threads::Threads)
endif()
