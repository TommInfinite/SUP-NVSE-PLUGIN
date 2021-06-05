
#define SUPVERSION 300

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

#include <vector>
#include <memory>
#include <NanoTimer.h>




// "From JiP" indicates that part of the code was copied from JiP NVSE Plugin.(https://github.com/jazzisparis/JIP-LN-NVSE)
// "From JG" indicates that part of the code was copied from JohnnyGuitarNVSE.(https://github.com/carxt/JohnnyGuitarNVSE/tree/master/JG)






#ifndef RegisterScriptCommand
#define RegisterScriptCommand(name) 	nvse->RegisterCommand(&kCommandInfo_ ##name);
#endif

IDebugLog		gLog("SUPNVSE.log");


PluginHandle	g_pluginHandle = kPluginHandle_Invalid;

NVSEMessagingInterface* g_messagingInterface = NULL;
NVSEArrayVarInterface* ArrIfc = NULL;
NVSEInterface* g_nvseInterface = NULL;
NVSECommandTableInterface* g_cmdTable;
const CommandInfo* g_TFC = NULL;

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
OSInputGlobals* g_inputGlobals = NULL; // From JIP
tList<GradualSetFloat>* g_queuedGradualSetFloat = (tList<GradualSetFloat>*)0x11F3348; // From JiP
SystemColorManager* g_sysColorManager = NULL;

LockPickMenu** g_lockPickMenu = (LockPickMenu**)0x11DA204; // JIP
bool* g_menuVisibilityJIP = (bool*)0x11F308F; // JIP


FontManagerJIP* g_fontManager = NULL; // FROM JIP

//PlayerCharacter* g_ThePlayer = *(PlayerCharacter**)0x11DEA3C;
//VATSCameraData* g_VATSCameraData = (VATSCameraData*)0x11F2250; // From JIP
VATSMenu** g_VATSMenu = (VATSMenu**)0x11DB0D4;
ProcessManager* g_processManager = (ProcessManager*)0x11E0E80; // From JiP
UInt32 SUPNVSEVersion = SUPVERSION;
#define NUM_ARGS *((UInt8*)scriptData + *opcodeOffsetPtr)
#define REFR_RES *(UInt32*)result // From JIP
#define IS_ACTOR(form) ((*(UInt32**)form)[0x40] == 0x8D0360) // From JIP
#define GetRandomIntInRange(iMin, iMax) ThisCall<SInt32, SInt32>(0xAA5230, (void*)0x11C4180, iMax - iMin) + iMin // From JIP

#define HBIter g_HUDBArsArrayV[iKey]
#define HBIterV (*it)
#define VectorIter (*it)
#define NiBlockIter (*it).niBlock
#define VectorEventCallerIter (*it_EventCaller)


//Events
int g_PlayerCamState = 0;



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

bool (*FunctionCallScript)(Script* funcScript, TESObjectREFR* callingObj, TESObjectREFR* container, NVSEArrayElement* result, UInt8 numArgs, ...);


bool IsGradualSetFloatValue(Tile* tile, UInt32 valueID) // Written by JazzisParis
{
	auto iter = g_queuedGradualSetFloat->Head();
	GradualSetFloat* grad;
	do
	{
		grad = iter->data;
		if (grad && (grad->tile == tile) && (grad->valueID == valueID))
			return true;
	} while (iter = iter->next);
	return false;
}


bool IsGradualSetFloatTile(Tile* tile) // Written by JazzisParis,Edited by me.
{
	auto iter = g_queuedGradualSetFloat->Head();
	GradualSetFloat* grad;
	do
	{
		grad = iter->data;
		if (grad && (grad->tile == tile))
			return true;
	} while (iter = iter->next);
	return false;
}


float IsGradualSetFloatTimer(Tile* tile, UInt32 valueID, int iRemaining) // Written by JazzisParis,Edited by me.
{
	auto iter = g_queuedGradualSetFloat->Head();
	GradualSetFloat* grad;
	do
	{
		grad = iter->data;
		if (grad && (grad->tile == tile) && (grad->valueID == valueID))
		{
			double fTickCount = GetTickCount();
			if (iRemaining)
			{
				return ((grad->durationMS - (fTickCount - grad->startTimeMS)) / 1000);

			}
			else {
				return ((fTickCount - grad->startTimeMS) / 1000);
			}

		}

	} while (iter = iter->next);
	return -1;
}

int IsGradualSetFloatMode(Tile* tile, UInt32 valueID, int iRemaining) // Written by JazzisParis,Edited by me.
{
	auto iter = g_queuedGradualSetFloat->Head();
	GradualSetFloat* grad;
	do
	{
		grad = iter->data;
		if (grad && (grad->tile == tile) && (grad->valueID == valueID))
		{
			return (grad->changeMode);
		}

	} while (iter = iter->next);
	return -1;
}


#include <Tomm_HUDBars.h>
#include "Tomm_FunctionCaller.h"
#include "Tomm_EventCaller.h"
#include "Tomm_Nif_Immerse.h"
#include "Tomm_Nif_InterpolatorEventCaller.h"
#include "Tomm_Nif_Interpolator.h"



Tile* InterfaceManager::GetActiveTile() //// From JIP jip_nvse.h
{
	return activeTile ? activeTile : activeTileAlt;
}


UInt8 TESForm::GetOverridingModIdx()// From JG
{
	ModInfo* info = mods.GetLastItem();
	return info ? info->modIndex : 0xFF;
}

enum ObjectVtbl
{
	kVtbl_BGSTextureSet = 0x1033D1C,
	kVtbl_BGSMenuIcon = 0x1033654,
	kVtbl_TESGlobal = 0x1036524,
	kVtbl_TESClass = 0x1048BB4,
	kVtbl_TESFaction = 0x10498DC,
	kVtbl_BGSHeadPart = 0x10464B4,
	kVtbl_TESHair = 0x1049B9C,
	kVtbl_TESEyes = 0x104973C,
	kVtbl_TESRace = 0x104B4BC,
	kVtbl_TESSound = 0x1044FFC,
	kVtbl_BGSAcousticSpace = 0x10320FC,
	kVtbl_TESSkill = 0x104CC0C,
	kVtbl_EffectSetting = 0x1012834,
	kVtbl_Script = 0x1037094,
	kVtbl_TESLandTexture = 0x102E6C4,
	kVtbl_EnchantmentItem = 0x1012EA4,
	kVtbl_SpellItem = 0x1013F8C,
	kVtbl_TESObjectACTI = 0x1029D5C,
	kVtbl_BGSTalkingActivator = 0x1025594,
	kVtbl_BGSTerminal = 0x1025914,
	kVtbl_TESObjectARMO = 0x102A62C,
	kVtbl_TESObjectBOOK = 0x102A9C4,
	kVtbl_TESObjectCLOT = 0x102AC0C,
	kVtbl_TESObjectCONT = 0x102AEB4,
	kVtbl_TESObjectDOOR = 0x102B1FC,
	kVtbl_IngredientItem = 0x1013284,
	kVtbl_TESObjectLIGH = 0x1028EE4,
	kVtbl_TESObjectMISC = 0x102B844,
	kVtbl_TESObjectSTAT = 0x102BA2C,
	kVtbl_BGSStaticCollection = 0x102535C,
	kVtbl_BGSMovableStatic = 0x1024CEC,	//	Has an irregular structure; the enumed vtbl is the "effective" one used at runtime; actual vtbl is 0x1024E84
	kVtbl_BGSPlaceableWater = 0x1024F4C,
	kVtbl_TESGrass = 0x102814C,
	kVtbl_TESObjectTREE = 0x102BC94,
	kVtbl_TESFlora = 0x1026BD0,
	kVtbl_TESFurniture = 0x1026D0C,
	kVtbl_TESObjectWEAP = 0x102C51C,
	kVtbl_TESAmmo = 0x1026064,
	kVtbl_TESNPC = 0x104A2F4,
	kVtbl_TESCreature = 0x1048F5C,
	kVtbl_TESLevCreature = 0x102886C,
	kVtbl_TESLevCharacter = 0x102864C,
	kVtbl_TESKey = 0x1028444,
	kVtbl_AlchemyItem = 0x1011964,
	kVtbl_BGSIdleMarker = 0x104664C,
	kVtbl_BGSNote = 0x1046874,
	kVtbl_BGSConstructibleObject = 0x10245A4,
	kVtbl_BGSProjectile = 0x10251AC,
	kVtbl_TESLevItem = 0x1028A64,
	kVtbl_TESWeather = 0x103168C,
	kVtbl_TESClimate = 0x102D5C4,
	kVtbl_TESRegion = 0x102397C,
	kVtbl_NavMeshInfoMap = 0x106BB8C,
	kVtbl_TESObjectCELL = 0x102E9B4,
	kVtbl_TESObjectREFR = 0x102F55C,
	kVtbl_Character = 0x1086A6C,
	kVtbl_Creature = 0x10870AC,
	kVtbl_MissileProjectile = 0x108FA44,
	kVtbl_GrenadeProjectile = 0x108F674,
	kVtbl_BeamProjectile = 0x108C3C4,
	kVtbl_FlameProjectile = 0x108F2F4,
	kVtbl_Explosion = 0x108EE04,
	kVtbl_TESWorldSpace = 0x103195C,
	kVtbl_TESObjectLAND = 0x102DCD4,
	kVtbl_NavMesh = 0x106A0B4,
	kVtbl_TESTopic = 0x104D19C,
	kVtbl_TESTopicInfo = 0x104D5B4,
	kVtbl_TESQuest = 0x104AC44,
	kVtbl_TESIdleForm = 0x1049D0C,
	kVtbl_TESPackage = 0x106847C,
	kVtbl_TESCombatStyle = 0x10266E4,
	kVtbl_TESLoadScreen = 0x10366CC,
	kVtbl_TESLevSpell = 0x1028C5C,
	kVtbl_TESObjectANIO = 0x102A0A4,
	kVtbl_TESWaterForm = 0x103140C,
	kVtbl_TESEffectShader = 0x102685C,
	kVtbl_BGSExplosion = 0x1024A94,
	kVtbl_BGSDebris = 0x1024834,
	kVtbl_TESImageSpace = 0x102D7F4,
	kVtbl_TESImageSpaceModifier = 0x102D97C,
	kVtbl_BGSListForm = 0x10334B4,
	kVtbl_BGSPerk = 0x1046EC4,
	kVtbl_BGSBodyPartData = 0x1045504,
	kVtbl_BGSAddonNode = 0x1024214,
	kVtbl_ActorValueInfo = 0x1067A2C,
	kVtbl_BGSRadiationStage = 0x1033B34,
	kVtbl_BGSCameraShot = 0x10327F4,
	kVtbl_BGSCameraPath = 0x103245C,
	kVtbl_BGSVoiceType = 0x104733C,
	kVtbl_BGSImpactData = 0x1032F6C,
	kVtbl_BGSImpactDataSet = 0x103323C,
	kVtbl_TESObjectARMA = 0x102A31C,
	kVtbl_BGSEncounterZone = 0x102CBBC,
	kVtbl_BGSMessage = 0x10337C4,
	kVtbl_BGSRagdoll = 0x10470EC,
	kVtbl_BGSLightingTemplate = 0x102CD94,
	kVtbl_BGSMusicType = 0x103397C,
	kVtbl_TESObjectIMOD = 0x102B5AC,
	kVtbl_TESReputation = 0x104BA24,
	kVtbl_ContinuousBeamProjectile = 0x108EA64,
	kVtbl_TESRecipe = 0x1036B2C,
	kVtbl_TESRecipeCategory = 0x10369DC,
	kVtbl_TESCasinoChips = 0x10263DC,
	kVtbl_TESCasino = 0x1026574,
	kVtbl_TESLoadScreenType = 0x1036854,
	kVtbl_MediaSet = 0x10342EC,
	kVtbl_MediaLocationController = 0x10340C4,
	kVtbl_TESChallenge = 0x104891C,
	kVtbl_TESAmmoEffect = 0x103449C,
	kVtbl_TESCaravanCard = 0x103478C,
	kVtbl_TESCaravanMoney = 0x10349B4,
	kVtbl_TESCaravanDeck = 0x1034B4C,
	kVtbl_BGSDehydrationStage = 0x101144C,
	kVtbl_BGSHungerStage = 0x10115B4,
	kVtbl_BGSSleepDeprevationStage = 0x10116FC,
	kVtbl_PlayerCharacter = 0x108AA3C,

	kVtbl_BGSQuestObjective = 0x1047088,
	kVtbl_TESModelTextureSwap = 0x101D124,
	kVtbl_BGSPrimitiveBox = 0x101E8C4,
	kVtbl_BGSPrimitiveSphere = 0x101EA64,
	kVtbl_BGSPrimitivePlane = 0x101E75C,
	kVtbl_MagicShaderHitEffect = 0x107B70C,

	kVtbl_BGSQuestPerkEntry = 0x1046B84,
	kVtbl_BGSAbilityPerkEntry = 0x1046C44,
	kVtbl_BGSEntryPointPerkEntry = 0x1046D0C,
	kVtbl_BGSEntryPointFunctionDataOneValue = 0x10462C0,
	kVtbl_BGSEntryPointFunctionDataTwoValue = 0x1046300,
	kVtbl_BGSEntryPointFunctionDataLeveledList = 0x1046320,
	kVtbl_BGSEntryPointFunctionDataActivateChoice = 0x1046340,

	kVtbl_ExtraDataList = 0x10143E8,
	kVtbl_ExtraSeenData = 0x1014294,
	kVtbl_ExtraSpecialRenderFlags = 0x1014458,
	kVtbl_ExtraPrimitive = 0x10151B4,
	kVtbl_ExtraLinkedRef = 0x1015CC0,
	kVtbl_ExtraRadius = 0x1015208,
	kVtbl_ExtraCellWaterType = 0x1014270,
	kVtbl_ExtraCellImageSpace = 0x1014258,
	kVtbl_ExtraCellMusicType = 0x1014234,
	kVtbl_ExtraCellClimate = 0x101424C,
	kVtbl_ExtraTerminalState = 0x1015190,
	kVtbl_ExtraCellAcousticSpace = 0x1014240,
	kVtbl_ExtraOriginalReference = 0x1015BC4,
	kVtbl_ExtraContainerChanges = 0x1015BB8,
	kVtbl_ExtraWorn = 0x1015BDC,
	kVtbl_ExtraHealth = 0x10158E4,
	kVtbl_ExtraLock = 0x101589C,
	kVtbl_ExtraCount = 0x10158D8,
	kVtbl_ExtraTeleport = 0x10158A8,
	kVtbl_ExtraWeaponModFlags = 0x10159A4,
	kVtbl_ExtraHotkey = 0x101592C,
	kVtbl_ExtraCannotWear = 0x1015BF4,
	kVtbl_ExtraOwnership = 0x10158B4,
	kVtbl_ExtraRank = 0x10158CC,
	kVtbl_ExtraAction = 0x1015BAC,
	kVtbl_ExtraFactionChanges = 0x1015F30,
	kVtbl_ExtraScript = 0x1015914,
	kVtbl_ExtraObjectHealth = 0x1015184,
	kVtbl_ExtraStartingPosition = 0x1015B40,
	kVtbl_ExtraPoison = 0x101595C,
	kVtbl_ExtraCharge = 0x1015908,
	kVtbl_ExtraRadiation = 0x1015214,
	kVtbl_ExtraNorthRotation = 0x10142A0,
	kVtbl_ExtraTimeLeft = 0x10158FC,
	kVtbl_ExtraUses = 0x10158F0,

	kVtbl_SeenData = 0x1083FC4,
	kVtbl_IntSeenData = 0x1083FE4,

	kVtbl_TileMenu = 0x106ED44,

	kVtbl_MessageMenu = 0x107566C,
	kVtbl_InventoryMenu = 0x10739B4,
	kVtbl_StatsMenu = 0x106FFD4,
	kVtbl_HUDMainMenu = 0x1072DF4,
	kVtbl_LoadingMenu = 0x1073EBC,
	kVtbl_ContainerMenu = 0x10721AC,
	kVtbl_DialogMenu = 0x107257C,
	kVtbl_SleepWaitMenu = 0x10763AC,
	kVtbl_StartMenu = 0x1076D1C,
	kVtbl_LockpickMenu = 0x107439C,
	kVtbl_QuantityMenu = 0x10701C4,
	kVtbl_MapMenu = 0x1074D44,
	kVtbl_BookMenu = 0x1070ECC,
	kVtbl_LevelUpMenu = 0x1073CDC,
	kVtbl_RepairMenu = 0x1075C5C,
	kVtbl_RaceSexMenu = 0x1075974,
	kVtbl_CharGenMenu = 0x1071BB4,
	kVtbl_TextEditMenu = 0x1070034,
	kVtbl_BarterMenu = 0x10706EC,
	kVtbl_SurgeryMenu = 0x1070084,
	kVtbl_HackingMenu = 0x10728F4,
	kVtbl_VATSMenu = 0x10700D4,
	kVtbl_ComputersMenu = 0x1072004,
	kVtbl_RepairServicesMenu = 0x1075DB4,
	kVtbl_TutorialMenu = 0x106FF84,
	kVtbl_SpecialBookMenu = 0x1070124,
	kVtbl_ItemModMenu = 0x1073B7C,
	kVtbl_LoveTesterMenu = 0x1070174,
	kVtbl_CompanionWheelMenu = 0x1071D0C,
	kVtbl_TraitSelectMenu = 0x1077ABC,
	kVtbl_RecipeMenu = 0x107048C,
	kVtbl_SlotMachineMenu = 0x10764DC,
	kVtbl_BlackjackMenu = 0x10708FC,
	kVtbl_RouletteMenu = 0x1075F7C,
	kVtbl_CaravanMenu = 0x107108C,
	kVtbl_TraitMenu = 0x10779BC,

	kVtbl_NiNode = 0x109B5AC,
	kVtbl_BSFadeNode = 0x10A8F90,
	kVtbl_NiTriShape = 0x109D454,
	kVtbl_NiTriStrips = 0x109CD44,
	kVtbl_NiControllerManager = 0x109619C,
	kVtbl_BSScissorTriShape = 0x10C2E7C,
	kVtbl_NiPointLight = 0x109DD0C,
	kVtbl_NiSpotLight = 0x10A01CC,
	kVtbl_NiDirectionalLight = 0x109D7B4,
	kVtbl_NiAlphaProperty = 0x10162DC,
	kVtbl_NiMaterialProperty = 0x109D6C4,
	kVtbl_NiStencilProperty = 0x101E07C,
	kVtbl_TileShaderProperty = 0x10B9D28,
	kVtbl_WaterShaderProperty = 0x10AE438,
	kVtbl_BSShaderNoLightingProperty = 0x10AE670,
	kVtbl_BSShaderPPLightingProperty = 0x10AE0D0,
	kVtbl_SkyShaderProperty = 0x10B8CE0,
	kVtbl_NiStringExtraData = 0x109D39C,
	kVtbl_NiIntegerExtraData = 0x10A0D24,
	kVtbl_NiFloatExtraData = 0x10A0EC4,
	kVtbl_BSBound = 0x10C2B64,

	kVtbl_ImageSpaceModifierInstanceForm = 0x102D12C,

	kVtbl_hkpAabbPhantom = 0x10CC004,
	kVtbl_hkpSimpleShapePhantom = 0x10CE15C,
	kVtbl_hkpCachingShapePhantom = 0x10D087C,
	kVtbl_hkpRigidBody = 0x10C7888,
	kVtbl_hkpSphereMotion = 0x10C6D54,
	kVtbl_hkpBoxMotion = 0x10C6DC4,
	kVtbl_hkpThinBoxMotion = 0x10C6E34,
	kVtbl_ahkpCharacterProxy = 0x10C83E8,
};


const char* GetNthModNameAlt(UInt32 modIndex) // From JG
{
	if (modIndex == 255)
		return "Console";
	else if(modIndex >=1000)
		return "CustomPlugin";

	const ModInfo** activeModList = g_dataHandler->GetActiveModList();
	if (modIndex < g_dataHandler->GetActiveModCount() && activeModList[modIndex])
		return activeModList[modIndex]->name;
	else
		return "";
}




__declspec(naked) ExtraContainerChanges::EntryData* Actor::GetWeaponInfo() // From JIP jip_nvse.h
{
	__asm
	{
		mov		eax, [ecx + 0x68]
		test	eax, eax
		jz		done
		cmp		dword ptr[eax + 0x28], 1
		ja		retnNULL
		mov		eax, [eax + 0x114]
		retn
		retnNULL :
		xor eax, eax
			done :
		retn
	}
}

TESObjectWEAP* Actor::GetEquippedWeapon() // From JIP jip_nvse.h
{
	ExtraContainerChanges::EntryData* weaponInfo = GetWeaponInfo();
	return weaponInfo ? (TESObjectWEAP*)weaponInfo->type : NULL;
}


__declspec(naked) TESForm* TESObjectREFR::GetBaseForm()// From JIP jip_nvse.h
{
	__asm
	{
		mov		eax, [ecx + 0x20]
		test	eax, eax
		jz		done
		cmp		byte ptr[eax + 0xF], 0xFF
		jnz		done
		cmp		dword ptr[eax], kVtbl_BGSPlaceableWater
		jz		isWater
		push	eax
		push	kExtraData_LeveledCreature
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		pop		ecx
		test	eax, eax
		cmovz	eax, ecx
		jz		done
		mov		eax, [eax + 0xC]
		retn
		isWater :
		mov		eax, [eax + 0x4C]
			done :
			retn
	}
}

__declspec(naked) TESForm* TESObjectREFR::GetBaseForm2()// From JIP jip_nvse.h
{
	__asm
	{
		mov		eax, [ecx + 0x20]
		test	eax, eax
		jz		done
		cmp		byte ptr[eax + 0xF], 0xFF
		jnz		done
		push	kExtraData_LeveledCreature
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax + 0x10]
		done:
		retn
	}
}

__declspec(naked) ExtraContainerChanges::EntryData* TESObjectREFR::GetContainerChangesEntry(TESForm* itemForm) // From JIP jip_nvse.h
{
	__asm
	{
		push	kExtraData_ContainerChanges
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax + 0xC]
		test	eax, eax
		jz		done
		mov		ecx, [eax]
		mov		edx, [esp + 4]
		ALIGN 16
		itemIter:
		test	ecx, ecx
			jz		retnNULL
			mov		eax, [ecx]
			mov		ecx, [ecx + 4]
			test	eax, eax
			jz		itemIter
			cmp[eax + 8], edx
			jnz		itemIter
			retn	4
			retnNULL:
		xor eax, eax
			done :
		retn	4
	}
}


#define NOT_TYPE(form, type) (*(UInt32*)form != kVtbl_##type)

bool Actor::IsItemEquipped(TESForm* item) // From JIP jip_nvse.h
{
	if IS_ID(item, TESObjectWEAP)
	{
		//_MESSAGE("IS_ID(item, TESObjectWEAP)");
		return item == GetEquippedWeapon();

	}

	if (NOT_TYPE(item, TESObjectARMO) || (typeID == kFormType_Creature))
	{
		//_MESSAGE("if (NOT_TYPE(item, TESObjectARMO) || (typeID == kFormType_Creature))");
		return false;

	}

	ExtraContainerChanges::EntryData* entry = GetContainerChangesEntry(item); // From JIP jip_nvse.h
	if (!entry || !entry->extendData)
	{
		//_MESSAGE("!entry || !entry->extendData");
		return false;
	}

	ListNode<ExtraDataList>* node = entry->extendData->Head();
	ExtraDataList* xData;
	do
	{
		xData = node->data;
		if (xData && xData->HasType(kExtraData_Worn))
		{
			return true;
			//_MESSAGE("RETURN BAR TRUE");

		}

	} 	while (node = node->next);

	//_MESSAGE("RETURN BAR FALSE");
	return false;
}



UInt32 InterfaceManager::GetTopVisibleMenuIDJIP() // From JIP jip_nvse.h
{


	if (currentMode < 2) return 0;
	if (activeMenu) return activeMenu->id;
	UInt32* mnStack = &menuStack[1];
	while (*mnStack) mnStack++;
	UInt32 menuID = *(mnStack - 1);
	if (menuID != 1) return menuID;
	if (g_menuVisibilityJIP[kMenuType_Inventory])
		return kMenuType_Inventory;
	if (g_menuVisibilityJIP[kMenuType_Map])
		return kMenuType_Map;
	if (g_menuVisibilityJIP[kMenuType_Stats])
		return kMenuType_Stats;
	if (g_menuVisibilityJIP[kMenuType_Repair])
		return kMenuType_Repair;
	if (g_menuVisibilityJIP[kMenuType_ItemMod])
		return kMenuType_ItemMod;
	return 0;
}






// SaveFileVars
//////////////////////////////
char SavegameFolder[0x4000]{};
//Temp
char* s_SaveTemp = NULL;

//LastLoaded
UInt32 iLoadSGLength;
char LoadedSGName[0x4000]{};
char LoadedSGPathFOS[0x4000]{};
char LoadedSGPathNVSE[0x4000]{};

//LastSaved
UInt32 iSavedSGLength;
char SavedSGName[0x4000]{};
char SavedSGPathFOS[0x4000]{};
char SavedSGPathNVSE[0x4000]{};
////////////////////////////// GLOBALS

char FalloutFolderPath[0x4000]{};

/// //////////////////////////

//GLOBAL NVSE ARRAY VARS
NVSEArrayVar* TileArrayStore;
NVSEArrayVar* g_ar_temp;

//STRING GLOBAL
char g_FileInfoTempChar[0x4000]{};


//XML
const char kComponentTempXML[] = "sup_temp.xml";
const char kComponentTempBarsXML[] = "sup_tempBars.xml";

//XML TILE GLOBAL
Tile* g_SUPRect;
Tile* g_SUPRectBars;




__declspec(naked) float __vectorcall GetDistance3D(TESObjectREFR* ref1, TESObjectREFR* ref2) // From JIP ln_fn_math.h
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



char SUPPATH[FILENAME_MAX];
void foo() {};

void get_module_path(void* address)
{
	HMODULE hm = NULL;

	if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCSTR)address, // if &address - returns F:NV handle?? Check later.
		&hm))
	{}
	GetModuleFileNameA(hm, SUPPATH, sizeof(SUPPATH));

}
std::string base_name(std::string const& path)
{
	return path.substr(path.find_last_of("/\\") + 1);
}
void CheckForDLLName()
{
	get_module_path(foo);

	std::string Result;
	Result = base_name(SUPPATH);


	if (0 == stricmp(Result.c_str(), "supNVSE.dll"))
	{}
	else {
		_MESSAGE("SUP NVSE DLL PATH IS WRONG>>> %s", SUPPATH);
		_MESSAGE("Result is %s", Result.c_str());
		_MESSAGE("Please check if you have 2 SUP NVSE DLL files in the folder");
		Console_Print("Result is %s", Result.c_str());
		Console_Print("SUP NVSE DLL PATH IS WRONG>>> %s", SUPPATH);
		Console_Print("Please check if you have 2 SUP NVSE DLL files in the folder");
	}


}



void OnDeferredInit()
{
	CheckForDLLName();
	CSimpleIniA ini;
	ini.SetUnicode();
	if (FileExists("Data/nvse/plugins/supNVSE.ini"))
	{}
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


void GetFalloutDirectorySUP(void)
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

				_MESSAGE("fallout root = %s", s_falloutDirectory.c_str());
			}
			else
			{
				_MESSAGE("no slash in fallout path? (%s)", falloutPath.c_str());
			}
		}
		else
		{
			_MESSAGE("couldn't find fallout path (len = %d, err = %08X)", falloutPathLength, GetLastError());
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

		//g_PlayerCamState = g_ThePlayer->is3rdPerson;

		break;
	case NVSEMessagingInterface::kMessage_SaveGame:

		UInt32 iLength2;
		s_SaveTemp = (char*)msg->data;
		if (s_SaveTemp)
		{
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
		}

		s_SaveTemp = NULL;

		//_MESSAGE("Current Saved FOS name is %s", SavedSGPathFOS);
		//_MESSAGE("Current Saved NVSE name is %s", SavedSGPathNVSE);

		break;
	case NVSEMessagingInterface::kMessage_PreLoadGame:

		f_FuncCaller_RemoveOnGameLoad();
		f_Bars_BarRemoveOnGameLoad();
		f_NifInterpolator_RemoveOnGameLoad();


		s_gameLoadedInformedScriptsSUP.Clear();
		// Credits to C6 for the help.
		//_MESSAGE("Received PRELOAD message with file path %s", msg->data);

		UInt32 iLength;
		s_SaveTemp = (char*)msg->data;
		if (s_SaveTemp)
		{
			strcpy(LoadedSGName, "");
			iLength = strlen(s_SaveTemp) - 4;
			strncat(LoadedSGName, s_SaveTemp, iLength);
			strcpy(LoadedSGPathFOS, SavegameFolder);
			strcat(LoadedSGPathFOS, s_SaveTemp);

			strcpy(LoadedSGPathNVSE, SavegameFolder);
			iLoadSGLength = strlen(s_SaveTemp);
			iLength = iLoadSGLength - 3;
			strncat(LoadedSGPathNVSE, s_SaveTemp, iLength);
			strcat(LoadedSGPathNVSE, "nvse");
		}

		s_SaveTemp = NULL;

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
		g_fontManager = *(FontManagerJIP**)0x11F33F8; // From JIP
		g_dataHandler = DataHandler::Get();
		g_inputGlobals = *(OSInputGlobals**)0x11F35CC;
		g_sysColorManager = *(SystemColorManager**)0x11D8A88;
		//g_saveGameManagerUse = (SaveGameManager*)0x011DE134;
		//WriteRelJump(0xA23252, 0xA23296);



		CALL_MEMBER_FN(SaveGameManager::GetSingleton(), ConstructSavegamePath)(SavegameFolder);
		OnDeferredInit();
		GetFalloutDirectorySUP();
		break;

	case NVSEMessagingInterface::kMessage_MainGameLoop:


		//if (g_PlayerCamState != g_ThePlayer->is3rdPerson)
		//{
		//	g_PlayerCamState = g_ThePlayer->is3rdPerson;
		//	f_EventCaller_IterateEventCameraMode(0);
		//	Console_Print("g_PlayerCamState is %d", g_PlayerCamState);
		//}

		if (g_HudBarsIterate)
			f_Bars_Iterate();

		if (g_FuncCallerArrayIterate)
			f_FuncCaller_Iterate();


		if (g_NifInterpolatorArrayIterate)
			fn_NifInterpolator_Iterate();

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

#include "Tomm_fn_TFC.h"
#include "Tomm_fn_Screenshot.h"
#include "Tomm_fn_INI.h"
#include "Tomm_fn_Array.h"
#include "Tomm_fn_Math.h"
#include "Tomm_fn_HudBars.h"
#include "Tomm_fn_Misc.h"
#include "Tomm_fn_DialogueTopics.h"

#include "Tomm_fn_Excel.h"

#include "Tomm_fn_Nif.h"

DEFINE_COMMAND_PLUGIN(SUPTest, "", 0, 0, NULL)



bool Cmd_SUPTest_Execute(COMMAND_ARGS)
{

	return true;
}






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



	// version checks
	if (nvse->nvseVersion < 0x6010020)
	{
		_ERROR("NVSE version too old (expected at least xNVSE V6.12)");
		return false;
	}

	if (!nvse->isEditor)
	{
		g_script = (NVSEScriptInterface*)nvse->QueryInterface(kInterface_Script);
		ExtractArgsEx = g_script->ExtractArgsEx;
		FunctionCallScript = g_script->CallFunction;
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
	/*56*/RegisterScriptCommand(KillAllAlt);
	/*57*/RegisterScriptCommand(LunetteCMD); //Undocumented
	/*58*/RegisterScriptCommand(FindClosestActorFromRef);
	/*59*/RegisterScriptCommand(FindClosestActor);
	/*60*/RegisterScriptCommand(FindRandomActorFromRef);
	/*61*/RegisterScriptCommand(FindRandomActor);
	/*62*/RegisterScriptCommand(Ar_GetRandomKey);
	/*63*/REG_CMD_STR(Ar_GetRandomKeyMap);
	/*64*/RegisterScriptCommand(Ar_HasInvalidRefs);
	//  v.2.00
	/*65*/RegisterScriptCommand(GetUIValueType);
	/*66*/RegisterScriptCommand(IsProgramRunning); 
	/*67*/REG_CMD_ARR(GetFileTime,Array);
	/*68*/REG_CMD_STR(GetFileTimeSTR);
	/*69*/RegisterScriptCommand(StringToClipboard);
	/*70*/REG_CMD_STR(ClipboardToString);
	/*71*/RegisterScriptCommand(GetFontTrait);
	/*72*/RegisterScriptCommand(DebugTextCreate);
	/*73*/RegisterScriptCommand(DebugTextExists);
	/*74*/RegisterScriptCommand(DebugTextSetString);
	/*75*/RegisterScriptCommand(DebugTextDestroy);
	/*76*/RegisterScriptCommand(DebugTextSetPos);
	/*77*/RegisterScriptCommand(HudBarCreate)
	/*78*/RegisterScriptCommand(HudBarDestroy);
	/*79*/RegisterScriptCommand(HudBarGetBarTrait);
	/*80*/RegisterScriptCommand(HudBarExists);
	/*81*/RegisterScriptCommand(HudBarSetAutoPos);
	/*82*/RegisterScriptCommand(HudBarSetValueScriptVar);
	/*83*/RegisterScriptCommand(HudBarSetValueFloat);
	/*84*/RegisterScriptCommand(HudBarSetValuePercentage);
	/*85*/RegisterScriptCommand(HudBarSetValueMax);
	/*86*/RegisterScriptCommand(HudBarSetTextString);
	/*87*/RegisterScriptCommand(HudBarSetPos);
	/*88*/RegisterScriptCommand(HudBarSetVisible);
	/*89*/RegisterScriptCommand(HudBarSetTexture);
	/*90*/RegisterScriptCommand(HudBarSetSize);
	/*91*/RegisterScriptCommand(HudBarSetBarTrait);
	/*92*/RegisterScriptCommand(HudBarSetMeterTrait);
	/*92*/RegisterScriptCommand(CallFunctionNextFrame);
	/*93*/RegisterScriptCommand(HudBarSetValueUDF);
	/*94*/RegisterScriptCommand(HudBarShowBar);
	/*95*/REG_CMD_ARR(HudBarGetAllBars, Array);
	/*96*/REG_CMD_STR(HudBarGetElementUIPath);
	/*97*/RegisterScriptCommand(HudBarSetAlpha);
	/*98*/RegisterScriptCommand(HudBarDestroyAllBars);
	/*99*/RegisterScriptCommand(HudBarSetColor);
	/*100*/RegisterScriptCommand(HudBarSetSystemColor);
	/*101*/RegisterScriptCommand(HudBarSetTextTrait);
	/*102*/RegisterScriptCommand(HudBarSetSizeAlt);
	/*103*/RegisterScriptCommand(HudBarSetDepth);
	/*104*/RegisterScriptCommand(HudBarSetZoom);
	/*105*/RegisterScriptCommand(HudBarSetItem);
	/*106*/RegisterScriptCommand(HudBarSetRotation);
	/*107*/RegisterScriptCommand(HudBarSetFormList);
	/*108*/RegisterScriptCommand(HudBarGetDebugInfo);
	/*109*/RegisterScriptCommand(HudBarGetDebugInfoAll);
	/*110*/RegisterScriptCommand(HudBarSetIndent);
	/*111*/RegisterScriptCommand(DumpLoadOrder)
	/*112*/REG_CMD_STR(GetModTraitSTR);
	//  v.2.10
	/*113*/RegisterScriptCommand(HudBarGetVisible);
	/*114*/REG_CMD_ARR(GetCurrentQuestObjectiveTeleportLinksAlt, Array);
	/*115*/RegisterScriptCommand(TopicInfoSetSaidOnce);
	/*116*/RegisterScriptCommand(TopicInfoAddChoice);
	/*117*/RegisterScriptCommand(TopicInfoHasChoice);
	/*118*/RegisterScriptCommand(TopicInfoGetChoiceCount);
	/*119*/REG_CMD_ARR(TopicInfoGetAllChoices);
	/*120*/REG_CMD_STR(TopicInfoSetPrompt);
	/*121*/RegisterScriptCommand(TopicSetPriority);
	/*122*/RegisterScriptCommand(TopicSetFlags);
	/*123*/RegisterScriptCommand(TopicGetType);
	/*124*/REG_CMD_ARR(TopicGetAllTopicInfos);
	//  v.2.20
	/*125*/RegisterScriptCommand(GetUIElementType);
	/*126*/RegisterScriptCommand(SetUIFloatMultiple);
	/*127*/RegisterScriptCommand(GetUIFloatGradualActiveForTileTrait);
	/*128*/RegisterScriptCommand(GetUIFloatGradualActiveForTile);
	/*129*/RegisterScriptCommand(GetUIFloatGradualTimer);
	/*130*/RegisterScriptCommand(GetUIFloatGradualMode);
	/*131*/RegisterScriptCommand(GetNifBlockTranslationToVars); //Undocumented
	//  v.2.30
	/*132*/RegisterScriptCommand(ExcelWriteFloat);
	/*133*/RegisterScriptCommand(ExcelWriteString);
	/*134*/RegisterScriptCommand(ExcelReadFloat);
	/*135*/REG_CMD_STR(ExcelReadString);
	/*136*/RegisterScriptCommand(ExcelGetWorkSheetCount);
	/*137*/RegisterScriptCommand(ExcelGetRowCount);
	/*138*/RegisterScriptCommand(ExcelGetColCount);
	/*139*/REG_CMD_STR(ExcelSetSheetName);
	/*140*/RegisterScriptCommand(ExcelGetCellValueType);
	/*141*/RegisterScriptCommand(ExcelEraseCell);
	/*142*/RegisterScriptCommand(ExcelEraseSheet);
	//  v.3.00
	/*143*/RegisterScriptCommand(GetMouseMovement);
	/*144*/RegisterScriptCommand(GetMouseWheelMovement);
	/*145*/RegisterScriptCommand(DebugTextSetZoom);
	/*146*/RegisterScriptCommand(GetNifBlockNumVerticles);
	/*147*/REG_CMD_ARR(CompareNifBlocks); //Undocumented
	/*148*/RegisterScriptCommand(NifBlockInterpolateScale);
	/*149*/RegisterScriptCommand(NifBlockInterpolateRotation);
	/*150*/RegisterScriptCommand(NifBlockInterpolateTranslation);
	/*151*/RegisterScriptCommand(IsNifBlockInterpolated);
	/*152*/RegisterScriptCommand(NifBlockInterpolateStop);
	/*150*/RegisterScriptCommand(NifBlockInterpolateSetTrait);
	/*153*/RegisterScriptCommand(SetSUPOnInterpolateFinishEvent);
	/*153*/RegisterScriptCommand(RemoveSUPOnInterpolateFinishEvent);
	/*154*/RegisterScriptCommand(SetBobbyPinHealth);
	/*155*/RegisterScriptCommand(GetLockPickMenuValue);

	//*20*/REG_CMD_ARR(SupTestArray, Array);
	//*114*/RegisterScriptCommand(HudBarSetOnVisibilityChangeEvent);
	//*116*/RegisterScriptCommand(TopicInfoDeleteChoiceNth);
	//*116*/RegisterScriptCommand(TopicInfoDeleteAllChoices);
	//*61*/RegisterScriptCommand(UIUpdateField);

	///*43*/RegisterScriptCommand(SUPPlayMP3File);

	return true;
}



int fn_ext_GETVERSION()
{
	return  SUPNVSEVersion;

}









