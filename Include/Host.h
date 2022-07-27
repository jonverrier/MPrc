/////////////////////////////////////////
// Host.h
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

#ifndef HOST_INCLUDED
#define HOST_INCLUDED

// Import / export for DLL linkage 
#if (defined WINXX)
#ifdef HOST_EXPORTS
#define HOST_API __declspec(dllexport)
#else
#define HOST_API __declspec(dllimport)
#endif
#endif

#if (defined LINUX)
#ifdef HOST_EXPORTS
#define HOST_API __declspec(dllexport)
#else
#define HOST_API __declspec(dllimport)
#endif
#endif

#define HOST_STRING_BUFFER_SIZE 512

// Reduce lint / compiler warnings if have unused parameters
#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) (P)
#endif

#endif // HOST_INCLUDED