#pragma once

class HUDBarsInterface
{
public:

	class HudBar
	{
	public:
		int iBarVisible = 0;
		char Name[0x4000]{};
		int MeterType = 0;
		int ValueType = 0;

		Tile* TileRect = NULL;


		Tile* TileMeter = NULL;
		float MeterWidth = 0;
		float MeterHeight = 0;



		float FrameWidth = 0;
		float FrameHeight = 0;

		ScriptVar* ScriptVariable = NULL;
		float Value = 0;
		float MaxValue = 100;
		Tile* FrameImage = NULL;
		Tile* MeterImage = NULL;
		Tile* MeterText = NULL;

		char Prefix[0x4000]{};
		char PostFix[0x4000]{};

	};


};



typedef HUDBarsInterface::HudBar HUDBarElement;
HUDBarElement g_HUDBArsArray[100]{};
int g_HudBarsArraySize = 0;


void f_Bars_Iterate()
{

	ScriptVar* ScriptVariable = NULL;
	float fMaxValue = 0, fWidth = 0, fValueSet = 0;
	Tile* TileMeter;
	float fBarVarValue = 0;
	int iMeterType = 0, iValueType  = 0;
	for (int iKey = 0; iKey < g_HudBarsArraySize; iKey++) {
		//Console_Print("NAME is %s", IterElement.Name);

		if (HudBarIterElement.iBarVisible){}else{ continue; }
		


		iMeterType = HudBarIterElement.MeterType;
		iValueType = HudBarIterElement.ValueType;
		ScriptVariable = HudBarIterElement.ScriptVariable;
		fMaxValue = HudBarIterElement.MaxValue;
		TileMeter = HudBarIterElement.TileMeter;

		_MESSAGE("iMeterType is %d", iMeterType);
		_MESSAGE("iValueType is %d", iValueType);
		_MESSAGE("fMaxValue is %f", HudBarIterElement.MaxValue);
		_MESSAGE("IterElement.MeterWidth is %f", HudBarIterElement.MeterWidth);

	
		switch (iValueType) {
		case 0:
			fBarVarValue = ScriptVariable->data.num;

			fValueSet = fBarVarValue / fMaxValue;
			if (fValueSet > 1)
				fValueSet = 1;
			else if (fValueSet < 0)
				fValueSet = 0;
			break;
		case 1:
			fBarVarValue = HudBarIterElement.Value;
			fValueSet = fBarVarValue / fMaxValue;
			if (fValueSet > 1)
				fValueSet = 1;
			else if (fValueSet < 0)
				fValueSet = 0;
			break;
		case 2:
			fBarVarValue = HudBarIterElement.Value;
			break;
		}



	
		switch (iMeterType) {
		case 0:
			fValueSet *= HudBarIterElement.MeterWidth;
			TileMeter->SetFloat(kTileValue_width, fValueSet);
			break;
		case 1:
			fValueSet *= HudBarIterElement.MeterHeight;
			TileMeter->SetFloat(kTileValue_height, fValueSet);
			break;

		case 2:
			fValueSet *= 255;
			TileMeter->SetFloat(kTileValue_alpha, fValueSet);
			break;

		case 3:
			fValueSet = fBarVarValue / fMaxValue;
			if (fValueSet > 1)
				fValueSet = 1;
			else if (fValueSet < 0)
				fValueSet = 0;
			fValueSet *= 255;
			TileMeter->SetFloat(kTileValue_alpha, fValueSet);
			break;

		case 4:
			char TempText[0x4000]{};
			int iToShow = fBarVarValue;
			sprintf(TempText, "%s%d%s", HudBarIterElement.Prefix, iToShow, HudBarIterElement.PostFix);
			TileMeter->SetString(kTileValue_string, TempText);
		//default:
			//break;
		}

	}

}




//Console_Print("HUDBARAREAYSIZE is %d", std::size(g_HUDBArsArray));
//Console_Print("g_HUDBArsArray SIZE is %d", g_HudBarsArraySize);
//for (HUDBarElement IterElement : g_HUDBArsArray)
//{}


