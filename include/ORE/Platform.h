
#ifndef _ORE_PLATFORM_H_
#define _ORE_PLATFORM_H_



# if defined (WIN32) || defined (_WIN32)
#  define ORE_PLATFORM_WINDOWS
#  define ORE_PLATFORM_IDENTIFIED
# endif

#if defined (linux) || defined (__linux) || defined (__linux__)
#  define ORE_PLATFORM_LINUX
#  define ORE_PLATFORM_IDENTIFIED
# endif

# ifdef __APPLE__
#  define ORE_PLATFORM_MACOSX
#  define ORE_PLATFORM_IDENTIFIED
# endif



//	Not sure how this works on Mac OSX x86_64 applications, but I guess we'll see.
# if defined (LP64) || defined (__LP64__) || defined (_WIN64)
#  define ORE_PLATFORM_64BIT
# else
#  define ORE_PLATFORM_32BIT
# endif



# ifndef ORE_PLATFORM_IDENTIFIED
#  error Compiling for unidentified (and possibly unsupported) platform.
# endif



#endif