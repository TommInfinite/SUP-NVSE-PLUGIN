#pragma once




DEFINE_COMMAND_PLUGIN(GetCalculatedPos, "Gets current PosXYZ", 1, 4, kParams_Tomm_TwoFloats_TwoScriptVars)
DEFINE_COMMAND_PLUGIN(GetSUPVersion, "Get plugin version", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(GetFileSize, "Get file size", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_PLUGIN(GetLoadedSaveSize, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(GetSavedSaveSize, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(GetSaveName, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(RoundAlt, "", 0, 3, kParams_Tomm_OneFloat_TwoOptionalInt)
DEFINE_COMMAND_PLUGIN(Round, "", 0, 1, kParams_Tomm_OneFloat)
DEFINE_COMMAND_PLUGIN(MarkScriptOnLoad, "", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(IsScriptMarkedOnLoad, "", 0, 1, kParams_Tomm_OneForm)
DEFINE_COMMAND_PLUGIN(GetNearCells, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(GetNearMapMarkers, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(SUPTest, "", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(GetPCTrait, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(GetHeadingAngleAlt, "", 0, 4, kParams_Tomm_ThreeFloats_OneObjectRef)
DEFINE_COMMAND_PLUGIN(GetHeadingAngleBetweenPoints, "", 0, 5, kParams_Tomm_FiveFloats)
DEFINE_COMMAND_PLUGIN(GetCalculatedAngleZForTFCCamera, "", 0, 2, kParams_Tomm_TwoFloats)
DEFINE_COMMAND_PLUGIN(GetCalculatedAngleXForTFCCamera, "", 0, 3, kParams_Tomm_ThreeFloats)
DEFINE_COMMAND_PLUGIN(IsNumberNAN, "", 0, 1, kParams_Tomm_OneFloatOptional)
DEFINE_COMMAND_PLUGIN(GetDistanceBetweenPoints, "", 0, 6, kParams_Tomm_SixFloats)
DEFINE_COMMAND_PLUGIN(GetCalculatedPosFrame, "", 0, 10, kParams_Tomm_SevenFloats_ThreeScriptVars)
DEFINE_COMMAND_PLUGIN(GetCalculatedPosAlt, "Gets current PosXYZ", 0, 6, kParams_Tomm_TwoFloats_TwoScriptVars_TwoFloats)
DEFINE_COMMAND_PLUGIN(ReadINIStringFromFile, "", 0, 3, kParams_Tomm_ThreeStrings)
DEFINE_COMMAND_PLUGIN(SetFloatsFromArray, "", 0, 11, kParams_Tomm_SetFloatsFromArray)
DEFINE_COMMAND_PLUGIN(GetMousePosition, "", 0, 2, kParams_Tomm_TwoScriptVars)
DEFINE_COMMAND_PLUGIN(FakeMouseMovement, "", 0, 2, kParams_Tomm_TwoFloats)
DEFINE_COMMAND_PLUGIN(GetGrenadeTimeHeld, "", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(IsPlayerOverencumbered, "", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(SetCaughtPCPickpocketting, "", 1, 1, kParams_Tomm_OneIntOptional)
DEFINE_COMMAND_PLUGIN(SupTestArray, "", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(KillAll2, "", 0, 0, NULL)









bool Cmd_GetCalculatedPos_Execute(COMMAND_ARGS) //cos sin values taken from JiP

{

	ScriptVar* outX, * outY;
	float fAngle, fDistance;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &fAngle, &fDistance, &outX, &outY))
	{
		outX->data.num = ((thisObj->posX) + fDistance * (sin(fAngle * 0.017453292519943295)));
		outY->data.num = ((thisObj->posY) + fDistance * (cos(fAngle * 0.017453292519943295)));
	}

	return true;

}

bool Cmd_GetCalculatedPosAlt_Execute(COMMAND_ARGS) //cos sin values taken from JiP

{
	ScriptVar* outX, * outY;
	float fAngle, fDistance;
	float fXIn, fYIn;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &fAngle, &fDistance, &outX, &outY, &fXIn, &fYIn))
	{
		outX->data.num = (fXIn + fDistance * (sin(fAngle * 0.017453292519943295)));
		outY->data.num = (fYIn + fDistance * (cos(fAngle * 0.017453292519943295)));
	}

	return true;

}




bool Cmd_GetSUPVersion_Execute(COMMAND_ARGS)
{
	*result = -1;
	Console_Print("%s %d", "SUPNVSEVersion>>", SUPNVSEVersion);
	return true;

}






bool Cmd_GetFileSize_Execute(COMMAND_ARGS)
{
	alignas(16) char s_strArgTemp[0x4000];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgTemp))
	{
		if (strstr(s_strArgTemp, ":\\"))
		{ 
			*result = -1;
			return true;
		}
		if (strstr(s_strArgTemp, "..\\")) // thanks C6 
		{
			*result = -1;
			return true; 
		}
		if (strstr(s_strArgTemp, "://"))
		{
			*result = -1;
			return true;
		}
		if (strstr(s_strArgTemp, "..//")) 
		{
			*result = -1;
			return true;
		}
		FileStream srcFile;
		if (!srcFile.Open(s_strArgTemp))
		{
			*result = -1;
			return true;
		}
		UInt32 length = srcFile.GetLength();
		*result = length;
		srcFile.Close();
		return true;
	}
}


//0 for FOS
//1 for NVSE
bool Cmd_GetLoadedSaveSize_Execute(COMMAND_ARGS)
{
	FileStream srcFile;
	UInt32 iRequest = 0;
	char* SavegameName;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &iRequest))
	{

		if (iRequest == 0)
		{SavegameName = LoadedSGPathFOS;}
		else
		{SavegameName = LoadedSGPathNVSE;}

		if (!srcFile.Open(SavegameName))
		{
			*result = -1;
		}
		else
		{
			UInt32 length = srcFile.GetLength();
			*result = length;
		}
		return true;
	}
}


//0 for FOS
//1 for NVSE
bool Cmd_GetSavedSaveSize_Execute(COMMAND_ARGS)
{
	FileStream srcFile;
	UInt32 iRequest = 0;
	char* SavegameName;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &iRequest))
	{
		if (iRequest == 0)
		{
			SavegameName = SavedSGPathFOS;
		}
		else
		{
			SavegameName = SavedSGPathNVSE;
		}

		if (!srcFile.Open(SavegameName))
		{
			*result = -1;
		}
		else
		{
			UInt32 length = srcFile.GetLength();
			*result = length;
		}
		return true;
	}
}




//0 for Last saved
//1 for Last Loaded
bool Cmd_GetSaveName_Execute(COMMAND_ARGS)
{
	UInt32 iRequest = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &iRequest))
	{
		if (iRequest == 1)
		{if (iLoadSGLength > 0){StrIfc->Assign(PASS_COMMAND_ARGS, LoadedSGName);}}
		else
		{if (iSavedSGLength > 0){StrIfc->Assign(PASS_COMMAND_ARGS, SavedSGName);}}
	}
	return true;
}


bool Cmd_RoundAlt_Execute(COMMAND_ARGS) //made by me for a change
{
	float fValue, fAdd = 0;
	UInt32 iRound = 0, iPrecision = 0, iMult;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &fValue,&iPrecision,&iRound))
	{
		if (iPrecision == 0) {iMult = 1;}else { iMult = pow(10, iPrecision); }

		if (iRound == 1){fAdd = 0.5;};

		float value = (int)((fValue * iMult) + fAdd);
		*result = (float)value / iMult;
	}
	return true;
}



bool Cmd_Round_Execute(COMMAND_ARGS) //math taken from MATH
{
	float fValue;


	if (ExtractArgsEx(EXTRACT_ARGS_EX, &fValue))
	{
		*result = round(fValue);
	}
	return true;
}

UnorderedSet<UInt32> s_gameLoadedInformedScriptsSUP;

bool Cmd_MarkScriptOnLoad_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (scriptObj)
	{
		if (s_gameLoadedInformedScriptsSUP.Insert(scriptObj->refID))
		{
			*result = 1;
		}
	}
	return true;
}

bool Cmd_IsScriptMarkedOnLoad_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm* scriptArg = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &scriptArg))
	{
		if (scriptArg)
		{
			if (s_gameLoadedInformedScriptsSUP.HasKey(scriptArg->refID))
			{
				*result = 1;
			}
		}
	}

	return true;
}





struct CellScanInfo
{
	TESObjectCELL::RefList::Iterator	prev;	//last ref returned to script
		TESObjectCELL* curCell;					//cell currently being scanned
		TESObjectCELL* cell;						//player's current cell
		TESWorldSpace* world;
	SInt8	curX;										//offsets of curCell from player's cell
	SInt8	curY;
	UInt8	cellDepth;									//depth of adjacent cells to scan


	CellScanInfo() {}
	CellScanInfo(UInt8 _cellDepth, TESObjectCELL* _cell)
		: curCell(NULL), cell(_cell), world(NULL), curX(0), curY(0),  cellDepth(_cellDepth)
	{
		world = cell->worldSpace;

		if (world && cellDepth)		//exterior, cell depth > 0
		{
			curX = cell->coords->x - cellDepth;
			curY = cell->coords->y - cellDepth;
			UInt32 key = (curX << 16) + ((curY << 16) >> 16);
			curCell = world->cellMap->Lookup(key);
		}
		else
		{
			cellDepth = 0;
			curCell = cell;
			curX = cell->coords->x;
			curY = cell->coords->y;
		}
	}

	bool NextCell()		//advance to next cell in area
	{
		if (!world || !cellDepth)
		{
			curCell = NULL;
			return false;
		}

		do
		{
			if (curX - cell->coords->x == cellDepth)
			{
				if (curY - cell->coords->y == cellDepth)
				{
					curCell = NULL;
					return false;
				}
				else
				{
					curY++;
					curX -= cellDepth * 2;
					UInt32 key = (curX << 16) + ((curY << 16) >> 16);
					curCell = world->cellMap->Lookup(key);
				}
			}
			else
			{
				curX++;
				UInt32 key = (curX << 16) + ((curY << 16) >> 16);
				curCell = world->cellMap->Lookup(key);
			}
		} while (!curCell);

		return true;
	}

	void FirstCell()	//init curCell to point to first valid cell
	{
		if (!curCell)
			NextCell();
	}

};






bool Cmd_GetNearCells_Execute(COMMAND_ARGS) // from NVSE
{
	double uGrid = 0;

	NVSEArrayVar* CellsArr = ArrIfc->CreateArray(NULL, 0, scriptObj);
	SInt32 cellDepth = 1; 

	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &cellDepth)) return true;

	PlayerCharacter* pc = PlayerCharacter::GetSingleton();
	if (!pc || !(pc->parentCell))
		return true;						//avoid crash when these functions called in main menu before parentCell instantiated

	TESObjectCELL* cell = NULL;
	cell = pc->parentCell;

	if (!cell)
		return true;

	if (cellDepth == -127)
		cellDepth = 0;
	else if (cellDepth == -1)
		if (GetNumericIniSetting("uGridsToLoad:General", &uGrid))
			cellDepth = uGrid;
		else
			cellDepth = 0;


	//Console_Print("%s %d", "Cell depth is>>", cellDepth);

	CellScanInfo info(cellDepth,cell);
	info.FirstCell();

	while (info.curCell)
	{
		ArrIfc->AppendElement(CellsArr, NVSEArrayElement(info.curCell));
		
		info.NextCell();
	}
	ArrIfc->AssignCommandResult(CellsArr, result);
	return true;
}

//NVSEArrayVar* CellsArr = ArrIfc->CreateStringMap(NULL, NULL, 0, scriptObj);
//ArrIfc->SetElement(CellsArr, "dff", 2);
//ArrIfc->SetElement(CellsArr, "dff34234", 3);






bool Cmd_GetNearMapMarkers_Execute(COMMAND_ARGS) // from NVSE
{
	double uGrid = 0;
	NVSEArrayVar* MarkArr = ArrIfc->CreateArray(NULL, 0, scriptObj);
	SInt32 cellDepth = 1;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &cellDepth)) return true;
	PlayerCharacter* pc = PlayerCharacter::GetSingleton();
	if (!pc || !(pc->parentCell))
		return true;						//avoid crash when these functions called in main menu before parentCell instantiated
	TESObjectCELL* cell = NULL;
	cell = pc->parentCell;

	if (!cell)
		return true;

	if (cellDepth == -127)
		cellDepth = 0;
	else if (cellDepth == -1)
		if (GetNumericIniSetting("uGridsToLoad:General", &uGrid))
			cellDepth = uGrid;
		else
			cellDepth = 0;

	CellScanInfo info(cellDepth, cell);
	info.FirstCell();

	while (info.curCell)
	{
		//_MESSAGE("Current NVSE CELL REF IS [%x]", info.curCell->refID);
		const TESObjectCELL::RefList& refList = info.curCell->objectList;
		for (TESObjectCELL::RefList::Iterator iter = refList.Begin(); !iter.End(); ++iter)
		{
			TESObjectREFR* pRefr = iter.Get();
			if (pRefr)
				if (pRefr->baseForm->typeID == 32)
				{
					ExtraMapMarker* xMarker = GetExtraType(&pRefr->extraDataList, MapMarker);
					if (xMarker)	{
						ArrIfc->AppendElement(MarkArr, NVSEArrayElement(pRefr));
					}
				}
		}
		info.NextCell();
	}

	ArrIfc->AssignCommandResult(MarkArr, result);
	return true;
}










bool Cmd_GetHeadingAngleAlt_Execute(COMMAND_ARGS) //https://stackoverflow.com/questions/48227928/calculating-heading-angle-from-2-points
{

	float CallX, CallY, dir, TargetX, TargetY;
	TESObjectREFR* TargetRef = NULL; 

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &CallX, &CallY, &dir, &TargetRef)) 
	{
		//dir = g_ThePlayer->flycamZRot / 0.017453292519943295;

		TargetX = TargetRef->posX;
		TargetY = TargetRef->posY;

		double heading = std::atan2(TargetX - CallX, TargetY - CallY) * 180 / 3.1415926535897 - dir;
		if (heading < -180) heading += 360;
		if (heading > 180) heading -= 360;

		*result = heading;
	}

	return true;
}


bool Cmd_GetHeadingAngleBetweenPoints_Execute(COMMAND_ARGS) //https://stackoverflow.com/questions/48227928/calculating-heading-angle-from-2-points
{

	float CallX, CallY, dir, TargetX, TargetY;


	if (ExtractArgsEx(EXTRACT_ARGS_EX, &CallX, &CallY, &dir, &TargetX, &TargetY))
	{
		//dir = g_ThePlayer->flycamZRot / 0.017453292519943295;

		double heading = std::atan2(TargetX - CallX, TargetY - CallY) * 180 / 3.1415926535897 - dir;
		if (heading < -180) heading += 360;
		if (heading > 180) heading -= 360;

		*result = heading;
	}

	return true;
}


bool Cmd_GetCalculatedAngleZForTFCCamera_Execute(COMMAND_ARGS) //https://stackoverflow.com/questions/48227928/calculating-heading-angle-from-2-points
{

	float CallX, CallY, TargetX, TargetY;


	if (ExtractArgsEx(EXTRACT_ARGS_EX, &TargetX, &TargetY))
	{
		//dir = g_ThePlayer->flycamZRot / 0.017453292519943295;
		CallX = g_ThePlayer->flycamPosX;
		CallY = g_ThePlayer->flycamPosY;

		double heading = std::atan2(TargetX - CallX, TargetY - CallY) * 180 / 3.1415926535897;
		if (heading < -180) heading += 360;
		if (heading > 180) heading -= 360;

		*result = heading;
	}

	return true;
}

bool Cmd_GetCalculatedAngleXForTFCCamera_Execute(COMMAND_ARGS)//I HATE MATH SO MUCH
{
	float x1, x2, y1, y2, z1, z2;
	float fSin, fPlayerAngle;
	if (ExtractArgsEx(EXTRACT_ARGS_EX,&x2, &y2, &z2))
	{
		x1 = g_ThePlayer->flycamPosX;
		y1 = g_ThePlayer->flycamPosY;
		z1 = g_ThePlayer->flycamPosZ;
		float fDistance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2) * 1.0);
		fSin = asin((z1 - z2) / fDistance);

		*result = fSin * 180/3.14159265;
	}
	return true;
}

//Console_Print("%s %f", "TFCZ is>>", z1);










WINBASEAPI
DWORD
WINAPI
GetTickCount(VOID);

bool Cmd_GetPCTrait_Execute(COMMAND_ARGS) //https://github.com/quantumcore/supercharge/issues/1
{
	int iTrait;
	*result = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &iTrait))
	{
		if (iTrait == 0 || iTrait == 1)
		{
			NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
			OSVERSIONINFOEXW osInfo;
			*reinterpret_cast<FARPROC*>(&RtlGetVersion) = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");
			if (nullptr != RtlGetVersion)
			{
				osInfo.dwOSVersionInfoSize = sizeof osInfo;
				RtlGetVersion(&osInfo);
				if (iTrait == 0)
				{*result = osInfo.dwMajorVersion;}
				else { *result = osInfo.dwMinorVersion; }
			}
			return true;
		}
		else if (iTrait == 2 || iTrait == 3 || iTrait == 4)
		{
			MEMORYSTATUSEX statex;
			statex.dwLength = sizeof(statex);
			GlobalMemoryStatusEx(&statex);
			if (iTrait == 2)
			{*result = statex.ullTotalPhys / 1024;}
			else if (iTrait == 3)
			{*result = statex.ullAvailPhys / 1024;}
			else if (iTrait == 4)
			{*result = statex.dwMemoryLoad;}
		}else if (iTrait == 5)
		{*result = GetTickCount();}
	}
	return true;
}



bool Cmd_IsNumberNAN_Execute(COMMAND_ARGS) //thanks Kormakur
{
	float fValue;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &fValue))
	{
		if (NUM_ARGS > 0)
		{
			if (isnan(fValue))
			{
				*result = 1;
			}
			else { *result = 0; }
		}else
		{
			*result = std::numeric_limits<float>::quiet_NaN();
		}
	}
	return true;
}



bool Cmd_GetDistanceBetweenPoints_Execute(COMMAND_ARGS)
{
	float x1, x2, y1, y2, z1, z2;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &x1, &y1, &z1, &x2, &y2, &z2))
	{
		float d = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2) * 1.0);
		*result = d;
	}
	return true;
}






bool Cmd_GetCalculatedPosFrame_Execute(COMMAND_ARGS) //Fallout2AM
{

	float PosX, PosY, PosZ, fEndPosX, fEndPosY, fEndPosZ, iFrame;

	float ChangeX, ChangeY, ChangeZ;

	ScriptVar *outX, *outY, *outZ;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &PosX, &PosY, &PosZ, &fEndPosX, &fEndPosY, &fEndPosZ, &iFrame, &outX, &outY, &outZ))
	{

		if (PosX > 0)
		{
			if (fEndPosX < 0)
			{ChangeX = -(PosX - fEndPosX) / iFrame;}
			else
				if (PosX > fEndPosX)
				{ChangeX = -(PosX - fEndPosX) / iFrame;}
				else { ChangeX = (fEndPosX - PosX) / iFrame; }
		}
		else
		{
			if (fEndPosX > 0)
				ChangeX  = (fEndPosX - PosX) / iFrame;
			else {
				if (PosX > fEndPosX)
					ChangeX  = (fEndPosX - PosX) / iFrame;
				else{ChangeX = -(PosX - fEndPosX) / iFrame;}
			}

		}


		if (PosY > 0)
		{
			if (fEndPosY < 0)
			{ChangeY = -(PosY - fEndPosY) / iFrame;}
			else
				if (PosY > fEndPosY)
				{ChangeY = -(PosY - fEndPosY) / iFrame;}
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
			{ChangeZ = -(PosZ - fEndPosZ) / iFrame;}
			else
				if (PosZ > fEndPosZ)
				{ChangeZ = -(PosZ - fEndPosZ) / iFrame;}
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

		outX->data.num = PosX + ChangeX;
		outY->data.num = PosY + ChangeY;
		outZ->data.num = PosZ + ChangeZ;


	}

	return true;
}

void MouseMove(float x, float y)
{
	INPUT  Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE;
	Input.mi.dx = x;
	Input.mi.dy = y;
	::SendInput(1, &Input, sizeof(INPUT));
}


bool Cmd_FakeMouseMovement_Execute(COMMAND_ARGS)
{
	float ChangeX, ChangeY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ChangeX, &ChangeY))
	{
		MouseMove(ChangeX, ChangeY);
		return true;
	}
}


bool Cmd_SUPTest_Execute(COMMAND_ARGS)
{

	//g_ThePlayer->ToggleFirstPerson(1);
	return true;
}

bool Cmd_KillAll2_Execute(COMMAND_ARGS)
{
	UInt32 count;
	Actor* actor;
	CombatActors* cmbActors = g_ThePlayer->combatActors;
	if (!cmbActors) return true;

	CombatTarget* targets = cmbActors->targets.data;
	for (count = cmbActors->targets.size; count; count--, targets++)
	{
		actor = targets->target;
		if ((actor) && IS_ACTOR(actor))
		{
			KillActorExecute(paramInfo, scriptData, actor, containingObj, scriptObj, eventList, result, opcodeOffsetPtr);
		}
	}
	return true;
}






bool Cmd_SupTestArray_Execute(COMMAND_ARGS)
{

	NVSEArrayVar* MarkArr = ArrIfc->CreateArray(NULL, 0, scriptObj);
	*result = 0;

	MobileObject** objArray = g_processManager->objects.data, ** arrEnd = objArray;
	objArray += g_processManager->beginOffsets[0];
	arrEnd += g_processManager->endOffsets[0];
	Actor* actor;


	for (; objArray != arrEnd; objArray++)
	{
		actor = (Actor*)*objArray;
		if (actor && IS_ACTOR(actor))
			ArrIfc->AppendElement(MarkArr, NVSEArrayElement(actor));
	}

	ArrIfc->AssignCommandResult(MarkArr, result);
	return true;

}








bool Cmd_SetCaughtPCPickpocketting_Execute(COMMAND_ARGS) // From JiP
{
	int iSet;
	Actor* actor = (Actor*)thisObj;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &iSet))
	{
		if (NUM_ARGS > 0)
		{
			((Actor*)thisObj)->baseProcess->SetHasCaughtPlayerPickpocketting(iSet);
		}
		else
		{
			bool iCaught = ((Actor*)thisObj)->baseProcess->GetHasCaughtPlayerPickpocketting(); 
			*result = iCaught;
		}
	}
	return true;
}







bool Cmd_IsPlayerOverencumbered_Execute(COMMAND_ARGS)
{
	*result = g_ThePlayer->Unk_D6(); // From JiP

	return true;
}



bool Cmd_GetMousePosition_Execute(COMMAND_ARGS)
{
	ScriptVar* outX, * outY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &outX, &outY))
	{
		POINT p;
		if (GetCursorPos(&p))
		{
			outX->data.num = p.x;
			outY->data.num = p.y;
		}
	}
	return true;
}

//int xPos = GET_X_LPARAM(lParam);
//int yPos = GET_Y_LPARAM(lParam);

	//Console_Print("%s %d", "X>>", xPos);
	//Console_Print("%s %d", "Y>>", yPos);






//bool Cmd_SUPTestVATS_Execute(COMMAND_ARGS)
//{
//	UInt32 menuID = 1056;
//	TileMenu* tileMenu = g_tileMenuArray[menuID - kMenuType_Min];
//	Menu* menu = tileMenu ? tileMenu->menu : NULL;
//
//	TESForm* menuRef = NULL;
//	VATSMenu* VATSMenuActual = (VATSMenu*)menu;
//	menuRef = VATSMenuActual->targetRef;
//	REFR_RES = menuRef->refID;
//
//	Actor* target;
//
//	if (ExtractArgsEx(EXTRACT_ARGS_EX, &target))
//	{
//		VATSMenuActual->targetRef = target;
//		Console_Print("%s %d", "TargetSet>>", 1);
//	}
//
//	return true;
//}










//


bool Cmd_SetFloatsFromArray_Execute(COMMAND_ARGS)
{
	UInt32 arrID;

	ScriptVar *V0, *V1 = NULL, *V2 = NULL, *V3 = NULL, *V4 = NULL, *V5 = NULL, *V6 = NULL, *V7 = NULL, *V8 = NULL, *V9 = NULL;

	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &arrID,&V0, &V1, &V2, &V3, &V4, &V5, &V6, &V7, &V8, &V9)) return true;

	NVSEArrayVar* inArr = ArrIfc->LookupArrayByID(arrID);
	if (!inArr) return true;
	UInt32 size = ArrIfc->GetArraySize(inArr);
	NVSEArrayElement* elements = new NVSEArrayElement[size];
	ArrIfc->GetElements(inArr, elements, NULL);
	int iNeededVars = NUM_ARGS - 1;
	//Console_Print("%s %d", "Array size is>>>>", size);
	//Console_Print("%s %d", "iNeededVars>>>>", iNeededVars);

	for (int i = 0; i < size; i++) {

		if (i > iNeededVars)
		{
			//Console_Print("%s %d,value ENDING", "ArrayEntry>>", i);
			break;
		}

		if (elements[i].Number())
		{
			//Console_Print("%s %d,value is>>> %f", "ArrayEntry>>", i, elements[i].Number());
			if (i == 0){if (V0)V0->data.num = elements[i].Number();}
			else if (i == 1) {if (V1)V1->data.num = elements[i].Number();}
			else if (i == 2) { if (V2)V2->data.num = elements[i].Number(); }
			else if (i == 3) { if (V3)V3->data.num = elements[i].Number(); }
			else if (i == 4) { if (V4)V4->data.num = elements[i].Number(); }
			else if (i == 5) { if (V5)V5->data.num = elements[i].Number(); }
			else if (i == 6) { if (V6)V6->data.num = elements[i].Number(); }
			else if (i == 7) { if (V7)V7->data.num = elements[i].Number(); }
			else if (i == 8) { if (V8)V8->data.num = elements[i].Number(); }
			else if (i == 9) { if (V9)V9->data.num = elements[i].Number(); }
		}
		else {
			//Console_Print("%s %d,value is not a number", "ArrayEntry>>", i);
		}
	}
	
	delete[] elements;
	*result = 0;

	return true;


}


bool Cmd_GetGrenadeTimeHeld_Execute(COMMAND_ARGS)
{
	*result = g_ThePlayer->timeGrenadeHeld;
	return true;
}





//void InitCmdPatches()
//{
//	CommandInfo* cmdInfo = GetCmdByOpcode(0x1024);
//	cmdInfo->execute = Hook_MenuMode_Execute;
//	cmdInfo->eval = (Cmd_Eval)Hook_MenuMode_Eval;
//	cmdInfo = GetCmdByOpcode(0x102F);
//	cmdInfo->execute = Hook_GetItemCount_Execute;
//	cmdInfo->eval = (Cmd_Eval)Hook_GetItemCount_Eval;
//	cmdInfo = GetCmdByOpcode(0x10CF);
//	cmdInfo->execute = Hook_GetContainer_Execute;
//	cmdInfo = GetCmdByOpcode(0x1174);
//	cmdInfo->execute = Hook_IsInList_Execute;
//	cmdInfo->eval = (Cmd_Eval)Hook_IsInList_Eval;
//	cmdInfo = GetCmdByOpcode(0x1187);
//	cmdInfo->execute = Hook_GetHitLocation_Execute;
//	cmdInfo = GetCmdByOpcode(0x11C1);
//	cmdInfo->execute = Hook_HasPerk_Execute;
//	cmdInfo->eval = (Cmd_Eval)Hook_HasPerk_Eval;
//	cmdInfo = GetCmdByOpcode(0x127D);
//	cmdInfo->execute = Cmd_GetNVSEVersionFull_Execute;
//	cmdInfo = GetCmdByOpcode(0x1403);
//	cmdInfo->execute = Hook_GetBaseObject_Execute;
//	cmdInfo = GetCmdByOpcode(0x1453);
//	cmdInfo->execute = Hook_IsKeyPressed_Execute;
//	cmdInfo->eval = (Cmd_Eval)Hook_IsKeyPressed_Eval;
//	cmdInfo = GetCmdByOpcode(0x1462);
//	cmdInfo->execute = Hook_DisableControl_Execute;
//	cmdInfo = GetCmdByOpcode(0x1463);
//	cmdInfo->execute = Hook_EnableControl_Execute;
//	cmdInfo = GetCmdByOpcode(0x1464);
//	cmdInfo->execute = Hook_TapControl_Execute;
//	cmdInfo = GetCmdByOpcode(0x146A);
//	cmdInfo->execute = Hook_IsControlDisabled_Execute;
//	cmdInfo = GetCmdByOpcode(0x146B);
//	cmdInfo->execute = Hook_IsControlPressed_Execute;
//	cmdInfo->eval = (Cmd_Eval)Hook_IsControlPressed_Eval;
//	cmdInfo = GetCmdByOpcode(0x149A);
//	cmdInfo->execute = Hook_GetNumericGameSetting_Execute;
//	cmdInfo = GetCmdByOpcode(0x149B);
//	cmdInfo->execute = Hook_SetNumericGameSetting_Execute;
//	cmdInfo->params = kParams_JIP_OneString_OneDouble;
//	cmdInfo = GetCmdByOpcode(0x149C);
//	cmdInfo->execute = Hook_GetNumericINISetting_Execute;
//	cmdInfo = GetCmdByOpcode(0x149D);
//	cmdInfo->execute = Hook_SetNumericINISetting_Execute;
//	cmdInfo->params = kParams_JIP_OneString_OneDouble;
//	cmdInfo = GetCmdByOpcode(0x1511);
//	cmdInfo->execute = Hook_IsRefInList_Execute;
//	cmdInfo->eval = (Cmd_Eval)Hook_IsRefInList_Eval;
//	cmdInfo = GetCmdByOpcode(0x152D);
//	cmdInfo->execute = Hook_Update3D_Execute;
//	cmdInfo = GetCmdByOpcode(0x158D);
//	cmdInfo->execute = Hook_ActorValueToStringC_Execute;
//	cmdInfo = GetCmdByOpcode(0x15DF);
//	IsPluginInstalled = cmdInfo->execute;
//	cmdInfo->execute = Hook_IsPluginInstalled_Execute;
//	cmdInfo = GetCmdByOpcode(0x15E0);
//	GetPluginVersion = cmdInfo->execute;
//	cmdInfo->execute = Hook_GetPluginVersion_Execute;
//
//	SayTo = GetCmdByOpcode(0x1034)->execute;
//	KillActor = GetCmdByOpcode(0x108B)->execute;
//	AddNote = GetCmdByOpcode(0x117C)->execute;
//	AttachAshPile = GetCmdByOpcode(0x1211)->execute;
//	GetRefs = GetCmdByOpcode(0x15C7)->execute;
//
//	PrintLog("> Command patches initialized successfully.\n");
//}






//*result = g_ThePlayer->pcControlFlags;
//g_ThePlayer->pcControlFlags = 13;


//UInt32 menuID = 1056;
//TileMenu* tileMenu = g_tileMenuArray[menuID - kMenuType_Min];
//Menu* menu = tileMenu ? tileMenu->menu : NULL;

//TESForm* menuRef = NULL;
//VATSMenu* VATSMenuActual = (VATSMenu*)menu;

//VATSMenuActual->tile02C->Dump();		// 02C
//VATSMenuActual->tile030->Dump();		// 030
//VATSMenuActual->tile034->Dump();		// 034
//VATSMenuActual->tile038->Dump();		// 038
//VATSMenuActual->tile03C->Dump();		// 03C
//VATSMenuActual->tile040->Dump();		// 040
//VATSMenuActual->tile044->Dump();		// 044
//VATSMenuActual->tile048->Dump();		// 048
//VATSMenuActual->tile04C->Dump();		// 04C
//VATSMenuActual->tile050->Dump();		// 050
//VATSMenuActual->tile054->Dump();		// 054
//VATSMenuActual->tile058->Dump();		// 058
//VATSMenuActual->tile05C->Dump();		// 05C
//VATSMenuActual->tile060->Dump();		// 060
//VATSMenuActual->tile064->Dump();		// 064
//VATSMenuActual->tile068->Dump();		// 068
//VATSMenuActual->tile06C->Dump();		// 06C
//VATSMenuActual->tile070->Dump();		// 070
//VATSMenuActual->tile074->Dump();		// 074
////TileRect* tile078->Dump();		// 078
////TileRect* tile07C->Dump();		// 07C
////TileRect* tile080->Dump();		// 080
//VATSMenuActual->tile084->Dump();		// 084
////TileRect* tile088->Dump();		// 088
//VATSMenuActual->tile08C->Dump();		// 08C
//VATSMenuActual->tile090->Dump();		// 090
//VATSMenuActual->tile094->Dump();		// 094
//VATSMenuActual->tile098->Dump();		// 098
//VATSMenuActual->tile09C->Dump();		// 09C
//VATSMenuActual->tile0A0->Dump();		// 0A0
//VATSMenuActual->tile0A4->Dump();		// 0A4

//float fResult = VATSMenuActual->isMissFortuneVisit;
//*result = fResult;
//REFR_RES = VATSMenuActual->targetRef->refID;

//TESObjectREFR* targetRef = g_VATSMenu->targetRef;
//float ClipAmmoMax = VATSMenuActual->ClipAmmoMax;
//float reserveAmmo = VATSMenuActual->reserveAmmo;
//float clipAmmo = VATSMenuActual->clipAmmo;
//float reserveAmmoMax = VATSMenuActual->reserveAmmoMax;
//UInt8 byte121 = VATSMenuActual->byte121;
//UInt8 byte123 = VATSMenuActual->byte123;

//UInt32 avCode = VATSMenuActual->avCode;
//float flt0FC = VATSMenuActual->flt0FC;
//int actionType = VATSMenuActual->actionType;
//Console_Print("%s %f", "ClipAmmoMax>>", ClipAmmoMax);
//Console_Print("%s %f", "reserveAmmo>>", reserveAmmo);
//Console_Print("%s %f", "clipAmmo>>", clipAmmo);
//Console_Print("%s %f", "reserveAmmoMax>>", reserveAmmoMax);
//Console_Print("%s %d", "avCode>>", avCode);
//Console_Print("%s %f", "flt0FC>>", flt0FC);
//Console_Print("%s %d", "actionType>>", actionType);

//Console_Print("%s %d", "byte121>>", byte121);
//Console_Print("%s %d", "byte123>>", byte123);









bool Cmd_SUPPlayMP3File_Execute(COMMAND_ARGS)
{
	//alignas(16) char s_strArgTemp[0x4000];
	//if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgTemp))
	//{
	//	FileStream srcFile;
	//	if (!srcFile.Open(s_strArgTemp))
	//	{
	//		*result = -1;
	//		return true;
	//	}
	//	srcFile.Close();
	//	MP3Player player;
	//	player.OpenFromFile(s_strArgTemp);
	//	player.Play();
	//	*result = 1;
		return true;

		//while (...) {
		//	// Do here your synchro in the demo using
		//	...
		//		double playerPositionInSeconds = player.GetPosition()
		//		...
		//}
		//player.Close();


	//}
}




//			set fSinus to (fPlayerHeight - ObjectRef.GetPos Z) / fDistanceDifference
//			set fFinalY to fResolutionY - (fPlayerAngleY - fasin fSinus) * MultY


//bool Cmd_SupTestCrosshair_Execute(COMMAND_ARGS)
//{
//	TESObjectREFR* target;
//
//	if (ExtractArgsEx(EXTRACT_ARGS_EX, &target))
//	{
//		Console_Print("%s %d", "TEST>>", ((TESFurniture*)target)->unk90);
//	}
//	return true;
//}



// RETURT CURRENT MEMORY LOAD
//MEMORYSTATUSEX statex;
//statex.dwLength = sizeof(statex);
//GlobalMemoryStatusEx(&statex);
//DWORD time = statex.dwMemoryLoad;
//Console_Print("%s %d", "SUP TEST COMMAND EXECUTED>>", time);

//WINBASEAPI
//DWORD
//WINAPI
//GetTickCount(VOID);

//	bool time = IsWindows7OrGreater();
	//WriteRelCall(0x00A1307E, (UInt32)&TestWriteRefCall);
	//DWORD time = GetSystemMetrics(SM_CMONITORS);
	//Console_Print("%s %d", "SUP TEST COMMAND EXECUTED>>", ret);
	//DIHookControl::GetSingleton().ProcessMouseData(1);

//bool Cmd_GetCalculatedAngleForTFCCamera_Execute(COMMAND_ARGS) //https://stackoverflow.com/questions/48227928/calculating-heading-angle-from-2-points
//{
//
//	float CallX, CallY, dir, TargetX, TargetY;
//
//
//	if (ExtractArgsEx(EXTRACT_ARGS_EX, &TargetX, &TargetY))
//	{
//		//dir = g_ThePlayer->flycamZRot / 0.017453292519943295;
//		dir = 0;
//		CallX = g_ThePlayer->flycamPosX;
//		CallY = g_ThePlayer->flycamPosY;
//
//		double heading = std::atan2(TargetX - CallX, TargetY - CallY) * 180 / 3.1415926535897 - dir;
//		if (heading < -180) heading += 360;
//		if (heading > 180) heading -= 360;
//
//		*result = heading;
//	}
//
//	return true;
//}

//DEFINE_COMMAND_PLUGIN(ToggleMouseInput, "", 0, 1, kParams_Tomm_OneIntOptional
//bool Cmd_ToggleMouseInput_Execute(COMMAND_ARGS)
//{
//	UInt32 iRequest;
//	*result = b_MouseInput;
//	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &iRequest) && (b_MouseInput == !iRequest))
//	{
//		b_MouseInput = !b_MouseInput;
//		if (iRequest)
//			SafeWriteBuf(0xA23252, "\x89\x45\xF8\x83\x7D", 5);
//		else WriteRelJump(0xA23252, 0xA23296);
//	}
//	return true;
//}

		//fPlayerAngle = g_ThePlayer->flycamXRot / 0.017453292519943295;


		///ListNode<HUDMainMenu::QueuedQuestText>* headNode = g_HUDMainMenu->queuedQuestTextList.Head(), * msgIter = headNode->next;
	/*	while (msgIter)
		{
			*result = 1;
			char *message = msgIter->data->msg;
			_MESSAGE("%s %d", message, 1);

		}*/

//bool Cmd_IsModuleLoaded_Execute(COMMAND_ARGS) //idea from Tweaks
//{
//	alignas(16) char s_strArgTemp[0x4000];
//	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgTemp))
//	{
//		if (GetModuleHandle(s_strArgTemp))
//		{
//			*result = 1;
//		}
//		else { *result = 0; }
//
//	}
//	Console_Print("%s %d", "ModuleLoaded>>", *result);
//	return true;
//}






//void gdiscreen(char s_strArgTemp[0x4000], float fPosXStart, float fPosXEnd, float fPosYStart, float fPosYEnd, int Format, int iUseGameResolution) {
//	using namespace Gdiplus;
//	IStream* istream;
//	HRESULT res = CreateStreamOnHGlobal(NULL, true, &istream);
//	GdiplusStartupInput gdiplusStartupInput;
//	ULONG_PTR gdiplusToken;
//	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
//	{
//
//		float fpicSizeX = fPosXEnd - fPosXStart;
//		float fpicSizeY = fPosYEnd - fPosYStart;
//
//		HDC scrdc, memdc;
//		HBITMAP membit;
//		scrdc = ::GetDC(0);
//		int Height = GetSystemMetrics(SM_CYSCREEN);
//		int Width = GetSystemMetrics(SM_CXSCREEN);
//		Console_Print("%s %d", "Height>>", Height);
//		Console_Print("%s %d", "Width>>", Width);
//		memdc = CreateCompatibleDC(scrdc);
//		_MESSAGE("Current SUP stage %i", 6);
//		//membit = CreateCompatibleBitmap(scrdc, Width, Height);
//		membit = CreateCompatibleBitmap(scrdc, 600, 600);
//		_MESSAGE("Current SUP stage %i", 7);
//		HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, membit);
//		//BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);
//		_MESSAGE("Current SUP stage %i", 8);
//		BitBlt(memdc, 0, 0, 600, 600, scrdc, 600, 600, SRCCOPY);
//		Gdiplus::Bitmap bitmap(membit, NULL);
//		CLSID clsid;
//		_MESSAGE("Current SUP stage %i", 9);
//		GetEncoderClsid(L"image/jpeg", &clsid);
//		bitmap.Save(L"111screen.jpeg", &clsid, NULL); // To save the jpeg to a file
//		_MESSAGE("Current SUP stage %i", 10);
//		bitmap.Save(istream, &clsid, NULL);
//		_MESSAGE("Current SUP stage %i", 11);
//		// Create a bitmap from the stream and save it to make sure the stream has the image
////		Gdiplus::Bitmap bmp(istream, NULL);
////		bmp.Save(L"t1est.jpeg", &clsid, NULL);             
//		// END
//		_MESSAGE("Current SUP stage %i", 12);
//		delete& clsid;
//		_MESSAGE("Current SUP stage %i", 13);
//		DeleteObject(memdc);
//		_MESSAGE("Current SUP stage %i", 14);
//		DeleteObject(membit);
//		_MESSAGE("Current SUP stage %i", 15);
//		::ReleaseDC(0, scrdc);
//	}
//	_MESSAGE("Current SUP stage %i", 16);
//	GdiplusShutdown(gdiplusToken);
//	_MESSAGE("Current SUP stage %i", 17);
//}

	//BGSCameraShot* camShot = g_VATSCameraData->camShot;
	//TESModel Cammodel = camShot->modelVATSCam;


	//g_VATSCameraData->mode = 4;


	//Console_Print("%s %d", "VATSMODE>>", g_VATSCameraData->mode); 