
#ifndef _DIAGNOSTICS_STRINGER_H_
#define _DIAGNOSTICS_STRINGER_H_

#include <string>
#include "Namespaces.h"
#include "Types.h"


/*
	If you're trying to figure out how to change the way that
		some diagnostics are reported, look at DiagnosticsStringer.cpp.
*/


#define ORE_DIAGNOSTICS_NAMESPACE_NAME Diagnostics
#define ORE_DIAGNOSTICS_NAMESPACE_START namespace ORE_DIAGNOSTICS_NAMESPACE_NAME {
#define ORE_DIAGNOSTICS_NAMESPACE_END }

#define ORE_DIAGNOSTICS_INTERNAL_NAMESPACE_NAME Internal
#define ORE_DIAGNOSTICS_INTERNAL_NAMESPACE_START namespace ORE_DIAGNOSTICS_INTERNAL_NAMESPACE_NAME {
#define ORE_DIAGNOSTICS_INTERNAL_NAMESPACE_END }

#ifdef GetMessage
#undef GetMessage
#endif



ORE_NAMESPACE_START
 ORE_DIAGNOSTICS_NAMESPACE_START

  ORE_DIAGNOSTICS_INTERNAL_NAMESPACE_START
	class DiagnosticsStringer;
  ORE_DIAGNOSTICS_INTERNAL_NAMESPACE_END

	class DiagnosticsStringerInterface
	{
	public:
		DiagnosticsStringerInterface (Internal::DiagnosticsStringer * stringer);
		t_String GetMessage ();
		t_UInteger GetCode ();

	private:
		Internal::DiagnosticsStringer * _stringer;
	};

  ORE_DIAGNOSTICS_INTERNAL_NAMESPACE_START

	class DiagnosticsStringer;

	void DisplayMessage	(DiagnosticsStringer &);
	void DisplayWarning	(DiagnosticsStringer &);
	void DisplayError	(DiagnosticsStringer &);

	class DiagnosticsStringer
	{
	public:

		enum e_MessageType
		{
			MSG_MESSAGE,
			MSG_WARNING,
			MSG_ERROR,
			MSG_INVALID
		};

		DiagnosticsStringer (const DiagnosticsStringer & stringer);
		DiagnosticsStringer (bool displayMessage, e_MessageType messageType);

		template <typename T>
		INLINE DiagnosticsStringer & Append (const T& data);
		DiagnosticsStringer & ErrorCode (t_UInteger code);

		void Display ();

		friend void ORE_DIAGNOSTICS_INTERNAL_NAMESPACE_NAME::DisplayMessage	(DiagnosticsStringer &);
		friend void ORE_DIAGNOSTICS_INTERNAL_NAMESPACE_NAME::DisplayWarning	(DiagnosticsStringer &);
		friend void ORE_DIAGNOSTICS_INTERNAL_NAMESPACE_NAME::DisplayError	(DiagnosticsStringer &);

		friend class ORE_DIAGNOSTICS_NAMESPACE_NAME::DiagnosticsStringerInterface;

	protected:
		bool _displayMessage;
		t_String * _message;
		e_MessageType _messageType;
		t_UInteger _errorCode;

		const t_String & _GetMessage ();
		const t_String & _GetMessage () const;
	};


  ORE_DIAGNOSTICS_INTERNAL_NAMESPACE_END
 ORE_DIAGNOSTICS_NAMESPACE_END
ORE_NAMESPACE_END





ORE_NAMESPACE_START
 ORE_DIAGNOSTICS_NAMESPACE_START
  ORE_DIAGNOSTICS_INTERNAL_NAMESPACE_START

	template <typename T>
	DiagnosticsStringer & DiagnosticsStringer::Append (const T& data)
	{
		if (_displayMessage)
		{
			*_message += RealToString (data);
		}

		return *this;
	}

	template <>
	INLINE DiagnosticsStringer & DiagnosticsStringer::Append <t_String> (const t_String & message)
	{
		if (_displayMessage)
		{
			*_message += message;
		}

		return *this;
	}

	template <>
	INLINE DiagnosticsStringer & DiagnosticsStringer::Append <char> (const char & character)
	{
		if (_displayMessage)
		{
			*_message += character;
		}

		return *this;
	}

  ORE_DIAGNOSTICS_INTERNAL_NAMESPACE_END
 ORE_DIAGNOSTICS_NAMESPACE_END
ORE_NAMESPACE_END

#endif
