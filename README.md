## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Design](#design)
* [Installation](#installation)
* [Licence](#licence)

## General info
TXP Media Processing is a suite of tools for programatically generating interesting video content, such as sub-sampled / stuttery video, color processing, and assembly of a video from a set of frames.  

## Technologies
TXP Media Processing is created with:
* C++ 20 
* Visual Studio Community 2022
* ICU library for unicode processing 
* TCLAP library for command line parsing.
* spdylog library for logging.
* OpenEXR file format libary.
* OpenColorIO color management library. 
* FFMPEG video processing library. 
	
## Design
TXP Media Processing is composed of several different components, each of which is implemented as a separate library for development purposes:
* Host - defines & implements wrappers to operating specific calls. At some point, a Linux port will be created, and this will be the only library that needs substantial change. 
* Media - has a light dependency on 'Host', but is mostly operating system independent. As much logic as possible is included here to maximise portability. Media uses OpenEXR, OpenColorIO, and FFMPEG libraries to do the heavy lifting. 
* Cmd - command line tools. Shell only, all the actual logic is in Media
* TestXXX - contains the test suites. 
 

## Installation
To install & build a local copy for development:
* Install Visual Studio Community 2022.  
* Use 'vcpkg' to install ICU, TCLAP, OpenEXR, OpenColorIO, FFMPEG. 
* Open & build the 'MPrc' solution (media processing). The solution builds developer versions to the 'x64/Debug' directory, and release versions to 'x64/Release'.

## Licence

MIT. 
