/////////////////////////////////////////
// MediaLogger.h
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#ifndef MEDIALOGGER_INCLUDED
#define MEDIALOGGER_INCLUDED

#include <string_view>

#include "Host.h"
#include "Media.h"

#define SPDLOG_HEADER_ONLY
#include "spdlog/spdlog.h"
#include "spdlog/logger.h"

namespace Media {

    /// <summary>
    /// LoggerFactory - holds singleton for logging
    /// </summary>
    class MEDIA_API LoggerFactory {

    public:
        // Note - not thread safe, assumption is that this is called very early in file of program, during startup, and not called again until safe shutdown (if ever)
        // The old logger is returned to allow caller to set it back to previous
        // It is initially NULL, which means log to console
        // Callers job to reset it & delete any new objects created
        static std::shared_ptr<spdlog::logger> get();
    };

}

#endif // MEDIALOGGER_INCLUDED
