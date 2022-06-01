#include "pch.h"

#include "../../Include/MediaUtf16Utf8.h"

TEST(MediaUtf16Utf8, Transcoding) {

	std::string mbs("A string.");
	std::wstring wcs = Media::Utf8ToUtf16(mbs);

	std::string mbs2 = Media::Utf16ToUtf8(wcs);

    EXPECT_EQ(mbs, mbs2);
}