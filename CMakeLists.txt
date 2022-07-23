# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("MPrc")

# Include sub-projects.
add_subdirectory ("Host")

add_subdirectory ("TestHost")
add_dependencies(TestHost Host)

add_subdirectory ("Media")
add_dependencies(Media Host)

add_subdirectory ("TestMedia")
#add_dependencies(TestMedia Media)