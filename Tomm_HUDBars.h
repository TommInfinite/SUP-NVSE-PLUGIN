#pragma once

using namespace std;


#define k_HBMeterTypeHorizontal 0
#define k_HBMeterTypeVertical 1
#define k_HBMeterTypeAlpha 2
#define k_HBMeterTypeText 3
#define k_HBMeterTypeTextPercentage 4
#define k_HBMeterTypeSize 5

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

		float IndentMeter = 0;
		float IndentFrame = 0;
		float IndentImageEx = 0;



		//ValueTypeScript
		ScriptVar* ScriptVariable = NULL;
		Script* FunctionToCall = NULL;
		TESObjectREFR* FunctionCaller = NULL;
		int iScriptValueCalculateMax = 0;

		//MeterTypeAlpha
		float fAlphaMin = 0;
		float fAlphaMax = 255;
		float fAlphaDifference = 255;

		//MeterTypeSize
		float fSizeXMin = 0;
		float fSizeXMax = 0;
		float fSizeXDelta =0;
		float fSizeYMin = 0;
		float fSizeYMax = 0;
		float fSizeYDelta = 0;

		float fMeterPosX = 0;
		float fMeterPosY = 0;

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
		int iDisableWhenWeaponOut = 0;
		int iDisableWhenNotEquipped = 0;
		int iHookedItemIsAList = 0;
		TESForm* HookedItem = NULL;
		BGSListForm* listForm = NULL;
		int iCameraModeTypeShow = 0;
	

		bool operator==(const HudBar& other) const
		{
			return ID == other.ID;
		}

	};


};



typedef HUDBarsInterface::HudBar HUDBarElement;


int g_HudBarsIterate = 0;

vector<HUDBarElement> g_HUDBArsArrayV;
int g_HUDBArsArrayID = 0;



void f_Bars_BarSetHidden(int iKey, int iRequest)
{
	HBIter.TileRect->SetFloat(kTileValue_visible, iRequest);
}





vector<HUDBarElement>::iterator f_Bars_BarDestroy(vector<HUDBarElement>::iterator it)
{

	_MESSAGE("Destroying bar named::->>%s, ID::%d", HBIterV.Name, HBIterV.ID);

	HBIterV.TileRect->Destroy(true);

	it = g_HUDBArsArrayV.erase(it);

	if (g_HUDBArsArrayV.size() == 0)
	{
		g_HudBarsIterate = 0;
	}



	return it;
}


void f_Bars_BarRemoveOnGameLoad()
{

			vector<HUDBarElement>::iterator it;
			for (it = g_HUDBArsArrayV.begin(); it != g_HUDBArsArrayV.end();)
			{

				if ( HBIterV.iRemoveOnGameLoad == 1)
				{
					it = f_Bars_BarDestroy(it);
				}
				else
				{++it;}
			}


		if (g_HUDBArsArrayV.size() == 0)
		{
			g_HudBarsIterate = 0;
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
	int iToShow = 0;

	char TempText[0x4000]{};
	switch (HBIter.MeterType) {
	case k_HBMeterTypeHorizontal:

		if (HBIter.TileMeter->GetValue(kTileValue_zoom)->num == -1)
		{
			float fAddPosX = HBIter.IndentMeter * (1 - fBarVarValue);
			fBarVarValue *= HBIter.MeterWidth;
			HBIter.TileMeter->SetFloat(kTileValue_width, fBarVarValue);
			HBIter.TileMeter->SetFloat(kTileValue_x, fAddPosX);


		}
		else {
			fBarVarValue *= HBIter.MeterWidthAlt;
			fBarVarValue += HBIter.IndentMeter;
			HBIter.TileMeter->SetFloat(kTileValue_width, fBarVarValue);
		}


		break;
	case k_HBMeterTypeVertical:

		if (HBIter.TileMeter->GetValue(kTileValue_zoom)->num == -1)
		{
			float fAddPosY = HBIter.IndentMeter * (1 - fBarVarValue);
			fBarVarValue *= HBIter.MeterHeight;
			HBIter.TileMeter->SetFloat(kTileValue_height, fBarVarValue);
			HBIter.TileMeter->SetFloat(kTileValue_y, fAddPosY);
		}
		else {
			fBarVarValue *= HBIter.MeterHeightAlt;
			fBarVarValue += HBIter.IndentMeter;
			HBIter.TileMeter->SetFloat(kTileValue_height, fBarVarValue);
		}
		break;

	case k_HBMeterTypeAlpha:
		if (HBIter.iHidden == 0)
		{
			//Console_Print("fBarVarValue START is %f", fBarVarValue);

			if (IsGradualSetFloat(HBIter.TileMeter, kTileValue_alpha) == 0)
			{
				fBarVarValue *= HBIter.fAlphaDifference;
				fBarVarValue += HBIter.fAlphaMin;
				HBIter.TileMeter->SetFloat(kTileValue_alpha, fBarVarValue);
			}
			else {
				//Console_Print("ALPHA GRADUAL SET FLOAT");

			}


			//Console_Print("HBIter.fAlphaDifference is %f", HBIter.fAlphaDifference);
			//Console_Print("HBIter.fAlphaMax is %f", HBIter.fAlphaMax);
			//Console_Print("HBIter.fAlphaMin is %f", HBIter.fAlphaMin);
			//Console_Print("fBarVarValue END is %f", fBarVarValue);
		}

		break;
	case k_HBMeterTypeText:
		//_MESSAGE("Setting k_HBMeterTypeText");
		//Console_Print("HudBarIterElement.MaxValue is %f ", HudBarIterElement.MaxValue);

		if (HBIter.iShowMaxValue == 1)
		{sprintf(TempText, "%s%.0f/%.0f%s", HBIter.Prefix, fBarVarValue, HBIter.MaxValue, HBIter.PostFix);}
		else{sprintf(TempText, "%s%.0f%s", HBIter.Prefix, fBarVarValue, HBIter.PostFix);}

		HBIter.TileText->SetString(kTileValue_string, TempText);
		break;
	case k_HBMeterTypeTextPercentage:
		//_MESSAGE("Setting k_HBMeterTypeTextPercentage");
		iToShow = fBarVarValue * 100;
		sprintf(TempText, "%s%d%s", HBIter.Prefix, iToShow, HBIter.PostFix);
		HBIter.TileText->SetString(kTileValue_string, TempText);
		break;
	case k_HBMeterTypeSize:

		float fSizeX = HBIter.fSizeXMin + ((fBarVarValue * 100) * HBIter.fSizeXDelta);
		float fPosX = HBIter.fMeterPosX - (0.5 * fSizeX);

		float fSizeY = HBIter.fSizeYMin + ((fBarVarValue * 100) * HBIter.fSizeYDelta);
		float fPosY = HBIter.fMeterPosY - (0.5 * fSizeY);

		//Console_Print("HBIter.fSizeXDelta is %f", HBIter.fSizeXDelta);
		//Console_Print("HBIter.fSizeYDelta is %f", HBIter.fSizeYDelta);

		HBIter.TileMeter->SetFloat(kTileValue_width, fSizeX);
		HBIter.TileMeter->SetFloat(kTileValue_height, fSizeY);
		HBIter.TileMeter->SetFloat(kTileValue_x, fPosX);
		HBIter.TileMeter->SetFloat(kTileValue_y, fPosY);

		break;

		
	}

	return 1;
}



plf::nanotimer g_timer;
float g_TimeElapsed = 0;
float g_TimerEvalHBVars = 0;
int	  g_EvalHBVarsTime = 0;
int	  g_TimerStarted = 0;

int g_iMyInt;

void f_Bars_Iterate()
{
	//_MESSAGE("Script execution START");
	//plf::nanotimer perf_timer;
	//perf_timer.start();








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

		if (HBIterV.iBarVisible) {}
		else { continue; }

		iHide = 0;

		//_MESSAGE("Current Bar VISIBLE is %s ", HBIterV.Name);

		if (g_EvalHBVarsTime == 1)
		{
			if (HBIterV.iMenuTypeShow && HBIterV.iMenuTypeShow != g_interfaceManager->currentMode)
			{
				iHide = 1;
			}

			if (HBIterV.iDisableControlMovement)
			{
				if (g_ThePlayer->pcControlFlags & (1 << 0))
				{
					iHide = 1;
				}
			}

			if (HBIterV.iDisableWhenCombat)
			{
				if (HBIterV.iDisableWhenCombat == 1)
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

			if (HBIterV.iDisableWhenSneaking)
			{
				if (HBIterV.iDisableWhenSneaking == 1)
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

			if (HBIterV.iDisableWhenNotEquipped)
			{
				//_MESSAGE("HudBarIterElementV.iDisableWhenNotEquipped::%s", HBIterV.Name);


					if (HBIterV.iHookedItemIsAList)
					{
						if (HBIterV.listForm)
						{
							//_MESSAGE("HudBarIterElementV.iHookedItemIsAList == 1::%s", HBIterV.Name);
							TESForm* item;
							int iFoundItem = 0;
							ListNode<TESForm>* traverse = HBIterV.listForm->list.Head();
							do
							{
								//_MESSAGE("TRAVERSING THROUGH LIST::%s", HBIterV.Name);
								if (traverse->data)
								{
									//_MESSAGE("ITEM EXISTS::: %s", traverse->data->GetName());
									if (g_ThePlayer->IsItemEquipped(traverse->data) == 1)
									{
										//_MESSAGE("FOUND ITEM", HBIterV.Name);
										iFoundItem = 1;
										break;
									}

								}
							}
							while (traverse = traverse->next);

							if (iFoundItem == 0)
							{
								iHide = 1;
							}

						}
					}
					else
					{
						if (HBIterV.HookedItem)
						{
							//_MESSAGE("CHECKING IF PLAYER EQUIPS::%s", HudBarIterElementV.HookedItem->GetName());
							//Console_Print("CHECKING IF PLAYER EQUIPS::%s", HudBarIterElementV.HookedItem->GetName());
							if (g_ThePlayer->IsItemEquipped(HBIterV.HookedItem) == 0)
							{
								iHide = 1;
								//_MESSAGE("HIDING BECAUSE ITEM IS NOT EQUIPPED FOR::%s", HudBarIterElementV.Name);
								//_MESSAGE("ITEM IS::%s", HudBarIterElementV.HookedItem->GetName());
							}
						}
					}
				}
				else {

					//_MESSAGE("NO HOOKED ITEM::%s", HudBarIterElementV.Name);
				}


			if (HBIterV.iDisableWhenAiming)
			{
				if (HBIterV.iDisableWhenAiming == 1)
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

			if (HBIterV.iDisableWhenWeaponOut)
			{
				if (HBIterV.iDisableWhenWeaponOut == 1)
				{
					if (g_ThePlayer->baseProcess->IsWeaponOut())
					{
						iHide = 1;
					}
				}
				else {
					if (g_ThePlayer->baseProcess->IsWeaponOut() == 0)
					{
						iHide = 1;
					}
				}

			}


			if (HBIterV.iCameraModeTypeShow)
			{
				if (HBIterV.iCameraModeTypeShow == 1)
				{
					if (g_ThePlayer->bThirdPerson)
					{
						iHide = 1;
					}
				}
				else {
					if (g_ThePlayer->bThirdPerson == 0)
					{
						iHide = 1;
					}
				}

			}


			



			if (iHide)
			{
				if (HBIterV.iHidden)
				{}
				else {
					//_MESSAGE("HIDING BAR:: %s ", HudBarIterElementV.Name);
					HBIterV.iHidden = 1;
					ThisCall(0xA07DC0, HBIterV.TileFrame, kTileValue_alpha);
					ThisCall(0xA07DC0, HBIterV.TileText, kTileValue_alpha);
					ThisCall(0xA07DC0, HBIterV.TileMeter, kTileValue_alpha);
					ThisCall(0xA07DC0, HBIterV.TileImageEx, kTileValue_alpha);

					if (HBIterV.iGradualVanishingMovement)
					{
						switch (HBIterV.iGradualVanishingMovement) {
							case 1:
								ThisCall(0xA07DC0, HBIterV.TileMeter, kTileValue_x);
								HBIterV.TileRect->GradualSetFloat(kTileValue_x, HBIterV.fRectPosX, HBIterV.fRectPosX + HBIterV.fGradualVanishingPosShift, HBIterV.fGradualVanishingTimer, 0);
								break;
							case 2:
								ThisCall(0xA07DC0, HBIterV.TileMeter, kTileValue_y);
								HBIterV.TileRect->GradualSetFloat(kTileValue_y, HBIterV.fRectPosY, HBIterV.fRectPosY + HBIterV.fGradualVanishingPosShift, HBIterV.fGradualVanishingTimer, 0);
								break;
						}
					}

					if (HBIterV.iGradualVanishing)
					{

						HBIterV.TileMeter->GradualSetFloat(kTileValue_alpha, HBIterV.TileMeter->GetValueFloat(kTileValue_alpha), 0, HBIterV.fGradualVanishingTimer, 0);
						HBIterV.TileFrame->GradualSetFloat(kTileValue_alpha, 255, 0, HBIterV.fGradualVanishingTimer, 0);
						HBIterV.TileText->GradualSetFloat(kTileValue_alpha, 255, 0, HBIterV.fGradualVanishingTimer, 0);
						HBIterV.TileImageEx->GradualSetFloat(kTileValue_alpha, 255, 0, HBIterV.fGradualVanishingTimer, 0);
					}
					else {
						HBIterV.TileFrame->SetFloat(kTileValue_alpha, 0);
						HBIterV.TileText->SetFloat(kTileValue_alpha, 0);
						HBIterV.TileMeter->SetFloat(kTileValue_alpha, 0);
						HBIterV.TileImageEx->SetFloat(kTileValue_alpha, 0);
					}

				}
				continue;
			}
			else {
				if (HBIterV.iHidden)
				{
					//_MESSAGE("SHOWING BAR:: %s ", HudBarIterElementV.Name);

					HBIterV.iHidden = 0;


					if (HBIterV.iGradualAppearingMovement)
					{
						switch (HBIterV.iGradualAppearingMovement) {
						case 1:
							ThisCall(0xA07DC0, HBIterV.TileMeter, kTileValue_x);
							HBIterV.TileRect->GradualSetFloat(kTileValue_x, HBIterV.fRectPosX + HBIterV.fGradualAppearingPosShift, HBIterV.fRectPosX, HBIterV.fGradualAppearingTimer, 0);
							break;
						case 2:
							ThisCall(0xA07DC0, HBIterV.TileMeter, kTileValue_y);
							HBIterV.TileRect->GradualSetFloat(kTileValue_y, HBIterV.fRectPosY + HBIterV.fGradualAppearingPosShift, HBIterV.fRectPosY, HBIterV.fGradualAppearingTimer, 0);
							break;
						}
					}





					ThisCall(0xA07DC0, HBIterV.TileFrame, kTileValue_alpha);
					ThisCall(0xA07DC0, HBIterV.TileText, kTileValue_alpha);
					ThisCall(0xA07DC0, HBIterV.TileMeter, kTileValue_alpha);
					ThisCall(0xA07DC0, HBIterV.TileImageEx, kTileValue_alpha);

					if (HBIterV.iGradualAppearing)
					{
						HBIterV.TileFrame->GradualSetFloat(kTileValue_alpha, 0, 255, HBIterV.fGradualAppearingTimer, 0);
						HBIterV.TileText->GradualSetFloat(kTileValue_alpha, 0, 255, HBIterV.fGradualAppearingTimer, 0);
						HBIterV.TileMeter->GradualSetFloat(kTileValue_alpha, 0, 255, HBIterV.fGradualAppearingTimer, 0);
						HBIterV.TileImageEx->GradualSetFloat(kTileValue_alpha, 0, 255, HBIterV.fGradualAppearingTimer, 0);
					}
					else {
						HBIterV.TileFrame->SetFloat(kTileValue_alpha, 255);
						HBIterV.TileText->SetFloat(kTileValue_alpha, 255);
						HBIterV.TileMeter->SetFloat(kTileValue_alpha, 255);
						HBIterV.TileImageEx->SetFloat(kTileValue_alpha, 255);
					}
				}
			}

		}


		if (HBIterV.ValueType > k_HBValueTypeNoScriptVar) {}
		else { continue; }

		if (HBIterV.fCurTimer < 0)
		{
			HBIterV.fCurTimer = HBIterV.fUpdateTimer;
			//_MESSAGE("SETTING VAL");
		}
		else {
			HBIterV.fCurTimer -= elapsedTime;
			//_MESSAGE("CONTINUING WITHOUT SETTING VAL");
			continue;
		}
		

		if (HBIterV.ValueType == k_HBValueTypeScriptVar)
		{
			ScriptVariable = HBIterV.ScriptVariable;
			fBarVarValue = ScriptVariable->data.num;
			if (HBIterV.iScriptValueCalculateMax)
			{
				fMaxValue = HBIterV.MaxValue;
				fBarVarValue = fBarVarValue / fMaxValue;
				if (fBarVarValue > 1)
					fBarVarValue = 1;
				else if (fBarVarValue < 0)
					fBarVarValue = 0;
			}
		}else if (HBIterV.ValueType == k_HBValueTypeScriptFunction)
		{
			NVSEArrayElement element;
			element.num = std::numeric_limits<float>::quiet_NaN();

			if (HBIterV.FunctionToCall)
			{
				if (HBIterV.FunctionCaller)
				{
					FunctionCallScript(HBIterV.FunctionToCall, HBIterV.FunctionCaller, NULL, &element, NULL);
				}
				else {
					FunctionCallScript(HBIterV.FunctionToCall, NULL, NULL, &element, NULL);
				}

				if (element.num != std::numeric_limits<float>::quiet_NaN())
				{
					fBarVarValue = element.num;
					if (HBIterV.iScriptValueCalculateMax)
					{
						fBarVarValue = fBarVarValue / fMaxValue;
						if (fBarVarValue > 1)
							fBarVarValue = 1;
						else if (fBarVarValue < 0)
							fBarVarValue = 0;
						//Console_Print("fBarVarValue is %f ", element.num);
					}
	
	
				}else{
					fBarVarValue = 0;
					//_MESSAGE("Function result is not valid for bar->>%s", HBIterV.Name);
				}



			}
			else {
				//_MESSAGE("Current FUNCTION DOESNT EXIST");
			}

		}

		//_MESSAGE("NAME for scriptVAR-->> %s", HudBarIterElementV.Name);

		f_Bars_BarSetBarValue(it - g_HUDBArsArrayV.begin(),fBarVarValue);

	}

	g_EvalHBVarsTime = 0;


	//_MESSAGE("Script execution END ms::%f", perf_timer.get_elapsed_ms());
}




//Console_Print("HUDBARAREAYSIZE is %d", std::size(g_HUDBArsArray));
//Console_Print("g_HUDBArsArray SIZE is %d", g_HudBarsArraySize);
//for (HUDBarElement IterElement : g_HUDBArsArray)
//{}


