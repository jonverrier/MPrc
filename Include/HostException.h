/////////////////////////////////////////
// HostException.h
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#ifndef HOSTEXCEPTION_INCLUDED
#define HOSTEXCEPTION_INCLUDED

#include "Host.h"
#include <string_view>

namespace Host {

    /// <summary>
    /// ExceptionLogger - signature for exception logger, defined here so users of this library can pass their own one up via dependency injection. 
    /// </summary>
    class HOST_API ExceptionLogger {
        
    public:
        virtual void log (const std::wstring_view formatted) const = 0;
        
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
    };

    /// <summary>
    /// Exception - describes exceptions specific to the underlying operating system
    /// </summary>
    class HOST_API Exception
    {
    public:

        // Constructors 
        Exception(unsigned int errorCode,
                  std::wstring_view fileName, 
                  unsigned lineNumber);

        Exception(const Exception& copyMe);

        virtual
        ~Exception(void);

        // Attributes
        unsigned int errorCode() const;

        std::wstring_view sourceFilename() const;

        unsigned int sourceLineNumber() const;

        std::wstring formatted() const;

        // Comparison Operators
        bool operator== (const Exception& rhs) const;

        bool operator!= (const Exception& rhs) const;

        // Operations
        Exception& operator= (const Exception& copyMe);

    protected:

    private:
        void
        logException(unsigned int errorCode,
                     std::wstring_view fileName, 
                     unsigned int lineNumber,
                     std::wstring& formattedOutput);

        unsigned int  m_lineNumber;
        unsigned int  m_errorCode;

#pragma warning (push)
#pragma warning (disable: 4251)
        std::wstring_view  m_fileName;
        std::wstring  m_formatted;
#pragma warning (pop)

    };

}

#endif // MHOSTEXCEPTION_INCLUDED
