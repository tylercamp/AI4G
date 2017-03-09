#ifndef _DIAGNOSTICS_H_
#define _DIAGNOSTICS_H_

#include <string>

#include "DiagnosticsStringer.h"

/*
 If you're trying to figure out how to change the way that
 some diagnostics are reported, look at DiagnosticsStringer.cpp.
 */

/* Debug Macros */

/* The following are defined:

 - ORE_DEBUG (Conditional, if not already defined)
 - ORE_RELEASE (Conditional, if not already defined)
 - DebugStatement(x)
 
 */

/*
 ORE_DEBUG or ORE_RELEASE should manually be defined through
 project settings (or at least at the beginning of this
 header), but in certain cases it's just as accurate to
 use the compiler-defined constants for debug/release mode.
 We generate the ORE_DEBUG or ORE_RELEASE directives
 here, if they weren't already. Note that support for this
 has to be added manually for each compiler.
 */
#if !defined (ORE_DEBUG) && !defined (ORE_RELEASE)
//	Fill in with other compiler-specific directives as needed.
# ifdef _MSC_VER
#  ifdef _DEBUG
#   define ORE_DEBUG
#  else // _DEBUG
#   define ORE_RELEASE
#  endif // _DEBUG
# endif // _MSC_VER
#endif // !defined (ORE_DEBUG) && !defined (ORE_RELEASE)
#if !defined (ORE_DEBUG) && !defined (ORE_RELEASE)
/*
 There's no standardized way for displaying warnings (which would be
 appropriate), so instead an error is shown to at least give a
 heads-up. DebugStatement can be defined manually, the issue is
 just that determining whether you're running in DEBUG or not is
 often compiler-specific.
 */
# error "No runtime mode could be determined - aborting."
#endif // !defined (ORE_DEBUG) && !defined (ORE_RELEASE)

//	Use the DebugStatement macro for operations that stay throughout
//		the project, but aren't meant to be in the release build.
#if defined (ORE_DEBUG)
# define DebugStatement(x) x
#elif defined (ORE_RELEASE)
# define DebugStatement(x)
#endif

//	Ensures that the code given to the macro is ran only ONCE within the lifetime of the
//		program. Note that the code ran exists within its own subscope. Useful for, i.e,
//		ensuring that a warning only occurs once during the operation of a program.
#define DEBUG_RUN_ONCE(CODE)							\
	{													\
		static bool __debug_run_once_has_ran = false;	\
		if (!__debug_run_once_has_ran)					\
		{												\
			__debug_run_once_has_ran = false;			\
			CODE										\
		}												\
	}

ORE_NAMESPACE_START
ORE_DIAGNOSTICS_NAMESPACE_START

/*
 Stringer Example:

 DiagnosticsMessage (true)
 .Append ("This string ")
 .Append ("is concatenated with this one and ")
 .Append (15)
 .Append (" <- that number.")
 .Display ();

 The .Append method concatenates the contents to the message, and the Display method
 uses whatever is defined in "diagnostics stringer.cpp" The parameter to
 DiagnosticsMessage determines whether or not the message is disabled. If a message
 is disabled, all calls to Append are ignored and the Display method does nothing.
 */

inline Internal::DiagnosticsStringer Message (bool displayMessage)
{
    return Internal::DiagnosticsStringer (displayMessage, Internal::DiagnosticsStringer::MSG_MESSAGE);
}

inline Internal::DiagnosticsStringer Warning (bool displayWarning)
{
    return Internal::DiagnosticsStringer (displayWarning, Internal::DiagnosticsStringer::MSG_WARNING);
}

inline Internal::DiagnosticsStringer Error (bool displayError)
{
    return Internal::DiagnosticsStringer (displayError, Internal::DiagnosticsStringer::MSG_ERROR);
}

typedef void (*t_DiagnosticsReportHandler) (DiagnosticsStringerInterface & stringer);

void SetCustomMessageHandler (t_DiagnosticsReportHandler messageHandler);
void SetCustomWarningHandler (t_DiagnosticsReportHandler warningHandler);
void SetCustomErrorHandler (t_DiagnosticsReportHandler errorHandler);

#define ERROR_NONE			0
#define ERROR_INVALID_CODE -1
t_UInteger LastError ();
void SetLastError (t_UInteger code);
void ClearLastError ();

ORE_DIAGNOSTICS_NAMESPACE_END
ORE_NAMESPACE_END

#endif
