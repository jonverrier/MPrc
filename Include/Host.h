/////////////////////////////////////////
// Host.h
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

// Import / export for DLL linkage 
#if (defined WINXX)
#ifdef HOST_NOEXPORTS
#define HOST_API 
#else
#ifdef HOST_EXPORTS
#define HOST_API __declspec(dllexport)
#else
#define HOST_API __declspec(dllimport)
#endif
#endif
#endif

// Package name for resource strings
#if (defined WINXX)
#ifdef NDEBUG
#define HOST_PACKAGE_NAME L"Hostrw64.dll"
#else
#define HOST_PACKAGE_NAME L"Hostdw64.dll"
#endif
#endif

#if (defined LINUX)
#ifdef NDEBUG
#error "Linux release package not yet defined."
#else
#error "Linux debug package not yet defined."
#endif
#endif

#define HOST_STRING_BUFFER_SIZE 512

#define HOST_WIDEN2(x) L ## x
#define HOST_WIDEN(x) HOST_WIDEN2(x)
#define HOST_WFILE HOST_WIDEN(__FILE__)