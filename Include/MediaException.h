/////////////////////////////////////////
// MediaException.h
// Copyright (c) 2020 TXPCo Ltd
/////////////////////////////////////////

#ifndef MEDIAEXCEPTION_INCLUDED
#define MEDIAEXCEPTION_INCLUDED

#include <string_view>
#include "Host.h"
#include "HostException.h"
#include "Media.h"

namespace Media {

    /// <summary>
    /// MediaExceptionLogger -  implements a logger to pass to Host layer for logging by dependency injection
    /// </summary>
    class MEDIA_API HostExceptionLogger : public Host::ExceptionLogger {
        
    public:
        virtual void log (const std::wstring_view formatted) const;
        
        virtual
        ~HostExceptionLogger(void);
    }; 

}

#endif // MEDIAEXCEPTION_INCLUDED
