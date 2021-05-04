#pragma once

#define k_HBMeterTypeHorizontal 0
#define k_HBMeterTypeVertical 1
#define k_HBMeterTypeAlpha 2
#define k_HBMeterTypeText 3
#define k_HBMeterTypeTextPercentage 4

#define	k_HBValueTypeNoScriptVar 0
#define k_HBValueTypeScriptVar 1
#define k_HBValueTypeScriptFunction 2



class HUDBarsInterface
{
public:

	class HudBar
	{
	public:

		//Main
		int iBarVisible = 0;
		int iReasonHidden = 0;
		int iMenuTypeShow = 0;
		int iShowMaxValue = 0;
		char Name[0x4000]{};
		int MeterType = k_HBMeterTypeHorizontal;
		int ValueType = k_HBValueTypeNoScriptVar;
		int TileTextAdded = 0;
		int TileFrameAdded = 0;
		int TileMeterAdded = 0;
		float Value = 0;
		float MaxValue = 100;
		UInt8 modIdx = -1;

		//Tiles
		Tile* TileRect = NULL;
		Tile* TileFrame = NULL;
		Tile* TileMeter = NULL;
		Tile* TileText = NULL;

		//Size
		float MeterWidth = 0;
		float MeterWidthAlt = 0;
		float MeterHeight = 0;
		float MeterHeightAlt = 0;
		float FrameWidth = 0;
		float FrameHeight = 0;
		float FrameWidthAlt = 0;
		float FrameHeightAlt = 0;
		float Indent = 0;


		//ValueTypeScript
		ScriptVar* ScriptVariable = NULL;
		Script* FunctionToCall = NULL;
		TESObjectREFR* FunctionCaller = NULL;
		int iScriptValueCalculateMax = 0;

		//MeterTypeAlpha
		float fAlphaMin = 0;
		float fAlphaMax = 255;
		float fAlphaDifference = 255;

		//MeterTypeText
		char Prefix[0x4000]{};
		char PostFix[0x4000]{};
	};


};



typedef HUDBarsInterface::HudBar HUDBarElement;
HUDBarElement g_HUDBArsArray[100]{};
int g_HudBarsArraySize = 0;






void f_Bars_BarSetHidden(int iKey, int iRequest)
{
	HudBarIterElement.TileRect->SetFloat(kTileValue_visible, iRequest);
}







void f_Bars_BarDestroy(int iKey)
{
	HudBarIterElement.TileRect->Destroy(true);

	delete &HudBarIterElement;

	HUDBarElement* element = &g_HUDBArsArray[iKey];
	delete element;

	g_HudBarsArraySize -= 1;
	for (int j = iKey; j < g_HudBarsArraySize; j++)
		g_HUDBArsArray[j] = g_HUDBArsArray[j + 1];
}






int f_Bars_BarSetBarValue(int iKey, float fBarVarValue)
{

	//_MESSAGE("f_Bars_BarSetBarValue started for is %s", HudBarIterElement.Name);
	//_MESSAGE("iMeterType is %d", HudBarIterElement.MeterType);
	//_MESSAGE("iValueType is %d", HudBarIterElement.ValueType);
	//_MESSAGE("Value is %f", HudBarIterElement.Value);
	//_MESSAGE("fMaxValue is %f", HudBarIterElement.MaxValue);
	//_MESSAGE("IterElement.MeterWidth is %f", HudBarIterElement.MeterWidth);
	//_MESSAGE("fBarVarValue is %f", fBarVarValue);
	

	char TempText[0x4000]{};
	switch (HudBarIterElement.MeterType) {
	case k_HBMeterTypeHorizontal:
		fBarVarValue *= HudBarIterElement.MeterWidthAlt;
		fBarVarValue += HudBarIterElement.Indent;

		//_MESSAGE("HudBarIterElement.MeterWidthAlt >>>>>%f", HudBarIterElement.MeterWidthAlt);
		//_MESSAGE("HudBarIterElement.Indent >>>>>%f", HudBarIterElement.Indent);
		//_MESSAGE("Setting width Value to>>>>>%f", fBarVarValue);
		HudBarIterElement.TileMeter->SetFloat(kTileValue_width, fBarVarValue);
		break;
	case k_HBMeterTypeVertical:
		fBarVarValue *= HudBarIterElement.MeterHeightAlt;
		fBarVarValue += HudBarIterElement.Indent;
		HudBarIterElement.TileMeter->SetFloat(kTileValue_height, fBarVarValue);
		break;

	case k_HBMeterTypeAlpha:
		fBarVarValue *= HudBarIterElement.fAlphaDifference;
		fBarVarValue += HudBarIterElement.fAlphaMin;
		HudBarIterElement.TileMeter->SetFloat(kTileValue_alpha, fBarVarValue);
		break;
	case k_HBMeterTypeText:
		_MESSAGE("Setting k_HBMeterTypeText");
		Console_Print("HudBarIterElement.MaxValue is %f ", HudBarIterElement.MaxValue);

		if (HudBarIterElement.iShowMaxValue == 1)
		{sprintf(TempText, "%s%.0f/%.0f%s", HudBarIterElement.Prefix, fBarVarValue, HudBarIterElement.MaxValue, HudBarIterElement.PostFix);}
		else{sprintf(TempText, "%s%.0f%s", HudBarIterElement.Prefix, fBarVarValue, HudBarIterElement.PostFix);}

		HudBarIterElement.TileText->SetString(kTileValue_string, TempText);
		break;
	case k_HBMeterTypeTextPercentage:
		_MESSAGE("Setting k_HBMeterTypeTextPercentage");
		int iToShow = fBarVarValue * 100;
		sprintf(TempText, "%s%d%s", HudBarIterElement.Prefix, iToShow, HudBarIterElement.PostFix);
		HudBarIterElement.TileText->SetString(kTileValue_string, TempText);
		break;
		//default:
			//break;
	}

	return 1;
}




void f_Bars_Iterate()
{
	ScriptVar* ScriptVariable = nullptr;
	float fMaxValue = 0, fWidth = 0;
	Tile* TileMeter;
	float fBarVarValue = 0;
	int iMeterType = 0, iValueType  = 0;
	for (int iKey = 0; iKey < g_HudBarsArraySize; iKey++) {
		Console_Print("Current Bar is %s, iKey is %d", HudBarIterElement.Name, iKey);

		if (HudBarIterElement.iBarVisible) {}
		else { continue; }

		if (HudBarIterElement.iMenuTypeShow)
		{
			//Console_Print("iMenuTypeShow is %d, currentMode is %d", HudBarIterElement.iMenuTypeShow, g_interfaceManager->currentMode);
			if (HudBarIterElement.iMenuTypeShow == g_interfaceManager->currentMode)
			{
				if (HudBarIterElement.iReasonHidden != 0)
				{
					HudBarIterElement.iReasonHidden = 0;
					HudBarIterElement.TileRect->SetFloat(kTileValue_visible, 1);
				}
			}
			else
			{
				if (HudBarIterElement.iReasonHidden == 0)
				{
					HudBarIterElement.iReasonHidden = 1;
					HudBarIterElement.TileRect->SetFloat(kTileValue_visible, 0);
				}
			}
		}
		else {
			if (HudBarIterElement.iReasonHidden == 1)
			{
				HudBarIterElement.iReasonHidden = 0;HudBarIterElement.TileRect->SetFloat(kTileValue_visible, 1);
			}
		}



		if (HudBarIterElement.ValueType >= k_HBValueTypeScriptVar) {}
		else { continue; }

		

		if (HudBarIterElement.ValueType == k_HBValueTypeScriptVar)
		{
			ScriptVariable = HudBarIterElement.ScriptVariable;
			fBarVarValue = ScriptVariable->data.num;
			if (HudBarIterElement.iScriptValueCalculateMax)
			{
				fMaxValue = HudBarIterElement.MaxValue;
				fBarVarValue = ScriptVariable->data.num;
				fBarVarValue = fBarVarValue / fMaxValue;
				if (fBarVarValue > 1)
					fBarVarValue = 1;
				else if (fBarVarValue < 0)
					fBarVarValue = 0;
			}
		}else if (HudBarIterElement.ValueType == k_HBValueTypeScriptFunction)
		{
			NVSEArrayElement element;
			element.num = std::numeric_limits<float>::quiet_NaN();

			if (HudBarIterElement.FunctionToCall)
			{
				if (HudBarIterElement.FunctionCaller)
				{
					FunctionCallScript(HudBarIterElement.FunctionToCall, HudBarIterElement.FunctionCaller, NULL, &element, NULL);
				}
				else {
					FunctionCallScript(HudBarIterElement.FunctionToCall, NULL, NULL, &element, NULL);
				}

				if (element.num != std::numeric_limits<float>::quiet_NaN())
				{
					fBarVarValue = element.num;
					if (HudBarIterElement.iScriptValueCalculateMax)
					{
						fBarVarValue = fBarVarValue / fMaxValue;
						if (fBarVarValue > 1)
							fBarVarValue = 1;
						else if (fBarVarValue < 0)
							fBarVarValue = 0;
						Console_Print("fBarVarValue is %f ", element.num);
					}
	
	
				}else{
					fBarVarValue = 0;
					_MESSAGE("Function result is not valid for bar->>%s", HudBarIterElement.Name);
				}



			}
			else {
				_MESSAGE("Current FUNCTION DOESNT EXIST");
			}

		}


		_MESSAGE("NAME for scriptVAR-->> %s", HudBarIterElement.Name);
		f_Bars_BarSetBarValue(iKey,fBarVarValue);

	}

}




//Console_Print("HUDBARAREAYSIZE is %d", std::size(g_HUDBArsArray));
//Console_Print("g_HUDBArsArray SIZE is %d", g_HudBarsArraySize);
//for (HUDBarElement IterElement : g_HUDBArsArray)
//{}


