# CMakeList.txt : CMake project for Host, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library (Host SHARED "src/Exception.cpp" "src/dllmain.cpp" "src/pch.cpp" )

# COnditional compiler flags for platform WINXX/LINUX
if (WINXX)
   target_compile_definitions(Host PRIVATE HOST_EXPORTS UNICODE WINXX)
endif()
if (LINUX)
   target_compile_definitions(Host PRIVATE HOST_EXPORTS UNICODE LINUX)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Host PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
