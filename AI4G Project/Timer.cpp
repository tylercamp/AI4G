
#include "Timer.h"


Timer::Timer( unsigned int startTime, unsigned int interval )
{
	this->startTime = startTime;
	this->interval = interval;
}


bool Timer::Check( unsigned int currentTime )
{
	if (GetTimeToAlarm (currentTime) >= interval)
	{
		startTime = currentTime;
		return true;
	}

	return false;
}


unsigned int Timer::GetTimeToAlarm( unsigned int currentTime )
{
	return currentTime - startTime;
}