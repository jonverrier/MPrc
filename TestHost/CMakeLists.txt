# CMakeList.txt : CMake project for TestHost, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (TestHost "src/TestException.cpp" "src/main.cpp" "src/pch.cpp")

# Set HOST_EXPORTS to generate DLL callable exported functions
target_compile_definitions(TestHost PRIVATE UNICODE)

# COnditional compiler flags for platform WINXX/LINUX
if (WINXX)
   target_compile_definitions(TestHost PRIVATE HOST_EXPORTS UNICODE WINXX)
endif()
if (LINUX)
   target_compile_definitions(TestHost PRIVATE HOST_EXPORTS UNICODE LINUX)
endif()

# To find and use gtest.hpp
find_package(GTest CONFIG REQUIRED)
target_link_libraries(TestHost PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main Host)

add_dependencies(TestHost Host)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TestHost PROPERTY CXX_STANDARD 20)
endif()

# Copy the Host Library across
add_custom_command ( TARGET TestHost POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:Host> $<TARGET_FILE_DIR:TestHost>
)

# TODO: Add tests and install targets if needed.
# install (TARGETS TestHost Host DESTINATION bin)
