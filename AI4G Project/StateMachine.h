
#pragma once

#include <memory>

#include "State.h"
#include "KinematicEntity.h"

#include "PropertyStore.h"
#include "WorldQuery.h"

class StateMachine
{
public:

	StateMachine( )
	{
		m_PreviousState = nullptr;
		m_CurrentState = nullptr;
	}

	virtual ~StateMachine( )
	{
	}

	void SetNewActiveState( State * newState )
	{
		//	Might want to jump on that shared_ptr usage, this IS leaking memory
		//if (m_PreviousState != nullptr)
		//	delete m_PreviousState;

		m_PreviousState = m_CurrentState;
		m_CurrentState = newState;
	}

	State * GetCurrentState( )
	{
		return m_CurrentState;
	}

	void ApplyToObject( KinematicEntity * target )
	{
		if (m_CurrentState == nullptr)
			return;

		State * nextState = m_CurrentState->Apply(target, &Properties);
		if (nextState != nullptr)
		{
			SetNewActiveState(nextState);
		}
		else
		{
			for (State::LogicBlock logic : m_CurrentState->AvailableLogic)
			{
				if (logic.Question && *(logic.Question))
					SetNewActiveState(logic.Response);
			}
		}
	}

	void RevertToPreviousState( )
	{
		State * buffer = m_CurrentState;
		m_CurrentState = m_PreviousState;
		m_PreviousState = buffer;
	}

	PropertyStore Properties;

protected:

	State * m_PreviousState;
	State * m_CurrentState;
};