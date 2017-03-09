
#ifndef _ORE_STRINGS_H_
#define _ORE_STRINGS_H_

//	Definition of the t_String type is the responsibility
//		of Types.h; this file provides various utilities
//		for parsing strings.

/*
	Point of discussion - String operations are fairly expensive, especially when Lua
		uses C-strings and a conversion is required to for our API that uses t_String.
		Some solution should be determined to prevent this overhead.
*/

#include <ORE\Namespaces.h>
#include <ORE\Types.h>

#define ORE_STRING_NAMESPACE_NAME String
#define ORE_STRING_NAMESPACE_START namespace ORE_STRING_NAMESPACE_NAME {
#define ORE_STRING_NAMESPACE_END }

ORE_NAMESPACE_START
 ORE_STRING_NAMESPACE_START

	t_StringSet * SplitStringAtSymbol (t_String text, t_String symbol, t_StringSet * resultsOutOptional = NULL);
	t_UInteger GetTermCount (t_String text, t_String term);
	t_String EliminateSymbolFromString (t_String text, t_String symbol);
	t_String ConvergeStringSet (t_StringSet * stringSet, t_String separator = "");
	t_String ReplaceAllOfSymbolInString (t_String string, t_String targetSymbol, t_String replacementSymbol);
	t_String GetStringComponentBySeparatorIndex (t_String str, int index, t_String separator);

	enum e_TrimOptions {
		TRIM_SPACE		= 0x01,
		TRIM_NEWLINE	= 0x02,
		TRIM_TABS		= 0x04,
		TRIM_ALL		= 0x07
	};

	t_String TrimString (t_String text, e_TrimOptions trimOptions = TRIM_ALL);

	t_String DecimalToHexadecimal (t_UInteger value);

	t_String ToLower (t_String str);
	t_String ToUpper (t_String str);

 ORE_STRING_NAMESPACE_END
ORE_NAMESPACE_END

#endif