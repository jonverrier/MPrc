/////////////////////////////////////////
// MediaFrame.h
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#ifndef MEDIAFRAME_INCLUDED
#define MEDIAFRAME_INCLUDED

#include <string_view>

#include "OpenEXR/ImfRgba.h"

#include "Host.h"
#include "HostException.h"
#include "Media.h"

namespace Media {


#pragma pack (push, 1)
    //
    // RGB pixel 8 bit packed
    //
    struct Rgb8
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;

        Rgb8() : r(0), g(0), b(0) {}
        Rgb8(unsigned char  r, unsigned char  g, unsigned char  b) : r(r), g(g), b(b) {}
    };

    //
    // RGBA pixel 8 bit packed
    //
    struct Rgba8
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;

        Rgba8() : r(0), g(0), b(0), a(0xFF) {}
        Rgba8(unsigned char  r, unsigned char  g, unsigned char  b, unsigned char  a = 0xFF) : r(r), g(g), b(b), a(a) {}
    };
#pragma pack (pop)

    typedef Imf_2_5::Rgba RgbaHalf;

    ///////////////////////////////////////////////////////////////////////////////
    // Frame
    ///////////////////////////////////////////////////////////////////////////////

    class MEDIA_API Frame
    {
    public:

        // Constructors
        Frame(unsigned cx, unsigned cy);
        Frame(const Frame& rhs);
        virtual ~Frame();

        // Attributes
        bool operator==(const Frame& rhs) const;
        bool operator!=(const Frame& rhs) const;

        // Operations
        Frame& operator=(const Frame& rhs);

        unsigned  width() const;
        unsigned  height() const;

    private:
        unsigned     m_cx, m_cy;
    };


    // These functions are outside the class, as they allow buffer to be allocated separately then passed in
    // eg if you are reading bytes from file, allocate the buffer, read them in, then pass buffer to this class ->saves an allocation
    Rgba8 MEDIA_API initialiseEmptyPixel(Rgba8& pixel);

    Media::RgbaHalf MEDIA_API initialiseEmptyPixel(Media::RgbaHalf& pixel);

    template <class APixel> unsigned stridePixels(unsigned cx, const APixel& pixel) {
        
        // for the pixel format we support (RGBA 8bit, RGBA HALF), stride is always equal to width as they are byte aligned, no partial bytes with half a pixel (e.g. 4 bit)
        return cx;
    }

    template <class APixel> unsigned strideBytes(unsigned cx, const APixel& pixel) {
        // for the pixel format we support (RGBA 8bit, RGBA HALF), stride is always equal to width as they are byte aligned, no partial bytes with half a pixel (e.g. 4 bit)
        return stridePixels(cx, pixel) * sizeof(pixel);
    }

    template <class APixel> std::shared_ptr <APixel[]> allocatePixelBuffer(unsigned cx, unsigned cy, const APixel& initialValue) {
        
        unsigned stride = stridePixels(cx, initialValue);

        std::shared_ptr <APixel[]> pixels(new APixel[stride * cy]);
        for (unsigned y = 0; y < cy; y++) {
            for (unsigned x = 0; x < cx; x++) {
                pixels[x + y * stride] = initialValue;
            }
        }

        return pixels;
    }

    template <class APixel> std::shared_ptr <APixel[]> allocateDuplicatePixelBuffer(unsigned cx, unsigned cy, std::shared_ptr <APixel[]> existingPixels) {

        APixel pixel;
        unsigned stride = stridePixels(cx, pixel);

        std::shared_ptr <APixel[]> pixels(new APixel[stride * cy]);
        for (unsigned y = 0; y < cy; y++) {
            memcpy(&pixels[y * stride], &existingPixels[y * stride], cx * sizeof(APixel));
        }

        return pixels;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // FrameOf<PixelClass>
    ///////////////////////////////////////////////////////////////////////////////
    template <class APixel> class FrameOf : public Frame
    {
    public:

        // Constructors
        FrameOf(unsigned cx, unsigned cy) : Frame (cx, cy) {

            APixel initialValue = initialiseEmptyPixel(initialValue);
            
            m_stride = stridePixels(cx, initialValue);
            m_pixels = allocatePixelBuffer(cx, cy, initialValue);
        }

        FrameOf(unsigned cx, unsigned cy, std::shared_ptr <APixel[]> pixels, unsigned stride) : Frame(cx, cy) {

            APixel initialValue = initialiseEmptyPixel(initialValue);

            m_stride = stride;
            m_pixels = pixels;
        }

        FrameOf(const FrameOf& rhs) : Frame(rhs.width(), rhs.height()) {
            m_stride = rhs.m_stride;
            m_pixels = allocateDuplicatePixelBuffer(width(), rhs.height(), rhs.m_pixels);
        }
        
        ~FrameOf() {
        }

        // Attributes
        bool operator==(const FrameOf& rhs) const {
            if (!Frame::operator == (rhs))
                return false;

            for (unsigned y = 0; y < height(); y++) {
                if (! (memcmp(&m_pixels[y * m_stride], &rhs.m_pixels[y * m_stride], width() * sizeof(APixel)) == 0)) {
                    return false;
                }
            }
            
            return true;
        }

        bool operator!=(const FrameOf& rhs) const {
            if (Frame::operator != (rhs))
                return true;

            for (unsigned y = 0; y < height(); y++) {
                if (memcmp(&m_pixels[y * m_stride], &rhs.m_pixels[y * m_stride], width() * sizeof(APixel)) != 0) {
                    return true;
                }
            }

            return false;
        }

        // Operations
        FrameOf& operator=(const FrameOf& rhs) {
            Frame::operator=(rhs);
            m_stride = rhs.m_stride;
            m_pixels = allocateDuplicatePixelBuffer(rhs.width(), rhs.height(), rhs.m_pixels);

            return *this;
        }

        APixel pixelAt(unsigned x, unsigned y) const {
            return m_pixels[static_cast<size_t>(y) * m_stride + x];          // static_cast to widen to 8 bytes & avoid warning about possible overflow 
        }

        void setPixelAt(unsigned x, unsigned y, const APixel& pixel) {
            m_pixels[static_cast<size_t>(y) * m_stride + x] = pixel;        // static_cast to widen to 8 bytes & avoid warning about possible overflow        
        }

    private:
        unsigned m_stride;
#pragma warning (push)
#pragma warning (disable: 4251) // Member is private anyway
        std::shared_ptr <APixel[]> m_pixels;
#pragma warning (pop)
    };

    typedef FrameOf<Rgba8> FrameRgba8;
    typedef FrameOf<RgbaHalf> FrameRgbaHalf;

    ///////////////////////////////////////////////////////////////////////////////
    // FrameColourOutputMapper
    ///////////////////////////////////////////////////////////////////////////////

    class MEDIA_API FrameColourOutputMapper
    {
    public:

        // Constructors
        FrameColourOutputMapper();
        virtual ~FrameColourOutputMapper();


        // Operations
        virtual void convert(FrameRgba8& poutput, const FrameRgbaHalf& input) = 0;

    private:
        FrameColourOutputMapper(const FrameColourOutputMapper& rhs);
    };

    ///////////////////////////////////////////////////////////////////////////////
    // FrameSrgbOutputMapper
    ///////////////////////////////////////////////////////////////////////////////

    class MEDIA_API FrameSrgbOutputMapper : public FrameColourOutputMapper
    {
    public:

        // Constructors
        FrameSrgbOutputMapper();
        virtual ~FrameSrgbOutputMapper();


        // Operations
        void convert(FrameRgba8& output, const FrameRgbaHalf& input);

    private:
        FrameSrgbOutputMapper(const FrameSrgbOutputMapper& rhs);
    };

    ///////////////////////////////////////////////////////////////////////////////
    // FrameColourInputMapper
    ///////////////////////////////////////////////////////////////////////////////

    class MEDIA_API FrameColourInputMapper
    {
    public:

        // Constructors
        FrameColourInputMapper();
        virtual ~FrameColourInputMapper();


        // Operations
        virtual void convert(FrameRgbaHalf& output, const FrameRgba8& input) = 0;

    private:
        FrameColourInputMapper(const FrameColourInputMapper& rhs);
    };

    ///////////////////////////////////////////////////////////////////////////////
    // FrameSrgbInputMapper
    ///////////////////////////////////////////////////////////////////////////////

    class MEDIA_API FrameSrgbInputMapper : public FrameColourInputMapper
    {
    public:

        // Constructors
        FrameSrgbInputMapper();
        virtual ~FrameSrgbInputMapper();


        // Operations
        void convert(FrameRgbaHalf& output, const FrameRgba8& input);

    private:
        FrameSrgbInputMapper(const FrameSrgbInputMapper& rhs);
    };
}

#endif // MEDIAFRAME_INCLUDED
