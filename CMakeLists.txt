cmake_minimum_required(VERSION 3.8)
project(Simple_P2P)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

find_package(Boost REQUIRED system program_options)

set(EXTERNAL_LIBS "Boost::system" "Boost::program_options" "Threads::Threads")

include_directories("${PROJECT_SOURCE_DIR}/include")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g3 -Wshadow -Wall -pedantic -Wextra -Wold-style-cast")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fdevirtualize -finline-functions -fvectorize -funroll-loop")

add_subdirectory(src)
#add_subdirectory(tests)

