
#define k_NifInterTypeScale 0
#define k_NifInterTypeRotation 1
#define k_NifInterTypeTranslation 2

#define k_NifInterModeStartEnd 0
#define k_NifInterModeStartEndBack 1
#define k_NifInterModeStartEndBackInf 2

//mode
//0 = from start to end in MS
//1 = from start to end to start in ms
//2 = from start to end to start INFINITE

//Types:
//0 - scale
//1 - rotation
//2 - translation




class NifInterpolator
{
public:
	TESObjectREFR* thisObj = NULL;
	UInt32	ObjectRefID;
	NiAVObject* niBlock = NULL;
	int modIdx = 0;

	int ID = -1;
	int mode = 0;
	int type = 0;
	int PCNode = 0;
	int stage = 0;
	int iTimesIterate = 0;
	float timer = 0;
	float timerMS = 0;
	float curtimer = 0;
	char blockName[0x40]{};
	int iSetStartValueOnEnd = 0;

	//Scale
	float StartValue = 0;
	float EndValue = 0;
	float CurValue = 0;
	float delta = 0;


	//Rotation
	float StartX = 0;
	float StartY = 0;
	float StartZ = 0;
	float EndX = 0;
	float EndY = 0;
	float EndZ = 0;
	float deltaX = 0;
	float deltaY = 0;
	float deltaZ = 0;

	//translation
	int world = 0;
};

vector<NifInterpolator> g_NifInterpolatorArrayV;
int g_NifInterpolatorArrayID = 0;
int g_NifInterpolatorArrayIterate = 0;





plf::nanotimer g_InterpolTimer;
float g_InterpolTimeElapsed = 0;
int	  g_InterpolTimerStarted = 0;




bool fn_NifInterpolatorEventCaller_CallFromLoop(vector<NifInterpolator>::iterator it)
{
	//_MESSAGE("fn_NifInterpolatorEventCaller_CallFromLoop");
	vector<NifInterpolatorEventCaller>::iterator it_EventCaller;


	for (it_EventCaller = g_NifInterpolatorEventCallerArrayV.begin(); it_EventCaller != g_NifInterpolatorEventCallerArrayV.end();)
	{
		if (VectorIter.modIdx == VectorEventCallerIter.modIdx && VectorEventCallerIter.SendFromLoops == 1)
		{
			FunctionCallScript(VectorEventCallerIter.FunctionToCall, VectorIter.thisObj, NULL, NULL, 4,NiBlockIter->GetName(), VectorIter.type,1, VectorIter.PCNode);
			return true;
		}
		++it_EventCaller;
	}

	return false;

}








vector<NifInterpolator>::iterator fn_NifInterpolator_Destroy(vector<NifInterpolator>::iterator it, int iSendEvent)
{


	int iCall = 0;
	_MESSAGE("Destroying Interpolator named::->>ID::%d", VectorIter.ID);

	vector<NifInterpolatorEventCaller>::iterator it_EventCaller;

	TESObjectREFR* thisObj = NULL;
	int type = 0;
	int PCNode = 0;
	const char* s_BlockName = NULL;

	for (it_EventCaller = g_NifInterpolatorEventCallerArrayV.begin(); it_EventCaller != g_NifInterpolatorEventCallerArrayV.end();)
	{
		if (VectorIter.modIdx == VectorEventCallerIter.modIdx)
		{
			thisObj = VectorIter.thisObj;
			type = VectorIter.type;
			PCNode - VectorIter.PCNode;
			s_BlockName = NiBlockIter->GetName();
			iCall = 1;

			break;
		}
		++it_EventCaller;
	}

	if (VectorIter.iSetStartValueOnEnd)
	{
		NiVector3 niVect;

		if (VectorIter.type == k_NifInterTypeScale)
		{
			NiBlockIter->m_localScale = VectorIter.StartValue;
			NiBlockIter->Update();

		}
		else if (VectorIter.type == k_NifInterTypeRotation)
		{
			niVect.x = VectorIter.StartX;
			niVect.y = VectorIter.StartY;
			niVect.z = VectorIter.StartZ;
			niVect *= kFltPId180;
			NiBlockIter->m_localRotate.RotationMatrix(&niVect);
			NiBlockIter->Update();


		}
		else if (VectorIter.type == k_NifInterTypeTranslation)
		{

			niVect.x = VectorIter.StartX;
			niVect.y = VectorIter.StartY;
			niVect.z = VectorIter.StartZ;

			if (VectorIter.world)
			{
				NiBlockIter->m_worldTranslate = niVect;
			}
			else {
				NiBlockIter->m_localTranslate = niVect;
			}
		}

	}


	it = g_NifInterpolatorArrayV.erase(it);

	if (g_NifInterpolatorArrayV.size() == 0)
	{
		g_NifInterpolatorArrayIterate = 0;
	}

	if (iCall == 1)
	{
		FunctionCallScript(VectorEventCallerIter.FunctionToCall, VectorIter.thisObj, NULL, NULL, 4, NiBlockIter->GetName(), VectorIter.type, 0, VectorIter.PCNode);
	}



	return it;
}



bool fn_NifInterpolator_AlreadyExists(TESObjectREFR* thisObj, NiAVObject* niBlock, int type, int PCNode)
{
	vector<NifInterpolator>::iterator it;

	//_MESSAGE("fn_NifInterpolator_AlreadyExists started for %s, Name is %s ", thisObj->GetName(), niBlock->GetName());
	for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
	{
		if ((VectorIter.thisObj == thisObj) && (VectorIter.niBlock == niBlock) && (VectorIter.type == type))
		{
			//MESSAGE("FOUND");
			return true;
		}

		++it;
	}
	return false;
}

vector<NifInterpolator>::iterator f_NifInterpolator_GetIter(TESObjectREFR* thisObj, NiAVObject* niBlock, int type, int PCNode)
{
	_MESSAGE("f_NifInterpolator_GetIter started");
	int iFound = 0;
	vector<NifInterpolator>::iterator itRes;
	vector<NifInterpolator>::iterator it;

	if (type == -1)
	{
		for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
		{

			if ((VectorIter.thisObj == thisObj) && (VectorIter.niBlock == niBlock))
			{
				itRes = it;
				iFound = 1;
			}
			++it;
		}
	}
	else if (niBlock == NULL)
	{

		for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
		{

			if ((VectorIter.thisObj == thisObj))
			{
				if (thisObj == g_ThePlayer)
				{
					if (VectorIter.PCNode == PCNode)
					{
						itRes = it;
						iFound = 1;
					}

				}
				else {
					itRes = it;
					iFound = 1;
				}
			}
			++it;
		}

	}
	else {

		for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
		{

			if ((VectorIter.thisObj == thisObj) && (VectorIter.niBlock == niBlock) && (VectorIter.type == type))
			{
				itRes = it;
				iFound = 1;
			}
			++it;
		}

	}




	if (iFound == 1)
	{
		return itRes;
	}
	else { return itRes; }
}






void f_NifInterpolator_RemoveOnGameLoad()
{

	_MESSAGE("SUP::g_NifInterpolatorArraySize ON GAME LOAD is %d", g_NifInterpolatorArrayV.size());

	std::vector<NifInterpolator>::iterator it;
	for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
	{
		it = g_NifInterpolatorArrayV.erase(it);
	}

	if (g_NifInterpolatorArrayV.size() == 0)
	{
		g_NifInterpolatorArrayIterate = 0;
	}

}




void fn_NifInterpolator_Iterate()
{

	float elapsedTime = g_InterpolTimer.get_elapsed_ms() - g_InterpolTimeElapsed;
	g_InterpolTimeElapsed = g_InterpolTimer.get_elapsed_ms();
	//_MESSAGE("elapsedTime::%f", elapsedTime);

	if (g_interfaceManager->currentMode == 2) return;

	NiVector3 niVect;

	vector<NifInterpolator>::iterator it;
	int index = 0;
	float fPercent = 0;

	for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
	{

		if (!LookupFormByRefID(VectorIter.ObjectRefID))
		{
			it = fn_NifInterpolator_Destroy(it,0);
			continue;
		}

		//if (VectorIter.thisObj->GetNiNode() == NULL)
		//{
		//	it = fn_NifInterpolator_Destroy(it,0);
		//	continue;
		//}

		//if (VectorIter.iCheckBlock == 1)
		//{
			NiAVObject* niBlock = GetNifBlock(VectorIter.thisObj, VectorIter.PCNode, VectorIter.blockName);
			if (!niBlock)
			{
				_MESSAGE("DESTROYING BLOCK BECAUSE NIF BLOCK DOESNT EXIST");
;				it = fn_NifInterpolator_Destroy(it, 0);
				continue;
			}
		//}




		_MESSAGE("ITERATING NAME::%s", VectorIter.niBlock->GetName());


		if (VectorIter.type == k_NifInterTypeScale)
		{

			if (VectorIter.stage)
			{
				if (VectorIter.CurValue > VectorIter.StartValue)
				{
					VectorIter.CurValue = VectorIter.CurValue - VectorIter.delta * elapsedTime;
					if (VectorIter.CurValue < VectorIter.StartValue)
						VectorIter.CurValue = VectorIter.StartValue;
					NiBlockIter->m_localScale = VectorIter.CurValue;
					NiBlockIter->Update();
					_MESSAGE("Setting scale B::%f", VectorIter.CurValue);
				}
				else {
					if (VectorIter.mode == k_NifInterModeStartEnd)
					{
						it = fn_NifInterpolator_Destroy(it,1);
						continue;
					}
					else if (VectorIter.mode == k_NifInterModeStartEndBack)
					{

						VectorIter.iTimesIterate++;
						if (VectorIter.iTimesIterate >= 2)
						{
							it = fn_NifInterpolator_Destroy(it,1);
							continue;
						}
						else {
							VectorIter.stage = 0;
							fn_NifInterpolatorEventCaller_CallFromLoop(it);
							++it;
							continue;
						}
					}
					else if (VectorIter.mode == k_NifInterModeStartEndBackInf)
					{
						VectorIter.stage = 0;
						//_MESSAGE("CALLING fn_NifInterpolatorEventCaller_CallFromLoop PRE");
						fn_NifInterpolatorEventCaller_CallFromLoop(it);

						++it;
						continue;
					}

				}
			}
			else {
				if (VectorIter.CurValue < VectorIter.EndValue)
				{
					VectorIter.CurValue = VectorIter.CurValue + VectorIter.delta * elapsedTime;
					if (VectorIter.CurValue > VectorIter.EndValue)
						VectorIter.CurValue = VectorIter.EndValue;
					NiBlockIter->m_localScale = VectorIter.CurValue;
					NiBlockIter->Update();

					_MESSAGE("Setting scale F::%f", VectorIter.CurValue);
				}
				else {

					if (VectorIter.mode == k_NifInterModeStartEnd)
					{
						it = fn_NifInterpolator_Destroy(it,1);
						continue;
					}
					else if (VectorIter.mode == k_NifInterModeStartEndBack)
					{

						VectorIter.iTimesIterate++;
						if (VectorIter.iTimesIterate >= 2)
						{
							//Console_Print("DESTROY");
							it = fn_NifInterpolator_Destroy(it,1);
							continue;
						}
						else {
							//Console_Print("CALL LOOP");
							VectorIter.stage = 1;
							fn_NifInterpolatorEventCaller_CallFromLoop(it);
							++it;
							continue;
						}
					}
					else if (VectorIter.mode == k_NifInterModeStartEndBackInf)
					{
						VectorIter.stage = 1;
						fn_NifInterpolatorEventCaller_CallFromLoop(it);
						++it;
						continue;
					}

				}
			}

		}
		else //if (VectorIter.type == k_NifInterTypeRotation)
		{


			if (VectorIter.stage)
			{
				if (VectorIter.curtimer > 0)
				{
					VectorIter.curtimer -= elapsedTime;

					fPercent = (1 - (VectorIter.curtimer / VectorIter.timerMS)) * 100;

					//_MESSAGE("Percent B::%f", fPercent);

					if (fPercent > 0)
					{
						niVect.x = VectorIter.EndX - (VectorIter.deltaX * fPercent);
						niVect.y = VectorIter.EndY - (VectorIter.deltaY * fPercent);
						niVect.z = VectorIter.EndZ - (VectorIter.deltaZ * fPercent);
					}
					else {

						niVect.x = VectorIter.StartX;
						niVect.y = VectorIter.StartY;
						niVect.z = VectorIter.StartZ;

						VectorIter.curtimer = 0;

					}
					if (VectorIter.type == k_NifInterTypeRotation)
					{
						_MESSAGE("Rotation B is, X::%f,Y::%f,Z::%f", niVect.x, niVect.y, niVect.z);
						niVect *= kFltPId180;
						NiBlockIter->m_localRotate.RotationMatrix(&niVect);
					}
					else {
						_MESSAGE("Translation B is, X::%f,Y::%f,Z::%f", niVect.x, niVect.y, niVect.z);
						if (VectorIter.world)
						{
							NiBlockIter->m_worldTranslate = niVect;
						}
						else {
							NiBlockIter->m_localTranslate = niVect;
						}
					}

					NiBlockIter->Update();
				}
				else {

					if (VectorIter.mode == k_NifInterModeStartEnd)
					{
						it = fn_NifInterpolator_Destroy(it,1);
						continue;
					}
					else if (VectorIter.mode == k_NifInterModeStartEndBack)
					{

						VectorIter.iTimesIterate++;
						if (VectorIter.iTimesIterate >= 2)
						{
							it = fn_NifInterpolator_Destroy(it,1);
							continue;
						}
						else {
							VectorIter.stage = 0;
							fn_NifInterpolatorEventCaller_CallFromLoop(it);
							++it;
							continue;
						}
					}
					else if (VectorIter.mode == k_NifInterModeStartEndBackInf)
					{
						VectorIter.stage = 0;
						fn_NifInterpolatorEventCaller_CallFromLoop(it);
						++it;
						continue;
					}

				}
			}
			else {

				if (VectorIter.curtimer < VectorIter.timerMS)
				{
					VectorIter.curtimer += elapsedTime;
					fPercent = (VectorIter.curtimer / VectorIter.timerMS) * 100;

					_MESSAGE("Percent F::%f", fPercent);

					if (fPercent < 100)
					{
						niVect.x = VectorIter.StartX + (VectorIter.deltaX * fPercent);
						niVect.y = VectorIter.StartY + (VectorIter.deltaY * fPercent);
						niVect.z = VectorIter.StartZ + (VectorIter.deltaZ * fPercent);
					}
					else {

						niVect.x = VectorIter.EndX;
						niVect.y = VectorIter.EndY;
						niVect.z = VectorIter.EndZ;

						VectorIter.curtimer = VectorIter.timerMS;
					}

					if (VectorIter.type == k_NifInterTypeRotation)
					{
						_MESSAGE("Rotation F is, X::%f,Y::%f,Z::%f", niVect.x, niVect.y, niVect.z);
						niVect *= kFltPId180;
						NiBlockIter->m_localRotate.RotationMatrix(&niVect);
					}
					else {
						_MESSAGE("Translation F is, X::%f,Y::%f,Z::%f", niVect.x, niVect.y, niVect.z);
						if (VectorIter.world)
						{
							NiBlockIter->m_worldTranslate = niVect;
						}
						else {
							NiBlockIter->m_localTranslate = niVect;
						}
					}

					NiBlockIter->Update();
				}
				else {


					if (VectorIter.mode == k_NifInterModeStartEnd)
					{
						it = fn_NifInterpolator_Destroy(it,1);
						continue;
					}
					else if (VectorIter.mode == k_NifInterModeStartEndBack)
					{

						VectorIter.iTimesIterate++;
						if (VectorIter.iTimesIterate >= 2)
						{
							it = fn_NifInterpolator_Destroy(it,1);
							continue;
						}
						else {
							VectorIter.stage = 1;
							fn_NifInterpolatorEventCaller_CallFromLoop(it);
							++it;
							continue;
						}
					}
					else if (VectorIter.mode == k_NifInterModeStartEndBackInf)
					{
						VectorIter.stage = 1;
						fn_NifInterpolatorEventCaller_CallFromLoop(it);
						++it;
						continue;
					}

				}


			}
		}

		++it;
	}

	//Console_Print("Interpolator Array size::%d", g_NifInterpolatorArrayV.size());

	if (g_NifInterpolatorArrayV.size() == 0)
	{
		g_NifInterpolatorArrayIterate = 0;
	}
}





NiVector3 fn_NifInterpolator_CalcVector_Frame2(float PosX, float PosY, float PosZ, float fEndPosX, float fEndPosY, float fEndPosZ, float iFrame) //Fallout2AM
{
	NiVector3 outV{};

	float ChangeX = 0, ChangeY = 0, ChangeZ = 0;

	if (PosX > 0)
	{
		if (fEndPosX < 0)
		{
			ChangeX = -(PosX - fEndPosX) / iFrame;
		}
		else
			if (PosX > fEndPosX)
			{
				ChangeX = -(PosX - fEndPosX) / iFrame;
			}
			else { ChangeX = (fEndPosX - PosX) / iFrame; }
	}
	else
	{
		if (fEndPosX > 0)
			ChangeX = (fEndPosX - PosX) / iFrame;
		else {
			if (PosX > fEndPosX)
				ChangeX = (fEndPosX - PosX) / iFrame;
			else { ChangeX = -(PosX - fEndPosX) / iFrame; }
		}

	}


	if (PosY > 0)
	{
		if (fEndPosY < 0)
		{
			ChangeY = -(PosY - fEndPosY) / iFrame;
		}
		else
			if (PosY > fEndPosY)
			{
				ChangeY = -(PosY - fEndPosY) / iFrame;
			}
			else { ChangeY = (fEndPosY - PosY) / iFrame; }
	}
	else
	{
		if (fEndPosY > 0)
			ChangeY = (fEndPosY - PosY) / iFrame;
		else {
			if (PosY > fEndPosY)
				ChangeY = (fEndPosY - PosY) / iFrame;
			else { ChangeY = -(PosY - fEndPosY) / iFrame; }
		}

	}

	if (PosZ > 0)
	{
		if (fEndPosZ < 0)
		{
			ChangeZ = -(PosZ - fEndPosZ) / iFrame;
		}
		else
			if (PosZ > fEndPosZ)
			{
				ChangeZ = -(PosZ - fEndPosZ) / iFrame;
			}
			else { ChangeZ = (fEndPosZ - PosZ) / iFrame; }
	}
	else
	{
		if (fEndPosZ > 0)
			ChangeZ = (fEndPosZ - PosZ) / iFrame;
		else {
			if (PosZ > fEndPosZ)
				ChangeZ = (fEndPosZ - PosZ) / iFrame;
			else { ChangeZ = -(PosZ - fEndPosZ) / iFrame; }
		}

	}

	outV.x = PosX + ChangeX;
	outV.y = PosY + ChangeY;

	_MESSAGE("PosZ::%f, CHANGEZ::%f", PosZ, ChangeZ);
	outV.z = PosZ + ChangeZ;

	return outV;
}





struct fNifIntFunctionReturn
{
	int iFound = 0;
	vector<NifInterpolator>::iterator it;
};

fNifIntFunctionReturn fn_NifInterpolator_AlreadyExists_GetIter(TESObjectREFR* thisObj, NiAVObject* niBlock, int type, int PCNode)
{
	vector<NifInterpolator>::iterator it;

	for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
	{
		if ((VectorIter.thisObj == thisObj) && (VectorIter.niBlock == niBlock) && (VectorIter.type == type))
		{
			fNifIntFunctionReturn outRes;
			outRes.iFound = 1;
			outRes.it = it;
			return outRes;
		}

		++it;
	}

}


//fNifIntFunctionReturn fRes;
//
//fRes = fn_NifInterpolator_AlreadyExists_GetIter(thisObj, niBlock, type, PCNode);
//if (fRes.iFound)
//{
//	Console_Print("FOUND::%d", fRes.iFound);
//	vector<NifInterpolator>::iterator it = fRes.it;
//	Console_Print(" ITER ID::%d", (*it).ID);
//
//}





//NifBlockInterpolateTranslation "Flag" 0 0 0 0 0 10 2 2 0
//NifBlockInterpolateRotation "Flag" 0 0 0 0 0 180 2 2
//Player.NifBlockInterpolateScale "Bip01 Head" 1 2 2 2 0
//Player.IsNifBlockInterpolated "Bip01 Head" 0 0
//Player.NifBlockInterpolateStop "Bip01 Head" 0 0