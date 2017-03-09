
#ifndef _ORE_TYPES_H_
#define _ORE_TYPES_H_

#include <list>
#include <string>
#include <sstream>

#include <ORE/Namespaces.h>
#include <ORE/Platform.h>



ORE_NAMESPACE_START

#	ifdef _MSC_VER
#	 define INLINE __forceinline
#	else
#	 define INLINE inline
#	endif

#	define MASK_32_BIT_

	//	Definite-size types
#	ifdef _MSC_VER

	typedef				__int8	t_Int8;
	typedef unsigned	__int8	t_UInt8;
	typedef				__int16	t_Int16;
	typedef unsigned	__int16	t_UInt16;
	typedef				__int32	t_Int32;
	typedef unsigned	__int32	t_UInt32;
	typedef				__int64	t_Int64;
	typedef unsigned	__int64	t_UInt64;

#	else

	typedef				char	t_Int8;
	typedef unsigned	char	t_UInt8;
	typedef				short	t_Int16;
	typedef unsigned	short	t_UInt16;
	typedef				int		t_Int32;
	typedef unsigned	int		t_UInt32;

	//	We're just going to assume this is supported by the compiler.
	typedef				long long	t_Int64;
	typedef unsigned	long long	t_UInt64;

#	endif

	typedef float				t_Float32;
	typedef double				t_Float64;



	//	If we ever want to swap out string implementation
	typedef std::string t_String;

	//	Implementation-agnostic conversions to c-strings.
#	define C_STRING(str) ((str).c_str ())

	//	Reading clarification
	typedef t_UInt8 t_Byte;

	//	Standardized integer type, for when the specific
	//		size doesn't really matter.
	typedef t_UInt32 t_UInteger;

	//	Standardized string set definition
	typedef std::list <t_String> t_StringSet;



	//	Type conversion convenience functions
	template <typename T>
	inline T StringToReal (std::string Str);

	template <typename T>
	inline std::string RealToString (T Real);




	//	Platform-agnostic handle type, we switch based on OS types since handles
	//		may be full-blown pointers (There's no real standard on what can
	//		or can't be a handle)
#	ifdef ORE_PLATFORM_32BIT
	typedef t_UInt32 t_Handle;
	static const t_Handle INVALID_HANDLE = 0xFFFFFFFF;
#	else
	typedef t_UInt64 t_Handle;
	static const t_Handle INVALID_HANDLE = 0xFFFFFFFFFFFFFFFF;
#	endif

	//	Please use this function. If, for some reason, we decide on using
	//		definite-byte types for handles, you're going to be rewriting
	//		a lot of casting code.
	template <typename T>
	T * CastHandle (t_Handle handle);

ORE_NAMESPACE_END



ORE_NAMESPACE_START

	template <typename T>
	T * CastHandle (t_Handle handle)
	{
		return (T *)handle;
	}

	template <typename T>
	T StringToReal (std::string Str)
	{
		T ret;
		std::istringstream strm (Str);
		strm >> ret;
		return ret;
	}

	template <typename T>
	std::string RealToString (T Real)
	{
		std::ostringstream strm;
		strm << Real;
		return strm.str ();
	}

ORE_NAMESPACE_END

#endif