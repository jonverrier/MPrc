/////////////////////////////////////////
// ExceptionLogger.cpp
// Copyright (c) 2020 TXPCo Ltd
/////////////////////////////////////////

#include "pch.h"

#include <string>
#include <iostream>

#include "../../Include/HostException.h"
#include "../../Include/MediaException.h"

namespace Media {

    ///////////////////////////////////////////////////////////////////////////////
    // MediaHostExceptionLogger
    ///////////////////////////////////////////////////////////////////////////////
    void HostExceptionLogger::log(const std::wstring_view formatted) const {

        std::wcout << formatted;

    };

    HostExceptionLogger::~HostExceptionLogger(void) {
    };

}