/////////////////////////////////////////
// Frame.cpp
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#include "pch.h"

#include <cassert>
#include <algorithm>
#include <string>

#include "../../Include/MediaFrame.h"

namespace Media {

    ///////////////////////////////////////////////////////////////////////////////
    // Frame
    ///////////////////////////////////////////////////////////////////////////////

    Frame::Frame(unsigned cx, unsigned cy)
    {
        m_cx = cx;
        m_cy = cy;
    }

    Frame::Frame(const Frame& rhs)
    {
        m_cx = rhs.m_cx;
        m_cy = rhs.m_cy;
    }

    Frame::~Frame()
    {
    }

    Frame& Frame::operator=(const Frame& rhs)
    {
        if (this == &rhs)
            return *this;

        m_cx = rhs.m_cx;
        m_cy = rhs.m_cy;

        return *this;
    }

    bool
        Frame::operator==(const Frame& rhs) const
    {
        if (this == &rhs)
            return true;

        return ((m_cx == rhs.m_cx)
            && (m_cy == rhs.m_cy)
            );
    }

    bool
        Frame::operator!=(const Frame& rhs) const
    {
        if (this == &rhs)
            return false;

        return ((m_cx != rhs.m_cx)
            || (m_cy != rhs.m_cy)
            );
    }


    unsigned
        Frame::width() const
    {
        return (m_cx);
    }

    unsigned
        Frame::height() const
    {
        return (m_cy);
    }

    Rgba8 initialiseEmptyPixel(Rgba8& pixel) {

        pixel.r = pixel.g = pixel.b = 0;
        pixel.a = 0xFF;

        return pixel;
    }

    Media::RgbaHalf initialiseEmptyPixel(Media::RgbaHalf& pixel) {

        pixel.r = pixel.g = pixel.b = 0;
        pixel.a = 1.0;

        return pixel;
    }

}