#pragma once
DEFINE_COMMAND_ALT_PLUGIN(HudBarCreate, HBCreate, "", 0, 4, kParams_Tomm_HudBarCreate)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetValueScriptVar, HBSetValueScriptVar, "", 0, 3, kParams_Tomm_HudBarSetScriptVariable)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetValueFloat, HBSetValueFloat, "", 0, 2, kParams_Tomm_HudBarSetFloatValue)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetValuePercentage, HBSetValuePercentage, "", 0, 2, kParams_Tomm_HudBarSetFloatValue)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetValueMax, HBSetValueMax, "", 0, 2, kParams_Tomm_HudBarSetFloatValue)
DEFINE_COMMAND_ALT_PLUGIN(HudBarExists, HBExists, "", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetAutoPos, HBSetAutoPos, "", 0, 2, kParams_Tomm_OneString_OneOptionalInt)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetTextString, HBSetTextString, "", 0, 3, kParams_Tomm_HudBarSetStringEx)
DEFINE_COMMAND_ALT_PLUGIN(HudBarDestroy, HBDestroy, "", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetBarTrait, HBSetBarTrait, "", 0, 3, kParams_Tomm_HudBarSetTraitInt)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetPos, HBSetPos, "", 0, 4, kParams_Tomm_HudBarSetTwoFloatValuesEx)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetVisible, HBSetVisible, "", 0, 3, kParams_Tomm_HudBarSetIntEx)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetTexture, HBSetTexture, "", 0, 4, kParams_Tomm_HudBarSetTexture)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetSize, HBSetSize, "", 0, 4, kParams_Tomm_HudBarSetTwoFloatValuesEx)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetMeterTrait, HBSetMeterTrait, "", 0, 3, kParams_Tomm_HudBarSetFloatValueEx)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetValueUDF, HBSetValueUDF, "", 0, 4, kParams_Tomm_HudBarSetValueUDF)
DEFINE_COMMAND_ALT_PLUGIN(HudBarShowBar, HBShowBar, "", 0, 2, kParams_Tomm_HudBarSetInt)
DEFINE_COMMAND_ALT_PLUGIN(HudBarGetAllBars, HBGetAllBars, "", 0, 1, kParams_Tomm_OneIntOptional)
DEFINE_COMMAND_ALT_PLUGIN(HudBarGetElementUIPath, HBGetElementUIPath, "", 0, 2, kParams_Tomm_HudBarGetInt)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetAlpha, HBSetAlpha, "", 0, 3, kParams_Tomm_HudBarSetFloatValueEx)
DEFINE_COMMAND_ALT_PLUGIN(HudBarDestroyAllBars, HBDestroyAllBars, "", 0, 0, NULL)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetColor, HBSetColor, "", 0, 5, kParams_Tomm_HudBarSetColor)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetSystemColor, HBSetSystemColor, "", 0, 3, kParams_Tomm_HudBarSetSystemColor)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetTextTrait, HBSetTextTrait, "", 0, 4, kParams_Tomm_HudBarSetTextTrait)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetSizeAlt, HBSetSizeAlt, "", 0, 4, kParams_Tomm_HudBarSetTwoFloatValuesEx)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetDepth, HBSetDepth, "", 0, 3, kParams_Tomm_HudBarSetFloatValueEx)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetZoom, HBSetZoom, "", 0, 3, kParams_Tomm_HudBarSetFloatValueEx)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetRotation, HBSetRotation, "", 0, 4, kParams_Tomm_HudBarSetIntFloatEx)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetItem, HBSetItem, "", 0, 2, kParams_Tomm_HudBarSetItem)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetFormList, HBSetFormList, "", 0, 2, kParams_Tomm_HudBarSetFormList)
DEFINE_COMMAND_ALT_PLUGIN(HudBarGetDebugInfo, HBGetDebugInfo, "", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_ALT_PLUGIN(HudBarGetDebugInfoAll, HBGetDebugInfoAll, "", 0, 0, NULL)
DEFINE_COMMAND_ALT_PLUGIN(HudBarGetBarTrait, HBGetBarTrait, "", 0, 3, kParams_Tomm_HudBarGetBarTrait)
DEFINE_COMMAND_ALT_PLUGIN(HudBarSetIndent, HBSetIndent, "", 0, 3, kParams_Tomm_HudBarSetFloatValueEx)





//int f_Bars_BarExists(char* s_BarName)
//{
//	char s_ToCheck[0x4000]{};
//	sprintf(s_ToCheck, "%s%s/%s", "HUDMainMenu/SUPBars/", s_BarName, kHUDBarTileMeterName);
//	Tile* component = GetTargetComponent(s_ToCheck);
//	if (component) { return 1; }
//	else { return 0; }
//}








float f_Bars_GetFontHeight(int iFontID) 
{
	if (iFontID && (iFontID <= 89) && (iFontID != 9))
	{}else { return 0; }

	FontInfoJIP* fontInfo = g_fontManager->fontInfos[iFontID - 1];

	if (fontInfo)
	{
		//return fontInfo->bufferData->baseHeight;
		return fontInfo->bufferData->lineHeight;
	}
}

float f_Bars_GetTextureWidth(char* s_FileName) // Thanks C6 
{
	char sPath[0x4000]{};
	sprintf(sPath, "%s%s", "Data\\textures\\", s_FileName);

	FileStream sourceFile;
	if (sourceFile.OpenAt(sPath, 0x10))
	{
		DWORD width;
		sourceFile.ReadBuf(&width, 4);
		float fResult = width;
		_MESSAGE("READING WIDTH of %s>>>>>%f", sPath, fResult);
		return width;
	}

}


float f_Bars_GetTextureHeight(char* s_FileName) // Thanks C6 
{
	char sPath[0x4000]{};
	sprintf(sPath, "%s%s", "Data\\textures\\", s_FileName);

	FileStream sourceFile;
	if (sourceFile.OpenAt(sPath, 0x0C))
	{
		DWORD height;
		sourceFile.ReadBuf(&height, 4);
		float fResult = height;
		_MESSAGE("READING HEIGHT of %s>>>>>%f", sPath, fResult);
		return height;
	}

}







vector<HUDBarElement>::iterator f_Bars_BarGetIter(char* s_BarName, UInt8 modIdx)
{
	int iFound = 0;
	int i;
	vector<HUDBarElement>::iterator itRes;

	for (vector<HUDBarElement>::iterator it =
		g_HUDBArsArrayV.begin(); it != g_HUDBArsArrayV.end(); ++it)
	{
		if (HBIterV.modIdx == modIdx && (0 == stricmp(HBIterV.Name, s_BarName)))//VectorIter.Name == s_BarName)
		{
			itRes = it;
			iFound = 1;
			break;
		}
	}

	if (iFound == 1)
	{
		return itRes;
	}
	else { return itRes; }
}

int f_Bars_BarGetKey(char* s_BarName, UInt8 modIdx)
{
	int iFound = 0;
	int i;
	int iIndex;


	if (modIdx == 255)
	{
		for (std::vector<HUDBarElement>::iterator it =
			g_HUDBArsArrayV.begin(); it != g_HUDBArsArrayV.end(); ++it)
		{
			if (0 == stricmp(HBIterV.Name, s_BarName))
			{
				iFound = 1;
				iIndex = it - g_HUDBArsArrayV.begin();
				break;
			}
		}


	}
	else {

		for (std::vector<HUDBarElement>::iterator it =
			g_HUDBArsArrayV.begin(); it != g_HUDBArsArrayV.end(); ++it)
		{
			if (HBIterV.modIdx == modIdx && (0 == stricmp(HBIterV.Name, s_BarName)))//VectorIter.Name == s_BarName)
			{
				iFound = 1;
				iIndex = it - g_HUDBArsArrayV.begin();
				break;
			}
		}


	}




	if (iFound == 1)
	{
		return iIndex;
	}
	else { return -1; }
}

int f_Bars_BarGetKeyAlt(char* s_BarName, UInt8 modIdx)
{
	int iFound = 0;
	int i;
	int iIndex;
	for (std::vector<HUDBarElement>::iterator it =
		g_HUDBArsArrayV.begin(); it != g_HUDBArsArrayV.end(); ++it)
	{
		if (0 == stricmp(HBIterV.Name, s_BarName))//VectorIter.Name == s_BarName)
		{
			iFound = 1;
			iIndex = it - g_HUDBArsArrayV.begin();
			break;
		}
	}

	if (iFound == 1)
	{
		return iIndex;
	}
	else { return -1; }
}

int f_Bars_BarSetSize(char* s_BarName, int iElement, float fWidth, float fHeight, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iElement) {
	case 0:
		HBIter.MeterWidth = fWidth;
		HBIter.MeterHeight = fHeight;
		HBIter.MeterWidthAlt = fWidth;
		HBIter.MeterHeightAlt = fHeight;
		HBIter.TileMeter->SetFloat(kTileValue_height, fHeight);
		HBIter.TileMeter->SetFloat(kTileValue_width, fWidth);
		break;
	case 1:
		HBIter.FrameWidth = fWidth;
		HBIter.FrameHeight = fHeight;
		HBIter.FrameWidthAlt = fWidth;
		HBIter.FrameHeightAlt = fHeight;
		HBIter.TileFrame->SetFloat(kTileValue_height, fHeight);
		HBIter.TileFrame->SetFloat(kTileValue_width, fWidth);
		break;
	case 3:
		HBIter.ImageExWidth = fWidth;
		HBIter.ImageExHeight = fHeight;
		HBIter.ImageExWidthAlt = fWidth;
		HBIter.ImageExHeightAlt = fHeight;
		HBIter.TileImageEx->SetFloat(kTileValue_height, fHeight);
		HBIter.TileImageEx->SetFloat(kTileValue_width, fWidth);
		break;
	}

	return 1;
}












bool f_Bars_CreateTemplate()
{
	Tile* component = GetTargetComponent("HUDMainMenu");
	if (component)
	{
		char s_ToInject[0x4000]{};
		sprintf(s_ToInject, "%s", "<rect name=\"SUPBars\"> <template name=\"SUPNVSEBarsTemplateDoNotUse\"> <rect name=\"Bar\"> 	<locus> &true; </locus> <visible>0</visible> <image name=\"MeterImage\"> <depth>400</depth> <zoom>0</zoom> <height>0</height> <width>0</width> <x>0</x> <y>0</y> <systemcolor>1</systemcolor> <red>255</red> <green>255</green> <blue>255</blue> <filename>Interface\\Shared\\solid.dds</filename> <visible>1</visible> </image>      <image name=\"FrameImage\"> <depth>400</depth> <height>0</height> <width>0</width> <x>0</x> <y>0</y> <systemcolor>1</systemcolor> <red>255</red> <green>255</green> <blue>255</blue>  <filename>Interface\\Shared\\solid.dds</filename> <visible>1</visible> </image>  <image name=\"ImageEX\"> <depth>400</depth> <height>0</height> <width>0</width> <x>0</x> <y>0</y> <systemcolor>1</systemcolor> <red>255</red> <green>255</green> <blue>255</blue>  <filename>Interface\\Shared\\solid.dds</filename> <visible>1</visible> </image>  <text name=\"MeterText\"> <depth>400</depth> <string></string> <font>1</font> <visible>0</visible>  <systemcolor>1</systemcolor> <red>255</red> <green>255</green> <blue>255</blue>  </text>  </rect> </template> </rect>");
		FileStream tempFile;
		if (tempFile.Create(kComponentTempBarsXML))
		{
			tempFile.WriteStr(s_ToInject);
			tempFile.Close();
			component->ReadXML(kComponentTempBarsXML) ? 1 : 0;
			_MESSAGE("SUP BARS::Creating rect");

			g_SUPRectBars = g_HUDMainMenu->tile->GetComponentTile("SUPBars");
			remove(kComponentTempBarsXML);
			return true;
		}
	}

}

bool f_Bars_AddNewElement(char* s_BarName, float fPosX, float fPosY, int iMeterType, UInt8 modIdx)
{
	if (g_SUPRectBars)
	{}
	else { f_Bars_CreateTemplate(); }

	if (g_SUPRectBars) {}
	else { return false; }


	if (f_Bars_BarGetKey(s_BarName, modIdx) >= 0)
	{
		_MESSAGE("BAR %s already exists", s_BarName);
		return false;
	}

	if (g_TimerStarted == 0)
	{
		g_TimerStarted = 1;
		g_timer.start();
		g_TimeElapsed = g_timer.get_elapsed_ms();
	}


	Tile* component = GetTargetComponent("HUDMainMenu");
	TileMenu* menu = GetMenuTile("HUDMainMenu");


	component = menu->menu->AddTileFromTemplate(g_SUPRectBars, "SUPNVSEBarsTemplateDoNotUse");

	if (component)
	{


		HUDBarElement HUDBAR;
		sprintf(HUDBAR.Name, "%s", s_BarName);

		sprintf(s_BarName, "%s_%d", s_BarName, modIdx);
		component->name.Set(s_BarName);

		Tile* TileFrame = component->GetComponentTile("FrameImage");
		Tile* TileMeter = component->GetComponentTile("MeterImage");
		Tile* TileText = component->GetComponentTile("MeterText");
		Tile* TileImageEX = component->GetComponentTile("ImageEX");


		_MESSAGE("Adding meter type of %d ", iMeterType);


		HUDBAR.TileRect = component;
		HUDBAR.TileFrame = TileFrame;
		HUDBAR.TileMeter = TileMeter;
		HUDBAR.TileText = TileText;
		HUDBAR.TileImageEx = TileImageEX;

		HUDBAR.modIdx = modIdx;



		HUDBAR.MeterType = iMeterType;

		component->SetFloat(kTileValue_x, fPosX);
		component->SetFloat(kTileValue_y, fPosY);

		HUDBAR.fRectPosX = fPosX;
		HUDBAR.fRectPosY = fPosY;



		if (iMeterType < 3) // if meter type <3 - TileMeter is MeterImage
		{
			HUDBAR.TileMeterAdded = 1;
		}
		else // if meter type -4 - TileMeter is Text
		{
			HUDBAR.TileTextAdded = 1;
			TileText->SetFloat(kTileValue_visible, 1);
		}

		HUDBAR.ID = g_HUDBArsArrayID;
		g_HUDBArsArrayID += 1;


		g_HUDBArsArrayV.push_back(HUDBAR);
		g_HudBarsIterate = 1;


		_MESSAGE("Creating bar named::->>%s, ID::%d modIdx::%d modName::%s", HUDBAR.Name, HUDBAR.ID, HUDBAR.modIdx, GetNthModNameAlt(modIdx));
		




	}


	return true;
}





bool Cmd_HudBarCreate_Execute(COMMAND_ARGS)
{
	float fPosX = 0,fPosY = 0;
	int iMeterType;
	char s_BarName[0x4000]{};
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName,&fPosX,&fPosY, &iMeterType)) return true;
	*result = f_Bars_AddNewElement(s_BarName, fPosX, fPosY, iMeterType, modIdx);
	return true;
}





int f_Bars_BarSetValueScriptVar(char* s_BarName, ScriptVar* ScriptVariable,int iScriptValueCalculateMax, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	HBIter.iScriptValueCalculateMax = iScriptValueCalculateMax;
	HBIter.ScriptVariable = ScriptVariable;
	HBIter.ValueType = k_HBValueTypeScriptVar;
	return 1;
}

bool Cmd_HudBarSetValueScriptVar_Execute(COMMAND_ARGS)
{
	ScriptVar* ScriptVariable = NULL;
	int iScriptValueCalculateMax = 0;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	char s_BarName[0x4000]{};
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &ScriptVariable, &iScriptValueCalculateMax)) return true;
	*result = f_Bars_BarSetValueScriptVar(s_BarName, ScriptVariable, iScriptValueCalculateMax, modIdx);
	return true;
}




bool Cmd_HudBarSetValueFloat_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	float fValue;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &fValue)) return true;

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	if (HBIter.MeterType == k_HBMeterTypeText){}
	else {
		fValue = fValue / HBIter.MaxValue;
		if (fValue > 1)
			fValue = 1;
		else if (fValue < 0)
			fValue = 0;
	}

	HBIter.ValueType = k_HBValueTypeNoScriptVar;
	*result = f_Bars_BarSetBarValue(iKey, fValue);
	return true;
}





bool Cmd_HudBarSetValuePercentage_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	float fValue;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &fValue)) return true;

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	if (HBIter.MeterType == k_HBMeterTypeText)
		return -2;

	if (fValue > 1)
		fValue = 1;
	else if (fValue < 0)
		fValue = 0;


	HBIter.ValueType = k_HBValueTypeNoScriptVar;
	*result = f_Bars_BarSetBarValue(iKey, fValue);
	return true;
}


int f_Bars_BarSetValueMax(char* s_BarName, float fValue, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }
	HBIter.MaxValue = fValue;
	return 1;
}

bool Cmd_HudBarSetValueMax_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	float fValue;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &fValue)) return true;

	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	*result = f_Bars_BarSetValueMax(s_BarName, fValue, modIdx);
	return true;
}


bool Cmd_HudBarExists_Execute(COMMAND_ARGS)
{

	char s_BarName[0x4000]{};
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName)) return true;

	UInt8 modIdx = scriptObj->GetOverridingModIdx();

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) { *result = 1; }
	else { *result = 0; }

	return true;
}


//request
//0 - text on the right
//1 text below
//2 text above
int f_Bars_BarSetAutoPos(char* s_BarName, int iRequest, UInt8 modIdx)
{
	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	float fCalculatedX = 0, fCalculatedY = 0;
	float fFontHeight = 0;

	if (HBIter.MeterType < k_HBMeterTypeText)
	{
		if (HBIter.TileTextAdded == 1)
		{
			switch (iRequest) {
			case 0:
				fFontHeight = (f_Bars_GetFontHeight(HBIter.TileText->GetValueFloat(kTileValue_font))) / 2;

				if (HBIter.TileFrameAdded)
				{
					fCalculatedX = (HBIter.IndentFrame + HBIter.FrameWidthAlt) + 1;
					fCalculatedY = ((HBIter.FrameHeightAlt) / 2) - fFontHeight;
				}
				else
				{
					fCalculatedX = (HBIter.IndentMeter + HBIter.MeterWidthAlt) + 1;
					fCalculatedY = ((HBIter.MeterHeightAlt) / 2) - fFontHeight;
				}
				HBIter.TileText->SetFloat(kTileValue_x, fCalculatedX);
				HBIter.TileText->SetFloat(kTileValue_y, fCalculatedY);
				break;

			case 1:

				if (HBIter.TileFrameAdded)
				{
					HBIter.TileText->SetFloat(kTileValue_x, HBIter.IndentFrame);
					HBIter.TileText->SetFloat(kTileValue_y, HBIter.FrameHeightAlt + 7);
				}
				else
				{
					HBIter.TileText->SetFloat(kTileValue_x, HBIter.IndentMeter);
					HBIter.TileText->SetFloat(kTileValue_y, HBIter.MeterHeightAlt + 7);

				}
				break;

			case 2:
				fFontHeight = (f_Bars_GetFontHeight(HBIter.TileText->GetValueFloat(kTileValue_font)));
				fFontHeight *= -1;
				if (HBIter.TileFrameAdded)
				{
					HBIter.TileText->SetFloat(kTileValue_x, HBIter.IndentFrame);
					HBIter.TileText->SetFloat(kTileValue_y, fFontHeight - 1);
				}
				else
				{
					HBIter.TileText->SetFloat(kTileValue_x, HBIter.IndentMeter);
					HBIter.TileText->SetFloat(kTileValue_y, fFontHeight - 1);

				}
				break;


			}



		}

	}else if(HBIter.MeterType >= k_HBMeterTypeText)
	{

		switch (iRequest) {
		case 0:
			if (HBIter.TileFrameAdded == 1)
			{
				fCalculatedX = (HBIter.IndentFrame + HBIter.FrameWidthAlt) + 1;
				HBIter.TileText->SetFloat(kTileValue_x, fCalculatedX);

				float fFontHeight = (f_Bars_GetFontHeight(HBIter.TileText->GetValueFloat(kTileValue_font))) / 2;
				fCalculatedY = ((HBIter.FrameHeightAlt) / 2) - fFontHeight;
				HBIter.TileText->SetFloat(kTileValue_y, fCalculatedY);
			}
			break;
		case 1:
			if (HBIter.TileFrameAdded == 1)
			{
				HBIter.TileText->SetFloat(kTileValue_x, HBIter.IndentFrame);
				HBIter.TileText->SetFloat(kTileValue_y, HBIter.FrameHeightAlt + 1);
			}
			break;

		case 2:
			if (HBIter.TileFrameAdded == 1)
			{
				float fFontHeight = (f_Bars_GetFontHeight(HBIter.TileText->GetValueFloat(kTileValue_font))) / 2;
				fFontHeight *= -1;
				HBIter.TileText->SetFloat(kTileValue_x, HBIter.IndentFrame);
				HBIter.TileText->SetFloat(kTileValue_y, fFontHeight - 1);
			}
			break;



		}




	}


	return 1;
}




bool Cmd_HudBarSetAutoPos_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	int iRequest = 0;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iRequest)) return true;
	*result = f_Bars_BarSetAutoPos(s_BarName, iRequest, modIdx);
	return true;
}



bool Cmd_HudBarDestroy_Execute(COMMAND_ARGS)
{

	char s_BarName[0x4000]{};
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName)) return true;

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	f_Bars_BarDestroy(f_Bars_BarGetIter(s_BarName, modIdx));
	return true;
}



int f_Bars_BarSetBarTrait(char* s_BarName, int iTrait, float fValue, UInt8 modIdx)
{

	int iRound;

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iTrait) {
		case 0:
			iRound = fValue;
			HBIter.iMenuTypeShow = iRound;
			break;
		case 1:
			iRound = fValue;
			HBIter.iRemoveOnGameLoad = iRound;
			break;
		case 2:
			HBIter.fUpdateTimer = fValue;
			break;

		case 21:
			iRound = fValue;
			HBIter.iDisableControlMovement = iRound;
			break;
		case 22:
			iRound = fValue;
			HBIter.iDisableWhenSneaking = iRound;
			break;
		case 23:
			iRound = fValue;
			HBIter.iDisableWhenCombat = iRound;
			break;
		case 24:
			iRound = fValue;
			HBIter.iDisableWhenAiming = iRound;
			break;
		case 25:
			iRound = fValue;
			HBIter.iDisableWhenNotEquipped = iRound;
			break;
		case 26:
			iRound = fValue;
			HBIter.iDisableWhenWeaponOut = iRound;
			break;



			

		case 41:
			iRound = fValue;
			HBIter.iGradualVanishing = iRound;
			break;
		case 42:
			iRound = fValue;
			HBIter.iGradualVanishingMovement = iRound; //;1 Horizontal ;2 Vertical
			break;
		case 43:
			HBIter.fGradualVanishingPosShift = fValue;
			break;
		case 44:
			HBIter.fGradualVanishingTimer = fValue;
			break;


		case 61:
			iRound = fValue;
			HBIter.iGradualAppearing = iRound;
			break;
		case 62:
			iRound = fValue;
			HBIter.iGradualAppearingMovement = iRound; //;1 Horizontal ;2 Vertical
			break;
		case 63:
			HBIter.fGradualAppearingPosShift = fValue;
			break;
		case 64:
			HBIter.fGradualAppearingTimer = fValue;
			break;
	}

	return 1;
}



bool Cmd_HudBarSetBarTrait_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	int  iTrait = 0;
	float fValue;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName,&iTrait, &fValue)) return true;
	*result = f_Bars_BarSetBarTrait(s_BarName, iTrait, fValue, modIdx);
	return true;
}


//0 for Meter
//1 for Frame
//2 for Text
//10 For Bar
int f_Bars_BarSetPos(char* s_BarName, int iElement, float fPosX, float fPosY, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iElement) {
	case 0:
		HBIter.TileMeter->SetFloat(kTileValue_x, fPosX);
		HBIter.TileMeter->SetFloat(kTileValue_y, fPosY);
		break;
	case 1:
		HBIter.TileFrame->SetFloat(kTileValue_x, fPosX);
		HBIter.TileFrame->SetFloat(kTileValue_y, fPosY);
		break;
	case 2:
		HBIter.TileText->SetFloat(kTileValue_x, fPosX);
		HBIter.TileText->SetFloat(kTileValue_y, fPosY);
		break;
	case 3:
		HBIter.TileImageEx->SetFloat(kTileValue_x, fPosX);
		HBIter.TileImageEx->SetFloat(kTileValue_y, fPosY);
		break;


	case 10:
		HBIter.TileRect->SetFloat(kTileValue_x, fPosX);
		HBIter.TileRect->SetFloat(kTileValue_y, fPosY);
		HBIter.fRectPosX = fPosX;
		HBIter.fRectPosY = fPosY;

		break;
	}

	return 1;
}



bool Cmd_HudBarSetPos_Execute(COMMAND_ARGS)
{

	char s_BarName[0x4000]{};
	float fPosX, fPosY;
	int iElement;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iElement, &fPosX, &fPosY)) return true;
	*result = f_Bars_BarSetPos(s_BarName, iElement, fPosX, fPosY, modIdx);
	return true;
}






int f_Bars_BarSetVisible(char* s_BarName, int iElement, int iValue, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iElement) {
	case 0:
		HBIter.TileMeter->SetFloat(kTileValue_visible, iValue);
		break;
	case 1:
		HBIter.TileFrame->SetFloat(kTileValue_visible, iValue);
		break;
	case 2:
		HBIter.TileText->SetFloat(kTileValue_visible, iValue);
		break;
	case 3:
		HBIter.TileImageEx->SetFloat(kTileValue_visible, iValue);
		break;


	case 10:
		HBIter.TileFrame->SetFloat(kTileValue_visible, iValue);
		HBIter.TileMeter->SetFloat(kTileValue_visible, iValue);
		HBIter.TileText->SetFloat(kTileValue_visible, iValue);
		HBIter.TileImageEx->SetFloat(kTileValue_visible, iValue);
		break;
	}

	return 1;
}

bool Cmd_HudBarSetVisible_Execute(COMMAND_ARGS)
{

	char s_BarName[0x4000]{};
	int iElement = 0, iValue = 0;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iElement, &iValue)) return true;
	*result = f_Bars_BarSetVisible(s_BarName, iElement, iValue, modIdx);
	return true;
}


//0 For Meter
//1 For Frame
//3 for ImageEx
int f_Bars_BarSetTexture(char* s_BarName, int iElement, char* s_FileName, int iAutomaticSize, UInt8 modIdx)
{

	float fWidth = 0, fHeight = 0;
	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }


	switch (iElement) {
	case 0:
		HBIter.TileMeter->SetString(kTileValue_filename, s_FileName);
		HBIter.TileMeterAdded = 1;

		if (iAutomaticSize)
		{
			fWidth = f_Bars_GetTextureWidth(s_FileName);
			fHeight = f_Bars_GetTextureHeight(s_FileName);
			f_Bars_BarSetSize(s_BarName, iElement, fWidth, fHeight, modIdx);
		}
		break;
	case 1:
		HBIter.TileFrame->SetString(kTileValue_filename, s_FileName);
		HBIter.TileFrameAdded = 1;

		if (iAutomaticSize)
		{
			fWidth = f_Bars_GetTextureWidth(s_FileName);
			fHeight = f_Bars_GetTextureHeight(s_FileName);
			f_Bars_BarSetSize(s_BarName, iElement, fWidth, fHeight, modIdx);
		}
		break;
	case 3:
		HBIter.TileImageEx->SetString(kTileValue_filename, s_FileName);
		HBIter.TileImageExAdded = 1;

		if (iAutomaticSize)
		{
			fWidth = f_Bars_GetTextureWidth(s_FileName);
			fHeight = f_Bars_GetTextureHeight(s_FileName);
			f_Bars_BarSetSize(s_BarName, iElement, fWidth, fHeight, modIdx);
		}
		break;

	}

	return 1;
}


bool Cmd_HudBarSetTexture_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{}, s_FileName[0x4000]{};
	int iElement = 0;
	int iAutomaticSize = 0;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName,&iElement,&s_FileName, &iAutomaticSize)) return true;
	*result = f_Bars_BarSetTexture(s_BarName, iElement, s_FileName, iAutomaticSize, modIdx);
	return true;
}



//0 for String Value
//1 for Prefix
//2 for PostFix
int f_Bars_BarSetTextString(char* s_BarName,int iElement, char* s_Name, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }


	switch (iElement) {
	case 0:
		HBIter.TileText->SetString(kTileValue_string, s_Name);
		HBIter.TileText->SetFloat(kTileValue_visible, 1);
		HBIter.TileTextAdded = 1;
		break;
	case 1:
		sprintf(HBIter.Prefix, "%s", s_Name);
		break;
	case 2:
		sprintf(HBIter.PostFix, "%s", s_Name);
		break;

	}

	return 1;
}


bool Cmd_HudBarSetTextString_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{}, s_Name[0x4000]{};
	int iElement = 0;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName,&iElement, &s_Name)) return true;
	*result = f_Bars_BarSetTextString(s_BarName, iElement, s_Name, modIdx);
	return true;
}


//0 For Meter
//1 For Frame
//3 For ImageEx






bool Cmd_HudBarSetSize_Execute(COMMAND_ARGS)
{
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	char s_BarName[0x4000]{};
	float fWidth = 0, fHeight = 0;
	int iElement =0;

	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iElement, &fWidth, &fHeight)) return true;
	*result = f_Bars_BarSetSize(s_BarName, iElement, fWidth, fHeight, modIdx);
	return true;
}




//0 for AlphaMinValue
//1 for AlphaMaxValue
//2 for Show Max Value
int f_Bars_BarSetMeterTrait(char* s_BarName, int iTrait, float fValue, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }
	
	switch (iTrait) {
	case 0:
		HBIter.fAlphaMin = fValue;
		HBIter.fAlphaDifference = HBIter.fAlphaMax - HBIter.fAlphaMin;
		break;
	case 1:
		HBIter.fAlphaMax = fValue;
		HBIter.fAlphaDifference = HBIter.fAlphaMax - HBIter.fAlphaMin;
		break;
	case 2:
		HBIter.iShowMaxValue = fValue;
		break;
	}

	return 1;
}

bool Cmd_HudBarSetMeterTrait_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	int iTrait = 0;
	float fValue = 0;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iTrait, &fValue)) return true;
	*result = f_Bars_BarSetMeterTrait(s_BarName, iTrait, fValue, modIdx);
	return true;
}



int f_Bars_BarSetValueUDF(char* s_BarName, Script* FunctionToCall, int iScriptValueCalculateMax, TESObjectREFR* FunctionCaller,  UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	HBIter.FunctionToCall = FunctionToCall;
	HBIter.iScriptValueCalculateMax = iScriptValueCalculateMax;

	if (FunctionCaller)
	{HBIter.FunctionCaller = FunctionCaller;}


	HBIter.ValueType = k_HBValueTypeScriptFunction;
	return 1;
}




bool Cmd_HudBarSetValueUDF_Execute(COMMAND_ARGS)
{
	Script* FunctionToCall;
	TESObjectREFR* FunctionCaller;
	NVSEArrayElement* element;
	char s_BarName[0x4000]{};
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	int iScriptValueCalculateMax = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &FunctionToCall, &iScriptValueCalculateMax, &FunctionCaller)) return true; // From JIP

	*result = f_Bars_BarSetValueUDF(s_BarName, FunctionToCall, iScriptValueCalculateMax, FunctionCaller, modIdx);

	return true;
}



int f_Bars_BarShowBar(char* s_BarName, int iValue, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }


	HBIter.iBarVisible = iValue;
	HBIter.TileRect->SetFloat(kTileValue_visible, iValue);
	return 1;
}


bool Cmd_HudBarShowBar_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	int iValue;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iValue)) return true;

	*result = f_Bars_BarShowBar(s_BarName, iValue, modIdx);
	return true;
}


bool Cmd_HudBarGetAllBars_Execute(COMMAND_ARGS)
{
	int iValue = 0;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();

	NVSEArrayVar* Ar_ModNames;
	NVSEArrayVar* Ar_BarNames;
	NVSEArrayVar* Ar_ModIndexes;

	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &iValue)) return true;

	NVSEArrayVar* Ar_Result = ArrIfc->CreateArray(NULL, 0, scriptObj);

	if (iValue == 1)
	{
		Ar_ModNames = ArrIfc->CreateArray(NULL, 0, scriptObj);
		Ar_BarNames = ArrIfc->CreateArray(NULL, 0, scriptObj);
		Ar_ModIndexes = ArrIfc->CreateArray(NULL, 0, scriptObj);
	}

	vector<HUDBarElement>::iterator it;
	int index = 0;
	for (it = g_HUDBArsArrayV.begin(); it != g_HUDBArsArrayV.end();)
	{
		if (iValue == 1)
		{
			ArrIfc->AppendElement(Ar_ModNames, NVSEArrayElement(GetNthModNameAlt(HBIterV.modIdx)));
			ArrIfc->AppendElement(Ar_BarNames, NVSEArrayElement(HBIterV.Name));
			ArrIfc->AppendElement(Ar_ModIndexes, NVSEArrayElement(HBIterV.modIdx));
		}
		else
		{
			if (HBIterV.modIdx == modIdx)
			{
				ArrIfc->AppendElement(Ar_Result, NVSEArrayElement(HBIterV.Name));
			}
		}
		++it;
	}

	if (iValue == 1)
	{
		ArrIfc->AppendElement(Ar_Result, NVSEArrayElement(Ar_BarNames));
		ArrIfc->AppendElement(Ar_Result, NVSEArrayElement(Ar_ModNames));
		ArrIfc->AppendElement(Ar_Result, NVSEArrayElement(Ar_ModIndexes));
	}

	ArrIfc->AssignCommandResult(Ar_Result, result);
	return true;
}



//0 for Meter
//1 for Frame
//2 for Text
//3 for ImageEx
//10 For Bar
bool Cmd_HudBarGetElementUIPath_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	char s_Result[0x4000]{};
	int iElement = 0;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iElement)) return true;

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iElement) {
	case 0:
		HBIter.TileMeter->GetComponentFullName(s_Result);
		break;
	case 1:
		HBIter.TileFrame->GetComponentFullName(s_Result);
		break;
	case 2:
		HBIter.TileText->GetComponentFullName(s_Result);
		break;
	case 3:
		HBIter.TileImageEx->GetComponentFullName(s_Result);
		break;
	case 10:
		HBIter.TileRect->GetComponentFullName(s_Result);
		break;
	}

	StrIfc->Assign(PASS_COMMAND_ARGS, s_Result);


	return true;
}

//0 for Meter
//1 for Frame
//2 for Text
//3 for ImageEx

int f_Bars_BarSetAlpha(char* s_BarName, int iTrait, float fValue, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iTrait) {
	case 0:
		HBIter.TileMeter->SetFloat(kTileValue_alpha, fValue);
		break;
	case 1:
		HBIter.TileFrame->SetFloat(kTileValue_alpha, fValue);
		break;
	case 2:
		HBIter.TileText->SetFloat(kTileValue_alpha, fValue);
		break;
	case 3:
		HBIter.TileImageEx->SetFloat(kTileValue_alpha, fValue);
		break;
	case 10:
		HBIter.TileMeter->SetFloat(kTileValue_alpha, fValue);
		HBIter.TileFrame->SetFloat(kTileValue_alpha, fValue);
		HBIter.TileText->SetFloat(kTileValue_alpha, fValue);
		HBIter.TileImageEx->SetFloat(kTileValue_alpha, fValue);
		break;
	}

	return 1;
}


bool Cmd_HudBarSetAlpha_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	int iTrait = 0;
	float fValue;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iTrait, &fValue)) return true;
	*result = f_Bars_BarSetAlpha(s_BarName, iTrait, fValue, modIdx);
	return true;
}




bool Cmd_HudBarDestroyAllBars_Execute(COMMAND_ARGS)
{

	UInt8 modIdx = scriptObj->GetOverridingModIdx();


		vector<HUDBarElement>::iterator it;
		for (it = g_HUDBArsArrayV.begin(); it != g_HUDBArsArrayV.end();)
		{
			if (HBIterV.modIdx == modIdx)
			{
				it = f_Bars_BarDestroy(it);
			}
			else
			{
				++it;
			}
		}


	if (g_HUDBArsArrayV.size() == 0)
	{
		g_HudBarsIterate = 0;
	}

	return true;
}








//0 for Meter
//1 for Frame
//2 for Text
//3 for ImageEx
//10 for ALL
int f_Bars_BarSetColor(char* s_BarName, int iElement, float fRed, float fGreen, float fBlue, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iElement) {
	case 0:
		HBIter.TileMeter->SetFloat(kTileValue_red, fRed);
		HBIter.TileMeter->SetFloat(kTileValue_green, fGreen);
		HBIter.TileMeter->SetFloat(kTileValue_blue, fBlue);
		break;
	case 1:
		HBIter.TileFrame->SetFloat(kTileValue_red, fRed);
		HBIter.TileFrame->SetFloat(kTileValue_green, fGreen);
		HBIter.TileFrame->SetFloat(kTileValue_blue, fBlue);
		break;
	case 2:
		HBIter.TileText->SetFloat(kTileValue_red, fRed);
		HBIter.TileText->SetFloat(kTileValue_green, fGreen);
		HBIter.TileText->SetFloat(kTileValue_blue, fBlue);
		break;
	case 3:
		HBIter.TileImageEx->SetFloat(kTileValue_red, fRed);
		HBIter.TileImageEx->SetFloat(kTileValue_green, fGreen);
		HBIter.TileImageEx->SetFloat(kTileValue_blue, fBlue);
		break;
	case 10:
		HBIter.TileMeter->SetFloat(kTileValue_red, fRed);
		HBIter.TileMeter->SetFloat(kTileValue_green, fGreen);
		HBIter.TileMeter->SetFloat(kTileValue_blue, fBlue);
		HBIter.TileFrame->SetFloat(kTileValue_red, fRed);
		HBIter.TileFrame->SetFloat(kTileValue_green, fGreen);
		HBIter.TileFrame->SetFloat(kTileValue_blue, fBlue);
		HBIter.TileText->SetFloat(kTileValue_red, fRed);
		HBIter.TileText->SetFloat(kTileValue_green, fGreen);
		HBIter.TileText->SetFloat(kTileValue_blue, fBlue);
		HBIter.TileImageEx->SetFloat(kTileValue_red, fRed);
		HBIter.TileImageEx->SetFloat(kTileValue_green, fGreen);
		HBIter.TileImageEx->SetFloat(kTileValue_blue, fBlue);
		break;
	}

	return 1;
}

bool Cmd_HudBarSetColor_Execute(COMMAND_ARGS)
{

	char s_BarName[0x4000]{};
	float fRed, fGreen,fBlue;
	int iElement;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iElement, &fRed, &fGreen, &fBlue)) return true;

	*result = f_Bars_BarSetColor(s_BarName, iElement, fRed, fGreen, fBlue, modIdx);
	return true;
}








//0 for Meter
//1 for Frame
//2 for Text
//3 for ImageEx
//10 for ALL
int f_Bars_BarSetSystemColor(char* s_BarName, int iTrait, int iValue, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iTrait) {
	case 0:
		HBIter.TileMeter->SetFloat(kTileValue_systemcolor, iValue);
		break;
	case 1:
		HBIter.TileFrame->SetFloat(kTileValue_systemcolor, iValue);
		break;
	case 2:
		HBIter.TileText->SetFloat(kTileValue_systemcolor, iValue);
		break;
	case 3:
		HBIter.TileImageEx->SetFloat(kTileValue_systemcolor, iValue);
		break;

	case 10:
		HBIter.TileMeter->SetFloat(kTileValue_systemcolor, iValue);
		HBIter.TileFrame->SetFloat(kTileValue_systemcolor, iValue);
		HBIter.TileText->SetFloat(kTileValue_systemcolor, iValue);
		break;

	}

	return 1;
}


bool Cmd_HudBarSetSystemColor_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	int iTrait = 0, iValue = 0;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iTrait, &iValue)) return true;
	*result = f_Bars_BarSetSystemColor(s_BarName, iTrait, iValue, modIdx);
	return true;
}





int f_Bars_SetTextTrait(char* s_BarName, int iElement, int iTrait, float fValue, UInt8 modIdx)
{
	int iValue = fValue;
	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iElement) {
	case 0:
		switch (iTrait) {
		case 0:
			HBIter.TileText->SetFloat(kTileValue_font, iValue);
			break;
		case 1:
			HBIter.TileText->SetFloat(kTileValue_justify, iValue);
			break;
		}
	}
	return 1;
}


bool Cmd_HudBarSetTextTrait_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	int iTrait = 0;
	float fValue = 0;
	int iElement = 0;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName,&iElement, &iTrait, &fValue)) return true;
	*result = f_Bars_SetTextTrait(s_BarName, iElement, iTrait, fValue, modIdx);
	return true;
}



















int f_Bars_BarSetSizeAlt(char* s_BarName, int iElement, float fWidth, float fHeight, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iElement) {
	case 0:
		HBIter.MeterWidthAlt = fWidth;
		HBIter.MeterHeightAlt = fHeight;
		break;
	case 1:
		HBIter.FrameWidthAlt = fWidth;
		HBIter.FrameHeightAlt = fHeight;
		break;
	case 3:
		HBIter.ImageExWidthAlt = fWidth;
		HBIter.ImageExHeightAlt = fHeight;
		break;
	}

	return 1;
}



bool Cmd_HudBarSetSizeAlt_Execute(COMMAND_ARGS)
{
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	char s_BarName[0x4000]{};
	float fWidth = 0, fHeight = 0;
	int iElement = 0;

	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iElement, &fWidth, &fHeight)) return true;
	*result = f_Bars_BarSetSizeAlt(s_BarName, iElement, fWidth, fHeight, modIdx);
	return true;
}





//0 for Meter
//1 for Frame
//2 for Text
//3 for ImageEx

int f_Bars_BarSetDepth(char* s_BarName, int iElement, float fValue, UInt8 modIdx)
{
	int iRound = fValue;
	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iElement) {
	case 0:
		HBIter.TileMeter->SetFloat(kTileValue_depth, iRound);
		break;
	case 1:
		HBIter.TileFrame->SetFloat(kTileValue_depth, iRound);
		break;
	case 2:
		HBIter.TileText->SetFloat(kTileValue_depth, iRound);
		break;
	case 3:
		HBIter.TileImageEx->SetFloat(kTileValue_depth, iRound);
		break;
	case 10:
		HBIter.TileMeter->SetFloat(kTileValue_depth, iRound);
		HBIter.TileFrame->SetFloat(kTileValue_depth, iRound);
		HBIter.TileText->SetFloat(kTileValue_depth, iRound);
		HBIter.TileImageEx->SetFloat(kTileValue_depth, iRound);
		break;
	}
	return 1;
}


bool Cmd_HudBarSetDepth_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	int iElement = 0;
	float fValue;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iElement, &fValue)) return true;
	*result = f_Bars_BarSetDepth(s_BarName, iElement, fValue, modIdx);
	return true;
}


//0 for Meter
//1 for Frame
//2 for Text
//3 for ImageEx

int f_Bars_BarSetZoom(char* s_BarName, int iElement, float fValue, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iElement) {
	case 0:
		HBIter.TileMeter->SetFloat(kTileValue_zoom, fValue);
		break;
	case 1:
		HBIter.TileFrame->SetFloat(kTileValue_zoom, fValue);
		break;
	case 2:
		HBIter.TileText->SetFloat(kTileValue_zoom, fValue);
		break;
	case 3:
		HBIter.TileImageEx->SetFloat(kTileValue_zoom, fValue);
		break;
	case 10:
		HBIter.TileMeter->SetFloat(kTileValue_zoom, fValue);
		HBIter.TileFrame->SetFloat(kTileValue_zoom, fValue);
		HBIter.TileText->SetFloat(kTileValue_zoom, fValue);
		HBIter.TileImageEx->SetFloat(kTileValue_zoom, fValue);
		break;
	}
	return 1;
}


bool Cmd_HudBarSetZoom_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	int iElement = 0;
	float fValue;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iElement, &fValue)) return true;
	*result = f_Bars_BarSetZoom(s_BarName, iElement, fValue, modIdx);
	return true;
}

//0 for Meter
//1 for Frame
//2 for Text
//3 for ImageEx

int f_Bars_BarSetRotation(char* s_BarName, int iElement,int iTrait, float fValue, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iElement) {
	case 0:
		switch (iTrait)
		{
		case 0:
			HBIter.TileMeter->SetFloat(kTileValue_rotateangle, fValue);
			break;
		case 1:
			HBIter.TileMeter->SetFloat(kTileValue_rotateaxisx, fValue);
			break;
		case 2:
			HBIter.TileMeter->SetFloat(kTileValue_rotateaxisy, fValue);
			break;
		}
		break;
	case 1:
		switch (iTrait)
		{
		case 0:
			HBIter.TileFrame->SetFloat(kTileValue_rotateangle, fValue);
			break;
		case 1:
			HBIter.TileFrame->SetFloat(kTileValue_rotateaxisx, fValue);
			break;
		case 2:
			HBIter.TileFrame->SetFloat(kTileValue_rotateaxisy, fValue);
			break;
		}
		break;
	case 2:
		switch (iTrait)
		{
		case 0:
			HBIter.TileText->SetFloat(kTileValue_rotateangle, fValue);
			break;
		case 1:
			HBIter.TileText->SetFloat(kTileValue_rotateaxisx, fValue);
			break;
		case 2:
			HBIter.TileText->SetFloat(kTileValue_rotateaxisy, fValue);
			break;
		}
		break;



	case 3:
		switch (iTrait)
		{
		case 0:
			HBIter.TileImageEx->SetFloat(kTileValue_rotateangle, fValue);
			break;
		case 1:
			HBIter.TileImageEx->SetFloat(kTileValue_rotateaxisx, fValue);
			break;
		case 2:
			HBIter.TileImageEx->SetFloat(kTileValue_rotateaxisy, fValue);
			break;
		}
		break;
	case 10:
		switch (iTrait)
		{
		case 0:
			HBIter.TileFrame->SetFloat(kTileValue_rotateangle, fValue);
			HBIter.TileImageEx->SetFloat(kTileValue_rotateangle, fValue);
			HBIter.TileMeter->SetFloat(kTileValue_rotateangle, fValue);
			HBIter.TileText->SetFloat(kTileValue_rotateangle, fValue);
			break;
		case 1:
			HBIter.TileFrame->SetFloat(kTileValue_rotateaxisx, fValue);
			HBIter.TileImageEx->SetFloat(kTileValue_rotateaxisx, fValue);
			HBIter.TileMeter->SetFloat(kTileValue_rotateaxisx, fValue);
			HBIter.TileText->SetFloat(kTileValue_rotateaxisx, fValue);
			break;
		case 2:
			HBIter.TileFrame->SetFloat(kTileValue_rotateaxisy, fValue);
			HBIter.TileImageEx->SetFloat(kTileValue_rotateaxisy, fValue);
			HBIter.TileMeter->SetFloat(kTileValue_rotateaxisy, fValue);
			HBIter.TileText->SetFloat(kTileValue_rotateaxisy, fValue);
			break;
		}
		break;
	}
	return 1;
}


bool Cmd_HudBarSetRotation_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	int iElement = 0, iTrait = 0;
	float fValue = 0;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iElement,&iTrait, &fValue)) return true;
	*result = f_Bars_BarSetRotation(s_BarName, iElement, iTrait, fValue, modIdx);
	return true;
}






int f_Bars_BarSetItem(char* s_BarName, TESForm* HookedItem, UInt8 modIdx, int iRequest)
{
	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	if (iRequest) {
		HBIter.iHookedItemIsAList = 0;
		HBIter.HookedItem = HookedItem;
	}else 
	{HBIter.HookedItem = NULL;}





	return 1;
}


bool Cmd_HudBarSetItem_Execute(COMMAND_ARGS)
{

	char s_BarName[0x4000]{};
	int iElement = 0;
	TESForm* HookedItem = NULL;
	float fValue;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &HookedItem)) return true;

	if (NUM_ARGS > 1)
	{*result = f_Bars_BarSetItem(s_BarName, HookedItem, modIdx, 1);}
	else {*result = f_Bars_BarSetItem(s_BarName, HookedItem, modIdx, 0);}


	return true;
}



int f_Bars_BarSetFormList(char* s_BarName, BGSListForm* listForm, UInt8 modIdx, int iRequest)
{
	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	if (iRequest)
	{
		if (IS_ID(listForm, BGSListForm) == 0)
		{
			return -2;
		}

		HBIter.iHookedItemIsAList = 1;
		HBIter.listForm = listForm;
		//_MESSAGE("HOOKED FORM LIST");
	}
	else {
		HBIter.iHookedItemIsAList = 0;
		HBIter.listForm = NULL;
	}



	return 1;
}


bool Cmd_HudBarSetFormList_Execute(COMMAND_ARGS)
{

	char s_BarName[0x4000]{};
	int iElement = 0;
	BGSListForm* listForm = NULL;
	float fValue;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &listForm)) return true;

	if (NUM_ARGS > 1)
	{*result = f_Bars_BarSetFormList(s_BarName, listForm, modIdx, 1);}
	else {*result = f_Bars_BarSetFormList(s_BarName, listForm, modIdx, 0);}

	return true;
}

















int f_Bars_BarOutputDebugInfo(int iKey, UInt8 modIdx)
{
	_MESSAGE("-------");
	_MESSAGE("---------BAR_DEBUG_INFO starts for>>>>%s from mod::%s---------", HBIter.Name, GetNthModNameAlt(modIdx));

	_MESSAGE("ID is %d", HBIter.ID);
	_MESSAGE("modIdx is %d", HBIter.modIdx);
	_MESSAGE("Bar type  is %d", HBIter.MeterType);
	_MESSAGE("iMenuTypeShow  is %d", HBIter.iMenuTypeShow);
	_MESSAGE("fUpdateTimer is %f", HBIter.fUpdateTimer);
	_MESSAGE("Bar Visible is %d", HBIter.iBarVisible);
	_MESSAGE("Hidden  is %d", HBIter.iHidden);
	_MESSAGE("TileTextAdded is %d", HBIter.TileTextAdded);
	_MESSAGE("TileFrameAdded is %d", HBIter.TileFrameAdded);
	_MESSAGE("TileMeterAdded is %d", HBIter.TileMeterAdded);
	_MESSAGE("TileImageExAdded is %d", HBIter.TileImageExAdded);
	_MESSAGE("MaxValue is %d", HBIter.MaxValue);
	_MESSAGE("iGradualVanishing is %d", HBIter.iGradualVanishing);
	_MESSAGE("iGradualAppearing is %d", HBIter.iGradualAppearing);

	_MESSAGE("MeterWidth is %f", HBIter.MeterWidth);
	_MESSAGE("MeterWidthAlt is %f", HBIter.MeterWidthAlt);
	_MESSAGE("MeterHeight is %f", HBIter.MeterHeight);
	_MESSAGE("MeterHeightAlt is %f", HBIter.MeterHeightAlt);

	_MESSAGE("FrameWidth is %f", HBIter.FrameWidth);
	_MESSAGE("FrameWidthAlt is %f", HBIter.FrameWidthAlt);
	_MESSAGE("FrameHeight is %f", HBIter.FrameHeight);
	_MESSAGE("FrameHeightAlt is %f", HBIter.FrameHeightAlt);

	_MESSAGE("ImageExWidth is %f", HBIter.ImageExWidth);
	_MESSAGE("ImageExWidthAlt is %f", HBIter.ImageExWidthAlt);
	_MESSAGE("ImageExHeight is %f", HBIter.ImageExHeight);
	_MESSAGE("ImageExHeightAlt is %f", HBIter.ImageExHeightAlt);
	_MESSAGE("Indent Meter is %f", HBIter.IndentMeter);

	_MESSAGE("iDisableControlMovement is %d", HBIter.iDisableControlMovement);
	_MESSAGE("iDisableWhenSneaking is %d", HBIter.iDisableWhenSneaking);
	_MESSAGE("iDisableWhenCombat is %d", HBIter.iDisableWhenCombat);
	_MESSAGE("iDisableWhenAiming is %d", HBIter.iDisableWhenAiming);
	_MESSAGE("iDisableWhenWeaponOut is %d", HBIter.iDisableWhenWeaponOut);
	_MESSAGE("iDisableWhenNotEquipped is %d", HBIter.iDisableWhenNotEquipped);
	_MESSAGE("iHookedItemIsAList is %d", HBIter.iHookedItemIsAList);

	if (HBIter.HookedItem)
	{
		_MESSAGE("HookedItem is is %s", HBIter.HookedItem->GetName());
	}


	_MESSAGE("---DUMPING RECT---");

	HBIter.TileRect->DumpAlt();

	_MESSAGE("---DUMPING RECT END---");
	_MESSAGE("---------BAR_DEBUG_INFO ends for>>>>%s from mod::%s---------", HBIter.Name, GetNthModNameAlt(modIdx));
	_MESSAGE("-------");
	_MESSAGE("-------");
	_MESSAGE("-------");
	return 1;
}






int f_Bars_BarGetDebugInfo(char* s_BarName, UInt8 modIdx)
{
	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }
	f_Bars_BarOutputDebugInfo(iKey, modIdx);
	return 1;
}


bool Cmd_HudBarGetDebugInfo_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName)) return true;
	*result = f_Bars_BarGetDebugInfo(s_BarName, modIdx);
	return true;
}



int f_Bars_BarGetDebugInfoAll()
{

	for (std::vector<HUDBarElement>::iterator it = g_HUDBArsArrayV.begin(); it != g_HUDBArsArrayV.end(); ++it)
	{
		//_MESSAGE("VECTOR ARRAY is MODINDEX IS %d", HBIterV.modIdx);
		f_Bars_BarOutputDebugInfo(it - g_HUDBArsArrayV.begin(), HBIterV.modIdx);
	}


	return 1;
}



bool Cmd_HudBarGetDebugInfoAll_Execute(COMMAND_ARGS)
{
	*result = f_Bars_BarGetDebugInfoAll();
	return true;
}








float f_Bars_BarGetBarTrait(char* s_BarName, int iTrait, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iTrait) {
	case 0:
		return HBIter.iMenuTypeShow;
		break;
	case 1:
		return HBIter.iRemoveOnGameLoad;
		break;
	case 2:
		return HBIter.fUpdateTimer;
		break;
	case 21:
		return HBIter.iDisableControlMovement;
		break;
	case 22:
		return HBIter.iDisableWhenSneaking;
		break;
	case 23:

		return HBIter.iDisableWhenCombat;
		break;
	case 24:
		return HBIter.iDisableWhenAiming;
		break;
	case 25:
		return HBIter.iDisableWhenNotEquipped;
		break;
	case 26:
		return HBIter.iDisableWhenWeaponOut;
		break;



	case 41:
		return HBIter.iGradualVanishing;
		break;
	case 42:
		return HBIter.iGradualVanishingMovement; //;1 Horizontal ;2 Vertical
		break;
	case 43:
		return HBIter.fGradualVanishingPosShift;
		break;
	case 44:
		return HBIter.fGradualVanishingTimer;
		break;


	case 61:

		return HBIter.iGradualAppearing;
		break;
	case 62:
		return HBIter.iGradualAppearingMovement; //;1 Horizontal ;2 Vertical
		break;
	case 63:
		return HBIter.fGradualAppearingPosShift;
		break;
	case 64:
		return HBIter.fGradualAppearingTimer;
		break;
	}

	return -1;
}



bool Cmd_HudBarGetBarTrait_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	int  iTrait = 0, iModIndex = 0;
	float fValue;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iTrait, &iModIndex)) return true;

	if (NUM_ARGS > 2)
	{
		modIdx = iModIndex;
	}


	*result = f_Bars_BarGetBarTrait(s_BarName, iTrait, modIdx);
	return true;
}





//0 for Meter
//1 for Frame
//3 for ImageEx
//10 for ALL elements.
int f_Bars_BarSetIndent(char* s_BarName, int iElement, float fValue, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iElement) {
	case 0:
		HBIter.IndentMeter = fValue;
		break;
	case 1:
		HBIter.IndentFrame = fValue;
		break;
	case 3:
		HBIter.IndentImageEx = fValue;
	case 10:
		HBIter.IndentMeter = fValue;
		HBIter.IndentFrame = fValue;
		HBIter.IndentImageEx = fValue;
		break;
	}
	return 1;
}


bool Cmd_HudBarSetIndent_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	int iElement = 0;
	float fValue;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName, &iElement, &fValue)) return true;
	*result = f_Bars_BarSetIndent(s_BarName, iElement, fValue, modIdx);
	return true;
}

