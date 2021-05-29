#pragma once

DEFINE_COMMAND_PLUGIN(GetSUPVersion, "Get plugin version", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(GetLoadedSaveSize, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(GetSavedSaveSize, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(GetSaveName, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(MarkScriptOnLoad, "", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(IsScriptMarkedOnLoad, "", 0, 1, kParams_Tomm_OneForm)
DEFINE_COMMAND_PLUGIN(GetNearCells, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(GetNearMapMarkers, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(SUPTest, "", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(GetGrenadeTimeHeld, "", 0, 0, NULL)
DEFINE_CMD_ALT_COND_PLUGIN(IsPlayerOverencumbered, , , 0, 0, NULL);	
DEFINE_COMMAND_PLUGIN(SetCaughtPCPickpocketting, "", 1, 1, kParams_Tomm_OneIntOptional)
DEFINE_COMMAND_PLUGIN(KillAll2, "", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(LunetteCMD, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(FindClosestActorFromRef, "", 1, 3, kParams_Tomm_TwoIntsOptional_OneFloatOptional)
DEFINE_COMMAND_PLUGIN(FindClosestActor, "", 0, 6, kParams_Tomm_FindClosestActor)
DEFINE_COMMAND_PLUGIN(FindRandomActorFromRef, "", 1, 3, kParams_Tomm_TwoIntsOptional_OneFloatOptional)
DEFINE_COMMAND_PLUGIN(FindRandomActor, "", 0, 6, kParams_Tomm_FindClosestActor)
DEFINE_COMMAND_PLUGIN(CallFunctionNextFrame, "", 0, 3, kParams_Tomm_CallFunctionNextFrame)
DEFINE_COMMAND_ALT_PLUGIN(DumpLoadOrder, DumpLO, "", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(GetModTraitSTR, "", 0, 2, kParams_Tomm_TwoInts)
DEFINE_COMMAND_PLUGIN(GetCurrentQuestObjectiveTeleportLinksAlt, "", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(GetNifBlockTranslationToVars, "", 0, 6, kParams_Tomm_GetNifBlockTranslationToVars)




//DEFINE_COMMAND_PLUGIN(SetCellFullNameSUP, "", 0, 2, kParams_Tomm_OneCell_OneString)







bool Cmd_GetSUPVersion_Execute(COMMAND_ARGS)
{
	*result = -1;
	Console_Print("%s %d", "SUPNVSEVersion>>", SUPNVSEVersion);
	return true;

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








//#include <iostream>
//#include <string>
//#include <map>
//#include <fstream>
//
//using namespace std;
//map <string, HUDBarElement*> BARS_MAP;
//
//int STD_STRING_TEST()
//{
//
//
//	HUDBarElement* HUDBAR = new HUDBarElement;
//	HUDBAR->MeterType = GetRandomIntInRange(0,10000);
//	BARS_MAP.insert(std::pair<string, HUDBarElement*>("ONE", HUDBAR));
//
//
//	HUDBAR = new HUDBarElement;
//	HUDBAR->MeterType = GetRandomIntInRange(0, 10000);
//	BARS_MAP.insert(std::pair<string, HUDBarElement*>("TWO", HUDBAR));
//
//	HUDBAR = new HUDBarElement;
//	HUDBAR->MeterType = GetRandomIntInRange(0, 10000);
//	BARS_MAP.insert(std::pair<string, HUDBarElement*>("THREE", HUDBAR));
//
//
//	for (const pair<const string, HUDBarElement*>& p : BARS_MAP) {
//		//Console_Print(" MeterType is %d ",p.second->MeterType);
//		Console_Print("String is %s, MeterType is %d ", p.first.c_str(), p.second->MeterType);
//	}
//	Console_Print("Array size is %d ",BARS_MAP.size());
//	Console_Print("FIND TWO>>> %d ", BARS_MAP["TWO"]->MeterType);
//	
//
//
//
//	char s_BarName[0x4000]{};
//	sprintf(s_BarName, "TWO");
//	char* s_BarName2 = s_BarName;
//
//	BARS_MAP.erase(s_BarName2);
//
//	Console_Print("AFTER ERASING");
//
//
//
//	for (const pair<const string, HUDBarElement*>& p : BARS_MAP) {
//		//Console_Print(" MeterType is %d ",p.second->MeterType);
//		Console_Print("String is %s, MeterType is %d ", p.first.c_str(), p.second->MeterType);
//	}
//
//
//	Console_Print("END");
//	//map <string, HUDBarElement>::iterator cur;
//
//	//for (cur = BARS_MAP.begin();cur != BARS_MAP.end();cur++)
//	//{
//	//	Console_Print("MeterType is ", (*cur).second.MeterType);
//	//}
//
//
//	//HUDBAR = new HUDBarElement;
//	//HUDBAR->MeterType = 2;
//	//words.insert(std::pair<string, HUDBarElement*>("TWO", HUDBAR));
//
//
//
//	//HUDBAR = new HUDBarElement;
//	//HUDBAR->MeterType = 3;
//	//words.insert(std::pair<string, HUDBarElement*>("THREE", HUDBAR));
//
//
//
//
//	//while (in >> word)
//	//{
//	//	words[word]++;
//	//}
//	//ofstream out;
//	//out.open("out.txt");
//	//int count = 0;
//	//map <string, int>::iterator cur;
//	//out << "Words count:" << endl;
//
//
//
//	return 0;
//}

//bool (*FunctionCallScript)(Script* funcScript, TESObjectREFR* callingObj, TESObjectREFR* container, NVSEArrayElement* result, UInt8 numArgs, ...);


bool Cmd_CallFunctionNextFrame_Execute(COMMAND_ARGS)
{
	Script* script = NULL;
	NVSEArrayElement element;
	int iFramesRemain = 1;
	int iRemoveOnGameLoad = 1;

	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &iFramesRemain, &iRemoveOnGameLoad)) return true; // From JIP

	FunctionCaller NewFunctionCaller;

	if (g_FuncCallerArrayID > (INT_MAX - 5)) //Being paranoid hurts
		g_FuncCallerArrayID = 0;



	NewFunctionCaller.iID = g_FuncCallerArrayID;
	g_FuncCallerArrayID += 1;

	NewFunctionCaller.FunctionToCall = script;
	NewFunctionCaller.iRemoveOnGameLoad = iRemoveOnGameLoad;

	

	if (thisObj)
	{
		NewFunctionCaller.FunctionCallerRef = thisObj;
	}

	if (NUM_ARGS > 1)
	{
		if (iFramesRemain < 1)
		{iFramesRemain = 1;}
		else if (iFramesRemain > 1000)
		{iFramesRemain = 1000;}

		NewFunctionCaller.iFramesRemain = iFramesRemain;
	}
	g_FuncCallerArrayV.push_back(NewFunctionCaller);

	g_FuncCallerArrayIterate = 1;

	//_MESSAGE("SUP::Adding new function to call, ID::%d , Frames::%d", g_FuncCallerArrayID, iFramesRemain);

	*result = 1;
	return true;
}




template <typename I> std::string n2hexstr(I w, size_t hex_len = sizeof(I) << 1) {
	static const char* digits = "0123456789ABCDEF";
	std::string rc(hex_len, '0');
	for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
		rc[i] = digits[(w >> j) & 0x0f];
	return rc;
}



void f_Misc_DumpLoadOrder()
{
	int iKey = 0;
	int iModCount = 0;
	std::string s_temp{};

	size_t iHexLenght = 0;

	const ModInfo** activeModList = g_dataHandler->GetActiveModList();
	iModCount = g_dataHandler->GetActiveModCount();

	_MESSAGE("---Load order---");

		while (iKey < iModCount) {

			s_temp = n2hexstr(iKey, 2);
			_MESSAGE("%s (%d):%s", s_temp.c_str(), iKey, activeModList[iKey]->name);
			iKey++;
		}

	_MESSAGE("---Load order end---");


	//activeModList[modIndex]->name;

}

bool Cmd_DumpLoadOrder_Execute(COMMAND_ARGS)
{
	f_Misc_DumpLoadOrder();

	return true;
}



const char* f_Misc_GetModTraitSTR(UInt32 modIndex, int iRequest)
{

	char TempChar[0x4000]{};

	if (modIndex >= 255 || modIndex < 0)
		return "";

	const ModInfo** activeModList = g_dataHandler->GetActiveModList();
	if (modIndex < g_dataHandler->GetActiveModCount() && activeModList[modIndex])
		switch (iRequest) {
		case 0:
			sprintf(TempChar, "%s", activeModList[modIndex]->author);
			return TempChar;
			break;
		case 1:
			sprintf(TempChar, "%s", activeModList[modIndex]->description);
			return TempChar;
			break;
		}


	else
		return "";
}


bool Cmd_GetModTraitSTR_Execute(COMMAND_ARGS)
{

	int iModIndex = 0;
	int iRequest = 0;

	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &iModIndex, &iRequest)) return true;

	StrIfc->Assign(PASS_COMMAND_ARGS, f_Misc_GetModTraitSTR(iModIndex, iRequest));

}





#include "Tomm_Nif_blocks.h"
__declspec(naked) UInt32 NiAVObject::GetIndex()
{
	__asm
	{
		mov		eax, [ecx + 0x18]
		test	eax, eax
		jz		done
		mov		edx, [eax + 0xA0]
		movzx	eax, word ptr[eax + 0xA6]
		test	eax, eax
		jz		done
		ALIGN 16
		iterHead:
		cmp[edx + eax * 4 - 4], ecx
			jz		done
			dec		eax
			jnz		iterHead
			done :
		retn
	}
}

bool Cmd_SUPTest_Execute(COMMAND_ARGS)
{
	//NiNode* rootNode = thisObj->GetNiNode();

	//if (rootNode)
	//{
	//	NiAVObject** nodes = rootNode->m_children.data;
	//	int iKey = -1;

	//	while (rootNode->m_children.numObjs > (iKey += 1))
	//	{
	//		Console_Print("NAME IS %s", rootNode[iKey].GetName());

	//	}

	//}

	return true;
}




bool Cmd_GetCurrentQuestObjectiveTeleportLinksAlt_Execute(COMMAND_ARGS)
{

	NVSEArrayVar* MarkArr = ArrIfc->CreateArray(NULL, 0, scriptObj);
	QuestObjectiveTargets* targets = g_ThePlayer->GetCurrentQuestObjectiveTargets();
	if (targets)
	{
		for (tList<BGSQuestObjective::Target>::Iterator iter = targets->Begin(); !iter.End(); ++iter)
		{
			BGSQuestObjective::Target* data = iter.Get();
			if (data && data->target)
			{
				if (data && data->teleportLinks.size)
				{
					BGSQuestObjective::TeleportLink teleportLink = data->teleportLinks.data[0];
					ArrIfc->AppendElement(MarkArr, NVSEArrayElement(teleportLink.door));
				}
				else {

					ArrIfc->AppendElement(MarkArr, NVSEArrayElement(data->target));
				}
			}
		}
	}
	ArrIfc->AssignCommandResult(MarkArr, result);
	return true;

}






bool Cmd_GetNifBlockTranslationToVars_Execute(COMMAND_ARGS)
{
	*result = 0;
	char blockName[0x40];
	UInt32 getWorld = 0, pcNode = 0;
	ScriptVar* outX = NULL, * outY = NULL, * outZ = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &outX, &outY, &outZ, &blockName, &getWorld, &pcNode))
	{
		*result = -1;
		NiAVObject* niBlock = GetNifBlock(thisObj, pcNode, blockName);
		if (niBlock)
		{
			*result = 1;
			if (getWorld)
			{
				NiVector3& transltn = niBlock->m_worldTranslate;
				outX->data.num = transltn.x;
				outY->data.num = transltn.y;
				outZ->data.num = transltn.z;
			}else
			{
				NiVector3& transltn = niBlock->m_localTranslate;
				outX->data.num = transltn.x;
				outY->data.num = transltn.y;
				outZ->data.num = transltn.z;
			}
		}
	}
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


