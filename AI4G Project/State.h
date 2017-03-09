
#pragma once

#include <vector>

class KinematicEntity;
class PropertyStore;
class WorldQuery;

class State
{
public:

	virtual ~State( )
	{
	}

	struct LogicBlock
	{
		LogicBlock() {
			Question = nullptr;
			Response = nullptr;
		}

		LogicBlock( WorldQuery * question, State * response ) {
			Question = question;
			Response = response;
		}

		WorldQuery	* Question;
		State		* Response;
	};

	std::vector< LogicBlock > AvailableLogic;

	virtual void Initialize( KinematicEntity * target, PropertyStore * dataSource ) = 0;

	virtual State * Apply( KinematicEntity * target, PropertyStore * dataSource ) = 0;
};