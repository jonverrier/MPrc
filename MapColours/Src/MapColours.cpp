// MapColours.cpp : This file contains the 'main' function. Program execution begins and ends here.
//

#include <iostream>
#include <filesystem>
#include <tclap/CmdLine.h>

#ifdef WINXX
#define DIRSEPCHAR '\\'
#define DIRSEPSTRING "\\"
#define DIRDEFAULT ".\\"
#endif


#ifdef LINUX
#define DIRSEPCHAR '/'
#define DIRSEPSTRING "/"
#define DIRDEFAULT "./"
#endif

int main(int argc, char **argv)
{
	try {

		TCLAP::CmdLine cmd("MapColours", ' ', "0.1");

		// Need a file name to use as input
		TCLAP::ValueArg<std::string> fileNameInArg("f", "filename", "Filename to process as input (.png file for ingest, .exr file for output)", true, "", "string");
		cmd.add(fileNameInArg);

		// Optional path for storing output
		TCLAP::ValueArg<std::string> pathOutArg("p", "path", "Path to use to store output", false, DIRDEFAULT, "string");
		cmd.add(pathOutArg);

		// Switches to say if we are ingesting log to scene linear or outputting from linear to log. 
		TCLAP::SwitchArg ingestSwitchArg("i", "ingest", "Ingest file to scene linear workflow", false);
		TCLAP::SwitchArg outputSwitchArg("o", "output", "Output file from scene linear workflow as .png file", false);
		cmd.add(ingestSwitchArg);
		cmd.add(outputSwitchArg);

		// Parse the argv array.
		cmd.parse(argc, argv);

		// Get the value parsed by each arg. 
		std::string fileNameIn = fileNameInArg.getValue();
		std::string pathOut = pathOutArg.getValue();
		if (pathOut[pathOut.length()-1] != DIRSEPCHAR)
			pathOut = pathOut + DIRSEPSTRING;
		bool ingest = ingestSwitchArg.getValue();

		if (ingestSwitchArg.isSet()) {
			std::filesystem::path input(fileNameIn);
			input.replace_extension(".png");

			std::filesystem::path output(pathOut);
			output.replace_filename (fileNameIn);
			output.replace_extension(".exr");

			std::cout << "Ingesting to Linear colour space: " << input << ", output to:" << output << std::endl;
		}
		else {
			std::filesystem::path input(fileNameIn);
		    input.replace_extension(".exr");

			std::filesystem::path output(pathOut);
			output.replace_filename(fileNameIn);
			output.replace_extension(".png");

			std::cout << "Outputting to Log colourspace: " << input << ", output to:" << output << std::endl;
		}

	}
	catch (TCLAP::ArgException& e)  // catch exceptions
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}
}

