/////////////////////////////////////////
// ExceptionLogger.cpp
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#include "pch.h"

#include <string>
#include <iostream>
#include <memory>

#include "../../Include/MediaUtf16Utf8.h"

/* 
 * https://docs.microsoft.com/en-us/archive/msdn-magazine/2016/september/c-unicode-encoding-conversions-with-stl-strings-and-win32-apis#unicode-encoding-conversion-library
 */

namespace Media {

    std::string Utf16ToUtf8(const std::wstring_view& utf16, bool strict)
    {
        std::string utf8; // Result

        if (utf16.empty())
        {
            return utf8;
        }

        if (utf16.length() > static_cast<size_t>((std::numeric_limits<int>::max)()))
        {
            throw std::overflow_error(
                "Input string too long: size_t-length doesn't fit into int.");
        }

        // Safely convert from size_t(STL string's length)
        // to int (for Win32 APIs)
        const int utf16Length = static_cast<int>(utf16.length());

        // Allocate buffer, with a safe pointer
        std::unique_ptr<char> pUtf8Buffer(static_cast <char*> (malloc(utf16Length + 1)));

        // Copy it over  
        size_t copied = 0;
        auto err = wcstombs_s(
            &copied,
            pUtf8Buffer.get(),
            utf16Length + 1,
            utf16.data(),
            utf16Length);

        if (strict && copied != utf16Length + 1)
            throw  std::invalid_argument(
                "Unable to translate all of input string to UTF8.");

        utf8 = (pUtf8Buffer.get());

        return utf8;
    }

    std::wstring Utf8ToUtf16(const std::string_view& utf8, bool strict)
    {
        std::wstring utf16; // Result

        if (utf8.empty())
        {
            return utf16;
        }

        if (utf8.length() > static_cast<size_t>((std::numeric_limits<int>::max)()))
        {
            throw std::overflow_error(
                "Input string too long: size_t-length doesn't fit into int.");
        }

        // Safely convert from size_t(STL string's length)
        // to int (for Win32 APIs)
        const int utf8Length = static_cast<int>(utf8.length());

        // Allocate buffer, with a safe pointer
        std::unique_ptr<wchar_t> pUtf16Buffer(static_cast <wchar_t*> (malloc((utf8Length + 1) * sizeof (wchar_t))));
        if (!pUtf16Buffer.get()) {
            throw std::overflow_error(
                "Unable to allocate translation buffer.");
        }

        // Copy it over  
        size_t copied = 0;
        auto err = mbstowcs_s(
            &copied,
            pUtf16Buffer.get(),
            utf8Length + 1,
            utf8.data(),
            utf8Length);

        if (strict && copied != utf8Length + 1)
            throw  std::invalid_argument(
                "Unable to translate all of input string to UTF8.");

        utf16 = (pUtf16Buffer.get());

        return utf16;
    }

}