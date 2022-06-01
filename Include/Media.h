/////////////////////////////////////////
// Media.h
// Copyright (c) 2022 TXPCo Ltd
/////////////////////////////////////////

// Import / export for DLL linkage 
#if (defined WINXX)
#ifdef MEDIA_NOEXPORTS
#define MEDIA_API 
#else
#ifdef MEDIA_EXPORTS
#define MEDIA_API __declspec(dllexport)
#else
#define MEDIA_API __declspec(dllimport)
#endif
#endif
#endif

// Package name for resource strings
#if (defined WINXX)
#ifdef NDEBUG
#define MEDIA_PACKAGE_NAME L"Mediarw64.dll"
#else
#define MEDIA_PACKAGE_NAME L"Mediadw64.dll"
#endif
#endif

#if (defined LINUX)
#ifdef NDEBUG
#error "Linux release package not yet defined."
#else
#error "Linux debug package not yet defined."
#endif
#endif
