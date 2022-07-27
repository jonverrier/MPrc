/////////////////////////////////////////
// HostException.h
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#ifndef HOSTEXCEPTION_INCLUDED
#define HOSTEXCEPTION_INCLUDED

#include "Host.h"
#include <string_view>
#include <sstream>
#include <exception>
#include <iostream>


namespace Host {

    /// <summary>
    /// ExceptionLogger - signature for exception logger, defined here so users of this library can pass their own one up via dependency injection. 
    /// </summary>
    class HOST_API ExceptionLogger {
        
    public:
        virtual void log (const std::wstring_view formatted) const = 0;
        virtual void log (const std::string_view formatted) const = 0;

        virtual
        ~ExceptionLogger(void);
    }; 

    /// <summary>
    /// ExceptionLoggerFactory - holds singleton for exception logger to use. 
    /// </summary>
    class HOST_API ExceptionLoggerFactory {

    public:
        // Note - not thread safe, assumption is that this is called very early in file of program, during startup, and not called again until safe shutdown (if ever)
        // The old logger is returned to allow caller to set it back to previous
        // It is initially NULL, which means log to console
        // Callers job to reset it & delete any new objects created
        static const ExceptionLogger* set (const ExceptionLogger* pNewLogger);
        static const ExceptionLogger* get();
    };


    // Will be expanded over time
    enum ExceptionSource { kSystem = 1, kCRT = 2, kMedia };
    std::string HOST_API formatRaisedException(ExceptionSource source,
        unsigned errorNo,
        const char* pFileName,
        unsigned lineNo,
        const char* pMsg);

    /// <summary>
    /// EnrichedExFrom - describes exceptions 
    /// As a design approach - this is a 'rich' exception class that captures as much information as possible, but it also contains little logic since the system has gone wrong.
    /// We log all details at the time the exception is constructed. 
    /// Strings are kept to Char to avoid circular dependency on UTF16/8 translation classes, which can throw exceptions
    /// </summary>
    template <class AnException> class EnrichedExFrom : public  AnException  {
    public:
        EnrichedExFrom(ExceptionSource source,
            unsigned errorNo,
            const char* pFileName,
            unsigned lineNo,
            const char* pMsg = nullptr) :
            AnException (formatRaisedException (source,
                errorNo,
                pFileName,
                lineNo,
                pMsg).c_str())  {
            const ExceptionLogger* pLogger = ExceptionLoggerFactory::get();
            if (pLogger)
                pLogger->log(this->what());
            else
                std::cerr << this->what();
        }

    private:
    };

}

#define THROW_ENRICHED(type, source, errorNo, message) throw Host::EnrichedExFrom<type> ((source), (errorNo), __FILE__, __LINE__, (message))

#endif // MHOSTEXCEPTION_INCLUDED
