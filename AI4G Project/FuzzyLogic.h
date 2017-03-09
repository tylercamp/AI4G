
#pragma once

#include <cfloat>
#include "Utility.h"

/* An attempt to minimize problems that result from rounding errors when evaluating
 *	boolean tests/comparisons.
 */
#define FUZZY_DOM_COMPARISON_MAX_ALLOWED_ERROR (FLT_EPSILON * 3.0F)

namespace Fuzzy
{
	class DegreeOfMembership
	{
	public:

		float operator!( )
		{
			return 1.0F - m_MembershipNormal;
		}

		DegreeOfMembership operator&& (const DegreeOfMembership & other)
		{
			return min(MembershipNormal( ), other.MembershipNormal( ));
		}

		DegreeOfMembership operator||( const DegreeOfMembership & other )
		{
			return max(MembershipNormal( ), other.MembershipNormal( ));
		}

		bool operator!= ( const DegreeOfMembership & other )
		{
			return fabs(MembershipNormal( ) - other.MembershipNormal( )) <= FUZZY_DOM_COMPARISON_MAX_ALLOWED_ERROR;
		}

		DegreeOfMembership( float membershipNormal = 0.0F )
		{
			m_MembershipNormal = membershipNormal;
		}

		bool IsCompleteMember( ) const
		{
			return m_MembershipNormal >= 1.0F - FUZZY_DOM_COMPARISON_MAX_ALLOWED_ERROR;
		}

		bool IsPartialMember( ) const
		{
			return !IsCompleteMember( ) && m_MembershipNormal >= FUZZY_DOM_COMPARISON_MAX_ALLOWED_ERROR;
		}

		float MembershipNormal( ) const
		{
			return m_MembershipNormal;
		}

		void SetMembershipNormal( float membershipNormal )
		{
			m_MembershipNormal = membershipNormal;
		}

	private:
		float m_MembershipNormal;
	};



	class Function
	{
	public:
		virtual Fuzzy::DegreeOfMembership DegreeOfMembership() = 0;
	};
}