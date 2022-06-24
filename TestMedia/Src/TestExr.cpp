#include "pch.h"

#include "../../Include/MediaUtf16Utf8.h"
#include "../../Include/MediaFrame.h"

#include <OpenEXR/half.h>
#include <OpenEXR/ImfRgba.h>
#include <OpenEXR/ImfRgbaFile.h>

// Not really a test, but fails to build if OpenEXR is not installed properly. 

TEST(OpenEXR, Simplewrite) {

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