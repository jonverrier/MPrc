/////////////////////////////////////////
// ExceptionLogger.cpp
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#include "pch.h"

#include <string>
#include <iostream>
#include <memory>

#include "../../Include/HostException.h"
#include "../../Include/MediaException.h"
#include "../../Include/MediaLogger.h"
#include "../../Include/MediaUtf16Utf8.h"

namespace Media {

    ///////////////////////////////////////////////////////////////////////////////
    // MediaHostExceptionLogger
    ///////////////////////////////////////////////////////////////////////////////
    void ExceptionLogger::log(const std::wstring_view formatted) const {

        // Transcode without strict translation - so we at least get seomething in the log. 
        Media::LoggerFactory::get()->error (Media::Utf16ToUtf8 (formatted, false));

    };

    ExceptionLogger::~ExceptionLogger(void) {
    };

}