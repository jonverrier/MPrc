#include "pch.h"

#include <exception>

#include "../../Include/Host.h"
#include "../../Include/MediaFrameFilename.h"

TEST(MediaFrameFilename, ConstructCopyAssign) {

    Media::FrameFilename filename(L"project", 0, 0, 0, 1);
    Media::FrameFilename filename2(L"project", 0, 0, 0, 2);
    Media::FrameFilename filenameCopy(filename);
    Media::FrameFilename filenameAssign = filename2;

    EXPECT_EQ(filename, filenameCopy);
    EXPECT_EQ(filename2, filenameAssign);
    EXPECT_NE(filenameCopy, filenameAssign);
}

TEST(MediaFrameFilename, InvalidConstruct) {
    bool caught = false;

    try {
        Media::FrameFilename filename(L"proj_ect", 0, 0, 0, 1);
    }
    catch (const std::exception& e) {
        UNREFERENCED_PARAMETER(e);
        caught = true;
    }

    EXPECT_EQ(caught, true);
}

TEST(MediaFrameFilename, FormatParse) {

    Media::FrameFilename filename(L"project", 0, 0, 0, 1);

    auto str1 = Media::FrameFilename::format(filename);

    auto filenameBack = Media::FrameFilename::parse(str1);

    EXPECT_EQ(filename, filenameBack);
}