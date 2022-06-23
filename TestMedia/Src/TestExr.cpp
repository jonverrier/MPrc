#include "pch.h"

#include "../../Include/MediaUtf16Utf8.h"

#include <OpenEXR/half.h>
#include <OpenEXR/ImfRgba.h>
#include <OpenEXR/ImfRgbaFile.h>

// Not really a test, but fails to build if OpenEXR is not installed properly. 

TEST(OpenEXR, Simplewrite) {

	int width = 100, height = 100;
	std::string filename = "test.exr";

	{
		Imf_2_5::RgbaOutputFile outputFile(filename.c_str(), width, height, Imf_2_5::RgbaChannels::WRITE_RGBA);
		std::unique_ptr < Imf_2_5::Rgba[]> pixels(new Imf_2_5::Rgba[width * height]);

		int yStride = width;
		int xStride = 1;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				pixels[x * xStride + y * yStride].a = HALF_MAX;
				pixels[x * xStride + y * yStride].g = HALF_MIN;
				pixels[x * xStride + y * yStride].b = HALF_MIN;
				if (y % 2 == 0)
					pixels[x * xStride + y * yStride].r = HALF_MAX;
				else
					pixels[x * xStride + y * yStride].r = HALF_MIN;
			}
		}

		outputFile.setFrameBuffer(pixels.get(), 1, width);
		outputFile.writePixels(height);
	}
	Imf_2_5::RgbaInputFile inputFile(filename.c_str());

	EXPECT_EQ(inputFile.isComplete(), true);
}