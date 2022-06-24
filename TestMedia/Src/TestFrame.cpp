#include "pch.h"

#include "../../Include/MediaUtf16Utf8.h"
#include "../../Include/MediaFrame.h"

#include <OpenEXR/half.h>
#include <OpenEXR/ImfRgba.h>


TEST(Frame, ConstructCopy) {

	int width = 100, height = 100;

	Imf_2_5::Rgba initialValue(HALF_MAX, HALF_MIN, HALF_MIN, HALF_MAX);
	Imf_2_5::Rgba differentValue(HALF_MAX, HALF_MAX, HALF_MAX, HALF_MAX);

	Media::FrameRgbaHalf frame(width, height);
	Media::FrameRgbaHalf assignedFrame(0, 0);

	Media::FrameRgbaHalf copiedFrame(frame);
	assignedFrame = frame;
	

	EXPECT_EQ(frame, copiedFrame);
	EXPECT_EQ(frame, assignedFrame);
}

TEST(Frame, Amend) {

	int width = 100, height = 100;

	Imf_2_5::Rgba initialValue(HALF_MAX, HALF_MIN, HALF_MIN, HALF_MAX);
	Imf_2_5::Rgba differentValue(HALF_MAX, HALF_MAX, HALF_MAX, HALF_MAX);

	Media::FrameRgbaHalf frame(width, height);
	Media::FrameRgbaHalf differentFrame1(0, 0);
	Media::FrameRgbaHalf differentFrame2(0, 0);

	differentFrame1 = frame;
	differentFrame1.setPixelAt(0, 0, differentValue);

	differentFrame2 = frame;
	differentFrame2.setPixelAt(99, 99, differentValue);

	EXPECT_NE(frame, differentFrame1);
	EXPECT_NE(frame, differentFrame2);
}