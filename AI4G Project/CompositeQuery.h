
#pragma once

#include "WorldQuery.h"
#include <vector>

namespace AI
{
	class CompositeQuery : public WorldQuery
	{
	public:
		std::vector<WorldQuery *> Queries;

		enum Mode
		{
			AllTrue,
			AnyTrue,

			AllFalse,
			AnyFalse
		} TestMode;

		CompositeQuery(Mode testMode)
		{
			TestMode = testMode;
		}

	protected:

		bool Evaluate () const
		{
			if (Queries.size() == 0)
				return false;

			bool allTrue = true;
			bool allFalse = true;

			for (auto query : Queries)
			{
				bool currentResult = *query;
				if (TestMode == AnyTrue && currentResult)
					return true;

				if (TestMode == AnyFalse && !currentResult)
					return true;

				if (currentResult == true)
					allFalse = false;

				if (currentResult == false)
					allTrue = false;
			}

			if (TestMode == AllFalse && allFalse)
				return true;

			if (TestMode == AllTrue && allTrue)
				return true;


			//	This should never actually happen; might be an invalid enum value.
			return false;
		}
	};
}