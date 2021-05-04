#pragma once
DEFINE_COMMAND_PLUGIN(HudBarCreate, "", 0, 4, kParams_Tomm_HudBarCreate)
DEFINE_COMMAND_PLUGIN(HudBarSetValueScriptVar, "", 0, 3, kParams_Tomm_HudBarSetScriptVariable)
DEFINE_COMMAND_PLUGIN(HudBarSetValueFloat, "", 0, 2, kParams_Tomm_HudBarSetFloatValue)
DEFINE_COMMAND_PLUGIN(HudBarSetValuePercentage, "", 0, 2, kParams_Tomm_HudBarSetFloatValue)
DEFINE_COMMAND_PLUGIN(HudBarSetValueMax, "", 0, 2, kParams_Tomm_HudBarSetFloatValue)
DEFINE_COMMAND_PLUGIN(HudBarGetBarType, "", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_PLUGIN(HudBarExists, "", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_PLUGIN(HudBarSetAutoPos, "", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_PLUGIN(HudBarSetTextString, "", 0, 3, kParams_Tomm_HudBarSetStringEx)
DEFINE_COMMAND_PLUGIN(HudBarDestroy, "", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_PLUGIN(HudBarSetBarTrait, "", 0, 3, kParams_Tomm_HudBarSetTraitInt)
DEFINE_COMMAND_PLUGIN(HudBarSetPos, "", 0, 4, kParams_Tomm_HudBarSetTwoFloatValuesEx)
DEFINE_COMMAND_PLUGIN(HudBarSetVisible, "", 0, 3, kParams_Tomm_HudBarSetIntEx)
DEFINE_COMMAND_PLUGIN(HudBarSetTexture, "", 0, 3, kParams_Tomm_HudBarSetStringEx)
DEFINE_COMMAND_PLUGIN(HudBarSetSize, "", 0, 4, kParams_Tomm_HudBarSetTwoFloatValuesEx)
DEFINE_COMMAND_PLUGIN(HudBarSetMeterTrait, "", 0, 3, kParams_Tomm_HudBarSetFloatValueEx)
DEFINE_COMMAND_PLUGIN(HudBarSetValueUDF, "", 0, 4, kParams_Tomm_HudBarSetValueUDF)
DEFINE_COMMAND_PLUGIN(HudBarShowBar, "", 0, 2, kParams_Tomm_HudBarSetInt)





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








int f_Bars_BarGetKey(char* s_BarName, UInt8 modIdx)
{
	int iFound = 0;
	int i;
	for (i = 0; i < g_HudBarsArraySize; i++) {

		if (g_HUDBArsArray[i].modIdx == modIdx)
		{
			if (strcmp(g_HUDBArsArray[i].Name, s_BarName) == 0)
			{
				_MESSAGE("FOUND BAR,i>>> %d", i);
				iFound = 1;
				break;
			}
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
		sprintf(s_ToInject, "%s", "<rect name=\"SUPBars\"> <template name=\"SUPNVSEBarsTemplateDoNotUse\"> <rect name=\"Bar\"> 	<locus> &true; </locus> <visible>0</visible> <image name=\"MeterImage\"> <height>0</height> <width>0</width> <x>0</x> <y>0</y>  <filename>Interface\\Shared\\solid.dds</filename> <visible>1</visible> </image>      <image name=\"FrameImage\">  <height>0</height> <width>0</width> <x>0</x> <y>0</y>  <filename>Interface\\Shared\\solid.dds</filename> <visible>1</visible> </image>  <text name=\"MeterText\"> <string></string> <font>1</font> <visible>0</visible>  <systemcolor>1</systemcolor>   </text>  </rect> </template> </rect>");
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

bool f_Bars_AddNewElement(char* s_BarName, float fPosX, float fPosY, int iMeterType, UInt8 modIdx)
{
	if (g_SUPRectBars)
	{}else { f_Bars_CreateTemplate(); }

	if (g_SUPRectBars) {}
	else {return false;}



	if (f_Bars_BarGetKey(s_BarName, modIdx) >= 0)
	{
		Console_Print("BAR EXISTS");
		return false;
	}



	Tile* component = GetTargetComponent("HUDMainMenu");
	TileMenu* menu = GetMenuTile("HUDMainMenu");


	component = menu->menu->AddTileFromTemplate(g_SUPRectBars, "SUPNVSEBarsTemplateDoNotUse");

	if (component)
	{


		HUDBarElement* HUDBAR = new HUDBarElement;
		sprintf(HUDBAR->Name, "%s", s_BarName);

		sprintf(s_BarName, "%sx%d", s_BarName, modIdx);
		component->name.Set(s_BarName);

		Tile* TileFrame = component->GetComponentTile("FrameImage");
		Tile* TileMeter = component->GetComponentTile("MeterImage");
		Tile* TileText = component->GetComponentTile("MeterText");


		_MESSAGE("Adding meter type of %d ", iMeterType);


		HUDBAR->TileRect = component;
		HUDBAR->TileFrame = TileFrame;
		HUDBAR->TileMeter = TileMeter;
		HUDBAR->TileText = TileText;

		HUDBAR->modIdx = modIdx;



		HUDBAR->MeterType = iMeterType;

		component->SetFloat(kTileValue_x, fPosX);
		component->SetFloat(kTileValue_y, fPosY);

		if (iMeterType < 3) // if meter type <3 - TileMeter is MeterImage
		{
			HUDBAR->TileMeterAdded = 1;
		}
		else // if meter type -4 - TileMeter is Text
		{
			HUDBAR->TileTextAdded = 1;
			TileText->SetFloat(kTileValue_visible, 1);
		}
	

		_MESSAGE("SUCCESS adding to pos->>>%d ", g_HudBarsArraySize);

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

	HudBarIterElement.iScriptValueCalculateMax = iScriptValueCalculateMax;
	HudBarIterElement.ScriptVariable = ScriptVariable;
	HudBarIterElement.ValueType = k_HBValueTypeScriptVar;
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

	if (HudBarIterElement.MeterType == k_HBMeterTypeText){}
	else {
		fValue = fValue / HudBarIterElement.MaxValue;
		if (fValue > 1)
			fValue = 1;
		else if (fValue < 0)
			fValue = 0;
	}

	HudBarIterElement.ValueType = k_HBValueTypeNoScriptVar;
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

	if (HudBarIterElement.MeterType == k_HBMeterTypeText)
		return -2;


	HudBarIterElement.ValueType = k_HBValueTypeNoScriptVar;
	*result = f_Bars_BarSetBarValue(iKey, fValue);
	return true;
}


int f_Bars_BarSetValueMax(char* s_BarName, float fValue, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
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

bool Cmd_HudBarGetBarType_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName)) return true;

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {
		*result = HudBarIterElement.MeterType;
	}
	else { 
		*result = -1; 
	}

	return true;
}


int f_Bars_BarSetAutoPos(char* s_BarName, UInt8 modIdx)
{
	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	float fCalculatedX = 0, fCalculatedY = 0;

	if (HudBarIterElement.MeterType < k_HBMeterTypeText)
	{
		if (HudBarIterElement.TileTextAdded == 1)
		{

			float fFontHeight = (f_Bars_GetFontHeight(g_HUDBArsArray[iKey].TileText->GetValueFloat(kTileValue_font))) / 2;

			if (HudBarIterElement.TileFrameAdded)
			{
				fCalculatedX = (HudBarIterElement.FrameWidthAlt) + 1;
				fCalculatedY = ((HudBarIterElement.FrameHeightAlt) / 2) - fFontHeight;
				Console_Print("HudBarIterElement.TileFrameAdded");
			}
			else
			{
				fCalculatedX = (g_HUDBArsArray[iKey].MeterWidthAlt + g_HUDBArsArray[iKey].Indent) + 1;
				fCalculatedY = ((g_HUDBArsArray[iKey].MeterHeightAlt)/2) - fFontHeight;
				Console_Print("HudBarIterElement.TileFrameAdded NOT");
			}


			HudBarIterElement.TileText->SetFloat(kTileValue_x, fCalculatedX);
			HudBarIterElement.TileText->SetFloat(kTileValue_y, fCalculatedY);

			//Tile* MyTile = g_HUDBArsArray[iKey].TileMeter;
			//float fCalculatedX = MyTile->GetValueFloat(kTileValue_width);
			//g_HUDBArsArray[iKey].TileMeter->GetValueFloat(kTileValue_width)
		}
	}else if(HudBarIterElement.MeterType >= k_HBMeterTypeText)
	{

		if (HudBarIterElement.TileFrameAdded == 1)
		{
			Console_Print("HudBarIterElement.TileFrameAdded");
			fCalculatedX = (HudBarIterElement.FrameWidthAlt) + 1;
			HudBarIterElement.TileText->SetFloat(kTileValue_x, fCalculatedX);

			float fFontHeight = (f_Bars_GetFontHeight(g_HUDBArsArray[iKey].TileText->GetValueFloat(kTileValue_font)))/2  ;
			fCalculatedY = ((HudBarIterElement.FrameHeightAlt)/2) - fFontHeight;
			HudBarIterElement.TileText->SetFloat(kTileValue_y, fCalculatedY);

		}

	}


	return 1;
}




bool Cmd_HudBarSetAutoPos_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName)) return true;
	*result = f_Bars_BarSetAutoPos(s_BarName, modIdx);
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

	f_Bars_BarDestroy(iKey);
	return true;
}



int f_Bars_BarSetBarTrait(char* s_BarName, int iTrait, int iValue, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iTrait) {
		case 0:
			HudBarIterElement.iMenuTypeShow = iValue;
			break;
	}

	return 1;
}



bool Cmd_HudBarSetBarTrait_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	int iValue = 0, iTrait = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName,&iTrait, &iValue)) return true;
	*result = f_Bars_BarSetBarTrait(s_BarName, iTrait, iValue, modIdx);
	return true;
}








//0 for Meter
//1 for Frame
//2 for Text
//3 For Bar
int f_Bars_BarSetPos(char* s_BarName, int iElement, float fPosX, float fPosY, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iElement) {
	case 0:
		HudBarIterElement.TileMeter->SetFloat(kTileValue_x, fPosX);
		HudBarIterElement.TileMeter->SetFloat(kTileValue_y, fPosY);
		break;
	case 1:
		HudBarIterElement.TileFrame->SetFloat(kTileValue_x, fPosX);
		HudBarIterElement.TileFrame->SetFloat(kTileValue_y, fPosY);
		break;
	case 2:
		HudBarIterElement.TileText->SetFloat(kTileValue_x, fPosX);
		HudBarIterElement.TileText->SetFloat(kTileValue_y, fPosY);
		break;
	case 3:
		HudBarIterElement.TileRect->SetFloat(kTileValue_x, fPosX);
		HudBarIterElement.TileRect->SetFloat(kTileValue_y, fPosY);
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
		HudBarIterElement.TileMeter->SetFloat(kTileValue_visible, iValue);
		break;
	case 1:
		HudBarIterElement.TileFrame->SetFloat(kTileValue_visible, iValue);
		break;
	case 2:
		HudBarIterElement.TileText->SetFloat(kTileValue_visible, iValue);
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
int f_Bars_BarSetTexture(char* s_BarName, int iElement, char* s_FileName, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }


	switch (iElement) {
	case 0:
		HudBarIterElement.TileMeter->SetString(kTileValue_filename, s_FileName);
		HudBarIterElement.TileMeterAdded = 1;
		break;
	case 1:
		HudBarIterElement.TileFrame->SetString(kTileValue_filename, s_FileName);
		HudBarIterElement.TileFrameAdded = 1;
		break;

	}

	return 1;
}


bool Cmd_HudBarSetTexture_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{}, s_FileName[0x4000]{};
	int iElement = 0;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_BarName,&iElement, &s_FileName)) return true;
	*result = f_Bars_BarSetTexture(s_BarName, iElement, s_FileName, modIdx);
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
		HudBarIterElement.TileText->SetString(kTileValue_string, s_Name);
		HudBarIterElement.TileText->SetFloat(kTileValue_visible, 1);
		HudBarIterElement.TileTextAdded = 1;
		break;
	case 1:
		sprintf(HudBarIterElement.Prefix, "%s", s_Name);
		break;
	case 2:
		sprintf(HudBarIterElement.PostFix, "%s", s_Name);
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
//2 For MeterALT
//3 For FrameALT

int f_Bars_BarSetSize(char* s_BarName, int iElement, float fWidth, float fHeight, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }

	switch (iElement) {
	case 0:
		HudBarIterElement.MeterWidth = fWidth;
		HudBarIterElement.MeterHeight = fHeight;
		HudBarIterElement.MeterWidthAlt = fWidth;
		HudBarIterElement.MeterHeightAlt = fHeight;
		HudBarIterElement.TileMeter->SetFloat(kTileValue_height, fHeight);
		HudBarIterElement.TileMeter->SetFloat(kTileValue_width, fWidth);
		break;
	case 1:
		HudBarIterElement.FrameWidth = fWidth;
		HudBarIterElement.FrameHeight = fHeight;
		HudBarIterElement.FrameWidthAlt = fWidth;
		HudBarIterElement.FrameHeightAlt = fHeight;
		HudBarIterElement.TileFrame->SetFloat(kTileValue_height, fHeight);
		HudBarIterElement.TileFrame->SetFloat(kTileValue_width, fWidth);
		break;
	case 2:
		HudBarIterElement.MeterWidthAlt = fWidth;
		HudBarIterElement.MeterHeightAlt = fHeight;
		break;
	case 3:
		HudBarIterElement.FrameWidthAlt = fWidth;
		HudBarIterElement.FrameHeightAlt = fHeight;
		break;

	}

	return 1;
}



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



//0 for Indent
//1 for AlphaMinValue
//2 for AlphaMaxValue
//3 for Show Max Value
int f_Bars_BarSetMeterTrait(char* s_BarName, int iTrait, float fValue, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }
	
	switch (iTrait) {
	case 0:
		HudBarIterElement.Indent = fValue;
		break;
	case 1:
		HudBarIterElement.fAlphaMin = fValue;
		HudBarIterElement.fAlphaDifference = HudBarIterElement.fAlphaMax - HudBarIterElement.fAlphaMin;
	case 2:
		HudBarIterElement.fAlphaMax = fValue;
		HudBarIterElement.fAlphaDifference = HudBarIterElement.fAlphaMax - HudBarIterElement.fAlphaMin;
		break;
	case 3:
		HudBarIterElement.iShowMaxValue = fValue;
		break;
	}

	return 1;
}

bool Cmd_HudBarSetMeterTrait_Execute(COMMAND_ARGS)
{
	char s_BarName[0x4000]{};
	int iTrait = 0;
	float fValue;
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

	HudBarIterElement.FunctionToCall = FunctionToCall;
	HudBarIterElement.iScriptValueCalculateMax = iScriptValueCalculateMax;

	if (FunctionCaller)
	{HudBarIterElement.FunctionCaller = FunctionCaller;}


	HudBarIterElement.ValueType = k_HBValueTypeScriptFunction;
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










int f_Bars_BarShowBar(char* s_BarName,  int iValue, UInt8 modIdx)
{

	int iKey = f_Bars_BarGetKey(s_BarName, modIdx);
	if (iKey >= 0) {}
	else { return -1; }


	HudBarIterElement.iBarVisible = iValue;
	HudBarIterElement.TileRect->SetFloat(kTileValue_visible, iValue);
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






