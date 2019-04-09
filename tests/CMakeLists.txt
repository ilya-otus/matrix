cmake_minimum_required(VERSION 3.2)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_executable(test_version test_version.cpp)
add_executable(test_matrix test_matrix.cpp)
set_target_properties(test_version test_matrix PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)
target_link_libraries(test_version
    ${Boost_LIBRARIES}
    crs
)

target_link_libraries(test_matrix
    ${Boost_LIBRARIES}
    crs
)

set_target_properties(test_version test_matrix PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

