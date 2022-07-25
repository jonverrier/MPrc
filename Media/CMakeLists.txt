# CMakeList.txt : CMake project for Media, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library (Media SHARED "src/dllmain.cpp" "src/pch.cpp" "src/ExceptionLogger.cpp" "src/Logger.cpp" "src/Frame.cpp" "src/FrameColourMappers.cpp" "src/FrameFilename.cpp" "src/LetterBox.cpp" "src/Utf16Utf8.cpp" )

# Set HOST_EXPORTS to generate DLL callable exported functions
target_compile_definitions(Media PRIVATE WINXX MEDIA_EXPORTS UNICODE)

# To find and use OpenEXR.hpp
find_package(OpenEXR CONFIG REQUIRED)
target_link_libraries(Media PRIVATE OpenEXR::IlmImf OpenEXR::IlmImfUtil OpenEXR::IlmImfConfig Host)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Media PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
