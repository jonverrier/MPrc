/////////////////////////////////////////
// HostException.cpp
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#include "pch.h"

#include <string>
#include <iostream>
#include <atlbase.h> 
#include <OleAuto.h>
#include <D2DErr.h>
#include <mferror.h>

#include "../../Include/HostException.h"

const std::wstring defaultErrorString = L"Cannot obtain error information from system.\n";

static const Host::ExceptionLogger* pGlobalLogger = NULL;

namespace Host {

    ///////////////////////////////////////////////////////////////////////////////
    // ExceptionLoggerFactory
    ///////////////////////////////////////////////////////////////////////////////

    const ExceptionLogger* ExceptionLoggerFactory::set(const ExceptionLogger* pNewLogger) {
        
        const Host::ExceptionLogger* pOldLogger = pGlobalLogger;
        
        pGlobalLogger = pNewLogger;

        return pOldLogger;
    };


    ///////////////////////////////////////////////////////////////////////////////
    // ExceptionLogger
    ///////////////////////////////////////////////////////////////////////////////
    ExceptionLogger::~ExceptionLogger(void) {
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Exception
    ///////////////////////////////////////////////////////////////////////////////

    Exception::Exception(unsigned int errorCode,
        std::wstring_view fileName,
        unsigned lineNumber)
        : m_errorCode(errorCode),
        m_lineNumber(lineNumber),
        m_fileName(fileName),
        m_formatted()
    {
        std::wstring extraOut;

        logException(errorCode, fileName, lineNumber, m_formatted);
    }

    Exception::Exception(const Exception& copyMe)
        : m_errorCode(copyMe.m_errorCode),
        m_lineNumber(copyMe.m_lineNumber),
        m_fileName(copyMe.m_fileName),
        m_formatted(copyMe.m_formatted)
    {
    }

    Exception::~Exception()
    {
    }

    unsigned int Exception::errorCode() const
    {
        return m_errorCode;
    }

    std::wstring
    Exception::formatted() const
    {
        return m_formatted;
    }

    std::wstring_view
    Exception::sourceFilename() const
    {
        return m_fileName;
    }

    unsigned
    Exception::sourceLineNumber() const
    {
        return m_lineNumber;
    }

    bool
    Exception::operator== (const Exception& rhs) const
    {
        if (this == &rhs)
            return TRUE;

        return m_errorCode == rhs.m_errorCode
            && m_formatted == rhs.m_formatted
            && m_fileName == rhs.m_fileName
            && m_lineNumber == rhs.m_lineNumber;
    }

    bool
    Exception::operator!= (const Exception& rhs) const
    {
        if (this == &rhs)
            return FALSE;

        return m_errorCode != rhs.m_errorCode
            || m_formatted != rhs.m_formatted
            || m_fileName != rhs.m_fileName
            || m_lineNumber != rhs.m_lineNumber;
    }

    Exception&
    Exception::operator= (const Exception& copyMe)
    {
        m_errorCode = copyMe.m_errorCode;
        m_fileName = copyMe.m_fileName;
        m_lineNumber = copyMe.m_lineNumber;
        m_formatted = copyMe.m_formatted;

        return *this;
    }

    void
    Exception::logException(unsigned int errorCode,
            std::wstring_view fileName,
            unsigned int lineNumber,
            std::wstring& out)
    {
        out = {};

        // System error if non zero, and facility is unspecified or WIN32
        if (errorCode != 0 &&
            ((HRESULT_FACILITY(errorCode) == FACILITY_WIN32) || (HRESULT_FACILITY(errorCode) == 0))) {

            wchar_t sz[HOST_STRING_BUFFER_SIZE];
            LONG lRet = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                NULL, errorCode, 0,
                sz,
                HOST_STRING_BUFFER_SIZE - 1,
                NULL);

            if (lRet == 0)
                out = defaultErrorString;

            out = sz;
        }
        else
        if ((HRESULT_FACILITY(errorCode) == FACILITY_MF
             || (HRESULT_FACILITY(errorCode) == FACILITY_D2D)))
        {
            CComPtr<IErrorInfo> pErrInfo;

            HRESULT result = ::GetErrorInfo(0, &pErrInfo);
            if (SUCCEEDED(result) && pErrInfo != (NULL))
            {
                BSTR bstr;
                pErrInfo->GetDescription(&bstr);

                out = bstr;
                ::SysFreeString(bstr);
                ::SetErrorInfo(0, pErrInfo); // This propogates error info up the call chain
            }
            else {
                out = defaultErrorString;
            }
        }

        if (pGlobalLogger) {
            pGlobalLogger->log(out);
        }
        else {
            std::wcout << out;
        }
    }

}