#include "pch.h"

#define U_SHOW_CPLUSPLUS_API 1
#include <unicode/unistr.h>

#include "../../Include/MediaUtf16Utf8.h"

// Not really a test, but fails to build if ICU is not installed properly. 

TEST(MediaUnicode, Unicode) {

	std::string mbs("A string.");

	icu::UnicodeString ucs = icu::UnicodeString::fromUTF8(icu::StringPiece(mbs.c_str()));

	std::string mbs2;

	ucs.toUTF8String(mbs2);

	EXPECT_EQ(mbs, mbs2);
}