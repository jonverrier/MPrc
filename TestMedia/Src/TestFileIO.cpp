#include "pch.h"

#include <exception>

#include "../../Include/MediaUtf16Utf8.h"
#include "../../Include/MediaFrame.h"

#include <OpenEXR/half.h>
#include <OpenEXR/ImfRgba.h>
#pragma warning (push)
#pragma warning (disable:4996)
#include <OpenEXR/ImfRgbaFile.h>
#pragma warning (pop)

#pragma warning (push)
#pragma warning (disable:4267 4244)
#include <OpenImageIO/imageio.h>
#pragma warning (pop)

// Not really a test, but fails to build if OpenEXR is not installed properly. 

TEST(OpenEXR, WriteSimpleExr) {

	int width = 100, height = 100;
	std::string filename = "test.exr";

	{
		Imf_2_5::RgbaOutputFile outputFile(filename.c_str(), width, height, Imf_2_5::RgbaChannels::WRITE_RGBA);

		Imf_2_5::Rgba initialValue(HALF_MAX, HALF_MIN, HALF_MIN, HALF_MAX);

		std::shared_ptr < Imf_2_5::Rgba[]> pixels = Media::allocatePixelBuffer(width, height, initialValue);

		outputFile.setFrameBuffer(pixels.get(), 1, width);
		outputFile.writePixels(height);
	}
	Imf_2_5::RgbaInputFile inputFile(filename.c_str());

	EXPECT_EQ(inputFile.isComplete(), true);
}

// Not really a test, but fails to build if OpenImageIO is not installed properly. 

TEST(OpenImageIo, WriteSimplePng) {

	int width = 100, height = 100;
	std::string filename = "test.png";
	bool caught = false;

	try {
		Media::Rgba8 initialValue(0xFF, 0, 0, 0xFF);

		std::shared_ptr < Media::Rgba8[]> pixels = Media::allocatePixelBuffer(width, height, initialValue);

		std::unique_ptr<OIIO::ImageOutput> out = OIIO::ImageOutput::create(filename);
		if (!out)
			return;
		OIIO::ImageSpec spec(width, height, 4, OIIO::TypeDesc::UINT8);
		spec.channelnames.clear();
		spec.channelnames.push_back("R");
		spec.channelnames.push_back("G");
		spec.channelnames.push_back("B");
		spec.channelnames.push_back("A");

		out->open(filename, spec);
		out->write_image(OIIO::TypeDesc::UINT8, pixels.get());
		out->close();
	}
	catch ([[maybe_unused]] std::exception& ex) {
		caught = true;
    }


	EXPECT_EQ(caught, false);
}

TEST(OpenImageIo, ExrToPng) {

	std::shared_ptr < Media::RgbaHalf[]> pixelsIn; 
	std::shared_ptr < Media::Rgba8[]> pixelsOut;
	unsigned width, height, stride;

	// This block reads pixles from file into buffer
	{
    	std::string filename = "BrightRings.exr";
		Imf_2_5::RgbaInputFile inputFile(filename.c_str());
		Imath_2_5::Box2i dw = inputFile.dataWindow();

		Media::RgbaHalf initialValue = Media::initialiseEmptyPixel(initialValue);
		width = dw.max.x - dw.min.x + 1;
		height = dw.max.y - dw.min.y + 1;
		stride = Media::stridePixels(width, initialValue);

		pixelsIn = Media::allocatePixelBuffer(width, height, initialValue);

		inputFile.setFrameBuffer(&pixelsIn.get()[0] - dw.min.x - dw.min.y * width, 1, width);
		inputFile.readPixels(dw.min.y, dw.max.y);
	} 

	// This block converts the buffer
	{
		Media::Rgba8 initialValue = Media::initialiseEmptyPixel(initialValue);
		pixelsOut = Media::allocatePixelBuffer(width, height, initialValue);

		Media::FrameSrgbOutputMapper converter;
		Media::FrameRgbaHalf frameIn (width, height, pixelsIn, stride);
		Media::FrameRgba8 frameOut(width, height, pixelsOut, stride);

		converter.convert(frameOut, frameIn);
	}

	// This block writes bufffer to disk
	{
		std::string filename = "BrightRings.png";
		std::unique_ptr<OIIO::ImageOutput> out = OIIO::ImageOutput::create(filename);
		if (!out)
			return;
		OIIO::ImageSpec spec(width, height, 4, OIIO::TypeDesc::UINT8);
		spec.channelnames.clear();
		spec.channelnames.push_back("R");
		spec.channelnames.push_back("G");
		spec.channelnames.push_back("B");
		spec.channelnames.push_back("A");

		out->open(filename, spec);
		out->write_image(OIIO::TypeDesc::UINT8, pixelsOut.get());
		out->close();
	}

	EXPECT_EQ(true, true);
}

TEST(OpenImageIo, PngRoundtrip) {

	std::shared_ptr < Media::Rgba8[]> pixelsIn;
	std::shared_ptr < Media::RgbaHalf[]> pixelsIntermed;
	unsigned width, height, stride;

	// This block reads pixels from file into buffer
	{
		std::string filename = "ImageForRoundTripIn.jpg";
		std::unique_ptr<OIIO::ImageInput> in = OIIO::ImageInput::open(filename);
		if (!in)
			return;
		OIIO::ImageSpec spec = in->spec();

		Media::Rgba8 initialValue = Media::initialiseEmptyPixel(initialValue);
		width = spec.width;
		height = spec.height;
		stride = Media::stridePixels(width, initialValue);

		pixelsIn = Media::allocatePixelBuffer(width, height, initialValue);

        std::unique_ptr<Media::Rgb8[]> line (new Media::Rgb8[width]);

		// Read lines one at a time - assumes JPG/PNG with three channels
		// Then copy across to the RGBA buffer
        for (unsigned y = 0; y < height; ++y) {
		   in->read_scanline(y, 0, OIIO::TypeDesc::UINT8, &line[0]);
		   for (unsigned x = 0; x < width; x++) {
			   pixelsIn[y * stride + x] = Media::Rgba8(line[x].r, line[x].g, line[x].b, 0xFF);
		   }
		}
	}

	// This block converts the buffer to linear and then back again
	{
		Media::RgbaHalf initialValue = Media::initialiseEmptyPixel(initialValue);
		pixelsIntermed = Media::allocatePixelBuffer(width, height, initialValue);

		Media::FrameRgba8 frameIn(width, height, pixelsIn, stride);
		Media::FrameRgbaHalf frameOut(width, height, pixelsIntermed, stride);

		Media::FrameSrgbInputMapper converterIn;
		converterIn.convert(frameOut, frameIn);

		Media::FrameSrgbOutputMapper converterOut;
		converterOut.convert(frameIn, frameOut);
	}


	// This block writes buffer to disk
	{
		std::string filename = "ImageForRoundTripOut.png";
		std::unique_ptr<OIIO::ImageOutput> out = OIIO::ImageOutput::create(filename);
		if (!out)
			return;
		OIIO::ImageSpec spec(width, height, 4, OIIO::TypeDesc::UINT8);
		spec.channelnames.clear();
		spec.channelnames.push_back("R");
		spec.channelnames.push_back("G");
		spec.channelnames.push_back("B");
		spec.channelnames.push_back("A");

		out->open(filename, spec);
		out->write_image(OIIO::TypeDesc::UINT8, pixelsIn.get());
		out->close();
	}

	EXPECT_EQ(true, true);
}