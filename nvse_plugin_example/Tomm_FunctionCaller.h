#pragma once
using namespace std;

	class FunctionCaller
	{
	public:
		int iID;
		Script* FunctionToCall = NULL;
		int iFramesRemain = 1;
		int iRemoveOnGameLoad = 1;
		TESObjectREFR* FunctionCallerRef = NULL;

		bool operator==(const FunctionCaller& other) const
		{
			return iID == other.iID;
		}
	};



	vector<FunctionCaller> g_FuncCallerArrayV;
	int g_FuncCallerArrayID = 0;
	int g_FuncCallerArrayIterate = 0;


	


	void f_FuncCaller_RemoveOnGameLoad()
	{

		_MESSAGE("SUP::g_FuncCallerArraySize ON GAME LOAD is %d", g_FuncCallerArrayV.size());

			std::vector<FunctionCaller>::iterator it;
			for (it = g_FuncCallerArrayV.begin(); it != g_FuncCallerArrayV.end();)
			{
				if (VectorIter.iRemoveOnGameLoad == 1)
				{
					//_MESSAGE("Removing Function in game load, ID:: %d", VectorIter.iID);
					it = g_FuncCallerArrayV.erase(it);
				}
				else
				{++it;}
			}


		if (g_FuncCallerArrayV.size() == 0)
		{
			g_FuncCallerArrayIterate = 0;
		}


	}



	void f_FuncCaller_Iterate()
	{
		//Console_Print("SUP::g_FuncCallerArraySize is %d", g_FuncCallerArrayV.size());

			vector<FunctionCaller>::iterator it;
			int index = 0;
			for (it = g_FuncCallerArrayV.begin(); it != g_FuncCallerArrayV.end();)
			{
					if (VectorIter.FunctionToCall && VectorIter.iFramesRemain == 0)
					{

						//_MESSAGE("Calling Function ID:: %d", VectorIter.iID);
						if (VectorIter.FunctionCallerRef)
						{
							FunctionCallScript(VectorIter.FunctionToCall, VectorIter.FunctionCallerRef, NULL, NULL, NULL);
						}
						else {
							FunctionCallScript(VectorIter.FunctionToCall, NULL, NULL, NULL, NULL);
						}

						it = g_FuncCallerArrayV.erase(it);
					}
					else
					{
						VectorIter.iFramesRemain -= 1;
						++it;
					}
			}


		if (g_FuncCallerArrayV.size() == 0)
		{
			g_FuncCallerArrayIterate = 0;
		}
	}