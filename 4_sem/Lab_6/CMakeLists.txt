cmake_minimum_required (VERSION 3.11)

project(AhoCorasickLab6)

set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11")

add_executable (task_1 ./source/task_1.cpp)
add_executable (task_2 ./source/task_2.cpp)
add_executable (test_google ./test/test_google.cpp)

add_library(BorLib      ./source/functionsBor.cpp)
add_library(WildcardLib ./source/functionsWildcard.cpp)

target_link_libraries(task_1 BorLib)
target_link_libraries(task_2 WildcardLib)
target_link_libraries(task_2 BorLib)

include_directories(./include)

target_compile_features(BorLib PUBLIC cxx_std_11)
target_compile_features(WildcardLib PUBLIC cxx_std_11)

find_package(GTest REQUIRED)
target_include_directories(test_google PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(test_google PRIVATE ${GTEST_LIBRARIES} pthread WildcardLib)
target_link_libraries(test_google PRIVATE ${GTEST_LIBRARIES} pthread BorLib)
target_compile_features(test_google PRIVATE cxx_std_11)