# CMakeList.txt : CMake project for TestMedia, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (TestMedia "src/main.cpp" "src/pch.cpp" "src/TestException.cpp" "src/TestFileIO.cpp" "src/TestFrame.cpp" "src/TestFrameFilename.cpp" "src/TestGeometry.cpp" "src/TestUnicode.cpp" "src/TestUtf16Utf8.cpp")

# Set HOST_EXPORTS to generate DLL callable exported functions
target_compile_definitions(TestMedia PRIVATE WINXX UNICODE)

# To find and use gtest
find_package(GTest CONFIG REQUIRED)
target_link_libraries(TestMedia PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

# To find and use OpenEXR
find_package(OpenEXR CONFIG REQUIRED)
target_link_libraries(TestMedia PRIVATE OpenEXR::IlmImf OpenEXR::IlmImfUtil OpenEXR::IlmImfConfig)

# To find and use OpenImageIO
find_package(OpenImageIO CONFIG REQUIRED)
target_link_libraries(TestMedia PRIVATE OpenImageIO::OpenImageIO OpenImageIO::OpenImageIO_Util)

# To find and use ICU
find_package(ICU 69.0 COMPONENTS uc i18n REQUIRED)
target_link_libraries(TestMedia PRIVATE ICU::uc ICU::i18n)

target_link_libraries(TestMedia PRIVATE Media Host)
add_dependencies(TestMedia Media)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TestMedia PROPERTY CXX_STANDARD 20)
endif()

# Copy the Host & Media Library across
add_custom_command ( TARGET TestMedia POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Host> $<TARGET_FILE_DIR:TestMedia>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Media> $<TARGET_FILE_DIR:TestMedia>
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/TestMedia/ImageForRoundTripIn.jpg $<TARGET_FILE_DIR:TestMedia>
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/TestMedia/BrightRings.exr $<TARGET_FILE_DIR:TestMedia>
)

# TODO: Add tests and install targets if needed.
# install (TARGETS TestMedia Media DESTINATION bin)
