/////////////////////////////////////////
// ExceptionLogger.cpp
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#include "pch.h"

#include <string>
#include <iostream>

#include "../../Include/MediaLogger.h"
#include "../../Include/spdlog/sinks/stdout_color_sinks.h"

static std::shared_ptr<spdlog::logger> pGlobalLogger = NULL;

namespace Media {


    std::shared_ptr<spdlog::logger> LoggerFactory::get() {

        if (!pGlobalLogger) {
            pGlobalLogger = spdlog::stdout_color_mt("console");
        }
        return pGlobalLogger;
    };
}