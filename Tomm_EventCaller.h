
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


vector<EventCaller> g_EventAr_CamMode;

void f_EventCaller_IterateEventCameraMode(int iOnGameLoad)
{
	
	if (g_EventAr_CamMode.size() != 0)
	{

		std::vector<EventCaller>::iterator it;
		for (it = g_EventAr_CamMode.begin(); it != g_EventAr_CamMode.end();)
		{
			FunctionCallScript(VectorIter.FunctionToCall, NULL, NULL, NULL, 2, g_PlayerCamState,iOnGameLoad);
			++it;
		}

	}


}