/////////////////////////////////////////
// HostException.h
// Copyright (c) 2020 TXPCo Ltd
/////////////////////////////////////////

#ifndef HOSTEXCEPTION_INCLUDED
#define HOSTEXCEPTION_INCLUDED

#include "Host.h"
#include <string>

namespace Host {

    /// <summary>
    /// HostException - describes exceptions specific to the underlying operating system
    /// </summary>
    class HOST_API Exception
    {
    public:

        // Constructors 
        Exception(unsigned int errorCode,
                  const wchar_t* fileName, unsigned lineNumber);

        Exception(const Exception& copyMe);

        virtual
            ~Exception(void);

        // Attributes
        unsigned int errorCode() const;

        std::wstring sourceFilename() const;

        unsigned int sourceLineNumber() const;

        std::wstring formattedAsString() const;

        // Comparison Operators
        bool operator== (const Exception& rhs) const;

        bool operator!= (const Exception& rhs) const;

        // Operations
        Exception& operator= (const Exception& copyMe);

    protected:

    private:
        void
            logException(unsigned int errorCode,
                const wchar_t* fileName, unsigned int lineNumber, std::wstring& formattedOutput);

        unsigned int  m_lineNumber;
        unsigned int  m_errorCode;

#pragma warning (push)
#pragma warning (disable: 4251)
        std::wstring  m_fileName;
        std::wstring  m_formattedOutput;
#pragma warning (pop)

    };

}

#define THROW(e, code) throw e(code, HOSTWFILE, __LINE__)

#endif // MHOSTEXCEPTION_INCLUDED
