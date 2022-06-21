#include "pch.h"

#include "../../Include/MediaUtf16Utf8.h"
#define U_SHOW_CPLUSPLUS_API TRUE
#include <unicode/unistr.h>

TEST(MediaUnicode, Unicode) {

	std::string mbs("A string.");

	U_ICU_NAMESPACE::UnicodeString ucs = U_ICU_NAMESPACE::UnicodeString::fromUTF8(U_ICU_NAMESPACE::StringPiece(mbs.c_str()));

	std::string mbs2;

	ucs.toUTF8String(mbs2);

	EXPECT_EQ(mbs, mbs2);
}