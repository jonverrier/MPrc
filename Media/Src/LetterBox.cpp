/////////////////////////////////////////
// LetterBox.cpp
// Copyright (c) 2020 TXPCo Ltd
/////////////////////////////////////////

#include "pch.h"
#include <stdexcept>

#include "../../Include/MediaException.h"
#include "../../Include/MediaGeometry.h"


static const Media::MGBox nullBox = Media::MGBox(Media::MGPoint(0.0, 0.0), Media::MGPoint(0.0, 0.0));

namespace Media {

    bool MEDIA_API isValidForLetterBox(const MGSize& sz) {
        // Compute source/destination ratios.
        double width = sz.get<0>();
        double height = sz.get<1>();

        if (height <= 0.0 || width <= 0.0)
            return false;
        return true;
    }

    MGBox MEDIA_API letterBox(const MGSize& target, const MGSize& source) {
        MGBox fill1, fill2;

        return letterBox(target, source, fill1, fill2);
    }

    MGBox MEDIA_API letterBox(const MGSize& target, const MGSize& source, MGBox& fill1, MGBox& fill2) {

        // Compute source/destination ratios.
        double sourceWidth = source.get<0>();
        double sourceHeight = source.get<1>();
        double destinationWidth = target.get<0>();
        double destinationHeight = target.get<1>();
        double destinationLBWidth;
        double destinationLBHeight;
        MGBox letterBox;

        if (!isValidForLetterBox(source) || !isValidForLetterBox(source)) {
            fill1 = fill2 = nullBox;
            return nullBox;
        }

        if (((sourceWidth * destinationHeight) / sourceHeight) <= destinationWidth)
        {
            // This path is for Column letterboxing ("pillar box")
            destinationLBWidth = destinationHeight * sourceWidth / sourceHeight;
            destinationLBHeight = destinationHeight;

            // Create a centered rectangle within the current destination rect
            double left = ((destinationWidth - destinationLBWidth) / 2);
            double top = ((destinationHeight - destinationLBHeight) / 2);
            letterBox = MGBox(MGPoint(left, top), MGPoint (left + destinationLBWidth, top + destinationLBHeight));

            // create padding rectangles either side
            if (destinationWidth != destinationLBWidth || destinationHeight != destinationLBHeight)
            {
                fill1 = MGBox(MGPoint(0.0, 0.0), MGPoint(left - 1.0, destinationHeight));
                fill2 = MGBox(MGPoint(left + destinationLBWidth + 1.0, 0.0), MGPoint(destinationWidth, destinationHeight));
            }
            else
            {
                fill1 = fill2 = MGBox(MGPoint(0.0, 0.0), MGPoint(0.0, 0.0));
            }

            return letterBox;
        }
        else
        {
            // Row letterboxing.
            destinationLBWidth = destinationWidth;
            destinationLBHeight = destinationWidth * sourceHeight / sourceWidth;

            // Create a centered rectangle within the current destination rect
            double left = ((destinationWidth - destinationLBWidth) / 2);
            double top = ((destinationHeight - destinationLBHeight) / 2);
            letterBox = MGBox(MGPoint(left, top), MGPoint(left + destinationLBWidth, top + destinationLBHeight));

            // create padding rectangles top & bottom 
            if (destinationWidth != destinationLBWidth || destinationHeight != destinationLBHeight)
            {
                fill1 = MGBox(MGPoint(0.0, 0.0), MGPoint(destinationWidth, top));
                fill2 = MGBox(MGPoint(0.0, top + destinationLBHeight), MGPoint(destinationWidth, destinationHeight));
            }
            else
            {
                fill1 = fill2 = MGBox(MGPoint(0.0, 0.0), MGPoint(0.0, 0.0));
            }

            return letterBox;
        }
    }

}