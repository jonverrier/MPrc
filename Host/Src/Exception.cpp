/////////////////////////////////////////
// HostException.cpp
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#include "pch.h"

#include <string>
#include <format>

#ifdef WINXX
#include <atlbase.h> 
#include <OleAuto.h>
#include <D2DErr.h>
#include <mferror.h>
#endif

#include "../../Include/HostException.h"

static const Host::ExceptionLogger* pGlobalLogger = NULL;
const wchar_t* defaultErrorString = L"Cannot obtain error information from system.\n";

#if 0
static void formatSystemException(unsigned code, wchar_t buffer[], unsigned bufferSize) {

#ifdef WINXX
    // System error if non zero, and facility is unspecified or WIN32
    if (code != 0 &&
        ((HRESULT_FACILITY(code) == FACILITY_WIN32) || (HRESULT_FACILITY(code) == 0))) {

        LONG lRet = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, code, 0,
            buffer,
            bufferSize - 1,
            NULL);

        if (lRet == 0)
            wcsncpy_s (buffer, bufferSize, defaultErrorString, bufferSize - 1);
    }
    else
        if ((HRESULT_FACILITY(code) == FACILITY_MF
            || (HRESULT_FACILITY(code) == FACILITY_D2D)))
        {
            CComPtr<IErrorInfo> pErrInfo;

            HRESULT result = ::GetErrorInfo(0, &pErrInfo);
            if (SUCCEEDED(result) && pErrInfo != (NULL))
            {
                BSTR bstr;
                pErrInfo->GetDescription(&bstr);
                wcsncpy_s (buffer, bufferSize, bstr, bufferSize - 1);
                ::SysFreeString(bstr);
                ::SetErrorInfo(0, pErrInfo); // This propogates error info up the call chain
            }
            else
            {
                wcsncpy_s(buffer, bufferSize, defaultErrorString, bufferSize - 1);
            }
        }
#else
    wcsncpy(buffer, defaultErrorString, bufferSize - 1);
#endif
}

void formatCrtException(unsigned code, wchar_t buffer[], unsigned bufferSize) {

    char localBuffer[HOST_STRING_BUFFER_SIZE];
    localBuffer[0] = '0';

    strerror_s (localBuffer, HOST_STRING_BUFFER_SIZE, code);

    if (strlen (localBuffer) > 0) {
       size_t copied = 0;
       auto err = mbstowcs_s(
           &copied,
           buffer,
           bufferSize,
           localBuffer,
           bufferSize);
    }
    else {
        wcsncpy_s(buffer, bufferSize, defaultErrorString, bufferSize - 1);
    }
}

#endif


namespace Host {

    ///////////////////////////////////////////////////////////////////////////////
    // ExceptionLoggerFactory
    ///////////////////////////////////////////////////////////////////////////////

    const ExceptionLogger* ExceptionLoggerFactory::set(const ExceptionLogger* pNewLogger) {
        
        const Host::ExceptionLogger* pOldLogger = pGlobalLogger;
        
        pGlobalLogger = pNewLogger;

        return pOldLogger;
    };

    const ExceptionLogger* ExceptionLoggerFactory::get() {

        return pGlobalLogger;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // ExceptionLogger
    ///////////////////////////////////////////////////////////////////////////////
    ExceptionLogger::~ExceptionLogger(void) {
    }

    ///////////////////////////////////////////////////////////////////////////////
    // formatRaisedException
    ///////////////////////////////////////////////////////////////////////////////
    std::string HOST_API formatRaisedException(ExceptionSource source,
        unsigned errorNo,
        const char* pFileName,
        unsigned lineNo,
        const char* pMsg) {

        std::stringstream ss;

        ss << std::format("Exception code:{:#x} Source:{:#x} file:{} line:{} message:{}\n", errorNo, static_cast<unsigned>(source), pFileName, lineNo, pMsg);

        return ss.str();
    }
}
