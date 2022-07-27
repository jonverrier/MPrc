#include "pch.h"

#include <winerror.h>
#include <D2DErr.h>
#include <mferror.h>

#include "../../Include/Host.h"
#include "../../Include/HostException.h"

TEST(HostExceptionTest, SimpleConstruction) {

	Host::EnrichedExFrom<std::exception> ex(Host::ExceptionSource::kSystem, 0, __FILE__, __LINE__, "Hello");

	EXPECT_EQ(strlen(ex.what())>0, true);
}

TEST(HostExceptionTest, Throw) {

	bool caught = false;

	try {
		THROW_ENRICHED(std::exception, Host::ExceptionSource::kSystem, 1, "Hello thrown");
	}
	catch (std::exception& ex) {
		caught = true;
		UNREFERENCED_PARAMETER(ex);
	}

	EXPECT_EQ(caught, true);
}

