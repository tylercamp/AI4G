
#pragma once

class Timer
{
	unsigned int				startTime;
	unsigned int				interval;

public:
								Timer (unsigned int startTime, unsigned int interval);

	bool						Check (unsigned int currentTime);
	unsigned int				GetTimeToAlarm (unsigned int currentTime);
};