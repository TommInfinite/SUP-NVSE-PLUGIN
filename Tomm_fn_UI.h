#pragma once


DEFINE_COMMAND_PLUGIN(GetHUDVisibilityFlags, "Gets HUD visibility flags", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(SetHUDVisibilityFlags, "Sets HUD visibility flags", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(DumpTileInfo, "Dumps XML element info", 0, 2, kParams_Tomm_OneIntOptional_OneStringOptional)
DEFINE_COMMAND_PLUGIN(DumpTileInfoAll, "Dumps ALL kRootMenu which means everything plus cursor", 0, 1, kParams_Tomm_OneIntOptional)
DEFINE_COMMAND_PLUGIN(GetScreenTrait, "Get Screen traits", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(GetCursorTrait, "Gets cursor traits", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(SetCursorTrait, "Sets cursor traits", 0, 2, kParams_Tomm_OneInt_OneFloat)
DEFINE_COMMAND_PLUGIN(SetCursorTraitGradual, "Sets cursor trait gradually", 0, 5, kParams_Tomm_OneInt_ThreeOptFloats_OneOptInt)
DEFINE_COMMAND_PLUGIN(DumpTileInfoToArray, "Dumps XML element info to array", 0, 1, kParams_Tomm_OneStringOptional)
DEFINE_COMMAND_PLUGIN(GetUIValueType, "", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_PLUGIN(DebugTextCreate, "", 0, 4, kParams_Tomm_DebugTextCreate)
DEFINE_COMMAND_PLUGIN(DebugTextExists, "", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_PLUGIN(DebugTextSetString, "", 0, 2, kParams_Tomm_TwoStrings)
DEFINE_COMMAND_PLUGIN(DebugTextDestroy, "", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_PLUGIN(DebugTextSetPos, "", 0, 3, kParams_Tomm_DebugTextSetPos)





//DEFINE_COMMAND_PLUGIN(SetTileValueAction, "", 0, 2, kParams_Tomm_TwoStrings)





void(__cdecl* HUDMainMenu_UpdateVisibilityState)(signed int) = (void(__cdecl*)(signed int))(0x771700); //Credits to lStewieAL - From Tweaks

bool Cmd_SetHUDVisibilityFlags_Execute(COMMAND_ARGS)
{


	UInt32 iVisFLags = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &iVisFLags))
	{
		if (g_HUDMainMenu)
		{
			g_HUDMainMenu->visibilityOverrides = iVisFLags;
			HUDMainMenu_UpdateVisibilityState(HUDMainMenu::kRECALCULATE);
		}
	}
	return true;
}


bool Cmd_GetHUDVisibilityFlags_Execute(COMMAND_ARGS)
{
	*result = g_HUDMainMenu->visibilityOverrides; 
	return true;
}



TileMenu* __fastcall GetMenuTile(const char* componentPath) ///Taken from JiP or NVSE?? dont' remember.
{
	UInt32 menuID = s_menuNameToID.Get(componentPath);
	return menuID ? g_tileMenuArray[menuID - kMenuType_Min] : NULL;
}


Tile* __fastcall GetTargetComponent(const char* componentPath, Tile::Value** value = NULL)
{
	char* slashPos = SlashPos(componentPath);
	if (!slashPos)
		return GetMenuTile(componentPath);
	*slashPos = 0;
	Tile* component = GetMenuTile(componentPath);
	if (!component)
		return NULL;
	const char* trait = NULL;
	component = component->GetComponent(slashPos + 1, &trait);
	if (!component)
		return NULL;
	if (trait)
	{
		if (!value || !(*value = component->GetValueName(trait)))
			return NULL;
	}
	else if (value)
		return NULL;
	return component;
}






void Tile::DumpArray(void)
{
	ArrIfc->SetElement(TileArrayStore, "Tilename", name.m_data);
	for (UInt32 i = 0; i < values.size; i++)
	{
		Value* val = values[i];
		const char* traitName = TraitIDToName(val->id);
		char		traitNameIDBuf[16];

		if (!traitName)
		{
			sprintf_s(traitNameIDBuf, "%08X", val->id);
			traitName = traitNameIDBuf;
		}

		if (val->str)
			ArrIfc->SetElement(TileArrayStore, traitName, val->str);
		//else if (val->action)
			//ArrIfc->SetElement(TileArrayStore, traitName, "NoValue");
		else
			ArrIfc->SetElement(TileArrayStore, traitName, val->num);
	}
	//gLog.Outdent();
}



bool Cmd_DumpTileInfoToArray_Execute(COMMAND_ARGS)
{
	alignas(16) char s_strArgTemp2[0x4000];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgTemp2))
	{
		Tile* component = GetTargetComponent(s_strArgTemp2);
		if (component){
			//_MESSAGE("Extracting specified HUD component.");
			TileArrayStore = ArrIfc->CreateStringMap(NULL, NULL, 0, scriptObj);
			component->DumpArray();
			ArrIfc->AssignCommandResult(TileArrayStore, result);
		}else 
		{
			Tile* component = g_interfaceManager->GetActiveTile();
			if (component)
			{
				//_MESSAGE("Extracting active HUD component.");
				TileArrayStore = ArrIfc->CreateStringMap(NULL, NULL, 0, scriptObj);
				component->DumpArray();
				ArrIfc->AssignCommandResult(TileArrayStore, result);
			}
		}
	}
	return true;
}













bool Cmd_DumpTileInfo_Execute(COMMAND_ARGS) // used as debugging function so don't care about performance.
{

	UInt32 iToFile = 0;
	alignas(16) char s_strArgTemp2[0x4000];

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &iToFile, &s_strArgTemp2 ))
	{

		Tile* component = GetTargetComponent(s_strArgTemp2);

		if (component)
		{
			if (iToFile == 1)
			{
				_MESSAGE("Extracting specified HUD component.");
				component->DumpAlt();
			}
			else {
				Console_Print("Extracting specified HUD component.");
				component->Dump();
			}

		}
		else {

			Tile* component = g_interfaceManager->GetActiveTile();
			if (component)
			{
				if (iToFile == 1)
				{
					_MESSAGE("Extracting active HUD component.");
					component->DumpAlt();
				}
				else {
					Console_Print("Extracting active HUD component.");
					component->Dump();
				}

			}

		}

	}


	return true;
}



bool Cmd_DumpTileInfoAll_Execute(COMMAND_ARGS)
{
	UInt32 iToFile = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &iToFile))
	{
		if (iToFile == 1)
		{
			if (g_kMenuRoot)
			{
				_MESSAGE("<<Dumping ALL menus>>");
				g_kMenuRoot->DumpAlt();
			}

			if (g_Cursor)
			{
				_MESSAGE("<<Dumping Cursor>>");
				g_Cursor->DumpAlt();
			}


		}else
		{
			if (g_kMenuRoot)
			{
				Console_Print("<<Dumping ALL menus>>");
				g_kMenuRoot->Dump();
			}
			if (g_Cursor)
			{
				Console_Print("<<Dumping Cursor>>");
				g_Cursor->Dump();
			}
		}
	}
	return true;
}


// 0 for UI Width
// 1 for UI Height
// 2 for Real Weight
// 3 for Real Height
// 4 for Resolution Converter


bool Cmd_GetScreenTrait_Execute(COMMAND_ARGS)
{
	UInt32 iTrait = -1;
	Tile::Value* val = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &iTrait))
	{
		if (g_kMenuRoot)
		{
			switch (iTrait)
			{
			case 0:
				val = g_kMenuRoot->GetValue(kTileValue_width);
				*result = val->num;
				break;
			case 1:
				val = g_kMenuRoot->GetValue(kTileValue_height);
				*result = val->num;
				break;
			case 2:
				*result = g_screenWidth;
				break;
			case 3:
				*result = g_screenHeight;
				break;
			case 4:
				val = g_kMenuRoot->GetValue(kTileValue_resolutionconverter);
				*result = val->num;
				break;
			}
		}
	}
	return true;
}

//traits
//0 for Alpha
//1 for Width
//2 for Height

bool Cmd_GetCursorTrait_Execute(COMMAND_ARGS) ////////////Concept taken from JiP
{
	*result = 0;
	UInt32 iTrait = -1;
	Tile::Value* val = 0;
	const char* resStr = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &iTrait))
	{
		if (g_Cursor)
		{
			switch (iTrait)
			{
			case 0:
				val = g_Cursor->GetValue(kTileValue_alpha);
				*result = val->num;
				break;
			case 1:
				val = g_Cursor->GetValue(kTileValue_width);
				*result = val->num;
				break;
			case 2:
				val = g_Cursor->GetValue(kTileValue_height);
				*result = val->num;
				break;
			case 3:
				val = g_Cursor->GetValue(kTileValue_red);
				*result = val->num;
				break;
			case 4:
				val = g_Cursor->GetValue(kTileValue_green);
				*result = val->num;
				break;
			case 5:
				val = g_Cursor->GetValue(kTileValue_blue);
				*result = val->num;
				break;
			}
		}
	}
	return true;
}





//traits
//0 for Alpha
//1 for Width
//2 for Height
//3 for Red
//4 for Green
//5 for Blue


bool Cmd_SetCursorTrait_Execute(COMMAND_ARGS) ////////////Concept taken from JiP
{

	UInt32 iTrait;
	float fValue;
	Tile::Value* val = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &iTrait, &fValue))
	{
		if (g_Cursor)
		{
			switch (iTrait)
			{
			case 0:
				g_Cursor->SetFloat(kTileValue_alpha, fValue);
				break;
			case 1:
				g_Cursor->SetFloat(kTileValue_width, fValue);
				break;
			case 2:
				g_Cursor->SetFloat(kTileValue_height, fValue);
				break;
			case 3:
				g_Cursor->SetFloat(kTileValue_red, fValue);
				break;
			case 4:
				g_Cursor->SetFloat(kTileValue_green, fValue);
				break;
			case 5:
				g_Cursor->SetFloat(kTileValue_blue, fValue);
				break;

			}
		}
	}
	return true;
}




bool Cmd_SetCursorTraitGradual_Execute(COMMAND_ARGS) ////////////SetUIFloatGradual By JIP
{
	UInt32 iTrait,iMode, iNumArgs =NUM_ARGS;
	float fStartVal,fEndVal,fSec;
	Tile::Value* val = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX,&iTrait, &fStartVal, &fEndVal, &fSec, &iMode))
	{
		if (g_Cursor)
		{
			if (iNumArgs >1)
			{
				switch (iTrait)
				{
				case 0:
					g_Cursor->GradualSetFloat(kTileValue_alpha, fStartVal, fEndVal, fSec, iMode);
					break;
				case 1:
					g_Cursor->GradualSetFloat(kTileValue_width, fStartVal, fEndVal, fSec, iMode);
					break;
				case 2:
					g_Cursor->GradualSetFloat(kTileValue_height, fStartVal, fEndVal, fSec, iMode);
					break;
				case 3:
					g_Cursor->GradualSetFloat(kTileValue_red, fStartVal, fEndVal, fSec, iMode);
					break;
				case 4:
					g_Cursor->GradualSetFloat(kTileValue_green, fStartVal, fEndVal, fSec, iMode);
					break;
				case 5:
					g_Cursor->GradualSetFloat(kTileValue_blue, fStartVal, fEndVal, fSec, iMode);
					break;
				}
			}
			else {
				switch (iTrait)
				{
				case 0:
					ThisCall(0xA07DC0, g_Cursor, kTileValue_alpha);
					break;
				case 1:
					ThisCall(0xA07DC0, g_Cursor, kTileValue_width);
					break;
				case 2:
					ThisCall(0xA07DC0, g_Cursor, kTileValue_height);
					break;
				case 3:
					ThisCall(0xA07DC0, g_Cursor, kTileValue_red);
					break;
				case 4:
					ThisCall(0xA07DC0, g_Cursor, kTileValue_green);
					break;
				case 5:
					ThisCall(0xA07DC0, g_Cursor, kTileValue_blue);
					break;
				}
			}
		}
	}
	return true;
}



bool Cmd_GetUIValueType_Execute(COMMAND_ARGS)
{
	alignas(16) char s_strArgTemp2[0x4000];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgTemp2))
	{
		Tile::Value* tileVal = NULL;
		if (GetTargetComponent(s_strArgTemp2, &tileVal) && tileVal)
		{ 

			if (tileVal->num)
				*result = 1;
			else if (tileVal->str)
				*result = 2;
			else if (tileVal->action)
				*result = 3;
			else
				*result = 4;
		}
		else {*result = -1;}
	}
	return true;
}


bool f_DebugTextCreateSUPRect()
{
	Tile* component = GetTargetComponent("HUDMainMenu");
	if (component)
	{
		char s_ToInject[0x4000]{};
		sprintf(s_ToInject, "%s", "<rect name=\"SUPDebugText\"> <template name=\"SUPNVSETextTemplateDoNotUse\"> <rect name=\"TextTemplate\"> <text name=\"Text\"> <string></string> <font>1</font> <visible>1</visible>  <systemcolor>1</systemcolor>   </text>   </rect> </template>      </rect>");
		FileStream tempFile;
		if (tempFile.Create(kComponentTempXML))
		{
			tempFile.WriteStr(s_ToInject);
			tempFile.Close();
			component->ReadXML(kComponentTempXML) ? 1 : 0;
			//Console_Print("FIRST TIME CREATING");

			g_SUPRect = g_HUDMainMenu->tile->GetComponentTile("SUPDebugText");
			remove(kComponentTempXML);
			return true;
		}
	}

}


int f_DebugTextExists(char* s_textName)
{
	char s_ToCheck[0x4000]{};
	sprintf(s_ToCheck, "%s%s/%s", "HUDMainMenu/SUPDebugText/", s_textName, "Text");
	Tile* component = GetTargetComponent(s_ToCheck);
	if (component) {return 1; }
	else { return 0; }
}


bool Cmd_DebugTextCreate_Execute(COMMAND_ARGS)
{
	alignas(16) char s_textName[0x4000]{};
	char s_ToInject[0x4000]{};
	char s_StringName[0x4000]{};
	float fXPos = 0,fYPos = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_textName,&fXPos,&fYPos,&s_StringName))
	{

		Tile* component = GetTargetComponent("HUDMainMenu");
		TileMenu* menu = GetMenuTile("HUDMainMenu");

		if (component)
		{
			if (g_SUPRect){}
			else { f_DebugTextCreateSUPRect(); }

			if (g_SUPRect) {}
			else { 
				*result = -2;
				return true;
			}

			if (f_DebugTextExists(s_textName) == 1)
			{
				*result = -1;
				return true;
			}

			component = menu->menu->AddTileFromTemplate(g_SUPRect, "SUPNVSETextTemplateDoNotUse");

			if (component)
			{
				///Console_Print("NEW COMPONENT  EXISTS");
				component->name.Set(s_textName);
				component = component->GetComponentTile("Text");
				if (component)
				{
					//component->name.Set(s_textName);
					if (NUM_ARGS > 1 )
						component->SetFloat(kTileValue_x, fXPos);

					if (NUM_ARGS > 2)
						component->SetFloat(kTileValue_y, fYPos);

					if (NUM_ARGS > 3)
						component->SetString(kTileValue_string, s_StringName);

					*result = 1;
				}
				else {
					component->Destroy(true);
					*result = -3;
				}
			}
			else { *result = -4; }
		}
	}


	return true;
}



bool Cmd_DebugTextExists_Execute(COMMAND_ARGS)
{
	alignas(16) char s_textName[0x4000]{};

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_textName))
	{
		*result = f_DebugTextExists(s_textName);
	}
	return true;
}



bool Cmd_DebugTextSetString_Execute(COMMAND_ARGS)
{
	alignas(16) char s_textName[0x4000]{};
	char s_ToCheck[0x4000]{};
	char s_NewString[0x4000]{};
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_textName, &s_NewString))
	{
		sprintf(s_ToCheck, "%s%s/%s", "HUDMainMenu/SUPDebugText/", s_textName, "Text");
		Tile* component = GetTargetComponent(s_ToCheck);
		if (component)
		{component->SetString(kTileValue_string, s_NewString);}
	}
	return true;
}


bool Cmd_DebugTextDestroy_Execute(COMMAND_ARGS)
{
	alignas(16) char s_textName[0x4000]{};
	char s_ToCheck[0x4000]{};
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_textName))
	{
		sprintf(s_ToCheck, "%s%s", "HUDMainMenu/SUPDebugText/", s_textName);
		Tile* component = GetTargetComponent(s_ToCheck);
		if (component)
		{component->Destroy(true);}
	}
	return true;
}



bool Cmd_DebugTextSetPos_Execute(COMMAND_ARGS)
{
	alignas(16) char s_textName[0x4000]{};
	char s_ToCheck[0x4000]{};
	char s_NewString[0x4000]{};
	float fXPos, fYPos;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_textName, &fXPos, &fYPos))
	{
		sprintf(s_ToCheck, "%s%s/%s", "HUDMainMenu/SUPDebugText/", s_textName, "Text");
		Tile* component = GetTargetComponent(s_ToCheck);
		if (component)
		{
			component->SetFloat(kTileValue_x, fXPos);
			component->SetFloat(kTileValue_y, fYPos);
		}
	}
	return true;
}







//bool Cmd_SetTileValueAction_Execute(COMMAND_ARGS)
//{
//	alignas(16) char s_TileSource[0x4000];
//	alignas(16) char s_TileTarget[0x4000];
//	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_TileSource,&s_TileTarget))
//	{
//		Tile::Value* tileValSource = NULL;
//		Tile::Value* tileValTarget = NULL;
//		if (GetTargetComponent(s_TileSource, &tileValSource) && tileValSource)
//		{
//
//			if (tileValSource->action)
//			{
//
//
//				if (GetTargetComponent(s_TileTarget, &tileValTarget) && tileValTarget)
//				{
//					if (tileValTarget->action)
//					{
//						Console_Print("OLD SOURCE Action %08X", tileValSource->action);
//						Console_Print("OLD TARGET Action %08X", tileValTarget->action);
//						tileValTarget->action = tileValSource->action;
//						Console_Print("NEW TARGET Action %08X", tileValTarget->action);
//						*result = 1;
//					}
//
//				}
//
//			}
//
//		}
//		else { *result = -1; }
//	}
//	return true;
//}

//SetTileValueAction "HUDMainMenu\VRWM_HUD\VRWMTextModOff1\red" "HUDMainMenu\VRWM_HUD\VRWMTextModOff1\green"