//
// pch.h
//

#pragma once

#include <exception>

#define U_SHOW_CPLUSPLUS_API 1
#include <unicode/unistr.h>

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

#include "../../Include/Host.h"
#include "../../Include/HostException.h"

#include "../../Include/MediaException.h"
#include "../../Include/MediaUtf16Utf8.h"
#include "../../Include/MediaFrameFilename.h"
#include "../../Include/MediaFrame.h"
#include "../../Include/MediaGeometry.h"

#include "gtest/gtest.h"