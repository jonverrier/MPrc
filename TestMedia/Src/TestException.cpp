#include "pch.h"

#include "../../Include/Host.h"
#include "../../Include/HostException.h"
#include "../../Include/MediaException.h"

TEST(MediaHostExceptionLogging, Logging) {

  Media::ExceptionLogger*plogger = new Media::ExceptionLogger();

  Host::ExceptionLoggerFactory::set(plogger);

  bool caught = false;

  try {
	  THROW(Host::Exception, 1);
  }
  catch (Host::Exception& ex) {
	  caught = true;
	  UNREFERENCED_PARAMETER(ex);
  }

  EXPECT_EQ(caught, true);
}