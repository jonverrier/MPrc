/////////////////////////////////////////
// ExceptionLogger.cpp
// Copyright (c) 2020 TXPCo Ltd
/////////////////////////////////////////

#include "pch.h"

#include <string>
#include <iostream>
#include <memory>

#include "../../Include/HostException.h"
#include "../../Include/MediaException.h"
#include "../../Include/MediaLogger.h"

/* 
 * https://docs.microsoft.com/en-us/archive/msdn-magazine/2016/september/c-unicode-encoding-conversions-with-stl-strings-and-win32-apis#unicode-encoding-conversion-library
 */

std::string Utf16ToUtf8(const std::wstring_view& utf16);

std::string Utf16ToUtf8(const std::wstring_view& utf16)
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
    std::unique_ptr<char> pUtf8Buffer (static_cast <char*> (malloc (utf16Length + 1)));

    // Copy it over  
    size_t copied = 0;
    auto err = wcstombs_s(
        &copied,
        pUtf8Buffer.get(),
        utf16Length + 1,
        utf16.data(),
        utf16Length);

    utf8 = (pUtf8Buffer.get());

    return utf8;
}

namespace Media {

    ///////////////////////////////////////////////////////////////////////////////
    // MediaHostExceptionLogger
    ///////////////////////////////////////////////////////////////////////////////
    void ExceptionLogger::log(const std::wstring_view formatted) const {

        Media::LoggerFactory::get()->error (Utf16ToUtf8 (formatted));

    };

    ExceptionLogger::~ExceptionLogger(void) {
    };

}