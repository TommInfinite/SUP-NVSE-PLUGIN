


#include "nvse/PluginAPI.h"
#include "nvse/CommandTable.h"
#include "nvse/GameAPI.h"
#include "nvse/ParamInfos.h"
#include "nvse/GameObjects.h"
#include "nvse/nvse/GameUI.h"
#include "decoding.h"
#include "params.h"
#include "nvse\nvse\utility.h"
#include "nvse\nvse\ArrayVar.h"
#include "nvse\nvse\iomanip"
#include "nvse\nvse\GameSettings.h"
#include "nvse\nvse\SafeWrite.h"
#include <string>
#include "nvse/GameProcess.h"


#include <windows.h>
#include <stdio.h>
#include <VersionHelpers.h>
#include <nvse/nvse/GameOSDepend.h>
#include <Tomm_JiP_FileStream.h>
#include <common\SimpleIni.h>
#include <cmath>
#include <iostream>
#include <nvse/nvse/GameData.h>















#ifndef RegisterScriptCommand
#define RegisterScriptCommand(name) 	nvse->RegisterCommand(&kCommandInfo_ ##name);
#endif

IDebugLog		gLog("SUPNVSE.log");


PluginHandle	g_pluginHandle = kPluginHandle_Invalid;

NVSEMessagingInterface* g_messagingInterface;
NVSEArrayVarInterface* ArrIfc = NULL;
NVSEInterface* g_nvseInterface;
NVSECommandTableInterface* g_cmdTable;
const CommandInfo* g_TFC;

bool (*ExtractArgsEx)(COMMAND_ARGS_EX, ...);
CommandInfo* (*GetCmdByOpcode)(UInt32 opcode);
Cmd_Execute KillActor;
NVSEStringVarInterface* StrIfc = NULL;
HUDMainMenu* g_HUDMainMenu = NULL;
InterfaceManager* g_interfaceManager = NULL;
TileMenu** g_tileMenuArray = NULL;
UInt32 g_screenWidth = 0;
UInt32 g_screenHeight = 0;
PlayerCharacter* g_ThePlayer = NULL;
Tile* g_kMenuRoot = NULL;
Tile* g_Cursor = NULL;
DataHandler* g_dataHandler = nullptr; // from JG

//PlayerCharacter* g_ThePlayer = *(PlayerCharacter**)0x11DEA3C;
//VATSCameraData* g_VATSCameraData = (VATSCameraData*)0x11F2250; // From JIP
VATSMenu** g_VATSMenu = (VATSMenu**)0x11DB0D4;
ProcessManager* g_processManager = (ProcessManager*)0x11E0E80; // From JiP
UInt32 SUPNVSEVersion = 140;
#define NUM_ARGS *((UInt8*)scriptData + *opcodeOffsetPtr)
#define REFR_RES *(UInt32*)result // From JIP
#define IS_ACTOR(form) ((*(UInt32**)form)[0x40] == 0x8D0360) // From JIP
#define GetRandomIntInRange(iMin, iMax) ThisCall<SInt32, SInt32>(0xAA5230, (void*)0x11C4180, iMax - iMin) + iMin // From JIP

#define HudBarIterElement g_HUDBArsArray[iKey]



//bool b_MouseInput = true;

//SETTINGS
int bTFCPosOnLoadFix = 0;


UnorderedMap<const char*, UInt32> s_menuNameToID({ {"MessageMenu", kMenuType_Message}, {"InventoryMenu", kMenuType_Inventory}, {"StatsMenu", kMenuType_Stats},
	{"HUDMainMenu", kMenuType_HUDMain}, {"LoadingMenu", kMenuType_Loading}, {"ContainerMenu", kMenuType_Container}, {"DialogMenu", kMenuType_Dialog},
	{"SleepWaitMenu", kMenuType_SleepWait}, {"StartMenu", kMenuType_Start}, {"LockpickMenu", kMenuType_LockPick}, {"QuantityMenu", kMenuType_Quantity},
	{"MapMenu", kMenuType_Map}, {"BookMenu", kMenuType_Book}, {"LevelUpMenu", kMenuType_LevelUp}, {"RepairMenu", kMenuType_Repair},
	{"RaceSexMenu", kMenuType_RaceSex}, {"CharGenMenu", kMenuType_CharGen}, {"TextEditMenu", kMenuType_TextEdit}, {"BarterMenu", kMenuType_Barter},
	{"SurgeryMenu", kMenuType_Surgery}, {"HackingMenu", kMenuType_Hacking}, {"VATSMenu", kMenuType_VATS}, {"ComputersMenu", kMenuType_Computers},
	{"RepairServicesMenu", kMenuType_RepairServices}, {"TutorialMenu", kMenuType_Tutorial}, {"SpecialBookMenu", kMenuType_SpecialBook},
	{"ItemModMenu", kMenuType_ItemMod}, {"LoveTesterMenu", kMenuType_LoveTester}, {"CompanionWheelMenu", kMenuType_CompanionWheel},
	{"TraitSelectMenu", kMenuType_TraitSelect}, {"RecipeMenu", kMenuType_Recipe}, {"SlotMachineMenu", kMenuType_SlotMachine},
	{"BlackjackMenu", kMenuType_Blackjack}, {"RouletteMenu", kMenuType_Roulette}, {"CaravanMenu", kMenuType_Caravan}, {"TraitMenu", kMenuType_Trait} });


typedef NVSEArrayVarInterface::Array NVSEArrayVar;
typedef NVSEArrayVarInterface::Element NVSEArrayElement;


#include <Tomm_HUDBars.h>


Tile* InterfaceManager::GetActiveTile() //proably from JiP
{
	return activeTile ? activeTile : activeTileAlt;
}


UInt8 TESForm::GetOverridingModIdx()
{
	ModInfo* info = mods.GetLastItem();
	return info ? info->modIndex : 0xFF;
}






// SaveFileVars
//////////////////////////////
char SavegameFolder[0x4000];
//Temp
char* s_SaveTemp;

//LastLoaded
UInt32 iLoadSGLength;
char LoadedSGName[0x4000] = "NULL";
char LoadedSGPathFOS[0x4000] = "NULL";
char LoadedSGPathNVSE[0x4000] = "NULL";

//LastSaved
UInt32 iSavedSGLength;
char SavedSGName[0x4000] = "NULL";
char SavedSGPathFOS[0x4000] = "NULL";
char SavedSGPathNVSE[0x4000] = "NULL";
////////////////////////////// GLOBALS

char FalloutFolderPath[0x4000] = "NULL";

/// //////////////////////////

//GLOBAL NVSE ARRAY VARS
NVSEArrayVar* TileArrayStore;
NVSEArrayVar* g_ar_temp;

//STRING GLOBAL
char g_FileInfoTempChar[0x4000] = "NULL";


//XML
const char kComponentTempXML[] = "sup_temp.xml";
const char kComponentTempBarsXML[] = "sup_tempBars.xml";

//XML TILE GLOBAL
Tile* g_SUPRect;
Tile* g_SUPRectBars;



__declspec(naked) float __vectorcall GetDistance3D(TESObjectREFR* ref1, TESObjectREFR* ref2) // From JIP
{
	__asm
	{
		movups	xmm0, [ecx + 0x30]
		movups	xmm1, [edx + 0x30]
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		movhlps	xmm1, xmm0
		addss	xmm1, xmm0
		psrlq	xmm0, 0x20
		addss	xmm1, xmm0
		sqrtss	xmm0, xmm1
		retn
	}
}

//__declspec(naked) float __vectorcall TESObjectREFR::GetDistance(TESObjectREFR* target)
//{
//	__asm
//	{
//		push	ecx
//		push	edx
//		call	TESObjectREFR::GetInSameCellOrWorld
//		pop		edx
//		pop		ecx
//		test	al, al
//		jz		fltMax
//		jmp		GetDistance3D
//		fltMax :
//		movss	xmm0, kFltMax
//			retn
//	}
//}


__declspec(naked) bool KillActorExecute(COMMAND_ARGS) // From JIP
{
	__asm	jmp		KillActor
}




void OnDeferredInit()
{

	CSimpleIniA ini;
	ini.SetUnicode();
	if (FileExists("Data/nvse/plugins/supNVSE.ini"))
	{
	}
	else {
		ini.SetValue("Settings", "bTFCPosOnLoadFix", "1");
		ini.SaveFile("Data//nvse//plugins//supNVSE.ini");
		_MESSAGE("Setting bTFCPosOnLoadFix >>>>> %i", bTFCPosOnLoadFix);
	}

	auto errVal = ini.LoadFile("Data//nvse//plugins//supNVSE.ini");
	if (errVal == SI_Error::SI_FILE) { /* handle error */ };

	bTFCPosOnLoadFix = ini.GetLongValue("Settings", "bTFCPosOnLoadFix");
	_MESSAGE("bTFCPosOnLoadFix  is %i", bTFCPosOnLoadFix);
}


const std::string& GetFalloutDirectorySUP(void)
{
	static std::string s_falloutDirectory;

	if (s_falloutDirectory.empty())
	{
		// can't determine how many bytes we'll need, hope it's not more than MAX_PATH
		char	falloutPathBuf[MAX_PATH];
		UInt32	falloutPathLength = GetModuleFileName(GetModuleHandle(NULL), falloutPathBuf, sizeof(falloutPathBuf));

		if (falloutPathLength && (falloutPathLength < sizeof(falloutPathBuf)))
		{
			std::string	falloutPath(falloutPathBuf, falloutPathLength);

			// truncate at last slash
			std::string::size_type	lastSlash = falloutPath.rfind('\\');
			if (lastSlash != std::string::npos)	// if we don't find a slash something is VERY WRONG
			{
				s_falloutDirectory = falloutPath.substr(0, lastSlash + 1);

				_DMESSAGE("fallout root = %s", s_falloutDirectory.c_str());
			}
			else
			{
				_WARNING("no slash in fallout path? (%s)", falloutPath.c_str());
			}
		}
		else
		{
			_WARNING("couldn't find fallout path (len = %d, err = %08X)", falloutPathLength, GetLastError());
		}
	}


	strcpy(FalloutFolderPath, s_falloutDirectory.c_str());

}



//HWND hDllHandle = NULL;
//LRESULT CALLBACK X_CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
//{
//	if (nCode == HCBT_CREATEWND)
//		hDllHandle = (HWND)wParam;
//	return CallNextHookEx(NULL, nCode, wParam, lParam);
//}
//
//HWND CallXAndGetHWND()
//{
//	HHOOK hDllHook = SetWindowsHookEx(WH_CBT, X_CBTProc, NULL, GetCurrentThreadId());
//	UnhookWindowsHookEx(hDllHook);
//	return hDllHandle;
//}

//CallXAndGetHWND();






extern UnorderedSet<UInt32> s_gameLoadedInformedScriptsSUP;

#if RUNTIME
NVSEScriptInterface* g_script;
#endif
// This is a message handler for nvse events
// With this, plugins can listen to messages such as whenever the game loads
void MessageHandler(NVSEMessagingInterface::Message* msg)
{

	switch (msg->type)
	{
	case NVSEMessagingInterface::kMessage_LoadGame: 



		break;
	case NVSEMessagingInterface::kMessage_SaveGame:

		UInt32 iLength2;
		s_SaveTemp = (char*)msg->data;
		strcpy(SavedSGName, "");
		iLength2 = strlen(s_SaveTemp) - 4;
		strncat(SavedSGName, s_SaveTemp, iLength2);



		strcpy(SavedSGPathFOS, SavegameFolder);
		strcat(SavedSGPathFOS, s_SaveTemp);

		strcpy(SavedSGPathNVSE, SavegameFolder);
		iSavedSGLength = strlen(s_SaveTemp);
		iLength2 = iSavedSGLength - 3;
		strncat(SavedSGPathNVSE, s_SaveTemp, iLength2);
		strcat(SavedSGPathNVSE, "nvse");

		//_MESSAGE("Current Saved FOS name is %s", SavedSGPathFOS);
		//_MESSAGE("Current Saved NVSE name is %s", SavedSGPathNVSE);

		break;
	case NVSEMessagingInterface::kMessage_PreLoadGame:

		s_gameLoadedInformedScriptsSUP.Clear();
		// Credits to C6 for the help.
		//_MESSAGE("Received PRELOAD message with file path %s", msg->data);

		UInt32 iLength;
		s_SaveTemp = (char*)msg->data;
		strcpy(LoadedSGName, "");
		iLength = strlen(s_SaveTemp) - 4;
		strncat(LoadedSGName, s_SaveTemp, iLength);



		//strncat(LoadedSGName, s_SaveTemp, iLength);

		strcpy(LoadedSGPathFOS, SavegameFolder);
		strcat(LoadedSGPathFOS, s_SaveTemp);

		strcpy(LoadedSGPathNVSE, SavegameFolder);
		iLoadSGLength = strlen(s_SaveTemp);
		iLength = iLoadSGLength - 3;
		strncat(LoadedSGPathNVSE, s_SaveTemp, iLength);
		strcat(LoadedSGPathNVSE, "nvse");


		if ((*g_osGlobals)->tfcState == 1 && bTFCPosOnLoadFix == 1) // Jazz
		{
			(*(OSGlobals**)0x11DEA0C)->tfcState = 0;

		}

		//_MESSAGE("Current Loaded FOS name is %s", LoadedSGPathFOS);
		//_MESSAGE("Current Loaded NVSE name is %s", LoadedSGPathNVSE);



		break;
	case NVSEMessagingInterface::kMessage_PostLoadGame:
		//_MESSAGE("Received POST POS LOAD GAME message with file path %s", msg->data);
		break;
	case NVSEMessagingInterface::kMessage_PostLoad:
		//_MESSAGE("Received POST LOAD message with file path %s", msg->data);

		break;
	case NVSEMessagingInterface::kMessage_PostPostLoad:
		//_MESSAGE("Received POST POS LOAD message with file path %s", msg->data);
		break;
	case NVSEMessagingInterface::kMessage_ExitGame:
		//_MESSAGE("Received exit game message");
		break;
	case NVSEMessagingInterface::kMessage_ExitGame_Console:
		//_MESSAGE("Received exit game via console qqq command message");
		break;
	case NVSEMessagingInterface::kMessage_ExitToMainMenu:
		//_MESSAGE("Received exit game to main menu message");
		break;
	case NVSEMessagingInterface::kMessage_Precompile:
		//_MESSAGE("Received precompile message with script at %08x", msg->data);
		break;

	case NVSEMessagingInterface::kMessage_DeleteGame:
		//_MESSAGE("Received DELETE message with file path %s", msg->data);
		break;

	case NVSEMessagingInterface::kMessage_DeferredInit: 
		g_HUDMainMenu = *(HUDMainMenu**)0x11D96C0;  // From JiP's patches game
		g_interfaceManager = *(InterfaceManager**)0x11D8A80; // From JiP's patches game
		g_tileMenuArray = *(TileMenu***)0x11F350C; // From JiP's patches game
		g_screenWidth = *(UInt32*)0x11C73E0; // From JiP's patches game
		g_screenHeight = *(UInt32*)0x11C7190; // From JiP's patches game
		g_ThePlayer = *(PlayerCharacter**)0x11DEA3C; // From JIP
		g_kMenuRoot = g_interfaceManager->menuRoot;
		g_Cursor = g_interfaceManager->cursor;
		KillActor = GetCmdByOpcode(0x108B)->execute;
		g_dataHandler = DataHandler::Get();
		
		//WriteRelJump(0xA23252, 0xA23296);



		CALL_MEMBER_FN(SaveGameManager::GetSingleton(), ConstructSavegamePath)(SavegameFolder);
		OnDeferredInit();
		GetFalloutDirectorySUP();
		break;

	case NVSEMessagingInterface::kMessage_MainGameLoop:
		if (g_HudBarsArraySize)
		{f_Bars_Iterate();}
		break;

	case NVSEMessagingInterface::kMessage_RuntimeScriptError:
		//_MESSAGE("Received runtime script error message %s", msg->data);
		break;
	default:
		break;
	}
}


#include "Tomm_fn_Utility.h"
#include "Tomm_fn_UI.h"
#include "Tomm_fn_Misc.h"
#include "Tomm_fn_TFC.h"
#include "Tomm_fn_Screenshot.h"
#include "Tomm_fn_INI.h"
#include "Tomm_fn_Array.h"
#include "Tomm_fn_Math.h"
#include "Tomm_fn_HudBars.h"









#if RUNTIME
//In here we define a script function
//Script functions must always follow the Cmd_FunctionName_Execute naming convention

#endif

//This defines a function without a condition, that does not take any arguments




bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	//_MESSAGE("query");

	// fill out the info structure
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "SUP NVSE Plugin";
	info->version = SUPNVSEVersion;

	
	//s_debug.CreateLog("Tomm_NVSE_Debug.log");

	// version checks
	if (nvse->nvseVersion < NVSE_VERSION_INTEGER)
	{
		_ERROR("NVSE version too old (got %08X expected at least %08X)", nvse->nvseVersion, NVSE_VERSION_INTEGER);
		return false;
	}

	if (!nvse->isEditor)
	{
		g_script = (NVSEScriptInterface*)nvse->QueryInterface(kInterface_Script);
		ExtractArgsEx = g_script->ExtractArgsEx;
		StrIfc = (NVSEStringVarInterface*)nvse->QueryInterface(kInterface_StringVar); // From JG

		ArrIfc = (NVSEArrayVarInterface*)nvse->QueryInterface(kInterface_ArrayVar); // From JG


		if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525)
		{
			_ERROR("incorrect runtime version (got %08X need at least %08X)", nvse->runtimeVersion, RUNTIME_VERSION_1_4_0_525);
			return false;
		}

		if (nvse->isNogore)
		{
			_ERROR("NoGore is not supported");
			return false;
		}
	}

	else
	{
		if (nvse->editorVersion < CS_VERSION_1_4_0_518)
		{
			_ERROR("incorrect editor version (got %08X need at least %08X)", nvse->editorVersion, CS_VERSION_1_4_0_518);
			return false;
		}
	}

	// version checks pass
	// any version compatibility checks should be done here
	return true;
}



bool NVSEPlugin_Load(const NVSEInterface* nvse)
{



	g_pluginHandle = nvse->GetPluginHandle();

	// save the NVSEinterface in cas we need it later
	g_nvseInterface = (NVSEInterface*)nvse;

	// register to receive messages from NVSE
	g_messagingInterface = (NVSEMessagingInterface*)nvse->QueryInterface(kInterface_Messaging);
	g_messagingInterface->RegisterListener(g_pluginHandle, "NVSE", MessageHandler);


	NVSECommandTableInterface* cmdTable = (NVSECommandTableInterface*)nvse->QueryInterface(kInterface_CommandTable);
	GetCmdByOpcode = cmdTable->GetByOpcode;
	


	_MESSAGE("SUPNVSE Version: %d", SUPNVSEVersion);




#if RUNTIME
	g_script = (NVSEScriptInterface*)nvse->QueryInterface(kInterface_Script);
#endif


	//https://geckwiki.com/index.php?title=NVSE_Opcode_Base

	 // register commands
	#define REG_TYPED_CMD(name, type) nvse->RegisterTypedCommand(&kCommandInfo_##name,kRetnType_##type);
	#define REG_CMD_STR(name) nvse->RegisterTypedCommand(&kCommandInfo_##name, kRetnType_String) // From JIP_NVSE.H
	#define REG_CMD_ARR(name) nvse->RegisterTypedCommand(&kCommandInfo_##name, kRetnType_Array) // From JIP_NVSE.H


	nvse->SetOpcodeBase(0x3961);
	//  v.1.00
	/*1*/RegisterScriptCommand(SetHUDVisibilityFlags);
	/*2*/RegisterScriptCommand(GetHUDVisibilityFlags);
	/*3*/RegisterScriptCommand(DumpTileInfo);
	/*4*/RegisterScriptCommand(DumpTileInfoAll);
	/*5*/RegisterScriptCommand(GetScreenTrait);
	/*6*/RegisterScriptCommand(GetCalculatedPos);
	/*7*/RegisterScriptCommand(GetCursorTrait); 
	/*8*/RegisterScriptCommand(SetCursorTrait); 
	/*9*/RegisterScriptCommand(GetSUPVersion);
	//  v.1.1
	/*10*/RegisterScriptCommand(SetCursorTraitGradual);
	/*11*/RegisterScriptCommand(GetFileSize);
	/*12*/RegisterScriptCommand(GetLoadedSaveSize);
	/*13*/RegisterScriptCommand(GetSavedSaveSize);
	/*14*/REG_CMD_STR(GetSaveName);
	/*15*/RegisterScriptCommand(RoundAlt);
	/*16*/RegisterScriptCommand(Round);
	/*17*/RegisterScriptCommand(MarkScriptOnLoad);
	/*18*/RegisterScriptCommand(IsScriptMarkedOnLoad);
	/*19*/REG_CMD_ARR(GetNearCells, Array);
	/*20*/REG_CMD_ARR(DumpTileInfoToArray, Array);
	//  v.1.25
	/*20*/REG_CMD_ARR(GetNearMapMarkers, Array);
	/*21*/RegisterScriptCommand(GetPCTrait);
	/*22*/RegisterScriptCommand(SUPTest);
	/*23*/RegisterScriptCommand(GetTFCAngleEx);
	/*24*/RegisterScriptCommand(SetTFCAngleEx);
	/*25*/RegisterScriptCommand(GetTFCRotEx);
	/*26*/RegisterScriptCommand(SetTFCRotEx);
	/*27*/RegisterScriptCommand(SetTFCPosEx);
	/*28*/RegisterScriptCommand(GetTFCPosEx);
	/*29*/RegisterScriptCommand(GetTFCPos);
	/*30*/RegisterScriptCommand(SetTFCPos);
	/*31*/RegisterScriptCommand(GetTFCAngle);
	/*32*/RegisterScriptCommand(SetTFCAngle);
	/*33*/RegisterScriptCommand(GetTFCRot);
	/*34*/RegisterScriptCommand(SetTFCRot);
	/*35*/RegisterScriptCommand(GetHeadingAngleAlt);
	/*36*/RegisterScriptCommand(GetHeadingAngleBetweenPoints);
	/*37*/RegisterScriptCommand(GetCalculatedAngleZForTFCCamera);
	/*38*/RegisterScriptCommand(GetCalculatedAngleXForTFCCamera);
	/*39*/RegisterScriptCommand(IsNumberNAN);
	/*40*/RegisterScriptCommand(GetDistanceBetweenPoints);
	/*41*/RegisterScriptCommand(GetCalculatedPosFrame);
	/*42*/RegisterScriptCommand(GetCalculatedPosAlt);
	/*43*/RegisterScriptCommand(CaptureScreenshot);
	/*44*/RegisterScriptCommand(DeleteScreenshot);
	/*44*/RegisterScriptCommand(ReadINIFloatFromFile);
	/*45*/RegisterScriptCommand(ReadINIFloatFromFileAlt);
    /*46*/REG_CMD_STR(ReadINIStringFromFile);
	/*47*/RegisterScriptCommand(CaptureScreenshotAlt);
	//  v.1.40
	/*48*/REG_CMD_ARR(Ar_SetFloatsFrom, Array);
	/*49*/REG_CMD_ARR(ReadINISectionsFromFile, Array);
	/*50*/REG_CMD_ARR(ReadINISectionKeysFromFile, Array);
	/*51*/RegisterScriptCommand(GetMousePosition);
	/*52*/RegisterScriptCommand(FakeMouseMovement);
	/*53*/RegisterScriptCommand(GetGrenadeTimeHeld);
	/*54*/RegisterScriptCommand(IsPlayerOverencumbered);
	/*55*/RegisterScriptCommand(SetCaughtPCPickpocketting);
	/*56*/RegisterScriptCommand(KillAll2);
	/*57*/RegisterScriptCommand(LunetteCMD);
	/*58*/RegisterScriptCommand(FindClosestActorFromRef);
	/*59*/RegisterScriptCommand(FindClosestActor);
	/*60*/RegisterScriptCommand(FindRandomActorFromRef);
	/*61*/RegisterScriptCommand(FindRandomActor);
	/*62*/RegisterScriptCommand(Ar_GetRandomKey);
	/*63*/REG_CMD_STR(Ar_GetRandomKeyMap);
	/*64*/RegisterScriptCommand(Ar_HasInvalidRefs);
	//  v.1.50
	/*65*/RegisterScriptCommand(GetUIValueType); /////////////TEST TEST TEST
	/*66*/RegisterScriptCommand(IsProgramRunning); 
	/*67*/REG_CMD_ARR(GetFileTime);
	/*68*/REG_CMD_STR(GetFileTimeSTR);
	/*69*/RegisterScriptCommand(StringToClipboard);
	/*70*/REG_CMD_STR(ClipboardToString);
	/*71*/RegisterScriptCommand(DebugTextCreate);
	/*72*/RegisterScriptCommand(DebugTextExists);
	/*73*/RegisterScriptCommand(DebugTextSetString);
	/*74*/RegisterScriptCommand(DebugTextDestroy);
	/*75*/RegisterScriptCommand(DebugTextSetPos);
	/*76*/RegisterScriptCommand(HudBarCreate);
	/*77*/RegisterScriptCommand(HudBarSetMeterSize);
	/*78*/RegisterScriptCommand(HudBarSetValueScriptVar);
	/*79*/RegisterScriptCommand(HudBarSetValueFloat);
	/*80*/RegisterScriptCommand(HudBarSetValuePercentage);
	/*81*/RegisterScriptCommand(HudBarSetValueMax);
	/*82*/RegisterScriptCommand(HudBarSetFrameImage);
	/*83*/RegisterScriptCommand(HudBarSetMeterImage);
	/*84*/RegisterScriptCommand(HudBarSetFrameVisible);
	/*85*/RegisterScriptCommand(HudBarSetMeterVisible);
	/*86*/RegisterScriptCommand(HudBarSetFrameSize);
	/*87*/RegisterScriptCommand(HudBarShowBar);
	/*87*/RegisterScriptCommand(HudBarSetTextPrefix);
	/*87*/RegisterScriptCommand(HudBarSetTextPostFix);



	///*66*/RegisterScriptCommand(SetTileValueAction);

	//*61*/RegisterScriptCommand(UIUpdateField);
	//*20*/REG_CMD_ARR(SupTestArray, Array);
	///*43*/RegisterScriptCommand(SUPPlayMP3File);
	//RegisterScriptCommand(ToggleMouseInput);
	//RegisterScriptCommand(GetUITraitValueType);
	
	return true;
}













