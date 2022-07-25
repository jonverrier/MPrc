# CMakeList.txt : CMake project for Viewer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (Viewer "src/Viewer.cpp" "src/Viewer.rc")

# Set HOST_EXPORTS to generate DLL callable exported functions
target_compile_definitions(Viewer PRIVATE WINXX UNICODE)

# To find and use OpenEXR
find_package(OpenEXR CONFIG REQUIRED)
target_link_libraries(Viewer PRIVATE OpenEXR::IlmImf OpenEXR::IlmImfUtil OpenEXR::IlmImfConfig)

# To find and use OpenImageIO
find_package(OpenImageIO CONFIG REQUIRED)
target_link_libraries(Viewer PRIVATE OpenImageIO::OpenImageIO OpenImageIO::OpenImageIO_Util)

# To find and use ICU
find_package(ICU 69.0 COMPONENTS uc i18n REQUIRED)
target_link_libraries(Viewer PRIVATE ICU::uc ICU::i18n)

target_link_libraries(Viewer PRIVATE Media Host)
add_dependencies(Viewer Media)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Viewer PROPERTY CXX_STANDARD 20)
endif()

target_link_options(Viewer PRIVATE "/SUBSYSTEM:WINDOWS")

# Copy the Host & Media Library across
add_custom_command ( TARGET Viewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Host> $<TARGET_FILE_DIR:Viewer>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Media> $<TARGET_FILE_DIR:Viewer>
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/Viewer/ImageLinear.png $<TARGET_FILE_DIR:Viewer>
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/Viewer/ImageLog.png $<TARGET_FILE_DIR:Viewer>
)

# TODO: Add tests and install targets if needed.
# install (TARGETS Viewer Media Host DESTINATION bin)
