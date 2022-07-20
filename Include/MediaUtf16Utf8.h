/////////////////////////////////////////
// MediaUtf16Utf8.h
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#ifndef MEDIAUTF16UTF8_INCLUDED
#define MEDIAUTF16UTF8_INCLUDED

#include <string_view>

#include "Host.h"
#include "HostException.h"
#include "Media.h"

#define SPDLOG_HEADER_ONLY
#include "spdlog/spdlog.h"
#include "spdlog/logger.h"

namespace Media {

    /// <summary>
    /// Utf16ToUtf8 - transcode from UTF16 to 8
    /// </summary>
    std::string MEDIA_API Utf16ToUtf8(const std::wstring_view& utf16, bool strict = true);

    /// <summary>
    /// Utf8ToUtf16 - transcode from UTF8 to 16
    /// </summary>
    std::wstring MEDIA_API Utf8ToUtf16(const std::string_view& utf8, bool strict = true);

}

#endif // MEDIAUTF16UTF8_INCLUDED
