cmake_minimum_required(VERSION 3.2)
set(MAJOR_VERSION 0)
set(MINOR_VERSION 0)
set(PATCH_VERSION 999)
if($ENV{MAJOR_VERSION})
    set(MAJOR_VERSION $ENV{MAJOR_VERSION})
endif()
if($ENV{MINOR_VERSION})
    set(MINOR_VERSION $ENV{MINOR_VERSION})
endif()
if($ENV{TRAVIS_BUILD_NUMBER})
    set(PATCH_VERSION $ENV{TRAVIS_BUILD_NUMBER})
endif()
project(helloworld VERSION ${MAJOR_VERSION}.${MAJOR_VERSION}.${PATCH_VERSION})
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
configure_file(version.h.in ${PROJECT_SOURCE_DIR}/version.h)
add_executable(helloworld main.cpp)
add_library(hello lib.cpp)
add_executable(test_version test_version.cpp)
set_target_properties(helloworld hello test_version PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)
set_target_properties(test_version PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)
target_link_libraries(helloworld
    hello
)
target_link_libraries(test_version
    ${Boost_LIBRARIES}
    hello
)
install(TARGETS helloworld RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT podshivalov.ilya@yandex.ru)
include(CPack)
enable_testing()
add_test(helloworld_tests test_version)
