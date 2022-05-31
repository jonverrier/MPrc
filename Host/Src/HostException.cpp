/////////////////////////////////////////
// HostException.cpp
// Copyright (c) 2020 TXPCo Ltd
/////////////////////////////////////////

#include "pch.h"

#include <string>
#include <iostream>
#include <atlbase.h> 
#include <OleAuto.h>
#include <D2DErr.h>
#include <mferror.h>

#include "../../Include/HostException.h"

//#define CODEFROMHRESULT(hr)        (((hr & 0xFFFF0000) == MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, 0)) ? HRESULT_CODE(hr) : (hr == S_OK ? 0 : -1))
//#define TEXTFROMHRESULT(hr)        (_com_error(hr).ErrorMessage())

const std::wstring defaultErrorString = L"Cannot obtain error information from system.\n";

namespace Host {

    ///////////////////////////////////////////////////////////////////////////////
    // HostException
    ///////////////////////////////////////////////////////////////////////////////

    Exception::Exception(unsigned int errorCode,
        const wchar_t* fileName,
        unsigned lineNumber)
        : m_errorCode(errorCode),
        m_lineNumber(lineNumber),
        m_fileName(fileName),
        m_formattedOutput()
    {
        std::wstring extraOut;

        logException(errorCode, fileName, lineNumber, extraOut);
        m_formattedOutput = extraOut;
    }

    Exception::Exception(const Exception& copyMe)
        : m_errorCode(copyMe.m_errorCode),
        m_lineNumber(copyMe.m_lineNumber),
        m_fileName(copyMe.m_fileName),
        m_formattedOutput(copyMe.m_formattedOutput)
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
    Exception::formattedAsString() const
    {
        return m_formattedOutput;
    }

    std::wstring
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
            && m_formattedOutput == rhs.m_formattedOutput
            && m_fileName == rhs.m_fileName
            && m_lineNumber == rhs.m_lineNumber;
    }

    bool
    Exception::operator!= (const Exception& rhs) const
    {
        if (this == &rhs)
            return FALSE;

        return m_errorCode != rhs.m_errorCode
            || m_formattedOutput != rhs.m_formattedOutput
            || m_fileName != rhs.m_fileName
            || m_lineNumber != rhs.m_lineNumber;
    }

    Exception&
    Exception::operator= (const Exception& copyMe)
    {
        m_errorCode = copyMe.m_errorCode;
        m_fileName = copyMe.m_fileName;
        m_lineNumber = copyMe.m_lineNumber;
        m_formattedOutput = copyMe.m_formattedOutput;

        return *this;
    }

    void
    Exception::logException(unsigned int errorCode,
            const wchar_t* fileName,
            unsigned int lineNumber,
            std::wstring& out)
    {
        out = L"";

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

        std::wcout << out;
    }

}