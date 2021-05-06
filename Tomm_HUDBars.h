#pragma once

using namespace std;


#define k_HBMeterTypeHorizontal 0
#define k_HBMeterTypeVertical 1
#define k_HBMeterTypeAlpha 2
#define k_HBMeterTypeText 3
#define k_HBMeterTypeTextPercentage 4

#define	k_HBValueTypeNoScriptVar 0
#define k_HBValueTypeScriptVar 1
#define k_HBValueTypeScriptFunction 2

int iSkipFrame;



class HUDBarsInterface
{
public:

	class HudBar
	{
	public:

		//Main
		int ID = 0;
		int iBarVisible = 0;
		int iHidden = 0;
		int iShowMaxValue = 0;
		int iRemoveOnGameLoad = 0;



		char Name[0x4000]{};
		int MeterType = k_HBMeterTypeHorizontal;
		int ValueType = k_HBValueTypeNoScriptVar;
		int TileTextAdded = 0;
		int TileFrameAdded = 0;
		int TileMeterAdded = 0;
		int TileImageExAdded = 0;
		float Value = 0;
		float MaxValue = 100;
		UInt8 modIdx = -1;

		//Tiles
		Tile* TileRect = NULL;
		Tile* TileFrame = NULL;
		Tile* TileMeter = NULL;
		Tile* TileText = NULL;
		Tile* TileImageEx = NULL;

		//Gradual Vanishing\appearing
		int iGradualVanishing = 1;
		int iGradualAppearing = 1;
		int iGradualVanishingMovement = 1;
		int iGradualAppearingMovement = 1;
		float fGradualVanishingPosShift = -200;
		float fGradualAppearingPosShift = -200;
		float fGradualVanishingTimer = 0.2;
		float fGradualAppearingTimer = 0.2;
		float fRectPosX = 0;
		float fRectPosY = 0;

		//Size
		float MeterWidth = 0;
		float MeterWidthAlt = 0;
		float MeterHeight = 0;
		float MeterHeightAlt = 0;
		float FrameWidth = 0;
		float FrameHeight = 0;
		float FrameWidthAlt = 0;
		float FrameHeightAlt = 0;
		float ImageExWidth = 0;
		float ImageExHeight = 0;
		float ImageExWidthAlt = 0;
		float ImageExHeightAlt = 0;

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


		//UpdateFrame
		float fCurTimer = 0.1;
		float fUpdateTimer = 0.1;


		//VisibilityFlags
		int iMenuTypeShow = 0;
		int iDisableControlMovement = 0;
		int iDisableWhenSneaking = 0;
		int iDisableWhenCombat = 0;
		int iDisableWhenAiming = 0;


		bool operator==(const HudBar& other) const
		{
			return ID == other.ID;
		}

	};


};



typedef HUDBarsInterface::HudBar HUDBarElement;

int g_HudBarsArraySize = 0;


vector<HUDBarElement> g_HUDBArsArrayV;
int g_HUDBArsArrayID = 0;



void f_Bars_BarSetHidden(int iKey, int iRequest)
{
	HudBarIterElement.TileRect->SetFloat(kTileValue_visible, iRequest);
}





vector<HUDBarElement>::iterator f_Bars_BarDestroy(vector<HUDBarElement>::iterator it)
{

	_MESSAGE("Destroying bar named::->>%s, ID::%d", HudBarIterElementV.Name, HudBarIterElementV.ID);

	HudBarIterElementV.TileRect->Destroy(true);
	g_HudBarsArraySize -= 1;
	it = g_HUDBArsArrayV.erase(it);
	return it;
}


void f_Bars_BarRemoveOnGameLoad()
{
	if (g_HudBarsArraySize > 0)
	{

			vector<HUDBarElement>::iterator it;
			for (it = g_HUDBArsArrayV.begin(); it != g_HUDBArsArrayV.end();)
			{

				if ( HudBarIterElementV.iRemoveOnGameLoad == 1)
				{
					it = f_Bars_BarDestroy(it);
				}
				else
				{++it;}
			}
		}

		if (g_HUDBArsArrayV.size() == 0)
		{
			g_HudBarsArraySize = 0;
		}
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
		if (HudBarIterElement.iHidden == 0)
		{
			fBarVarValue *= HudBarIterElement.fAlphaDifference;
			fBarVarValue += HudBarIterElement.fAlphaMin;
			HudBarIterElement.TileMeter->SetFloat(kTileValue_alpha, fBarVarValue);
		}

		break;
	case k_HBMeterTypeText:
		//_MESSAGE("Setting k_HBMeterTypeText");
		//Console_Print("HudBarIterElement.MaxValue is %f ", HudBarIterElement.MaxValue);

		if (HudBarIterElement.iShowMaxValue == 1)
		{sprintf(TempText, "%s%.0f/%.0f%s", HudBarIterElement.Prefix, fBarVarValue, HudBarIterElement.MaxValue, HudBarIterElement.PostFix);}
		else{sprintf(TempText, "%s%.0f%s", HudBarIterElement.Prefix, fBarVarValue, HudBarIterElement.PostFix);}

		HudBarIterElement.TileText->SetString(kTileValue_string, TempText);
		break;
	case k_HBMeterTypeTextPercentage:
		//_MESSAGE("Setting k_HBMeterTypeTextPercentage");
		int iToShow = fBarVarValue * 100;
		sprintf(TempText, "%s%d%s", HudBarIterElement.Prefix, iToShow, HudBarIterElement.PostFix);
		HudBarIterElement.TileText->SetString(kTileValue_string, TempText);
		break;
		//default:
			//break;
	}

	return 1;
}



plf::nanotimer g_timer;
float g_TimeElapsed = 0;
float g_TimerEvalHBVars = 0;
int	  g_EvalHBVarsTime = 0;
int	  g_TimerStarted = 0;

void f_Bars_Iterate()
{
	_MESSAGE("Script execution START");
	plf::nanotimer perf_timer;
	perf_timer.start();

	float elapsedTime = g_timer.get_elapsed_ms() - g_TimeElapsed;
	g_TimeElapsed = g_timer.get_elapsed_ms();
	//Console_Print("Elapsed ms::%f", elapsedTime);
	//Console_Print("g_TimeElapsed ms::%f", g_TimeElapsed);
	//Console_Print("g_timer.get_elapsed_ms() ms::%f", g_timer.get_elapsed_ms());


	

	ScriptVar* ScriptVariable = nullptr;
	float fMaxValue = 0, fWidth = 0;
	Tile* TileMeter;
	float fBarVarValue = 0;
	int iMeterType = 0, iValueType  = 0;
	int iHide = 0;


	//for (HUDBarElement& it2 : g_HUDBArsArrayV) {
	//	it2.
	//}

	//for (HUDBarElement& it2 : g_HUDBArsArrayV) {
	//iSkipFrame += 1;
	//if (iSkipFrame > 6)
	//	iSkipFrame = 0;

	//g_TimeEvalVars

	if (g_TimerEvalHBVars < 0)
	{
		g_TimerEvalHBVars = 0.1;
		g_EvalHBVarsTime = 1;
	}
	else {
		g_TimerEvalHBVars -= elapsedTime;
	}




	for (std::vector<HUDBarElement>::iterator it = g_HUDBArsArrayV.begin(); it != g_HUDBArsArrayV.end(); ++it)
	{
		//Console_Print("Current Bar  is %s ", HudBarIterElementV.Name);
		//Console_Print("Current Bar vis is %d ", HudBarIterElementV.iBarVisible);

		if (HudBarIterElementV.iBarVisible) {}
		else { continue; }

		iHide = 0;

		_MESSAGE("Current Bar VISIBLE is %s ", HudBarIterElementV.Name);

		if (g_EvalHBVarsTime == 1)
		{
			if (HudBarIterElementV.iMenuTypeShow && HudBarIterElementV.iMenuTypeShow != g_interfaceManager->currentMode)
			{
				iHide = 1;
			}

			if (HudBarIterElementV.iDisableControlMovement)
			{
				if (g_ThePlayer->pcControlFlags & (1 << 0))
				{
					iHide = 1;
				}
			}

			if (HudBarIterElementV.iDisableWhenCombat)
			{
				if (HudBarIterElementV.iDisableWhenCombat == 1)
				{
					if (g_ThePlayer->pcInCombat)
					{
						iHide = 1;
					}
				}
				else {

					if (g_ThePlayer->pcInCombat == 0)
					{
						iHide = 1;
					}


				}


			}

			if (HudBarIterElementV.iDisableWhenSneaking)
			{
				if (HudBarIterElementV.iDisableWhenSneaking == 1)
				{
					if ((g_ThePlayer->GetMovementFlags() & 0xC00) == 0x400) // From JiP
					{
						iHide = 1;
					}
				}
				else {
					if ((g_ThePlayer->GetMovementFlags() & 0xC00) != 0x400) 
					{
						iHide = 1;
					}
				}


			}

			if (HudBarIterElementV.iDisableWhenAiming)
			{
				if (HudBarIterElementV.iDisableWhenAiming == 1)
				{
					if (g_HUDMainMenu->isUsingScope || g_ThePlayer->ironSightNode && g_ThePlayer->baseProcess->IsWeaponOut())
					{
						iHide = 1;
					}

				}
				else {
					if (g_HUDMainMenu->isUsingScope == 0 && (g_ThePlayer->ironSightNode && g_ThePlayer->baseProcess->IsWeaponOut()) == 0)
					{
						iHide = 1;
					}


				}

			}

			if (iHide)
			{
				if (HudBarIterElementV.iHidden)
				{}
				else {
					//_MESSAGE("HIDING BAR:: %s ", HudBarIterElementV.Name);
					HudBarIterElementV.iHidden = 1;
					ThisCall(0xA07DC0, HudBarIterElementV.TileFrame, kTileValue_alpha);
					ThisCall(0xA07DC0, HudBarIterElementV.TileText, kTileValue_alpha);
					ThisCall(0xA07DC0, HudBarIterElementV.TileMeter, kTileValue_alpha);
					ThisCall(0xA07DC0, HudBarIterElementV.TileImageEx, kTileValue_alpha);

					if (HudBarIterElementV.iGradualVanishingMovement)
					{
						switch (HudBarIterElementV.iGradualVanishingMovement) {
							case 1:
								ThisCall(0xA07DC0, HudBarIterElementV.TileMeter, kTileValue_x);
								HudBarIterElementV.TileRect->GradualSetFloat(kTileValue_x, HudBarIterElementV.fRectPosX, HudBarIterElementV.fRectPosX + HudBarIterElementV.fGradualVanishingPosShift, HudBarIterElementV.fGradualVanishingTimer, 0);
								break;
							case 2:
								ThisCall(0xA07DC0, HudBarIterElementV.TileMeter, kTileValue_y);
								HudBarIterElementV.TileRect->GradualSetFloat(kTileValue_y, HudBarIterElementV.fRectPosY, HudBarIterElementV.fRectPosY + HudBarIterElementV.fGradualVanishingPosShift, HudBarIterElementV.fGradualVanishingTimer, 0);
								break;
						}
					}

					if (HudBarIterElementV.iGradualVanishing)
					{
						HudBarIterElementV.TileFrame->GradualSetFloat(kTileValue_alpha, 255, 0, HudBarIterElementV.fGradualVanishingTimer, 0);
						HudBarIterElementV.TileText->GradualSetFloat(kTileValue_alpha, 255, 0, HudBarIterElementV.fGradualVanishingTimer, 0);
						HudBarIterElementV.TileMeter->GradualSetFloat(kTileValue_alpha, 255, 0, HudBarIterElementV.fGradualVanishingTimer, 0);
						HudBarIterElementV.TileImageEx->GradualSetFloat(kTileValue_alpha, 255, 0, HudBarIterElementV.fGradualVanishingTimer, 0);
					}
					else {
						HudBarIterElementV.TileFrame->SetFloat(kTileValue_alpha, 0);
						HudBarIterElementV.TileText->SetFloat(kTileValue_alpha, 0);
						HudBarIterElementV.TileMeter->SetFloat(kTileValue_alpha, 0);
						HudBarIterElementV.TileImageEx->SetFloat(kTileValue_alpha, 0);
					}

				}
				continue;
			}
			else {
				if (HudBarIterElementV.iHidden)
				{
					//_MESSAGE("SHOWING BAR:: %s ", HudBarIterElementV.Name);

					HudBarIterElementV.iHidden = 0;


					if (HudBarIterElementV.iGradualAppearingMovement)
					{
						switch (HudBarIterElementV.iGradualAppearingMovement) {
						case 1:
							ThisCall(0xA07DC0, HudBarIterElementV.TileMeter, kTileValue_x);
							HudBarIterElementV.TileRect->GradualSetFloat(kTileValue_x, HudBarIterElementV.fRectPosX + HudBarIterElementV.fGradualAppearingPosShift, HudBarIterElementV.fRectPosX, HudBarIterElementV.fGradualAppearingTimer, 0);
							break;
						case 2:
							ThisCall(0xA07DC0, HudBarIterElementV.TileMeter, kTileValue_y);
							HudBarIterElementV.TileRect->GradualSetFloat(kTileValue_y, HudBarIterElementV.fRectPosY + HudBarIterElementV.fGradualAppearingPosShift, HudBarIterElementV.fRectPosY, HudBarIterElementV.fGradualAppearingTimer, 0);
							break;
						}
					}





					ThisCall(0xA07DC0, HudBarIterElementV.TileFrame, kTileValue_alpha);
					ThisCall(0xA07DC0, HudBarIterElementV.TileText, kTileValue_alpha);
					ThisCall(0xA07DC0, HudBarIterElementV.TileMeter, kTileValue_alpha);
					ThisCall(0xA07DC0, HudBarIterElementV.TileImageEx, kTileValue_alpha);

					if (HudBarIterElementV.iGradualAppearing)
					{
						HudBarIterElementV.TileFrame->GradualSetFloat(kTileValue_alpha, 0, 255, HudBarIterElementV.fGradualAppearingTimer, 0);
						HudBarIterElementV.TileText->GradualSetFloat(kTileValue_alpha, 0, 255, HudBarIterElementV.fGradualAppearingTimer, 0);
						HudBarIterElementV.TileMeter->GradualSetFloat(kTileValue_alpha, 0, 255, HudBarIterElementV.fGradualAppearingTimer, 0);
						HudBarIterElementV.TileImageEx->GradualSetFloat(kTileValue_alpha, 0, 255, HudBarIterElementV.fGradualAppearingTimer, 0);
					}
					else {
						HudBarIterElementV.TileFrame->SetFloat(kTileValue_alpha, 0);
						HudBarIterElementV.TileText->SetFloat(kTileValue_alpha, 0);
						HudBarIterElementV.TileMeter->SetFloat(kTileValue_alpha, 0);
						HudBarIterElementV.TileImageEx->SetFloat(kTileValue_alpha, 0);
					}
				}
			}

		}


		if (HudBarIterElementV.ValueType > k_HBValueTypeNoScriptVar) {}
		else { continue; }

		if (HudBarIterElementV.fCurTimer < 0)
		{
			HudBarIterElementV.fCurTimer = HudBarIterElementV.fUpdateTimer;
			_MESSAGE("SETTING VAL");
		}
		else {
			HudBarIterElementV.fCurTimer -= elapsedTime;
			_MESSAGE("CONTINUING WITHOUT SETTING VAL");
			continue;
		}
		

		if (HudBarIterElementV.ValueType == k_HBValueTypeScriptVar)
		{
			ScriptVariable = HudBarIterElementV.ScriptVariable;
			fBarVarValue = ScriptVariable->data.num;
			if (HudBarIterElementV.iScriptValueCalculateMax)
			{
				fMaxValue = HudBarIterElementV.MaxValue;
				fBarVarValue = fBarVarValue / fMaxValue;
				if (fBarVarValue > 1)
					fBarVarValue = 1;
				else if (fBarVarValue < 0)
					fBarVarValue = 0;
			}
		}else if (HudBarIterElementV.ValueType == k_HBValueTypeScriptFunction)
		{
			NVSEArrayElement element;
			element.num = std::numeric_limits<float>::quiet_NaN();

			if (HudBarIterElementV.FunctionToCall)
			{
				if (HudBarIterElementV.FunctionCaller)
				{
					FunctionCallScript(HudBarIterElementV.FunctionToCall, HudBarIterElementV.FunctionCaller, NULL, &element, NULL);
				}
				else {
					FunctionCallScript(HudBarIterElementV.FunctionToCall, NULL, NULL, &element, NULL);
				}

				if (element.num != std::numeric_limits<float>::quiet_NaN())
				{
					fBarVarValue = element.num;
					if (HudBarIterElementV.iScriptValueCalculateMax)
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
					_MESSAGE("Function result is not valid for bar->>%s", HudBarIterElementV.Name);
				}



			}
			else {
				_MESSAGE("Current FUNCTION DOESNT EXIST");
			}

		}

		//_MESSAGE("NAME for scriptVAR-->> %s", HudBarIterElementV.Name);

		f_Bars_BarSetBarValue(it - g_HUDBArsArrayV.begin(),fBarVarValue);

	}

	g_EvalHBVarsTime = 0;


	_MESSAGE("Script execution END ms::%f", perf_timer.get_elapsed_ms());
}




//Console_Print("HUDBARAREAYSIZE is %d", std::size(g_HUDBArsArray));
//Console_Print("g_HUDBArsArray SIZE is %d", g_HudBarsArraySize);
//for (HUDBarElement IterElement : g_HUDBArsArray)
//{}


