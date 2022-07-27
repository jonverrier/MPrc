# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("MPrc")

option(WINXX "Build for Windows" ON)
option(LINUX "Build for Linux" OFF)

# Include sub-projects.
add_subdirectory ("Host")

add_subdirectory ("TestHost")
add_dependencies(TestHost Host)

add_subdirectory ("Media")
add_dependencies(Media Host)

add_subdirectory ("TestMedia")
add_dependencies(TestMedia Media Host)

add_subdirectory ("MapColours")
add_dependencies(MapColours Media Host)

add_subdirectory ("Viewer")
add_dependencies(Viewer Media Host)

add_subdirectory ("Linux")
add_dependencies(TestLinux Media Host)