/////////////////////////////////////////
// MediaGeometry.h
// Copyright (c) 2020 TXPCo Ltd
/////////////////////////////////////////

#ifndef MEDIAGEOMETRY_INCLUDED
#define MEDIAGEOMETRY_INCLUDED

#include "Host.h"
#include "Media.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>

namespace MediaGeo = boost::geometry;

namespace Media {
	typedef MediaGeo::model::point<double, 2, MediaGeo::cs::cartesian> MGPoint;
	typedef MediaGeo::model::point<double, 2, MediaGeo::cs::cartesian> MGSize;
	typedef MediaGeo::model::box<MGPoint> MGBox;

	// Check that a source or target size if valid for LetterBox calculation
	bool MEDIA_API isValidForLetterBox(const MGSize& sz);

	// Calculate a 'letter box' size - how to place one size (Rectangle) within another for the best aspect ratio
	// with throw exception if either source or target is not 'isValidForLetterBox' (zero size) 
	MGBox MEDIA_API letterBox (const MGSize& target, const MGSize& source, MGBox& fill1, MGBox& fill2);

	// Calculate a 'letter box' size - how to place one size (Rectangle) within another for the best aspect ratio
	// with throw exception if either source or target is not 'isValidForLetterBox' (zero size) 
	MGBox MEDIA_API letterBox(const MGSize& target, const MGSize& source);
}

#endif // MEDIAGEOMETRY_INCLUDED
