cmake_minimum_required (VERSION 3.11)

project(algorithmKMP)

add_executable (task_1 ./source/stepic_1.cpp)
add_executable (task_2 ./source/stepic_2.cpp)
add_executable (test_google ./test/test_google.cpp)
add_executable (test_comparison  ./test/test_comparison.cpp)

add_library(NaiveLib ./source/functionsComparison.cpp)
add_library(StringLib ./source/functionsString.cpp)


target_link_libraries(task_1 StringLib)
target_link_libraries(task_2 StringLib)
target_link_libraries(test_comparison NaiveLib)

include_directories(./include)

target_compile_features(StringLib PUBLIC cxx_std_11)
target_compile_features(NaiveLib PUBLIC cxx_std_11)

find_package(GTest REQUIRED)
target_include_directories(test_google PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(test_google PRIVATE ${GTEST_LIBRARIES} pthread StringLib)
