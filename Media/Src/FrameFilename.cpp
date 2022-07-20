/////////////////////////////////////////
// Frame.cpp
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#include "pch.h"

#include "../../Include/MediaFrameFilename.h"

#include <string>
#include <memory>
#include <stdexcept>
#include <cwchar>
#include <stdarg.h>
#include <wchar.h>

static std::shared_ptr<wchar_t> SafeSprintf(const wchar_t* format, ...) {

    va_list arguments;
    //The va_start(va_list arguments, last) macro initializes and must be called first.
    //The argument last is the name of the last argument before the variable argument list, that is, the last argument of which the calling function knows the type.
    va_start(arguments, format);

    //Upon successful return, vsnprintf returns the number of characters printed (excluding the null byte used to end output to strings).
    //For that reason we add one at the end of the length.
    const int length = vswprintf(NULL, 0, format, arguments) + 1;

    //Each invocation of va_start() must be matched by a corresponding invocation of va_end() in the same function.
    // After the call va_end(arguments) the variable arguments is undefined.
    // Multiple traversals of the list, each bracketed by va_start() and va_end() are possible. va_end() may be a macro or a function.
    va_end(arguments);

    std::shared_ptr<wchar_t> pBuffer  (static_cast<wchar_t*> (malloc (length * sizeof(wchar_t))));
    if (!pBuffer.get()) {
        throw std::overflow_error(
            "Unable to allocate translation buffer.");
    }

    va_start(arguments, format);
    vswprintf(pBuffer.get(), (size_t)length, format, arguments);
    va_end(arguments);

    return pBuffer;
}

namespace Media {

     // Constructors
    FrameFilename::FrameFilename(std::wstring projectName, unsigned scene, unsigned shot, unsigned frame, unsigned version) 
        : m_projectName (projectName), m_scene (scene), m_shot (shot), m_frame (frame), m_version (version)
    {
        if (!isValidProjectName(projectName)) {
            throw std::invalid_argument(
                "Invalid project name.");
        }
    }
    
    FrameFilename::FrameFilename(const FrameFilename& rhs) 
        : m_projectName(rhs.m_projectName), m_scene(rhs.m_scene), m_shot(rhs.m_shot), m_frame(rhs.m_frame), m_version(rhs.m_version) {
    }

    FrameFilename::~FrameFilename() {
    }

    // Attributes
    bool FrameFilename::operator==(const FrameFilename& rhs) const {
        return (m_projectName == rhs.m_projectName 
            && m_scene == rhs.m_scene 
            && m_shot == rhs.m_shot 
            && m_frame == rhs.m_frame 
            && m_version == rhs.m_version);
    }

    bool FrameFilename::operator!=(const FrameFilename& rhs) const {
        return (m_projectName != rhs.m_projectName
            || m_scene != rhs.m_scene
            || m_shot != rhs.m_shot
            || m_frame != rhs.m_frame
            || m_version != rhs.m_version);
    }

    // Operations
    FrameFilename& FrameFilename::operator=(const FrameFilename& rhs) {
        
        m_projectName = rhs.m_projectName;
        m_scene = rhs.m_scene;
        m_shot = rhs.m_shot;
        m_frame = rhs.m_frame; 
        m_version = rhs.m_version;

        return* this;
    }

    // Attributes
    std::wstring  FrameFilename::projectName() const {
        return m_projectName;
    }

    bool FrameFilename::isValidProjectName(std::wstring_view candidateName) const {
        if (candidateName.find(L"_") == std::string_view::npos)
           return true;
        return false;
    }

    unsigned  FrameFilename::scene() const {
        return m_scene;
    }

    unsigned  FrameFilename::shot() const {
        return m_shot;
    }

    unsigned  FrameFilename::frame() const {
        return m_frame;
    }

    unsigned  FrameFilename::version() const {
        return m_version;
    }

    FrameFilename MEDIA_API FrameFilename::parse(std::wstring& input) {

        wchar_t buffer[HOST_STRING_BUFFER_SIZE];
        unsigned scene = 0, shot = 0, frame = 0, version = 0;

        unsigned n = swscanf_s (input.c_str(), L"%[^_]_%03d_%03d_%03d_v%03d", buffer, HOST_STRING_BUFFER_SIZE, &scene, &shot, &frame, &version);
        if (n != 5) {
            throw std::runtime_error(
                "Unable to parse frame parameters from filename.");
        }

        buffer[HOST_STRING_BUFFER_SIZE - 1] = L'\0';

        return FrameFilename (std::wstring (buffer), scene, shot, frame, version);

    }

    std::wstring MEDIA_API FrameFilename::format(const FrameFilename& frameFilename) {

        std::shared_ptr<wchar_t> pBuffer = SafeSprintf(L"%s_%03d_%03d_%03d_v%03d", frameFilename.projectName().c_str(), frameFilename.scene(), frameFilename.shot(), frameFilename.frame(), frameFilename.version());

        return std::wstring (pBuffer.get());
    }
}