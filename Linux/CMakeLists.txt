# CMakeList.txt : CMake project for TestLinux, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (TestLinux "src/main.cpp" )

# Set HOST_EXPORTS to generate DLL callable exported functions
target_compile_definitions(TestLinux PRIVATE LINUX UNICODE)

#add_dependencies(TestLinux Host)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TestLinux PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
# install (TARGETS TestLinux Host DESTINATION bin)
