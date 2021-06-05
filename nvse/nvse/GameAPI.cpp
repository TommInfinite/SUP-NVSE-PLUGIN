#include "GameAPI.h"
#include "GameRTTI.h"
#include "GameForms.h"
#include "GameObjects.h"
#include "GameTypes.h"
#include "GameScript.h"
#include "StringVar.h"
#include "printf.h"

#if NVSE_CORE
#include "Hooks_Script.h"
#include "ScriptUtils.h"
#include "Hooks_Other.h"
#endif


static NVSEStringVarInterface* s_StringVarInterface = NULL;
bool alternateUpdate3D = false;

// arg1 = 1, ignored if canCreateNew is false, passed to 'init' function if a new object is created
typedef void * (* _GetSingleton)(bool canCreateNew);

char s_tempStrArgBuffer[0x4000];

#if RUNTIME
TESForm* __stdcall LookupFormByID(UInt32 refID)
{
	NiTPointerMap<TESForm> *formsMap = *(NiTPointerMap<TESForm>**)0x11C54C0;
	return formsMap->Lookup(refID);
}

bool ExtractArgsEx(ParamInfo *paramInfo, void *scriptDataIn, UInt32 *scriptDataOffset, Script *scriptObj, ScriptEventList *eventList, ...);

#if USE_EXTRACT_ARGS_EX
const _ExtractArgs ExtractArgs = ExtractArgsEx;
#else
const _ExtractArgs ExtractArgs = (_ExtractArgs)0x005ACCB0;
#endif

const _FormHeap_Allocate FormHeap_Allocate = (_FormHeap_Allocate)0x00401000;
const _FormHeap_Free FormHeap_Free = (_FormHeap_Free)0x00401030;

const _CreateFormInstance CreateFormInstance = (_CreateFormInstance)0x00465110;

const _GetSingleton ConsoleManager_GetSingleton = (_GetSingleton)0x0071B160;
bool * bEchoConsole = (bool*)0x011F158C;

const _QueueUIMessage QueueUIMessage = (_QueueUIMessage)0x007052F0;	// Called from Cmd_AddSpell_Execute

const _ShowMessageBox ShowMessageBox = (_ShowMessageBox)0x00703E80;
const _ShowMessageBox_Callback ShowMessageBox_Callback = (_ShowMessageBox_Callback)0x005B4A70;
const _ShowMessageBox_pScriptRefID ShowMessageBox_pScriptRefID = (_ShowMessageBox_pScriptRefID)0x011CAC64;
const _ShowMessageBox_button ShowMessageBox_button = (_ShowMessageBox_button)0x0118C684;

const _GetActorValueName GetActorValueName = (_GetActorValueName)0x00066EAC0;	// See Cmd_GetActorValue_Eval
const UInt32 * g_TlsIndexPtr = (UInt32 *)0x0126FD98;
const _MarkBaseExtraListScriptEvent MarkBaseExtraListScriptEvent = (_MarkBaseExtraListScriptEvent)0x005AC750;
const _DoCheckScriptRunnerAndRun DoCheckScriptRunnerAndRun = (_DoCheckScriptRunnerAndRun)0x005AC190;

SaveGameManager ** g_saveGameManager = (SaveGameManager**)0x011DE134;

// Johnny Guitar supports this
const _GetFormByID GetFormByID = (_GetFormByID)(0x483A00);


#elif EDITOR

//	FormMap* g_FormMap = (FormMap *)0x009EE18C;		// currently unused
//	DataHandler ** g_dataHandler = (DataHandler **)0x00A0E064;
//	TES** g_TES = (TES**)0x00A0ABB0;
	const _LookupFormByID LookupFormByID = (_LookupFormByID)0x004F9620;	// Call between third reference to RTTI_TESWorldspace and RuntimeDynamicCast
	const _GetFormByID GetFormByID = (_GetFormByID)(0x004F9650); // Search for aNonPersistentR and aPlayer (third call below aPlayer, second is LookupFomrByID)
	const _FormHeap_Allocate FormHeap_Allocate = (_FormHeap_Allocate)0x00401000;
	const _FormHeap_Free FormHeap_Free = (_FormHeap_Free)0x0000401180;
	const _ShowCompilerError ShowCompilerError = (_ShowCompilerError)0x005C5730;	// Called with aNonPersistentR (still same sub as the other one)

// 0x5C64C0 <- start of huge editor function that IDA can't disassemble.

#endif

#if RUNTIME

struct TLSData
{
	// thread local storage

	UInt32	pad000[(0x260 - 0x000) >> 2];	// 000
	NiNode			* lastNiNode;			// 260	248 in FOSE
	TESObjectREFR	* lastNiNodeREFR;		// 264	24C in FOSE
	UInt8			consoleMode;			// 268
	UInt8			pad269[3];				// 269
	// 25C is used as do not head track the player , 
	// 2B8 is used to init QueudFile::unk0018, 
	// 28C might count the recursive calls to Activate, limited to 5.
};

STATIC_ASSERT(offsetof(TLSData, consoleMode) == 0x268);

static TLSData * GetTLSData()
{
	UInt32 TlsIndex = *g_TlsIndexPtr;
	TLSData * data = NULL;

	__asm {
		mov		ecx,	[TlsIndex]
		mov		edx,	fs:[2Ch]	// linear address of thread local storage array
		mov		eax,	[edx+ecx*4]
		mov		[data], eax
	}

	return data;
}

__declspec(naked) bool IsConsoleMode()
{
	__asm
	{
		mov		al, byte ptr ds:[0x11DEA2E]
		test	al, al
		jz		done
		mov		eax, dword ptr ds:[0x126FD98]
		mov		edx, fs:[0x2C]
		mov		eax, [edx+eax*4]
		test	eax, eax
		jz		done
		mov		al, [eax+0x268]
	done:
		retn
	}
}

bool GetConsoleEcho()
{
	return *bEchoConsole != 0;
}

void SetConsoleEcho(bool doEcho)
{
	*bEchoConsole = doEcho ? 1 : 0;
}

const char * GetFullName(TESForm * baseForm)
{
	if(baseForm)
	{
		TESFullName* fullName = baseForm->GetFullName();
		if(fullName && fullName->name.m_data)
		{
			if (fullName->name.m_dataLen)
				return fullName->name.m_data;
		}
	}

	return "<no name>";
}

ConsoleManager * ConsoleManager::GetSingleton(void)
{
	return (ConsoleManager *)ConsoleManager_GetSingleton(true);
}

char * ConsoleManager::GetConsoleOutputFilename(void)
{
	return GetSingleton()->COFileName;
};

bool ConsoleManager::HasConsoleOutputFilename(void) {
	return 0 != GetSingleton()->COFileName[0];
};

bool s_InsideOnActorEquipHook = false;
UInt32 s_CheckInsideOnActorEquipHook = 1;

void Console_Print(const char * fmt, ...)
{
	//if (!s_CheckInsideOnActorEquipHook || !s_InsideOnActorEquipHook) {
	ConsoleManager	* mgr = ConsoleManager::GetSingleton();
	if(mgr)
	{
		va_list	args;

		va_start(args, fmt);

		CALL_MEMBER_FN(mgr, Print)(fmt, args);

		va_end(args);
	}
	//}
}

TESSaveLoadGame * TESSaveLoadGame::Get()
{
	return (TESSaveLoadGame *)0x011DE45C;
}

SaveGameManager* SaveGameManager::GetSingleton()
{
	return *g_saveGameManager;
}

std::string GetSavegamePath()
{
	char path[0x104];
	CALL_MEMBER_FN(SaveGameManager::GetSingleton(), ConstructSavegamePath)(path);
	return path;
}

// ExtractArgsEx code
ScriptEventList *ResolveExternalVar(ScriptEventList *in_EventList, Script *in_Script, UInt8 *&scriptData)
{
	Script::RefVariable *refVar = in_Script->GetRefFromRefList(*(UInt16*)(scriptData + 1));
	if (refVar)
	{
		refVar->Resolve(in_EventList);
		TESForm *refObj = refVar->form;
		if (refObj)
		{
			scriptData += 3;
			if (refObj->typeID == kFormType_TESQuest)
				return ((TESQuest*)refObj)->scriptEventList;
			else if (((refObj->typeID >= kFormType_TESObjectREFR) && (refObj->typeID <= kFormType_FlameProjectile)) || IS_ID(refObj, ContinuousBeamProjectile))
				return ((TESObjectREFR*)refObj)->GetEventList();
		}
	}
	return NULL;
}

TESGlobal *ResolveGlobalVar(Script *scriptObj, UInt8 *&scriptData)
{
	Script::RefVariable *globalRef = scriptObj->GetRefFromRefList(*(UInt16*)(scriptData + 1));
	if (!globalRef || !globalRef->form || NOT_ID(globalRef->form, TESGlobal))
		return NULL;
	scriptData += 3;
	return (TESGlobal*)globalRef->form;
}

void ScriptEventList::Destructor()
{
// OBLIVION	ThisStdCall(0x004FB4E0, this);
	if (m_eventList)
		m_eventList->RemoveAll();
	while (m_vars) {
		if (m_vars->var) {
			FormHeap_Free(m_vars->var);
		}
		VarEntry* next = m_vars->next;
		FormHeap_Free(m_vars);
		m_vars = next;
	}
}

tList<ScriptEventList::Var>* ScriptEventList::GetVars() const
{
	return reinterpret_cast<tList<Var>*>(m_vars);
}

ScriptEventList::Var *ExtractScriptVar(UInt8 *&scriptData, Script *scriptObj, ScriptEventList *eventList)
{
	if (*scriptData == 'r')		//reference to var in another script
	{
		Script::RefVariable *refVar = scriptObj->GetRefFromRefList(*(UInt16*)(scriptData + 1));
		if (!refVar) return NULL;

		refVar->Resolve(eventList);
		TESForm *refObj = refVar->form;
		if (!refObj) return NULL;

		if IS_ID(refObj, TESQuest)
			eventList = ((TESQuest*)refObj)->scriptEventList;
		else if (((refObj->typeID >= kFormType_TESObjectREFR) && (refObj->typeID <= kFormType_FlameProjectile)) || IS_ID(refObj, ContinuousBeamProjectile))
			eventList = ((TESObjectREFR*)refObj)->GetEventList();
		else eventList = NULL;

		if (!eventList)			//couldn't resolve script ref
			return NULL;
		scriptData += 3;
	}
	if ((*scriptData == 'f') || (*scriptData == 's'))
		return eventList->GetVariable(*(UInt16*)(scriptData + 1));
	return NULL;
}

static bool ExtractFloat(double *out, UInt8 *&scriptData, Script *scriptObj, ScriptEventList *eventList)
{
	if (*scriptData == 'z')
	{
		*out = *(double*)(scriptData + 1);
		scriptData += 9;
		return true;
	}
	else if (*scriptData == 'n')
	{
		*out = *(SInt32*)(scriptData + 1);
		scriptData += 5;
		return true;
	}
	else if (*scriptData == 'G')
	{
		Script::RefVariable *globalRef = scriptObj->GetRefFromRefList(*(UInt16*)(scriptData + 1));
		if (globalRef && globalRef->form && IS_ID(globalRef->form, TESGlobal))
		{
			*out = ((TESGlobal*)globalRef->form)->data;
			scriptData += 3;
			return true;
		}
	}
	else
	{
		ScriptEventList::Var *var = ExtractScriptVar(scriptData, scriptObj, eventList);
		if (var)
		{
			scriptData += 3;
			*out = var->data;
			return true;
		}
	}
	return false;
}

TESForm* ExtractFormFromFloat(UInt8* &scriptData, Script* scriptObj, ScriptEventList* eventList)
{
	TESForm* outForm = NULL;
	if (*scriptData == 'r')		//doesn't work as intended yet so refs must be local vars
	{
		eventList = ResolveExternalVar(eventList, scriptObj, scriptData);
		if (!eventList)
			return NULL;
	}

	UInt16 varIdx = *(UInt16*)++scriptData;
	scriptData += 2;

	ScriptEventList::Var* var = eventList->GetVariable(varIdx);
	if (var)
		outForm = LookupFormByID(*((UInt64 *)&var->data));

	return outForm;
}

TESForm* ResolveForm(UInt8* &scriptData, Script* scriptObj, ScriptEventList* eventList)
{
	TESForm* outForm = NULL;
	char argType = *scriptData;
	UInt16	varIdx = *((UInt16 *)(scriptData+1));
//	scriptData += 2;

	switch (argType)
	{
	case 'r':
		{
			Script::RefVariable	* var = scriptObj->GetRefFromRefList(varIdx);
			if(var)
			{
				var->Resolve(eventList);
				outForm = var->form;
				scriptData += 3;
			}
		}
		break;
	case 'f':
			outForm = ExtractFormFromFloat(scriptData, scriptObj, eventList);
			break;
	}
	return outForm;
}

static const char* StringFromStringVar(UInt32 strID)
{
#if NVSE_CORE
	StringVar* strVar = g_StringMap.Get(strID);
	return strVar ? strVar->GetCString() : "";
#else
	if (s_StringVarInterface)
		return s_StringVarInterface->GetString(strID);
	else
		return "";
#endif
}

static const char* ResolveStringArgument(ScriptEventList* eventList, const char* stringArg)
{
	const char* result = stringArg;

	if (stringArg && stringArg[0] == '$')
	{
		VariableInfo* varInfo = eventList->m_script->GetVariableByName(stringArg + 1);
		if (varInfo)
		{
			ScriptEventList::Var* var = eventList->GetVariable(varInfo->idx);
			if (var)
				result = StringFromStringVar(var->data);
		}
	}

	return result;
}

const UInt8 kClassifyParamExtract[] =
{
	0, 1, 4, 6, 6, 2, 6, 6, 3, 6, 2, 6, 6, 6, 6, 6, 6, 6, 2, 6, 6, 6, 8, 1, 6, 6, 6, 6, 2, 6, 6, 6, 3, 6, 6,
	6, 6, 6, 6, 6, 6, 2, 6, 6, 5, 7, 8, 6, 8, 6, 6, 2, 2, 6, 6, 2, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6
};

const bool kInventoryType[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0
};

#if NVSE_CORE
static bool v_ExtractArgsEx(UInt32 numArgs, ParamInfo *paramInfo, UInt8 *&scriptData, Script *scriptObj, ScriptEventList *eventList, va_list args)
{
	UInt32 paramType;
	for (UInt32 i = 0; i < numArgs; i++)
	{
		paramType = paramInfo[i].typeID;
		if (paramType > kParamType_Region)
			return false;

		switch (kClassifyParamExtract[paramType])
		{
			case 0:
			{
				char *out = va_arg(args, char*);
				UInt32 length = *(UInt16*)scriptData;
				scriptData += 2;
				out[length] = 0;

				if (length)
				{
					memcpy(out, scriptData, length);
					scriptData += length;

					if ((length > 1) && (*out == '$'))
					{
						VariableInfo *varInfo = scriptObj->GetVariableByName(out + 1);
						if (varInfo)
						{
							ScriptEventList::Var *var = eventList->GetVariable(varInfo->idx);
							if (var)
							{
								StringVar *strVar = g_StringMap.Get((int)var->data);
								if (strVar)
								{
									length = strVar->GetLength();
									if (length)
										memcpy(out, strVar->GetCString(), length + 1);
									else *out = 0;
								}
							}
						}
					}

					if ((length == 2) && (*out == '%') && ((out[1] | 0x20) == 'e'))
						*out = 0;
				}
				break;
			}
			case 1:
			{
				SInt32 *out = va_arg(args, SInt32*);
				if (*scriptData == 'n')
				{
					*out = *(SInt32*)(scriptData + 1);
					scriptData += 5;
				}
				else
				{
					double data;
					if (!ExtractFloat(&data, scriptData, scriptObj, eventList))
						return false;
					*out = data;
				}
				break;
			}
			case 2:
			{
				UInt32 *out = va_arg(args, UInt32*);
				*out = *(UInt16*)scriptData;
				scriptData += 2;
				break;
			}
			case 3:
			{
				UInt8 *out = va_arg(args, UInt8*);
				*out = *(UInt8*)scriptData;
				scriptData += 1;
				break;
			}
			case 4:
			{
				double data;
				if (!ExtractFloat(&data, scriptData, scriptObj, eventList))
					return false;
				float *out = va_arg(args, float*);
				*out = data;
				break;
			}
			case 5:
			{
				double *out = va_arg(args, double*);
				if (!ExtractFloat(out, scriptData, scriptObj, eventList))
					return false;
				break;
			}
			case 6:
			{
				TESForm *form = NULL;
				if (*scriptData == 'r')
				{
					Script::RefVariable	*var = scriptObj->GetRefFromRefList(*(UInt16*)(scriptData + 1));
					if (var)
					{
						var->Resolve(eventList);
						form = var->form;
					}
				}
				else if (*scriptData == 'f')
				{
					ScriptEventList::Var *var = eventList->GetVariable(*(UInt16*)(scriptData + 1));
					if (var)
						form = LookupFormByRefID(*(UInt32*)&var->data);
				}
				if (!form) return false;

				switch (paramType)
				{
					case kParamType_ObjectID:
						if (!kInventoryType[form->typeID])
							return false;
						break;
					case kParamType_ObjectRef:
						if ((form->typeID < kFormType_TESObjectREFR) || ((form->typeID > kFormType_FlameProjectile) && NOT_ID(form, ContinuousBeamProjectile)))
							return false;
						break;
					case kParamType_Actor:
						if (NOT_ID(form, Character) && NOT_ID(form, Creature))
							return false;
						break;
					case kParamType_SpellItem:
						if (NOT_ID(form, SpellItem) && NOT_ID(form, TESObjectBOOK))
							return false;
						break;
					case kParamType_Cell:
						if NOT_ID(form, TESObjectCELL)
							return false;
						break;
					case kParamType_MagicItem:
					{
						MagicItem *magicItem = DYNAMIC_CAST(form, TESForm, MagicItem);
						if (!magicItem) return false;
						form = (TESForm*)magicItem;
						break;
					}
					case kParamType_Sound:
						if NOT_ID(form, TESSound)
							return false;
						break;
					case kParamType_Topic:
						if NOT_ID(form, TESTopic)
							return false;
						break;
					case kParamType_Quest:
						if NOT_ID(form, TESQuest)
							return false;
						break;
					case kParamType_Race:
						if NOT_ID(form, TESRace)
							return false;
						break;
					case kParamType_Class:
						if NOT_ID(form, TESClass)
							return false;
						break;
					case kParamType_Faction:
						if NOT_ID(form, TESFaction)
							return false;
						break;
					case kParamType_Global:
						if NOT_ID(form, TESGlobal)
							return false;
						break;
					case kParamType_Furniture:
						if (NOT_ID(form, TESFurniture) && NOT_ID(form, BGSListForm))
							return false;
						break;
					case kParamType_TESObject:
						if (!form->IsBoundObject())
							return false;
						break;
					case kParamType_MapMarker:
						if (NOT_ID(form, TESObjectREFR) || (((TESObjectREFR*)form)->baseForm->refID != 0x10))
							return false;
						break;
					case kParamType_ActorBase:
						if (NOT_ID(form, TESNPC) && NOT_ID(form, TESCreature))
							return false;
						break;
					case kParamType_Container:
						if (NOT_ID(form, Character) && NOT_ID(form, Creature) && (NOT_ID(form, TESObjectREFR) || NOT_ID(((TESObjectREFR*)form)->baseForm, TESObjectCONT)))
							return false;
						break;
					case kParamType_WorldSpace:
						if NOT_ID(form, TESWorldSpace)
							return false;
						break;
					case kParamType_AIPackage:
						if NOT_ID(form, TESPackage)
							return false;
						break;
					case kParamType_CombatStyle:
						if NOT_ID(form, TESCombatStyle)
							return false;
						break;
					case kParamType_MagicEffect:
						if NOT_ID(form, EffectSetting)
							return false;
						break;
					case kParamType_WeatherID:
						if NOT_ID(form, TESWeather)
							return false;
						break;
					case kParamType_NPC:
						if NOT_ID(form, TESNPC)
							return false;
						break;
					case kParamType_Owner:
						if (NOT_ID(form, TESFaction) && NOT_ID(form, TESNPC))
							return false;
						break;
					case kParamType_EffectShader:
						if NOT_ID(form, TESEffectShader)
							return false;
						break;
					case kParamType_FormList:
						if NOT_ID(form, BGSListForm)
							return false;
						break;
					case kParamType_MenuIcon:
						if NOT_ID(form, BGSMenuIcon)
							return false;
						break;
					case kParamType_Perk:
						if NOT_ID(form, BGSPerk)
							return false;
						break;
					case kParamType_Note:
						if NOT_ID(form, BGSNote)
							return false;
						break;
					case kParamType_ImageSpaceModifier:
						if NOT_ID(form, TESImageSpaceModifier)
							return false;
						break;
					case kParamType_ImageSpace:
						if NOT_ID(form, TESImageSpace)
							return false;
						break;
					case kParamType_EncounterZone:
						if NOT_ID(form, BGSEncounterZone)
							return false;
						break;
					case kParamType_Message:
						if NOT_ID(form, BGSMessage)
							return false;
						break;
					case kParamType_InvObjOrFormList:
						if (NOT_ID(form, BGSListForm) && !kInventoryType[form->typeID])
							return false;
						break;
					case kParamType_NonFormList:
						if (IS_ID(form, BGSListForm) || !form->IsBoundObject())
							return false;
						break;
					case kParamType_SoundFile:
						if NOT_ID(form, BGSMusicType)
							return false;
						break;
					case kParamType_LeveledOrBaseChar:
						if (NOT_ID(form, TESNPC) && NOT_ID(form, TESLevCharacter))
							return false;
						break;
					case kParamType_LeveledOrBaseCreature:
						if (NOT_ID(form, TESCreature) && NOT_ID(form, TESLevCreature))
							return false;
						break;
					case kParamType_LeveledChar:
						if NOT_ID(form, TESLevCharacter)
							return false;
						break;
					case kParamType_LeveledCreature:
						if NOT_ID(form, TESLevCreature)
							return false;
						break;
					case kParamType_LeveledItem:
						if NOT_ID(form, TESLevItem)
							return false;
						break;
					case kParamType_Reputation:
						if NOT_ID(form, TESReputation)
							return false;
						break;
					case kParamType_Casino:
						if NOT_ID(form, TESCasino)
							return false;
						break;
					case kParamType_CasinoChip:
						if NOT_ID(form, TESCasinoChips)
							return false;
						break;
					case kParamType_Challenge:
						if NOT_ID(form, TESChallenge)
							return false;
						break;
					case kParamType_CaravanMoney:
						if NOT_ID(form, TESCaravanMoney)
							return false;
						break;
					case kParamType_CaravanCard:
						if NOT_ID(form, TESCaravanCard)
							return false;
						break;
					case kParamType_CaravanDeck:
						if NOT_ID(form, TESCaravanDeck)
							return false;
						break;
					case kParamType_Region:
						if NOT_ID(form, TESRegion)
							return false;
						break;
				}

				TESForm **out = va_arg(args, TESForm**);
				*out = form;
				scriptData += 3;
				break;
			}
			case 7:
			{
				ScriptEventList::Var *var = ExtractScriptVar(scriptData, scriptObj, eventList);
				if (!var) return false;

				ScriptEventList::Var **out = va_arg(args, ScriptEventList::Var**);
				*out = var;
				scriptData += 3;
				break;
			}
			default:
				return false;
		}
	}

	return true;
}
#endif

bool ExtractArgsRaw(ParamInfo * paramInfo, void * scriptDataIn, UInt32 * scriptDataOffset, Script * scriptObj, ScriptEventList * eventList, ...)
{
	va_list	args;
	va_start(args, eventList);

	UInt8	* scriptData = ((UInt8 *)scriptDataIn) + *scriptDataOffset;

	UInt32	numArgs = *((UInt16 *)scriptData);
	scriptData += sizeof(UInt16);

	for(UInt32 i = 0; i < numArgs; i++)
	{
		ParamInfo		* info = &paramInfo[i];
		ExtractedParam	* dst = va_arg(args, ExtractedParam *);

		dst->type = ExtractedParam::kType_Unknown;
		dst->isVar = false;

		switch(info->typeID)
		{
			case kParamType_String:
				dst->type = ExtractedParam::kType_String;
				dst->data.str.len = *((UInt16 *)scriptData);
				scriptData += sizeof(UInt16);
				dst->data.str.buf = (const char *)scriptData;
				scriptData += dst->data.str.len;
				break;

			case kParamType_ActorValue:
			case kParamType_AnimationGroup:
			case kParamType_Sex:
			case kParamType_CrimeType:
			case kParamType_MiscellaneousStat:
			case kParamType_Alignment:
			case kParamType_EquipType:
			case kParamType_CriticalStage:
				dst->type = ExtractedParam::kType_Imm16;
				dst->data.imm = *((UInt16 *)scriptData);
				scriptData += sizeof(UInt16);
				break;

			case kParamType_Axis:
			case kParamType_FormType:
				dst->type = ExtractedParam::kType_Imm8;
				dst->data.imm = *((UInt8 *)scriptData);
				scriptData += sizeof(UInt8);
				break;

			case kParamType_Integer:
			case kParamType_QuestStage:
			case kParamType_Float:
			{
				UInt8	type = *scriptData++;
				switch(type)
				{
					case 'n':	// 6E
						dst->type = ExtractedParam::kType_Imm32;
						dst->data.imm = *((UInt32 *)scriptData);
						scriptData += sizeof(UInt32);
						break;

					case 'z':	// 7A
						dst->type = ExtractedParam::kType_ImmDouble;
						dst->data.immDouble = (double *)scriptData;
						scriptData += sizeof(double);
						break;

					case 'f':	// 66
					case 'r':	// 72
					case 's':	// 73
					case 'G':	// 47
					{
						ScriptEventList	* srcEventList = eventList;
						UInt8			varType = *scriptData++;

						// remote reference?
						if(varType == 'r')
						{
							// swap the event list
							UInt16	varIdx = *((UInt16 *)scriptData);
							scriptData += 2;

							Script::RefVariable	* var = scriptObj->GetRefFromRefList(varIdx);
							if(var)
							{
								TESForm	* eventListSrc = var->form;
								switch(eventListSrc->typeID)
								{
									case kFormType_TESObjectREFR:
										srcEventList = ((TESObjectREFR *)eventListSrc)->GetEventList();
										break;

									case kFormType_TESQuest:
										srcEventList = ((TESQuest *)eventListSrc)->scriptEventList;
										break;

									default:
										_ERROR("ExtractArgsRaw: unknown remote reference in number var (%02X)", eventListSrc->typeID);
										return false;
								}
							}
							else
							{
								_ERROR("ExtractArgsRaw: couldn't find remote reference in number var");
								return false;
							}
						}
						
						else
						{
							// ###

							//default:
								_ERROR("ExtractArgsRaw: unknown number var type (%02X)", type);
								return false;
						}
					}
					break;

					default:
						_ERROR("ExtractArgsRaw: unknown number type (%02X)", type);
						return false;
				}
			}
			break;

			case kParamType_ObjectID:
			case kParamType_ObjectRef:
			case kParamType_Actor:
			case kParamType_SpellItem:
			case kParamType_Cell:
			case kParamType_MagicItem:
			case kParamType_Sound:
			case kParamType_Topic:
			case kParamType_Quest:
			case kParamType_Race:
			case kParamType_Class:
			case kParamType_Faction:
			case kParamType_Global:
			case kParamType_Furniture:
			case kParamType_TESObject:
			case kParamType_MapMarker:
			case kParamType_ActorBase:
			case kParamType_Container:
			case kParamType_WorldSpace:
			case kParamType_AIPackage:
			case kParamType_CombatStyle:
			case kParamType_MagicEffect:
			case kParamType_WeatherID:
			case kParamType_NPC:
			case kParamType_Owner:
			case kParamType_EffectShader:
			case kParamType_FormList:
			case kParamType_MenuIcon:
			case kParamType_Perk:
			case kParamType_Note:
			case kParamType_ImageSpaceModifier:
			case kParamType_ImageSpace:
			case kParamType_EncounterZone:
			case kParamType_Message:
			case kParamType_InvObjOrFormList:
			case kParamType_NonFormList:
			case kParamType_SoundFile:
			case kParamType_LeveledOrBaseChar:
			case kParamType_LeveledOrBaseCreature:
			case kParamType_LeveledChar:
			case kParamType_LeveledCreature:
			case kParamType_LeveledItem:
			case kParamType_AnyForm:
			case kParamType_Reputation:
			case kParamType_Casino:
			case kParamType_CasinoChip:
			case kParamType_Challenge:
			case kParamType_CaravanMoney:
			case kParamType_CaravanCard:
			case kParamType_CaravanDeck:
			case kParamType_Region:
			{
				UInt8	type = *scriptData++;
				switch(type)
				{
					case 'r':	// constant
					{
						UInt16	varIdx = *((UInt16 *)scriptData);
						scriptData += 2;

						Script::RefVariable	* var = scriptObj->GetRefFromRefList(varIdx);
						ASSERT(var);

						var->Resolve(eventList);

						dst->type = ExtractedParam::kType_Form;
						dst->data.form = var->form;
					}
					break;

					case 'f':	// variable
					{
						UInt16	varIdx = *((UInt16 *)scriptData);
						scriptData += 2;

						dst->type = ExtractedParam::kType_Form;
						dst->isVar = true;
						dst->data.var.var = eventList->GetVariable(varIdx);
						dst->data.var.parent = eventList;
					}
					break;

					default:
						_ERROR("ExtractArgsRaw: unknown form type (%02X)", type);
						return false;
				}
			}
			break;

			case kParamType_VariableName:
				// unhandled, fall through

			default:
				_ERROR("ExtractArgsRaw: unhandled type encountered, arg %d type %02X", i, info->typeID);
				HALT("unhandled type");
				break;
		}
	}

	return true;
}

#if NVSE_CORE
bool ExtractArgsEx(ParamInfo *paramInfo, void *scriptDataIn, UInt32 *scriptDataOffset, Script *scriptObj, ScriptEventList *eventList, ...)
{
	va_list	args;
	va_start(args, eventList);

	UInt8 *scriptData = (UInt8*)scriptDataIn + *scriptDataOffset;
	UInt32 numArgs = *(UInt16*)scriptData;
	scriptData += 2;

	//DEBUG_MESSAGE("scriptData:%08x numArgs:%d paramInfo:%08x scriptObj:%08x eventList:%08x", scriptData, numArgs, paramInfo, scriptObj, eventList);

	bool bExtracted = false;


	if (numArgs > 0x7FFF)
	{
		*scriptDataOffset += 2;
		const auto beforeOffset = *scriptDataOffset;
		if (ExtractArgsOverride::ExtractArgs(paramInfo, static_cast<UInt8*>(scriptDataIn), scriptObj, eventList, scriptDataOffset, args, false, numArgs))
		{
			scriptData += *scriptDataOffset - beforeOffset;
			bExtracted = true;
		}
		else
		{
			DEBUG_PRINT("v_ExtractArgsEx returns false");
		}
	}
	else

	if (v_ExtractArgsEx(numArgs, paramInfo, scriptData, scriptObj, eventList, args))
		bExtracted = true;

	va_end(args);
	return bExtracted;
}
#endif

void ScriptEventList::Dump(void)
{
	UInt32 nEvents = m_eventList->Count();

	for(SInt32 n = 0; n < nEvents; ++n)
	{
		Event* pEvent = m_eventList->GetNthItem(n);
		if(pEvent)
		{
			Console_Print("%08X (%s) %08X", pEvent->object, GetObjectClassName(pEvent->object), pEvent->eventMask);
		}
	}
}

UInt32 ScriptEventList::ResetAllVariables()
{
#if NVSE_CORE
	OtherHooks::CleanUpNVSEVars(this);
#endif
	UInt32 numVars = 0;
	for (VarEntry * entry = m_vars; entry; entry = entry->next)
		if (entry->var)
		{
			entry->var->data = 0.0;
			numVars++;
		}
	return numVars;
}

ScriptEventList::Var * ScriptEventList::GetVariable(UInt32 id)
{
	for(VarEntry * entry = m_vars; entry; entry = entry->next)
		if(entry->var && entry->var->id == id)
			return entry->var;

	return NULL;
}




ScriptEventList* EventListFromForm(TESForm* form)
{
	ScriptEventList* eventList = NULL;
	TESObjectREFR* refr = DYNAMIC_CAST(form, TESForm, TESObjectREFR);
	if (refr)
		eventList = refr->GetEventList();
	else
	{
		TESQuest* quest = DYNAMIC_CAST(form, TESForm, TESQuest);
		if (quest)
			eventList = quest->scriptEventList;
	}

	return eventList;
}

char* ConvertLiteralPercents(char *srcPtr)
{
	char *endPtr = srcPtr + StrLen(srcPtr);
	while (srcPtr = strchr(srcPtr, '%'))
	{
		srcPtr++;
		memmove(srcPtr + 1, srcPtr, endPtr - srcPtr);
		*srcPtr++ = '%';
		endPtr++;
	}
	*endPtr = 0;
	return endPtr;
}

static void SkipArgs(UInt8* &scriptData)
{
	switch (*scriptData)
	{
	case 'z':
		scriptData += sizeof(double) + 1;
		break;
	case 'r':
		scriptData += 6;
		break;
	default:
		scriptData += 3;
		break;
	}
}

//static bool ExtractFormattedString(UInt32 &numArgs, char* buffer, UInt8* &scriptData, Script* scriptObj, ScriptEventList* eventList)
bool ExtractFormattedString(FormatStringArgs& args, char* buffer)
{
	//extracts args based on format string, prints formatted string to buffer
	static const int maxArgs = 20;
	double f[maxArgs], data;
	UInt32 argIdx = 0;

	static char fmtBuffer[0x4000];

	char *resPtr = fmtBuffer, *srcPtr = args.GetFormatString(), *fmtPos, *strPtr, *omitEndPtr;
	int size;
	TESForm *form;

	//extract args
	while (fmtPos = strchr(srcPtr, '%'))
	{
		size = fmtPos - srcPtr;
		if (size)
		{
			memcpy(resPtr, srcPtr, size);
			resPtr += size;
		}
		fmtPos++;

		switch (*fmtPos)
		{
			case '%':										//literal %
				*(UInt16*)resPtr = '%%';
				resPtr += 2;
				break;
			case 'z':
			case 'Z':										//string variable
			{
				if (!args.Arg(args.kArgType_Float, &data))
					return false;
			
				strPtr = const_cast<char*>(StringFromStringVar(data));
				if (strPtr && *strPtr)
					resPtr = StrCopy(resPtr, strPtr);
				break;
			}
			case 'r':										//newline
			case 'R':
				*resPtr++ = '\n';
				break;
			case 'e':
			case 'E':										//workaround for CS not accepting empty strings
				break;
			case 'a':
			case 'A':										//character specified by ASCII code
			{
				if (args.Arg(args.kArgType_Float, &data))
					*resPtr++ = (char)data;
				else
					return false;
				break;
			}
			case 'n':										// name of obj/ref
			case 'N':
			{
				if (!args.Arg(args.kArgType_Form, &form))
					return false;

				StrCopy(resPtr, GetFullName(form));
				resPtr = ConvertLiteralPercents(resPtr);
				break;
			}
			case 'i':											//formID
			case 'I':
			{
				if (!args.Arg(args.kArgType_Form, &form))
					return false;

				resPtr += sprintf_s(resPtr, 9, "%08X", form ? form->refID : 0);
				break;
			}
			case 'c':											//named component of another object
			case 'C':											//2 args - object and index
			{
				if (!args.Arg(args.kArgType_Form, &form))
					return false;

				if (form)
				{
					if (!args.Arg(args.kArgType_Float, &data))
						return false;
					else
					{
						switch(form->typeID)
						{
							case kFormType_TESAmmo:
							{
								switch((int)data)
								{
									default:
									case 0:	// full name
										StrCopy(resPtr, GetFullName(form));
										break;
									case 1:	// short name
										StrCopy(resPtr, ((TESAmmo*)form)->shortName.CStr());
										break;
									case 2:	// abbrev
										StrCopy(resPtr, ((TESAmmo*)form)->abbreviation.CStr());
										break;
								}
								resPtr = ConvertLiteralPercents(resPtr);
								break;
							}
							case kFormType_TESFaction:
							{
								StrCopy(resPtr, ((TESFaction*)form)->GetNthRankName(data));
								resPtr = ConvertLiteralPercents(resPtr);
								break;
							}
						}
					}
				}
				break;
			}
			case 'k':
			case 'K':											//DX code
			{
				if (!args.Arg(args.kArgType_Float, &data))
					return false;

				resPtr = StrCopy(resPtr, GetDXDescription(data));
				break;
			}
			case 'v':
			case 'V':											//actor value
			{
				if (!args.Arg(args.kArgType_Float, &data))
					return false;

				resPtr = StrCopy(resPtr, GetActorValueString(data));
				break;
			}
			case 'p':
			case 'P':											//pronouns
			{
				fmtPos++;
				if (!args.Arg(args.kArgType_Form, &form))
					return false;

				if (form)
				{			
					if (form->GetIsReference())
						form = ((TESObjectREFR*)form)->baseForm;

					UInt8 objType = 0;
					if (form->typeID == kFormType_TESNPC)
						objType = ((TESNPC*)form)->baseData.IsFemale() ? 2 : 1;

					switch (*fmtPos)
					{
						case 'o':
						case 'O':
						{
							switch (objType)
							{
								default:
								case 0:
									*(UInt16*)resPtr = 'ti';
									resPtr += 2;
									break;
								case 1:
									*(UInt32*)resPtr = '\0mih';
									resPtr += 3;
									break;
								case 2:
									*(UInt32*)resPtr = '\0reh';
									resPtr += 3;
									break;
							}
							break;
						}
						case 's':
						case 'S':
						{
							switch (objType)
							{
								default:
								case 0:
									*(UInt16*)resPtr = 'ti';
									resPtr += 2;
									break;
								case 1:
									*(UInt16*)resPtr = 'eh';
									resPtr += 2;
									break;
								case 2:
									*(UInt32*)resPtr = '\0ehs';
									resPtr += 3;
									break;
							}
							break;
						}
						case 'p':
						case 'P':
						{
							switch (objType)
							{
								default:
								case 0:
									*(UInt32*)resPtr = '\0sti';
									break;
								case 1:
									*(UInt32*)resPtr = '\0sih';
									break;
								case 2:
									*(UInt32*)resPtr = '\0reh';
									break;
							}
							resPtr += 3;
							break;
						}
					}
				}
				break;
			}
			case 'q':
			case 'Q':											//double quote
				*resPtr++ = '\"';
				break;
			case '{':											//omit portion of string based on flag param
			{
				omitEndPtr = strstr(fmtPos + 1, "%}");
				if (omitEndPtr)
				{
					if (!args.Arg(args.kArgType_Float, &data))
						return false;

					if (data)
						omitEndPtr[1] = 'e';
					else
					{
						fmtPos++;
						while ((strPtr = strchr(fmtPos, '%')) && (strPtr < omitEndPtr) && args.HasMoreArgs())
						{
							strPtr++;
							switch (*strPtr)
							{
								case '%':
								case 'q':
								case 'Q':
								case 'r':
								case 'R':
									break;
								case 'c':
								case 'C':
									args.SkipArgs(2);
									break;
								default:
									args.SkipArgs(1);
							}
							fmtPos = strPtr + 1;
						}
						fmtPos = omitEndPtr + 1;
					}
				}
				break;
			}
			case '}':											//in case someone left a stray closing bracket
				break;
			case 'x':											//hex
			case 'X':
			{
				if (!args.Arg(args.kArgType_Float, &data))
					return false;

				*(UInt64*)(&f[argIdx++]) = data;
				*(UInt16*)resPtr = '0%';
				resPtr += 2;
				if ((fmtPos[1] >= '0') && (fmtPos[1] <= '9'))
				{
					*resPtr++ = fmtPos[1];
					fmtPos++;
				}
				*(UInt32*)resPtr = '\0Xll';
				resPtr += 3;
				break;
			}
			default:											//float
			{
				if (!args.Arg(args.kArgType_Float, &data))
					return false;

				f[argIdx++] = data;
				*resPtr++ = '%';
				*resPtr++ = *fmtPos;
				break;
			}
		}

		srcPtr = fmtPos + 1;
	}

	if (size = StrLen(srcPtr))
	{
		memcpy(resPtr, srcPtr, size);
		resPtr += size;
	}
	*resPtr = 0;

	if (fmtBuffer[0])
	{
		if (argIdx)
		{
			sprintf_s(buffer, kMaxMessageLength - 2, fmtBuffer, f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7], f[8], f[9], f[10], f[11], f[12], f[13], f[14], f[15], f[16], f[17], f[18], f[19]);
			buffer[kMaxMessageLength - 1] = 0;
		}
		else memcpy(buffer, fmtBuffer, (resPtr - fmtBuffer) + 1);
	}
	else *buffer = 0;

	return true;
}

void RegisterStringVarInterface(NVSEStringVarInterface* intfc)
{
	s_StringVarInterface = intfc;
}

UInt32 ScriptEventList::Var::GetFormId()
{
	return *reinterpret_cast<UInt32*>(&data);
}

#if NVSE_CORE

//fmtStringPos is index of fmtString param in paramInfo, with first param = 0
bool ExtractFormatStringArgs(UInt32 fmtStringPos, char* buffer, ParamInfo * paramInfo, void * scriptDataIn, UInt32 * scriptDataOffset, Script * scriptObj, ScriptEventList * eventList, UInt32 maxParams, ...)
{
	va_list args;
	va_start(args, maxParams);

	UInt8	* scriptData = ((UInt8 *)scriptDataIn) + *scriptDataOffset;
	SInt16	numArgs = *((SInt16 *)scriptData);
	scriptData += 2;

	if (numArgs < 0) {
		bool bResult = ExtractArgsOverride::ExtractFormattedString(paramInfo, static_cast<UInt8*>(scriptDataIn), scriptObj, eventList, scriptDataOffset, args,
			fmtStringPos, buffer, maxParams);
		if (!bResult) {
			DEBUG_PRINT("ExtractFormatStringArgs returns false");
		}
		return bResult;
	}

	numArgs -= fmtStringPos + 1;

	bool bExtracted = false;
	if (fmtStringPos > 0)
	{
		bExtracted = v_ExtractArgsEx(fmtStringPos, paramInfo, scriptData, scriptObj, eventList, args);
		if (!bExtracted)
			return false;
	}

	ScriptFormatStringArgs scriptArgs(numArgs, scriptData, scriptObj, eventList);
	bExtracted = ExtractFormattedString(scriptArgs, buffer);

	numArgs = scriptArgs.GetNumArgs();
	scriptData = scriptArgs.GetScriptData();
	//NOTE: if v_ExtractArgsEx was called above, passing args again in second call below = undefined behavior. Needs fixing.
	if (bExtracted && numArgs > 0)			//some optional normal params following format string params
	{
		if ((numArgs + fmtStringPos + 21) > maxParams)		//scripter included too many optional params - adjust to prevent crash
			numArgs = (maxParams - fmtStringPos - 21);

		bExtracted = v_ExtractArgsEx(numArgs, &(paramInfo[fmtStringPos + 21]), scriptData, scriptObj, eventList, args);
	}

	va_end(args);
	return bExtracted;
}

#endif

bool ExtractSetStatementVar(Script* script, ScriptEventList* eventList, void* scriptDataIn, double * outVarData, bool* makeTemporary, UInt32* opcodeOffsetPtr, UInt8* outModIndex)
{
	/*	DOES NOT WORK WITH FalloutNV, we are going to abuse the stack instead: ~ It does work, just have to adjust the value to 0x1D5 :^)
	//when script command called as righthand side of a set statement, the script data containing the variable
	//to assign to remains on the stack as arg to a previous function. We can get to it through scriptData in COMMAND_ARGS
	*/
	auto* scriptData = GetScriptDataPosition(script, scriptDataIn, opcodeOffsetPtr) - *opcodeOffsetPtr - 1;

	auto* dataStart = scriptData + 1; // should be 0x58 (or 0x72 if called with dot syntax)
	if (*dataStart != 0x58 && *dataStart != 0x72)
	{
		return false;
	}

	if (*(scriptData - 5) != 0x73) // make sure `set ... to` and not `if ...`
	{
		return false;
	}

	*makeTemporary = false;

	// Calculate frame pointer for 4 calls above:
	/*void* callerFramePointer;
	_asm {
		mov callerFramePointer, ebp
	}
	for (int i = 0; i < 3; i++)
		callerFramePointer = (void*)(*(UInt32*)callerFramePointer);
	if (!shortPath) {
		callerFramePointer = (void*)(*(UInt32*)callerFramePointer);	// sv_Destruct calls us directly, others goes through AssignToStringVar
		callerFramePointer = (void*)(*(UInt32*)callerFramePointer);	// one more added for when multiple commands are grouped (like GetBipedModelPath)
	}

	UInt32 scriptDataPtrAddr = (UInt32)(callerFramePointer) + 0x08;
	UInt32* scriptDataAddr = (UInt32*)scriptDataPtrAddr;
	UInt8* scriptData = (UInt8*)(*scriptDataAddr);

	SInt32 scriptDataOffset = (UInt32)scriptData - (UInt32)(script->data);*/
	//auto scriptData = reinterpret_cast<UInt8*>(g_lastScriptData);
	SInt32 scriptDataOffset = (UInt32)scriptData - (UInt32)(script->data);

	if (scriptDataOffset < 5)
		return false;

	bool bExtracted = false;
	scriptData -= 5;

	switch (*scriptData)			//work backwards from opcode to find lefthand var
	{
	case 'G':						//global
		{
			UInt16 refIdx = *(UInt16*)(scriptData + 1);
			Script::RefVariable* refVar = script->GetRefFromRefList(refIdx);
			if (!refVar)
				break;

			TESGlobal* globalVar = DYNAMIC_CAST(refVar->form, TESForm, TESGlobal);
			if (globalVar)
			{
				*outVarData = globalVar->data;
				if (outModIndex)
					*outModIndex = (globalVar->refID >> 24);
				bExtracted = true;
			}
		}
		break;
	case 'l':
	case 'f':
	case 's':
		{
			bool isExternalVar = false;
			if (scriptDataOffset >= 8 && *(scriptData - 3) == 'r')	//external var
			{
				isExternalVar = true;
				UInt16 refIdx = *(UInt16*)(scriptData - 2);
				Script::RefVariable* refVar = script->GetRefFromRefList(refIdx);
				if (!refVar)
					break;

				refVar->Resolve(eventList);
				TESForm* refForm = refVar->form;
				if (!refForm)
					break;

				if (refForm->typeID == kFormType_TESObjectREFR)
				{
					TESObjectREFR* refr = DYNAMIC_CAST(refForm, TESForm, TESObjectREFR);
					TESScriptableForm* scriptable = DYNAMIC_CAST(refr->baseForm, TESForm, TESScriptableForm);
					if (scriptable)
					{
						script = scriptable->script;
						eventList = refr->GetEventList();
					}
					else
						break;
				}
				else if (refForm->typeID == kFormType_TESQuest)
				{
					TESScriptableForm* scriptable = DYNAMIC_CAST(refForm, TESForm, TESScriptableForm);
					if (scriptable)
					{
						script = scriptable->script;
						TESQuest* quest = DYNAMIC_CAST(scriptable, TESScriptableForm, TESQuest);
						eventList = quest->scriptEventList;
					}
					else
						break;
				}
				else
					break;
			}

			UInt16 varIdx = *(UInt16*)(scriptData + 1);
			ScriptEventList::Var* var = eventList->GetVariable(varIdx);
			if (var)
			{
				*outVarData = var->data;
				if (outModIndex)
					*outModIndex = (script->refID >> 24);
				bExtracted = true;
				if (!isExternalVar)
				{
					if (script->IsUserDefinedFunction())
					{
						*makeTemporary = true;
					}
#if NVSE_CORE
					g_nvseVarGarbageCollectionMap[eventList].Emplace(varIdx, NVSEVarType::kVarType_String);
#endif
				}
			}
		}
		break;
	default:
		SCRIPT_ASSERT(false, script, "Function must be used within a Set statement");
	}

	return bExtracted;
}

// g_baseActorValueNames is only filled in after oblivion's global initializers run
const char* GetActorValueString(UInt32 actorValue)
{
	char* name = 0;
	if (actorValue <= eActorVal_FalloutMax)
		name = GetActorValueName(actorValue);
	if (!name)
		name = "unknown";

	return name;
}

UInt32 GetActorValueForScript(const char* avStr) 
{
	for (UInt32 i = 0; i <= eActorVal_FalloutMax; i++) {
		char* name = GetActorValueName(i);
		if (_stricmp(avStr, name) == 0)
			return i;
	}

	return eActorVal_NoActorValue;
}

UInt32 GetActorValueForString(const char* strActorVal, bool bForScript)
{
	if (bForScript)
		return GetActorValueForScript(strActorVal);

	for (UInt32 n = 0; n <= eActorVal_FalloutMax; n++) {
		char* name = GetActorValueName(n);
		if (_stricmp(strActorVal, name) == 0)
			return n;
	}
	return eActorVal_NoActorValue;
}

ScriptFormatStringArgs::ScriptFormatStringArgs(UInt32 _numArgs, UInt8* _scriptData, Script* _scriptObj, ScriptEventList* _eventList)
: numArgs(_numArgs), scriptData(_scriptData), scriptObj(_scriptObj), eventList(_eventList)
{
	//extract format string
	UInt32 len = *(UInt16*)scriptData;
	scriptData += 2;
	memcpy(s_tempStrArgBuffer, scriptData, len);
	s_tempStrArgBuffer[len] = 0;
	scriptData += len;
	if (s_tempStrArgBuffer[0] == '$')
	{
		VariableInfo *varInfo = eventList->m_script->GetVariableByName(s_tempStrArgBuffer + 1);
		if (varInfo)
		{
			ScriptEventList::Var *var = eventList->GetVariable(varInfo->idx);
			if (var)
			{
				const char *strVar = StringFromStringVar(var->data);
				if (strVar) StrCopy(s_tempStrArgBuffer, strVar);
			}
		}
	}
}

char *ScriptFormatStringArgs::GetFormatString()
{
	return s_tempStrArgBuffer;
}

bool ScriptFormatStringArgs::HasMoreArgs()
{
	return (numArgs > 0);
}

UInt32 ScriptFormatStringArgs::GetNumArgs()
{
	return numArgs;
}

UInt8* ScriptFormatStringArgs::GetScriptData()
{
	return scriptData;
}

bool ScriptFormatStringArgs::SkipArgs(UInt32 numToSkip)
{
	while (numToSkip--)
	{
		switch (*scriptData)
		{
		case 'z':
			scriptData += sizeof(double) + 1;
			break;
		case 'r':
			scriptData += 6;
			break;
		default:
			scriptData += 3;
			break;
		}

		numArgs--;
	}

	return true;
}

//Log error if expression evaluates to false
bool SCRIPT_ASSERT(bool expr, Script* script, const char * errorMsg, ...)
{
	//	static bool bAlerted = false;			//only alert user on first error
	//	static std::set<UInt32> naughtyScripts;	//one error per script to avoid thrashing
	//
	//	if (!expr && naughtyScripts.find(script->refID) == naughtyScripts.end())
	//	{
	//		const ModEntry ** activeMods = (*g_dataHandler)->GetActiveModList();
	//		UInt8 modIndex = script->GetModIndex();
	//		const ModEntry * modEntry = activeMods[modIndex];
	//
	//		const char * modName;
	//		if (modIndex != 0xFF && modEntry && modEntry->data && modEntry->data->name)
	//			modName = modEntry->data->name;
	//		else
	//			modName = "Unknown";
	//
	////		sprintf_s(errorHeader, sizeof(errorHeader) - 1, "** Error: Script %08X in file \"%s\" **", script->refID, modName);
	////		_MESSAGE("%s", errorHeader);
	//		_MESSAGE("** Script Error: Script %08x in file \"%s\" **", script->refID, modName);
	//
	//		va_list args;
	//		va_start(args, errorMsg);
	//
	//		char errorBuf[512];
	//		vsprintf_s(errorBuf, sizeof(errorBuf) - 1, errorMsg, args);
	//		va_end(args);
	//
	//		gLog.Indent();
	//		_MESSAGE("%s", errorBuf);
	//		gLog.Outdent();
	//
	//		if (!bAlerted)
	//		{
	//			MessageBoxAlert("NVSE has detected a script error. \n\nPlease check nvse.log for details.");
	//			bAlerted = true;
	//		}
	//
	//		naughtyScripts.insert(script->refID);
	//	}
	return expr;
}

bool ScriptFormatStringArgs::Arg(FormatStringArgs::argType asType, void * outResult)
{
	if (!SCRIPT_ASSERT((numArgs > 0), scriptObj, "Too few args for format specifier"))
		return false;

	numArgs--;

	switch (asType)
	{
		case kArgType_Float:
		{
			if (ExtractFloat((double*)outResult, scriptData, scriptObj, eventList))
				return true;
			break;
		}
		case kArgType_Form:
		{
			TESForm* form = ExtractFormFromFloat(scriptData, scriptObj, eventList);
			*((TESForm**)outResult) = form;
			return true;
		}
	}

	return false;
}

void ShowCompilerError(ScriptLineBuffer* lineBuf, const char * fmt, ...)
{

	char errorHeader[0x400];
	UInt32 offset = sprintf_s(errorHeader, 0x400, "Error on line %d\n\n", lineBuf->lineNumber);

	va_list	args;
	va_start(args, fmt);

	char	errorMsg[0x200];
	vsprintf_s(errorMsg, 0x200, fmt, args);

	strcat_s(errorHeader, 0x400, errorMsg);
	Console_Print(errorHeader);
	_MESSAGE(errorHeader);

	va_end(args);
}

UInt32 GetActorValueMax(UInt32 actorValueCode) {
	switch (actorValueCode ) {
		case eActorVal_Aggression:			return   3; break;
		case eActorVal_Confidence:			return   4; break;
		case eActorVal_Energy:				return 100; break;
		case eActorVal_Responsibility:		return 100; break;
		case eActorVal_Mood:				return   8; break;

		case eActorVal_Strength:			return  10; break;
		case eActorVal_Perception:			return  10; break;
		case eActorVal_Endurance:			return  10; break;
		case eActorVal_Charisma:			return  10; break;
		case eActorVal_Intelligence:		return  10; break;
		case eActorVal_Agility:				return  10; break;
		case eActorVal_Luck:				return  10; break;

		case eActorVal_ActionPoints:		return   1; break;
		case eActorVal_CarryWeight:			return   1; break;
		case eActorVal_CritChance:			return 100; break;
		case eActorVal_HealRate:			return   1; break;
		case eActorVal_Health:				return   1; break;
		case eActorVal_MeleeDamage:			return   1; break;
		case eActorVal_DamageResistance:	return   1; break;
		case eActorVal_PoisonResistance:	return   1; break;
		case eActorVal_RadResistance:		return   1; break;
		case eActorVal_SpeedMultiplier:		return   1; break;
		case eActorVal_Fatigue:				return   1; break;
		case eActorVal_Karma:				return   1; break;
		case eActorVal_XP:					return   1; break;

		case eActorVal_Head:				return 100; break;
		case eActorVal_Torso:				return 100; break;
		case eActorVal_LeftArm:				return 100; break;
		case eActorVal_RightArm:			return 100; break;
		case eActorVal_LeftLeg:				return 100; break;
		case eActorVal_RightLeg:			return 100; break;
		case eActorVal_Brain:				return 100; break;

		case eActorVal_Barter:				return 100; break;
		case eActorVal_BigGuns:				return 100; break;
		case eActorVal_EnergyWeapons:		return 100; break;
		case eActorVal_Explosives:			return 100; break;
		case eActorVal_Lockpick:			return 100; break;
		case eActorVal_Medicine:			return 100; break;
		case eActorVal_MeleeWeapons:		return 100; break;
		case eActorVal_Repair:				return 100; break;
		case eActorVal_Science:				return 100; break;
		case eActorVal_Guns:				return 100; break;
		case eActorVal_Sneak:				return 100; break;
		case eActorVal_Speech:				return 100; break;
		case eActorVal_Survival:			return 100; break;
		case eActorVal_Unarmed:				return 100; break;

		case eActorVal_InventoryWeight:		return   1; break;
		case eActorVal_Paralysis:			return   1; break;
		case eActorVal_Invisibility:		return   1; break;
		case eActorVal_Chameleon:			return   1; break;
		case eActorVal_NightEye:			return   1; break;
		case eActorVal_Turbo:				return   1; break;
		case eActorVal_FireResistance:		return   1; break;
		case eActorVal_WaterBreathing:		return   1; break;
		case eActorVal_RadLevel:			return   1; break;
		case eActorVal_BloodyMess:			return   1; break;
		case eActorVal_UnarmedDamage:		return   1; break;
		case eActorVal_Assistance:			return   2; break;

		case eActorVal_ElectricResistance:	return   1; break;

		case eActorVal_EnergyResistance:	return   1; break;
		case eActorVal_EMPResistance:		return   1; break;
		case eActorVal_Var1Medical:			return   1; break;
		case eActorVal_Var2:				return   1; break;
		case eActorVal_Var3:				return   1; break;
		case eActorVal_Var4:				return   1; break;
		case eActorVal_Var5:				return   1; break;
		case eActorVal_Var6:				return   1; break;
		case eActorVal_Var7:				return   1; break;
		case eActorVal_Var8:				return   1; break;
		case eActorVal_Var9:				return   1; break;
		case eActorVal_Var10:				return   1; break;

		case eActorVal_IgnoreCrippledLimbs:	return   1; break;
		case eActorVal_Dehydration:			return   1; break;
		case eActorVal_Hunger:				return   1; break;
		case eActorVal_Sleepdeprevation:	return   1; break;
		case eActorVal_Damagethreshold:		return   1; break;
		default: return 1;
	}
}

typedef FontManager* (* _FontManager_GetSingleton)(void);
const _FontManager_GetSingleton FontManager_GetSingleton = (_FontManager_GetSingleton)0x011F33F8;

FontManager* FontManager::GetSingleton()
{
	return FontManager_GetSingleton();
}

FontManager::FontInfo* FontManager::FontInfo::Load(const char* path, UInt32 ID)
{
	FontInfo* info = (FontInfo*)FormHeap_Allocate(sizeof(FontInfo));
	return (FontManager::FontInfo*)ThisStdCall(0x00A12020, info, ID, path, 1);
}

bool FontManager::FontInfo::GetName(char* out)
{
	UInt32 len = strlen(path);
	len -= 4;					// '.fnt'
	UInt32 start = len;
	while (path[start-1] != '\\') {
		start--;
	}

	len -= start;

	memcpy(out, path+start, len);
	out[len] = 0;

	return true;
}

void Debug_DumpFontNames(void)
{
	FontManager::FontInfo** fonts = FontManager::GetSingleton()->fontInfos;

	for(UInt32 i = 0; i < FontArraySize; i++)
		_MESSAGE("Font %d is named %s", i+1, fonts[i]->path);

}

UInt8* GetScriptDataPosition(Script* script, void* scriptDataIn, const UInt32* opcodeOffsetPtrIn)
{
	if (scriptDataIn != script->data) // set ... to or if ..., script data is stored on stack and not heap
	{
		auto* scriptData = *(static_cast<UInt8**>(scriptDataIn) + 0x1D5);
		return scriptData + *opcodeOffsetPtrIn + 1;
	}
	return static_cast<UInt8*>(scriptDataIn) + *opcodeOffsetPtrIn;
}

#endif
