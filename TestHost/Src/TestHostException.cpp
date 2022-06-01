#include "pch.h"

#include <winerror.h>
#include <D2DErr.h>
#include <mferror.h>

#include "../../Include/Host.h"
#include "../../Include/HostException.h"

TEST(HostExceptionTest, SimpleConstruction) {

	Host::Exception ex(0, HOST_WFILE, 2);

	EXPECT_EQ(ex.errorCode(), 0);
	EXPECT_EQ(ex.sourceLineNumber(), 2);
	EXPECT_EQ(ex.sourceFilename(), HOST_WFILE);
	EXPECT_TRUE(ex.formatted().size() == 0);
}

TEST(HostExceptionTest, FormattingOS) {

	Host::Exception ex(ERROR_ACCESS_DENIED, HOST_WFILE, 2);

	EXPECT_EQ(ex.errorCode(), ERROR_ACCESS_DENIED);
	EXPECT_EQ(ex.sourceLineNumber(), 2);
	EXPECT_EQ(ex.sourceFilename(), HOST_WFILE);
	EXPECT_TRUE(ex.formatted().size() > 0);
}

TEST(HostExceptionTest, FormattingMF) {

	Host::Exception ex(MAKE_HRESULT(SEVERITY_ERROR, FACILITY_MF, MF_E_BUFFERTOOSMALL), HOST_WFILE, 2);

	EXPECT_EQ(ex.errorCode(), MAKE_HRESULT(SEVERITY_ERROR, FACILITY_MF, MF_E_BUFFERTOOSMALL));
	EXPECT_EQ(ex.sourceLineNumber(), 2);
	EXPECT_EQ(ex.sourceFilename(), HOST_WFILE);
	EXPECT_TRUE(ex.formatted().size() > 0);
}

TEST(HostExceptionTest, Throw) {

	Host::Exception ex(MAKE_HRESULT(SEVERITY_ERROR, FACILITY_MF, MF_E_BUFFERTOOSMALL), HOST_WFILE, 2);
	bool caught = false;

	try {
		THROW(Host::Exception, ERROR_ACCESS_DENIED);
	}
	catch (Host::Exception& ex) {
		caught = true;
		UNREFERENCED_PARAMETER(ex);
	}

	EXPECT_EQ(caught, true);
}