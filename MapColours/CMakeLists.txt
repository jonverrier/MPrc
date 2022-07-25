# CMakeList.txt : CMake project for MapColours, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (MapColours "src/MapColours.cpp")

# Set HOST_EXPORTS to generate DLL callable exported functions
target_compile_definitions(MapColours PRIVATE WINXX UNICODE)

# To find and use OpenEXR
find_package(OpenEXR CONFIG REQUIRED)
target_link_libraries(MapColours PRIVATE OpenEXR::IlmImf OpenEXR::IlmImfUtil OpenEXR::IlmImfConfig)

# To find and use OpenImageIO
find_package(OpenImageIO CONFIG REQUIRED)
target_link_libraries(MapColours PRIVATE OpenImageIO::OpenImageIO OpenImageIO::OpenImageIO_Util)

# To find and use ICU
find_package(ICU 69.0 COMPONENTS uc i18n REQUIRED)
target_link_libraries(MapColours PRIVATE ICU::uc ICU::i18n)

target_link_libraries(MapColours PRIVATE Media Host)
add_dependencies(MapColours Media)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MapColours PROPERTY CXX_STANDARD 20)
endif()

# Copy the Host & Media Library across
add_custom_command ( TARGET MapColours POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Host> $<TARGET_FILE_DIR:MapColours>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Media> $<TARGET_FILE_DIR:MapColours>
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/MapColours/BrightRings.exr $<TARGET_FILE_DIR:MapColours>
)

# TODO: Add tests and install targets if needed.
# install (TARGETS MapColours Media DESTINATION bin)
