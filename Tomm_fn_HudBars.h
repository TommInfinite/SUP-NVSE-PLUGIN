#pragma once
DEFINE_COMMAND_PLUGIN(HudBarCreate, "", 0, 4, kParams_Tomm_HudBarCreate)
DEFINE_COMMAND_PLUGIN(HudBarSetMeterSize, "", 0, 3, kParams_Tomm_HudBarSetImageSize)
DEFINE_COMMAND_PLUGIN(HudBarSetFrameSize, "", 0, 3, kParams_Tomm_HudBarSetImageSize)
DEFINE_COMMAND_PLUGIN(HudBarSetFrameImage, "", 0, 2, kParams_Tomm_HudBarSetString)
DEFINE_COMMAND_PLUGIN(HudBarSetMeterImage, "", 0, 2, kParams_Tomm_HudBarSetString)
DEFINE_COMMAND_PLUGIN(HudBarSetFrameVisible, "", 0, 2, kParams_Tomm_HudBarSetInt)
DEFINE_COMMAND_PLUGIN(HudBarSetMeterVisible, "", 0, 2, kParams_Tomm_HudBarSetInt)
DEFINE_COMMAND_PLUGIN(HudBarSetValueScriptVar, "", 0, 2, kParams_Tomm_HudBarSetScriptVariable)
DEFINE_COMMAND_PLUGIN(HudBarSetValueFloat, "", 0, 2, kParams_Tomm_HudBarSetFloatValue)
DEFINE_COMMAND_PLUGIN(HudBarSetValuePercentage, "", 0, 2, kParams_Tomm_HudBarSetFloatValue)
DEFINE_COMMAND_PLUGIN(HudBarSetValueMax, "", 0, 2, kParams_Tomm_HudBarSetFloatValue)
DEFINE_COMMAND_PLUGIN(HudBarShowBar, "", 0, 2, kParams_Tomm_HudBarSetInt)
DEFINE_COMMAND_PLUGIN(HudBarSetTextPrefix, "", 0, 2, kParams_Tomm_HudBarSetString)
DEFINE_COMMAND_PLUGIN(HudBarSetTextPostFix, "", 0, 2, kParams_Tomm_HudBarSetString)






//int f_Bars_BarExists(char* s_BarName)
//{
//	char s_ToCheck[0x4000]{};
//	sprintf(s_ToCheck, "%s%s/%s", "HUDMainMenu/SUPBars/", s_BarName, kHUDBarTileMeterName);
//	Tile* component = GetTargetComponent(s_ToCheck);
//	if (component) { return 1; }
//	else { return 0; }
//}

int f_Bars_BarGetKey(char* s_BarName)
{

	int iFound = 0;
	int i;
	for (i = 0; i < g_HudBarsArraySize; i++) {
		if (strcmp(g_HUDBArsArray[i].Name, s_BarName) == 0)
		{
			_MESSAGE("FOUND BAR,i>>> %d", i);
			iFound = 1;
			break;
		}

	}
	if (iFound == 1)
	{return i;}
	else {return -1;}



}



bool f_Bars_CreateTemplate()
{
	Tile* component = GetTargetComponent("HUDMainMenu");
	if (component)
	{
		char s_ToInject[0x4000]{};
		sprintf(s_ToInject, "%s", "<rect name=\"SUPBars\"> <template name=\"SUPNVSEBarsTemplateDoNotUse\"> <rect name=\"Bar\"> <visible>0</visible> <image name=\"MeterImage\"> <height>0</height> <width>0</width> <x>0</x> <y>0</y>  <filename>Interface\\Shared\\solid.dds</filename> <visible>1</visible> </image>      <image name=\"FrameImage\"> <height>0</height> <width>0</width> <x>0</x> <y>0</y>  <filename>Interface\\Shared\\solid.dds</filename> <visible>1</visible> </image>  <text name=\"MeterText\"> <string></string> <font>1</font> <visible>0</visible>  <systemcolor>1</systemcolor>   </text>  </rect> </template> </rect>");
		FileStream tempFile;
		if (tempFile.Create(kComponentTempBarsXML))
		{
			tempFile.WriteStr(s_ToInject);
			tempFile.Close();
			component->ReadXML(kComponentTempBarsXML) ? 1 : 0;
			//Console_Print("FIRST TIME CREATING");

			g_SUPRectBars = g_HUDMainMenu->tile->GetComponentTile("SUPBars");
			remove(kComponentTempBarsXML);
			return true;
		}
	}

}

bool f_Bars_AddNewElement(char* s_BarName, float fPosX, float fPosY, int iMeterType)
{
	if (g_SUPRectBars)
	{}else { f_Bars_CreateTemplate(); }

	if (g_SUPRectBars) {}
	else {return false;}



	if (f_Bars_BarGetKey(s_BarName) >= 0)
	{
		Console_Print("BAR EXISTS");
		return false;
	}



	Tile* component = GetTargetComponent("HUDMainMenu");
	TileMenu* menu = GetMenuTile("HUDMainMenu");


	component = menu->menu->AddTileFromTemplate(g_SUPRectBars, "SUPNVSEBarsTemplateDoNotUse");

	if (component)
	{
		component->name.Set(s_BarName);

		Tile* FrameImage = component->GetComponentTile("FrameImage");
		Tile* MeterImage = component->GetComponentTile("MeterImage");
		Tile* MeterText = component->GetComponentTile("MeterText");


		Console_Print("Adding meter type of %d ", iMeterType);

		HUDBarElement* HUDBAR = new HUDBarElement;
		
		HUDBAR->TileRect = component;
		sprintf(HUDBAR->Name, "%s", s_BarName);
		HUDBAR->MeterType = iMeterType;


		if (iMeterType < 3) // if meter type <3 - TileMeter is MeterImage
		{
			HUDBAR->TileMeter = MeterImage;
			MeterImage->SetFloat(kTileValue_x, fPosX);
			MeterImage->SetFloat(kTileValue_y, fPosY);
			FrameImage->SetFloat(kTileValue_x, fPosX);
			FrameImage->SetFloat(kTileValue_y, fPosY);
		}
		else // if meter type -4 - TileMeter is Text
		{
			HUDBAR->TileMeter = MeterText;
			MeterText->SetFloat(kTileValue_x, fPosX);
			MeterText->SetFloat(kTileValue_y, fPosY);
			MeterText->SetFloat(kTileValue_visible, 1);
		}
	

		g_HUDBArsArray[g_HudBarsArraySize] = *HUDBAR;
		g_HudBarsArraySize += 1;


	}


	return true;
}








bool Cmd_HudBarCreate_Execute(COMMAND_ARGS)
{
	float fPosX = 0,fPosY = 0;
	int iMeterType;
	char s_BarName[0x4000]{};
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName,&fPosX,&fPosY, &iMeterType)) return true;
	*result = f_Bars_AddNewElement(s_BarName, fPosX, fPosY, iMeterType);
	return true;
}


int f_Bars_BarSetMeterSize(char* s_BarName, float fWidth, float fHeight)
{
	int iKey;
	iKey = f_Bars_BarGetKey(s_BarName);
	if (iKey >= 0) {}
	else { return -1; }
	if (HudBarIterElement.MeterType == 4)
	{
		return -2;
	}

	HudBarIterElement.MeterWidth = fWidth;
	HudBarIterElement.MeterHeight = fHeight;
	HudBarIterElement.TileMeter->SetFloat(kTileValue_height, fHeight);
	HudBarIterElement.TileMeter->SetFloat(kTileValue_width, fWidth);
	return 1;
}


bool Cmd_HudBarSetMeterSize_Execute(COMMAND_ARGS)
{
	float fWidth = 0, fHeight = 0;
	char s_BarName[0x4000]{};
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &fWidth, &fHeight)) return true;
	*result = f_Bars_BarSetMeterSize(s_BarName, fWidth, fHeight);
	return true;
}


int f_Bars_BarSetFrameSize(char* s_BarName, float fWidth, float fHeight)
{
	int iKey;
	iKey = f_Bars_BarGetKey(s_BarName);
	if (iKey >= 0) {}
	else { return -1; }
	if (HudBarIterElement.MeterType == 4)
	{
		return -2;
	}

	HudBarIterElement.FrameWidth = fWidth;
	HudBarIterElement.FrameHeight = fHeight;
	HudBarIterElement.FrameImage->SetFloat(kTileValue_height, fHeight);
	HudBarIterElement.FrameImage->SetFloat(kTileValue_width, fWidth);
	return 1;
}


bool Cmd_HudBarSetFrameSize_Execute(COMMAND_ARGS)
{
	float fWidth = 0, fHeight = 0;
	char s_BarName[0x4000]{};
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &fWidth, &fHeight)) return true;
	*result = f_Bars_BarSetFrameSize(s_BarName, fWidth, fHeight);
	return true;
}


int f_Bars_BarSetValueScriptVar(char* s_BarName, ScriptVar* ScriptVariable)
{
	int iKey;
	iKey = f_Bars_BarGetKey(s_BarName);
	if (iKey >= 0) {}
	else { return -1; }
	HudBarIterElement.ScriptVariable = ScriptVariable;
	HudBarIterElement.ValueType = 0;
	return 1;
}

bool Cmd_HudBarSetValueScriptVar_Execute(COMMAND_ARGS)
{
	ScriptVar* ScriptVariable = NULL;
	char s_BarName[0x4000]{};
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &ScriptVariable)) return true;
	*result = f_Bars_BarSetValueScriptVar(s_BarName, ScriptVariable);
	return true;
}




int f_Bars_BarSetValueFloat(char* s_BarName, float fValue)
{
	int iKey;
	iKey = f_Bars_BarGetKey(s_BarName);
	if (iKey >= 0) {}
	else { return -1; }
	HudBarIterElement.Value = fValue;
	HudBarIterElement.ValueType = 1;
	return 1;
}

bool Cmd_HudBarSetValueFloat_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	float fValue;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &fValue)) return true;
	*result = f_Bars_BarSetValueFloat(s_BarName, fValue);
	return true;
}





int f_Bars_BarSetValuePercentage(char* s_BarName, float fValue)
{
	int iKey;
	iKey = f_Bars_BarGetKey(s_BarName);
	if (iKey >= 0) {}
	else { return -1; }
	HudBarIterElement.Value = fValue;
	HudBarIterElement.ValueType = 2;
	return 1;
}

bool Cmd_HudBarSetValuePercentage_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	float fValue;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &fValue)) return true;
	*result = f_Bars_BarSetValuePercentage(s_BarName, fValue);
	return true;
}


int f_Bars_BarSetValueMax(char* s_BarName, float fValue)
{
	int iKey;
	iKey = f_Bars_BarGetKey(s_BarName);
	if (iKey >= 0) {}
	else { return -1; }
	HudBarIterElement.MaxValue = fValue;
	return 1;
}

bool Cmd_HudBarSetValueMax_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	float fValue;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &fValue)) return true;
	*result = f_Bars_BarSetValueMax(s_BarName, fValue);
	return true;
}














int f_Bars_BarSetFrameImage(char* s_BarName, char* s_FileName)
{
	int iKey;
	iKey = f_Bars_BarGetKey(s_BarName);
	if (iKey >= 0) {}else { return -1; }
	HudBarIterElement.FrameImage->SetString(kTileValue_filename, s_FileName);
	return 1;
}


bool Cmd_HudBarSetFrameImage_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{}, s_FileName[0x4000]{};
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &s_FileName)) return true;
	*result = f_Bars_BarSetFrameImage(s_BarName, s_FileName);
	return true;
}




int f_Bars_BarSetMeterImage(char* s_BarName, char* s_FileName)
{
	int iKey;
	iKey = f_Bars_BarGetKey(s_BarName);
	if (iKey >= 0) {}
	else { return -1; }

	if (HudBarIterElement.MeterType == 4)
	{return -2;}
	HudBarIterElement.TileMeter->SetString(kTileValue_filename, s_FileName);
	return 1;
}


bool Cmd_HudBarSetMeterImage_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{}, s_FileName[0x4000]{};
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &s_FileName)) return true;
	*result = f_Bars_BarSetMeterImage(s_BarName, s_FileName);
	return true;
}




int f_Bars_BarSetFrameVisible(char* s_BarName, int iRequest)
{
	int iKey;
	iKey = f_Bars_BarGetKey(s_BarName);
	if (iKey >= 0) {}
	else { return -1; }

	HudBarIterElement.FrameImage->SetFloat(kTileValue_visible, iRequest);
	return 1;
}



bool Cmd_HudBarSetFrameVisible_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	int iRequest;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iRequest)) return true;
	*result = f_Bars_BarSetFrameVisible(s_BarName, iRequest);
	return true;
}



int f_Bars_BarSetMeterVisible(char* s_BarName, int iRequest)
{
	int iKey;
	iKey = f_Bars_BarGetKey(s_BarName);
	if (iKey >= 0) {}
	else { return -1; }

	HudBarIterElement.TileMeter->SetFloat(kTileValue_visible, iRequest);
	return 1;
}



bool Cmd_HudBarSetMeterVisible_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	int iRequest;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iRequest)) return true;
	*result = f_Bars_BarSetMeterVisible(s_BarName, iRequest);
	return true;
}






int f_Bars_BarShowBar(char* s_BarName, int iRequest)
{
	int iKey;
	iKey = f_Bars_BarGetKey(s_BarName);
	if (iKey >= 0) {}
	else { return -1; }
	HudBarIterElement.iBarVisible = iRequest;
	HudBarIterElement.TileRect->SetFloat(kTileValue_visible, iRequest);


	return 1;
}

bool Cmd_HudBarShowBar_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	int iRequest;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iRequest)) return true;
	*result = f_Bars_BarShowBar(s_BarName, iRequest);
	return true;
}



int f_Bars_BarSetTextPrefix(char* s_BarName, char* s_Name)
{
	int iKey;
	iKey = f_Bars_BarGetKey(s_BarName);
	if (iKey >= 0) {}
	else { return -1; }
	sprintf(HudBarIterElement.Prefix, "%s", s_Name);
	return 1;
}


bool Cmd_HudBarSetTextPrefix_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{}, s_Name[0x4000]{};
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &s_Name)) return true;
	*result = f_Bars_BarSetTextPrefix(s_BarName, s_Name);
	return true;
}


int f_Bars_BarSetTextPostFix(char* s_BarName, char* s_Name)
{
	int iKey;
	iKey = f_Bars_BarGetKey(s_BarName);
	if (iKey >= 0) {}
	else { return -1; }
	sprintf(HudBarIterElement.PostFix, "%s", s_Name);
	return 1;
}


bool Cmd_HudBarSetTextPostFix_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{}, s_Name[0x4000]{};
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &s_Name)) return true;
	*result = f_Bars_BarSetTextPostFix(s_BarName, s_Name);
	return true;
}