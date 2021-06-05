class NifInterpolatorEventCaller
{
public:
	int ID = -1;
	int SendFromLoops = 0;
	int modIdx = 0;
	Script* FunctionToCall = NULL;
};

vector<NifInterpolatorEventCaller> g_NifInterpolatorEventCallerArrayV;
int g_NifInterpolatorEventCallerArrayID = 0;





bool fn_NifInterpolatorEventCaller_AlreadyExists(Script* FunctionToCall, UInt8 modIdx)
{
	vector<NifInterpolatorEventCaller>::iterator it;


	if (FunctionToCall)
	{
		for (it = g_NifInterpolatorEventCallerArrayV.begin(); it != g_NifInterpolatorEventCallerArrayV.end();)
		{
			if ((VectorIter.modIdx == modIdx) && (VectorIter.FunctionToCall == FunctionToCall))
			{
				return true;
			}
			++it;
		}
	}
	else {

		for (it = g_NifInterpolatorEventCallerArrayV.begin(); it != g_NifInterpolatorEventCallerArrayV.end();)
		{
			if (VectorIter.modIdx == modIdx)
			{
				return true;
			}
			++it;
		}

	}



}