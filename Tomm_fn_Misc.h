#pragma once

DEFINE_COMMAND_PLUGIN(GetSUPVersion, "Get plugin version", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(GetFileSize, "Get file size", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_PLUGIN(GetLoadedSaveSize, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(GetSavedSaveSize, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(GetSaveName, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(MarkScriptOnLoad, "", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(IsScriptMarkedOnLoad, "", 0, 1, kParams_Tomm_OneForm)
DEFINE_COMMAND_PLUGIN(GetNearCells, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(GetNearMapMarkers, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(SUPTest, "", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(GetPCTrait, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(GetMousePosition, "", 0, 2, kParams_Tomm_TwoScriptVars)
DEFINE_COMMAND_PLUGIN(FakeMouseMovement, "", 0, 2, kParams_Tomm_TwoFloats)
DEFINE_COMMAND_PLUGIN(GetGrenadeTimeHeld, "", 0, 0, NULL)
DEFINE_CMD_ALT_COND_PLUGIN(IsPlayerOverencumbered, , , 0, 0, NULL);	
DEFINE_COMMAND_PLUGIN(SetCaughtPCPickpocketting, "", 1, 1, kParams_Tomm_OneIntOptional)
DEFINE_COMMAND_PLUGIN(KillAll2, "", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(LunetteCMD, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(FindClosestActorFromRef, "", 1, 3, kParams_Tomm_TwoIntsOptional_OneFloatOptional)
DEFINE_COMMAND_PLUGIN(FindClosestActor, "", 0, 6, kParams_Tomm_FindClosestActor)
DEFINE_COMMAND_PLUGIN(FindRandomActorFromRef, "", 1, 3, kParams_Tomm_TwoIntsOptional_OneFloatOptional)
DEFINE_COMMAND_PLUGIN(FindRandomActor, "", 0, 6, kParams_Tomm_FindClosestActor)









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


bool Cmd_IsPlayerOverencumbered_Eval(COMMAND_ARGS_EVAL)
{
	*result = g_ThePlayer->Unk_D6(); // From JiP
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


bool Cmd_LunetteCMD_Execute(COMMAND_ARGS)
{
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (modIdx == 0xFF) return true;
	if (strcmp("Lunette.esm", g_dataHandler->GetNthModName(modIdx))) return true;

	int iRequest;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &iRequest))
	{
		if (iRequest == 1)
		{
			g_ThePlayer->ToggleFirstPerson(1);
		}
		else if(iRequest == 2)
		{
			g_ThePlayer->ToggleFirstPerson(0);
		}
	}


	return true;
}


bool Cmd_GetGrenadeTimeHeld_Execute(COMMAND_ARGS)
{
	*result = g_ThePlayer->timeGrenadeHeld;
	return true;
}

bool Cmd_FindClosestActorFromRef_Execute(COMMAND_ARGS)
{
	REFR_RES = 0;
	int iIncludeDead = 0, iIncludeTeammates = 0;
	MobileObject** objArray = g_processManager->objects.data, ** arrEnd = objArray;
	objArray += g_processManager->beginOffsets[0];
	arrEnd += g_processManager->endOffsets[0];
	Actor* actor = NULL;
	Actor* ChosenActor = NULL;
	float fDistance = 0, fDistanceMax = -1;
	float fDistanceMinimum = 9999999999999999999;

	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &iIncludeDead, &iIncludeTeammates, &fDistanceMax)) return true;

	if (fDistanceMax <= 0)fDistanceMax = 9999999999999999999;

	for (; objArray != arrEnd; objArray++)
	{
		actor = (Actor*)*objArray;
		if (actor && IS_ACTOR(actor) && actor != thisObj)
		{
			//Console_Print("Current actor >>>%08x", actor->refID);

			if (actor->GetDead())
			{
				if (iIncludeDead)
				{}else {continue; }
			}

			if (actor->isTeammate)
			{
				if (iIncludeTeammates)
				{}else { continue; }
			}

			fDistance = GetDistance3D(thisObj, actor);

			if (fDistanceMinimum > fDistance)
			{
				if (fDistance < fDistanceMax)
				{
					fDistanceMinimum = fDistance;
					ChosenActor = actor;
					//Console_Print("Chosen actor FOUND in ARRAY");
				}

			}
		}
	}

	if (ChosenActor && IS_ACTOR(ChosenActor))
	{
		//Console_Print("Chosen actor FOUND>>>%08x", ChosenActor->refID);
		REFR_RES = ChosenActor->refID;
	}

	return true;
}



bool Cmd_FindClosestActor_Execute(COMMAND_ARGS)
{
	REFR_RES = 0;
	int iIncludeDead = 0, iIncludeTeammates = 0;
	MobileObject** objArray = g_processManager->objects.data, ** arrEnd = objArray;
	objArray += g_processManager->beginOffsets[0];
	arrEnd += g_processManager->endOffsets[0];
	Actor* actor = NULL;
	Actor* ChosenActor = NULL;
	float fDistance = 0, fDistanceMax = -1;
	float x1, x2, y1, y2, z1, z2;
	float fDistanceMinimum = 9999999999999999999;

	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &x1, &y1, &z1, &iIncludeDead, &iIncludeTeammates, &fDistanceMax)) return true;
	if (fDistanceMax <= 0)fDistanceMax = 9999999999999999999;
	for (; objArray != arrEnd; objArray++)
	{
		actor = (Actor*)*objArray;
		if (actor && IS_ACTOR(actor))
		{
			//Console_Print("Current actor >>>%08x", actor->refID);
			if (actor->GetDead())
			{
				if (iIncludeDead)
				{}else { continue; }
			}

			if (actor->isTeammate)
			{
				if (iIncludeTeammates)
				{}else { continue; }
			}

			x2 = actor->posX;
			y2 = actor->posY;
			z2 = actor->posZ;
			fDistance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2) * 1.0);


			if (fDistanceMinimum > fDistance)
			{
				if (fDistance < fDistanceMax)
				{
					fDistanceMinimum = fDistance;
					ChosenActor = actor;
				}
			}
		}
	}

	if (ChosenActor && IS_ACTOR(ChosenActor))
	{
		REFR_RES = ChosenActor->refID;
		//Console_Print("Chosen actor SET AS RESULT>>>%08x", ChosenActor->refID);
	}

	return true;
}


bool Cmd_FindRandomActorFromRef_Execute(COMMAND_ARGS)
{
	REFR_RES = 0;
	int iIncludeDead = 0, iIncludeTeammates = 0;
	MobileObject** objArray = g_processManager->objects.data, ** arrEnd = objArray;
	objArray += g_processManager->beginOffsets[0];
	arrEnd += g_processManager->endOffsets[0];
	Actor* actor = NULL;
	float fDistance = 0, fDistanceMax = -1;


	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &iIncludeDead, &iIncludeTeammates, &fDistanceMax)) return true;

	NVSEArrayVar* ActorArr = ArrIfc->CreateArray(NULL, 0, scriptObj);

	if (fDistanceMax <= 0)fDistanceMax = 9999999999999999999;

	for (; objArray != arrEnd; objArray++)
	{
		actor = (Actor*)*objArray;
		if (actor && IS_ACTOR(actor) && actor != thisObj)
		{
			//Console_Print("Current actor >>>%08x", actor->refID);

			if (actor->GetDead())
			{
				if (iIncludeDead)
				{}else { continue; }
			}

			if (actor->isTeammate)
			{
				if (iIncludeTeammates)
				{}else { continue; }
			}

			fDistance = GetDistance3D(thisObj, actor);

			if (fDistance < fDistanceMax)
			{
				ArrIfc->AppendElement(ActorArr, NVSEArrayElement(actor));
				//Console_Print("Chosen actor FOUND in ARRAY");
			}


		}
	}


	int iSize = ArrIfc->GetArraySize(ActorArr);
	//Console_Print("Array size is %d", iSize);

	if (iSize > 0)
	{
		//iSize -= 1;
		iSize = GetRandomIntInRange(0, iSize);
		//Console_Print("Random number is--> %d", iSize);

		NVSEArrayElement actorRes;

		ArrIfc->GetElement(ActorArr, iSize, actorRes);
		//Console_Print("Chosen actor FOUND>>>%08x", actorRes.form->refID);
		REFR_RES = actorRes.form->refID;
	}

	return true;
}


bool Cmd_FindRandomActor_Execute(COMMAND_ARGS)
{
	REFR_RES = 0;
	int iIncludeDead = 0, iIncludeTeammates = 0;
	MobileObject** objArray = g_processManager->objects.data, ** arrEnd = objArray;
	objArray += g_processManager->beginOffsets[0];
	arrEnd += g_processManager->endOffsets[0];
	Actor* actor = NULL;
	float fDistance = 0, fDistanceMax = -1;
	float x1, x2, y1, y2, z1, z2;


	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &x1, &y1, &z1, &iIncludeDead, &iIncludeTeammates, &fDistanceMax)) return true;
	NVSEArrayVar* ActorArr = ArrIfc->CreateArray(NULL, 0, scriptObj);
	if (fDistanceMax <= 0)fDistanceMax = 9999999999999999999;
	for (; objArray != arrEnd; objArray++)
	{
		actor = (Actor*)*objArray;
		if (actor && IS_ACTOR(actor))
		{
			Console_Print("Current actor >>>%08x", actor->refID);
			if (actor->GetDead())
			{
				if (iIncludeDead)
				{}else { continue; }
			}

			if (actor->isTeammate)
			{
				if (iIncludeTeammates)
				{}else { continue; }
			}

			x2 = actor->posX;
			y2 = actor->posY;
			z2 = actor->posZ;
			fDistance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2) * 1.0);

			if (fDistance < fDistanceMax)
			{
				ArrIfc->AppendElement(ActorArr, NVSEArrayElement(actor));
				//Console_Print("Chosen actor FOUND in ARRAY");
			}
			
		}
	}

	int iSize = ArrIfc->GetArraySize(ActorArr);
	//Console_Print("Array size is %d", iSize);

	if (iSize > 0)
	{
		iSize = GetRandomIntInRange(0, iSize);
		//Console_Print("Random number is--> %d", iSize);

		NVSEArrayElement actorRes;

		ArrIfc->GetElement(ActorArr, iSize, actorRes);
		//Console_Print("Chosen actor FOUND>>>%08x", actorRes.form->refID);
		REFR_RES = actorRes.form->refID;
	}
	return true;
}

bool Cmd_SUPTest_Execute(COMMAND_ARGS)
{


	return true;
}






//bool Cmd_SupTestArray_Execute(COMMAND_ARGS)
//{
//
//	NVSEArrayVar* MarkArr = ArrIfc->CreateArray(NULL, 0, scriptObj);
//	*result = 0;
//
//	MobileObject** objArray = g_processManager->objects.data, ** arrEnd = objArray;
//	objArray += g_processManager->beginOffsets[0];
//	arrEnd += g_processManager->endOffsets[0];
//	Actor* actor;
//
//
//	for (; objArray != arrEnd; objArray++)
//	{
//		actor = (Actor*)*objArray;
//		if (actor && IS_ACTOR(actor))
//			ArrIfc->AppendElement(MarkArr, NVSEArrayElement(actor));
//	}
//
//	ArrIfc->AssignCommandResult(MarkArr, result);
//	return true;
//
//}


