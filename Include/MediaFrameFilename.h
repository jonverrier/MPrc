/////////////////////////////////////////
// MediaFrameFilename.h
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#ifndef MEDIAFRAMEFILENAME_INCLUDED
#define MEDIAFRAMEFILENAME_INCLUDED

#include <string_view>

#include "Host.h"
#include "HostException.h"
#include "Media.h"


namespace Media {

    ///////////////////////////////////////////////////////////////////////////////
    // FrameFilename
    // https://partnerhelp.netflixstudios.com/hc/en-us/articles/360057627473-VFX-Shot-and-Version-Naming-Recommendations
    // We use 'project' in place of show, scene, shot, frame, & version 
    ///////////////////////////////////////////////////////////////////////////////

    class MEDIA_API FrameFilename
    {
    public:

        // Constructors
        FrameFilename(std::wstring projectName, unsigned scene, unsigned shot, unsigned frame, unsigned version);
        FrameFilename(const FrameFilename& rhs);
        virtual ~FrameFilename();

        // Attributes
        bool operator==(const FrameFilename& rhs) const;
        bool operator!=(const FrameFilename& rhs) const;

        // Operations
        FrameFilename& operator=(const FrameFilename& rhs);

        // Attributes
        bool isValidProjectName(std::wstring_view candidateName) const;
        std::wstring  projectName() const;
        unsigned  scene() const;
        unsigned  shot() const;
        unsigned  frame() const;
        unsigned  version() const;

        static FrameFilename parse(std::wstring& input);
        static std::wstring format(const FrameFilename& frameFilename);

    private:
#pragma warning (push)
#pragma warning (disable: 4251) // Member is private anyway
        std::wstring m_projectName;
#pragma warning (pop)
        unsigned m_scene, m_shot, m_frame, m_version;
    };

}

#endif // MEDIAFRAMEFILENAME_INCLUDED
