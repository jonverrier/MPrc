#include "pch.h"

#include "../../Include/MediaGeometry.h"


// Not really a test, but fails to build if boost::geometry is not installed properly. 
TEST(MediaGeometry, GeometryExists) {

	Media::MGPoint pt(0.0, 1.0);
	Media::MGBox box(pt, pt);
    
	double result = MediaGeo::area(box);

	EXPECT_EQ(result, 0.0);
}

TEST(MediaGeometry, LetterBoxVFit) {

	Media::MGSize target(2048.0, 1024.0);
	Media::MGSize source(2028.0, 1024.0);

	Media::MGBox letterBox, fill1, fill2;

	letterBox = Media::letterBox(target, source, fill1, fill2);

	// Source is offset by 10 into X axis on each side
	EXPECT_EQ(letterBox.min_corner().get<0>(), 10.0);
	EXPECT_EQ(letterBox.min_corner().get<1>(), 0.0);
	EXPECT_EQ(letterBox.max_corner().get<0>(), 2038.0);
	EXPECT_EQ(letterBox.max_corner().get<1>(), 1024.0);
}

TEST(MediaGeometry, LetterBoxHFit) {

	Media::MGSize target(2048.0, 1024.0);
	Media::MGSize source(2048.0, 1004.0);

	Media::MGBox letterBox, fill1, fill2;

	letterBox = Media::letterBox(target, source, fill1, fill2);

	// Source is offset by 10 into Y axis on each side
	EXPECT_EQ(letterBox.min_corner().get<0>(), 0.0);
	EXPECT_EQ(letterBox.min_corner().get<1>(), 10.0);
	EXPECT_EQ(letterBox.max_corner().get<0>(), 2048.0);
	EXPECT_EQ(letterBox.max_corner().get<1>(), 1014.0);
}