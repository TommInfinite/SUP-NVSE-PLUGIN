#pragma once
class EventCaller
{
public:
	Script* FunctionToCall = NULL;
	int iNotifyOnGameLoad = 1;


	bool operator==(const EventCaller& other) const
	{
		return FunctionToCall == other.FunctionToCall;
	}
};