
#pragma once

class WorldQuery
{
public:

	WorldQuery ()
	{
		Negate = false;
	}

	virtual ~WorldQuery()
	{
	}

	bool Negate;

	operator bool () const
	{
		if (Negate)
			return !this->Evaluate();
		else
			return this->Evaluate();
	}

	bool operator!() const
	{
		return !(*this);
	}

protected:
	virtual bool Evaluate () const = 0;
};