#pragma once

#include "GameForms.h"
#include "GameBSExtraData.h"
#include "GameExtraData.h"
#include "GameProcess.h"


template <typename T_Ret = void, typename ...Args> /////////////////////taken from JIP
__forceinline T_Ret ThisCallAlt(UInt32 _addr, void* _this, Args ...args)
{
	return ((T_Ret(__thiscall*)(void*, Args...))_addr)(_this, std::forward<Args>(args)...);
}



class TESObjectCELL;
struct ScriptEventList;
class ActiveEffect;
class NiNode;
class Animation;

// 008
class TESChildCell
{
public:
	TESChildCell();
	~TESChildCell();

	// no virtual destructor
	virtual TESObjectCELL *	GetPersistentCell(void);		// 000

//	void	** vtbl;	// 00
};

// 68
class TESObjectREFR : public TESForm
{
public:
	MEMBER_FN_PREFIX(TESObjectREFR);

	TESObjectREFR();
	~TESObjectREFR();

	virtual void		Unk_4E(void);	// GetStartingPosition(Position, Rotation, WorldOrCell)
	virtual void		Unk_4F(void);
	virtual void		Unk_50(void);
	virtual void		Unk_51(void);
	virtual bool		CastShadows(void);
	virtual void		Unk_53(void);
	virtual void		Unk_54(void);
	virtual void		Unk_55(void);
	virtual void		Unk_56(void);
	virtual void		Unk_57(void);
	virtual void		Unk_58(void);
	virtual void		Unk_59(void);
	virtual void		Unk_5A(void);
	virtual void		Unk_5B(void);
	virtual void		Unk_5C(void);
	virtual void		Unk_5D(void);
	virtual void		Unk_5E(void);
	virtual TESObjectREFR*		RemoveItem(TESForm* toRemove, BaseExtraList* extraList, UInt32 count, UInt32 unk3, UInt32 unk4, TESObjectREFR* destRef, 
		UInt32 unk6, UInt32 unk7, UInt32 unk8, UInt8 unk9);	// 40 unk2 quantity? Returns the reference assigned to the removed item.
	virtual void		Unk_60(void);
	virtual void		Unk_61(void);	// Linked to AddItem, (item, count, ExtraDataList), func0042 in OBSE
	virtual void		Unk_62(void);	// Linked to Unequip (and or equip maybe)
	virtual void		Unk_63(void);
	virtual void		AddItem(TESForm* item, ExtraDataList* xDataList, UInt32 Quantity);	// Needs confirmation
	virtual void		Unk_65(void);
	virtual void		Unk_66(void);
	virtual void		Unk_67(void);					// Actor: GetMagicEffectList
	virtual bool		GetIsChildSize(bool checkHeight);		// 068 Actor: GetIsChildSize
	virtual UInt32		GetActorUnk0148(void);			// result can be interchanged with baseForm, so TESForm* ?
	virtual void		SetActorUnk0148(UInt32 arg0);
	virtual void		Unk_6B(void);
	virtual void		Unk_6C(void);	// REFR: GetBSFaceGenNiNodeSkinned
	virtual void		Unk_6D(void);	// REFR: calls 006C
	virtual void		Unk_6E(void);	// MobileActor: calls 006D then NiNode::Func0040
	virtual void		Unk_6F(void);
	virtual void		Unk_70(void);
	virtual void		AnimateNiNode(void);					// same in FOSE ! identical to Func0052 in OBSE which says (inits animation-related data, and more)
	virtual void		GenerateNiNode(bool arg0);				// same in FOSE !
	virtual void		Set3D(NiNode* niNode, bool unloadArt);	// same in FOSE !
	virtual NiNode *	GetNiNode(void);						// same in FOSE !
	virtual void		Unk_75(void);
	virtual void		Unk_76(void);
	virtual void		Unk_77(void);
	virtual void		Unk_78(void);
	virtual AnimData*	GetAnimData(void);			// 0079
	virtual ValidBip01Names * GetValidBip01Names(void);	// 007A	Character only
	virtual ValidBip01Names * CallGetValidBip01Names(void);
	virtual void		SetValidBip01Names(ValidBip01Names validBip01Names);
	virtual void		GetPos(void);				// GetPos or GetDistance
	virtual void		Unk_7E(UInt32 arg0);
	virtual void		Unk_7F(void);
	virtual void		Unk_80(UInt32 arg0);
	virtual void		Unk_81(UInt32 arg0);
	virtual void		Unk_82(void);
	virtual UInt32		Unk_83(void);
	virtual void		Unk_84(UInt32 arg0);
	virtual UInt32		Unk_85(void);			// 0 or GetActor::Unk01AC
	virtual bool		Unk_86(void);			// return false for Projectile, Actor and Creature, true for character and PlayerCharacter
	virtual bool		Unk_87(void);			// seems to always return 0
	virtual bool		Unk_88(void);			// seems to always return 0
	virtual void		Unk_89(void);
	virtual void		Unk_8A(void);			// SetParentCell (Interior only ?)
	virtual void		Unk_8B(void);			// IsDead = HasNoHealth (baseForm health <= 0 or Flags bit23 set)
	virtual bool		Unk_8C(void);
	virtual bool		Unk_8D(void);
	virtual void		Unk_8E(void);
	virtual void		Unk_8F(void);

	enum {
		kFlags_Unk00000002	= 0x00000002,
		kFlags_Deleted		= 0x00000020,		// refr removed from .esp or savegame
		kFlags_Persistent	= 0x00000400,		//shared bit with kFormFlags_QuestItem
		kFlags_Temporary	= 0x00004000,
		kFlags_Taken		= kFlags_Deleted | kFlags_Unk00000002,

		kChanged_Inventory	= 0x08000000,
	};

	struct RenderState
	{
		UInt32	unk00;
		UInt32	unk04;
		float	unk08;		// waterLevel
		float	unk0C;
		UInt32	unk10;		// flags most likely
		NiNode	* niNode;	// same in FOSE
		// possibly more, need to find alloc
	};

	struct EditorData {
		UInt32	unk00;	// 00
	};
	// 0C

#ifdef EDITOR
	EditorData	editorData;			// +04
#endif

	TESChildCell	childCell;				// 018

	UInt32			unk1C;					// 01C

	TESForm			* baseForm;				// 020
	
	float			rotX, rotY, rotZ;		// 024 - either public or accessed via simple inline accessor common to all child classes
	float			posX, posY, posZ;		// 030 - seems to be private
	float			scale;					// 03C 

	TESObjectCELL	* parentCell;			// 040
	ExtraDataList	extraDataList;			// 044
	RenderState		* renderState;			// 064	- (05C in FOSE)

	ScriptEventList *	GetEventList() const;

	bool IsTaken() const { return (flags & kFlags_Taken) != 0; }
	bool IsPersistent() const { return (flags & kFlags_Persistent) != 0; }
	bool IsTemporary() { return (flags & kFlags_Temporary) ? true : false; }
	bool IsDeleted() const { return (flags & kFlags_Deleted) ? true : false; }

	bool Update3D();
	bool Update3D_v1c();	// Less worse version as used by some modders
	TESContainer* GetContainer();
	bool IsMapMarker();
	bool GetInventoryItems(InventoryItemsMap &invItems);
	TESForm* GetBaseForm();
	TESForm* GetBaseForm2();
	bool GetDisabled(); //////// all from JIP
	ExtraContainerChanges::EntryData* GetContainerChangesEntry(TESForm* itemForm);

	static TESObjectREFR* Create(bool bTemp = false);

	MEMBER_FN_PREFIX(TESObjectREFR);
#if RUNTIME
	DEFINE_MEMBER_FN(Activate, bool, 0x00573170, TESObjectREFR*, UInt32, UInt32, UInt32);	// Usage Activate(actionRef, 0, 0, 1); found inside Cmd_Activate_Execute as the last call (190 bytes)
	DEFINE_MEMBER_FN(Set3D, void, 0x0094EB40, NiNode*, bool);	// Usage Set3D(niNode, true); virtual func 0073
#endif
};

TESForm* GetPermanentBaseForm(TESObjectREFR* thisObj);	// For LevelledForm, find real baseForm, not temporary one.

STATIC_ASSERT(offsetof(TESObjectREFR, baseForm) == 0x020);
STATIC_ASSERT(offsetof(TESObjectREFR, extraDataList) == 0x044);
STATIC_ASSERT(sizeof(TESObjectREFR) == 0x068);

class BaseProcess;

// 088
class MobileObject : public TESObjectREFR
{
public:
	MobileObject();
	~MobileObject();

	virtual void		StartHighProcess(void);		// 090
	virtual void		Unk_91(void);
	virtual void		Unk_92(void);
	virtual void		Unk_93(void);
	virtual void		Unk_94(void);
	virtual void		Unk_95(void);
	virtual void		Unk_96(void);
	virtual void		Unk_97(void);
	virtual void		Unk_98(void);
	virtual void		Unk_99(void);
	virtual void		Unk_9A(void);
	virtual void		Unk_9B(void);
	virtual void		Unk_9C(void);
	virtual void		Unk_9D(void);
	virtual void		Unk_9E(void);
	virtual void		Unk_9F(void);
	virtual void		Unk_A0(void);	// StartConversation(targetActor, subjectLocationData, targetLocationData, headTrack, allowMovement, arg, topicID, arg, arg
	virtual void		Unk_A1(void);
	virtual void		Unk_A2(void);
	virtual void		Unk_A3(void);
	virtual void		Unk_A4(void);
	virtual void		Unk_A5(void);
	virtual void		Unk_A6(void);
	virtual void		Unk_A7(void);
	virtual void		Unk_A8(void);
	virtual void		Unk_A9(void);
	virtual void		Unk_AA(void);
	virtual void		Unk_AB(void);
	virtual void		Unk_AC(void);
	virtual void		Unk_AD(void);
	virtual void		Unk_AE(void);
	virtual void		Unk_AF(void);
	virtual void		Unk_B0(void);
	virtual void		Unk_B1(void);
	virtual void		Unk_B2(void);
	virtual void		Unk_B3(void);
	virtual void		Unk_B4(void);
	virtual void		Unk_B5(void);
	virtual void		Unk_B6(void);
	virtual void		Unk_B7(void);
	virtual void		Unk_B8(void);
	virtual void		Unk_B9(void);
	virtual void		Unk_BA(void);
	virtual void		Unk_BB(void);
	virtual void		Unk_BC(void);
	virtual void		Unk_BD(void);
	virtual void		Unk_BE(void);
	virtual void		Unk_BF(void);
	virtual void		Unk_C0(void);
	
	BaseProcess	* baseProcess;	// 068
	UInt32		unk06C;			// 06C - loaded	set to the talking actor ref when initialising ExtraTalkingActor
	UInt32		unk070;			// 070 - loaded
	UInt32		unk074;			// 074 - loaded
	UInt32		unk078;			// 078 - loaded
	UInt8		unk07C;			// 07C - loaded
	UInt8		unk07D;			// 07D - loaded
	UInt8		unk07E;			// 07E - loaded
	UInt8		unk07F;			// 07F - loaded
	UInt8		unk080;			// 080 - loaded
	UInt8		unk081;			// 081 - loaded
	UInt8		unk082;			// 082 - cleared when starting combat on player
	UInt8		unk083;			// 083 - loaded
	UInt8		unk084;			// 084 - loaded
	UInt8		unk085;			// 085 - loaded
	UInt8		unk086;			// 086 - loaded
	UInt8		unk087;			// 087	Init'd to the inverse of NoLowLevelProcessing
};
STATIC_ASSERT(offsetof(MobileObject, baseProcess) == 0x068);
STATIC_ASSERT(sizeof(MobileObject) == 0x088);

// 00C
class MagicCaster
{
public:
	MagicCaster();
	~MagicCaster();

	UInt32 vtabl;
	UInt32 unk004;	// 004
	UInt32 unk008;	// 008
};

STATIC_ASSERT(sizeof(MagicCaster) == 0x00C);

typedef tList<ActiveEffect> EffectNode;
// 010
class MagicTarget
{
public:
	MagicTarget();
	~MagicTarget();

	virtual void	Destructor(void);
	virtual TESObjectREFR *	GetParent(void);
	virtual EffectNode *	GetEffectList(void);

	UInt8			byt004;		// 004 
	UInt8			byt005;		// 005 
	UInt8			byt006[2];	// 006-7
	tList<void*>	lst008;		// 008

	void RemoveEffect(EffectItem *effItem);

	void StopEffect(void *arg0, bool arg1)
	{
		ThisStdCall(0x8248E0, this, arg0, arg1);
	}
};

STATIC_ASSERT(sizeof(MagicTarget) == 0x010);

class hkaRaycastInterface
{
public:
	hkaRaycastInterface();
	~hkaRaycastInterface();
	virtual hkaRaycastInterface*	Destroy(bool doFree);
	virtual void					Unk_01(void* arg0);
	virtual void					Unk_02(void);

	// Assumed to be 0x010 bytes due to context where the vftable is used
	UInt32	unk000[(0x010 - 0x004) >> 2];	// 0000
};
STATIC_ASSERT(sizeof(hkaRaycastInterface) == 0x010);

class bhkRagdollController : public hkaRaycastInterface
{
public:
	bhkRagdollController();
	~bhkRagdollController();

	UInt32	unk000[(0x021C - 0x010) >> 2];	// 0010
	UInt8	fill021C[3];					// 021C
	bool	bool021F;						// 021F	when null assume FIK status is false
	bool	fikStatus;						// 0220
	UInt8	fill0221[3];					// 0221
};
STATIC_ASSERT(sizeof(bhkRagdollController) == 0x0224);

class bhkRagdollPenetrationUtil;
class ActorMover;
class PlayerMover;
class ImageSpaceModifierInstanceDOF;
class ImageSpaceModifierInstanceDRB;

class ActorMover	// I need to call Func008
{
public:
	virtual void		Unk_00(void);
	virtual void		Unk_01(void);
	virtual void		ClearMovementFlag(void);
	virtual void		SetMovementFlag(void);
	virtual void		Unk_04(void);
	virtual void		Unk_05(void);
	virtual void		Unk_06(void);
	virtual void		Unk_07(void);
	virtual UInt32		GetMovementFlags();// for PlayerMover, it is GetMovementFlags
		// bit 11 = swimming 
		// bit 9 = sneaking
		// bit 8 = run
		// bit 7 = walk
		// bit 0 = keep moving (Q)
};

typedef std::vector<TESForm*> EquippedItemsList;

class Actor : public MobileObject
{
public:
	Actor();
	~Actor();

	virtual void		Unk_C1(void);
	virtual void		Unk_C2(void);
	virtual void		Unk_C3(void);
	virtual void		Unk_C4(void);
	virtual void		Unk_C5(void);
	virtual void		Unk_C6(void);
	virtual void		Unk_C7(void);
	virtual void		Unk_C8(void);
	virtual void		Unk_C9(void);
	virtual void		Unk_CA(void);
	virtual void		Unk_CB(void);
	virtual void		Unk_CC(void);
	virtual void		Unk_CD(void);
	virtual void		Unk_CE(void);
	virtual void		Unk_CF(void);
	virtual void		Unk_D0(void);
	virtual void		Unk_D1(void);
	virtual void		Unk_D2(void);
	virtual void		Unk_D3(void);
	virtual void		Unk_D4(void);
	virtual void		Unk_D5(void);
	virtual bool		Unk_D6(void);
	virtual bool		Unk_D7(void); // was void
	virtual void		Unk_D8(void);	// IsPlayerRef
	virtual void		Unk_D9(void);
	virtual void		Unk_DA(void);
	virtual void		Unk_DB(void);
	virtual void		Unk_DC(void);
	virtual void		Unk_DD(void);
	virtual void		Unk_DE(void);
	virtual void		Unk_DF(void);
	virtual void		Unk_E0(void);
	virtual void		Unk_E1(void);
	virtual void		Unk_E2(void);
	virtual void		Unk_E3(void);
	virtual void		Unk_E4(void);	// Creature = 0, Character = 1, PlayerCharacter = 2
	virtual void		Unk_E5(void);
	virtual void		Unk_E6(void);
	virtual void		Unk_E7(void);
	virtual void		Unk_E8(void);
	virtual void		Unk_E9(void);
	virtual void		Unk_EA(void);
	virtual void		DamageActorValue(UInt32 avCode, float damage, Actor* attacker);
	virtual void		Unk_EC(void);
	virtual void		Unk_ED(void);
	virtual void		Unk_EE(void);
	virtual void		Unk_EF(void);
	virtual void		Unk_F0(void);
	virtual void		Unk_F1(void);
	virtual void		Unk_F2(void);
	virtual void		Unk_F3(void);
	virtual void		Unk_F4(void);
	virtual void		Unk_F5(void);
	virtual void		Unk_F6(void);
	virtual void		Unk_F7(void);
	virtual void		Unk_F8(void);	// 0F8 bool AddSpell(spell)
	virtual void		Unk_F9(void);
	virtual void		Unk_FA(void);
	virtual void		Unk_FB(void);
	virtual void		Unk_FC(void);
	virtual void		Unk_FD(void);
	virtual void		Unk_FE(void);
	virtual void		Unk_FF(void);
	virtual void		Unk_100(void);
	virtual void		Unk_101(void);
	virtual void		Unk_102(void);
	virtual void		Unk_103(void);
	virtual void		Unk_104(void);
	virtual void		Unk_105(void);
	virtual void		Unk_106(void);
	virtual void		Unk_107(void);
	virtual void		Unk_108(void);
	virtual void		Unk_109(void);
	virtual bool		IsInCombat(void);
	virtual Actor		*GetCombatTarget(void);
	virtual void		Unk_10C(void);
	virtual void		Unk_10D(void);
	virtual void		Unk_10E(void);
	virtual void		Unk_10F(void);
	virtual void		Unk_110(void);
	virtual void		Unk_111(void);
	virtual void		Unk_112(void);
	virtual void		Unk_113(void);
	virtual void		Unk_114(void);
	virtual void		Unk_115(void);
	virtual void		Unk_116(void);
	virtual void		Unk_117(void);
	virtual void		Unk_118(void);
	virtual void		Unk_119(void);
	virtual void		Unk_11A(void);
	virtual void		Unk_11B(void);
	virtual void		Unk_11C(void);
	virtual void		Unk_11D(void);
	virtual void		Unk_11E(void);
	virtual void		Unk_11F(void);
	virtual void		Unk_120(void);
	virtual void		Unk_121(void);
	virtual void		Unk_122(void);	//	0122	Check for LevelUp
	virtual void		Unk_123(void);
	virtual void		Unk_124(void);
	virtual void		Unk_125(void);
	virtual void		SetPerkRank(BGSPerk* perk, UInt8 rank, bool alt);
	virtual void		Unk_127(void);
	virtual UInt8		GetPerkRank(BGSPerk* perk, bool alt);
	virtual void		Unk_129(void);
	virtual void		Unk_12A(void);
	virtual void		Unk_12B(void);
	virtual void		Unk_12C(void);
	virtual void		Unk_12D(void);
	virtual void		Unk_12E(void);
	virtual void		Unk_12F(void);
	virtual void		Unk_130(void);
	virtual void		Unk_131(void);
	virtual void		Unk_132(void);
	virtual void		Unk_133(void);
	virtual void		Unk_134(void);
	virtual void		Unk_135(void);
	virtual void		Unk_136(void);
	
	MagicCaster			magicCaster;			// 088
	MagicTarget			magicTarget;			// 094
	ActorValueOwner		avOwner;				// 0A4
	CachedValuesOwner	cvOwner;				// 0A8

	bhkRagdollController* ragDollController;			// 0AC FROM JIP
	bhkRagdollPenetrationUtil* ragDollPentrationUtil;		// 0B0
	UInt32								unk0B4;						// 0B4-
	float								flt0B8;						// 0B8
	UInt8								byte0BC;					// 0BC-
	UInt8								byte0BD;					// 0BD
	UInt8								byte0BE;					// 0BE
	UInt8								byte0BF;					// 0BF
	Actor* killer;					// 0C0
	UInt8								byte0C4;					// 0C4-
	UInt8								byte0C5;					// 0C5
	UInt8								byte0C6;					// 0C6
	UInt8								byte0C7;					// 0C7
	float								flt0C8;						// 0C8
	float								flt0CC;						// 0CC
	tList<void>							list0D0;					// 0D0
	UInt8								byte0D8;					// 0D8
	UInt8								byte0D9;					// 0D9
	UInt8								byte0DA;					// 0DA
	UInt8								byte0DB;					// 0DB
	UInt32								unk0DC;						// 0DC
	tList<void>							list0E0;					// 0E0
	UInt8								byte0E8;					// 0E8	const 1
	UInt8								byte0E9;					// 0E9
	UInt8								byte0EA;					// 0EA
	UInt8								byte0EB;					// 0EB
	UInt32								unk0EC;						// 0EC
	UInt8								byte0F0;					// 0F0-
	UInt8								byte0F1;					// 0F1-
	UInt8								byte0F2;					// 0F2
	UInt8								byte0F3;					// 0F3
	tList<void>							list0F4;					// 0F4
	tList<void>							list0FC;					// 0FC
	bool								isInCombat;					// 104
	UInt8								jipActorFlags1;				// 105
	UInt8								jipActorFlags2;				// 106
	UInt8								jipActorFlags3;				// 107
	UInt32								lifeState;					// 108	saved as byte HasHealth
	UInt32								criticalStage;				// 10C
	UInt32								unk110;						// 110-
	float								flt114;						// 114
	UInt8								byte118;					// 118-
	UInt8								byte119;					// 119+
	UInt8								byte11A;					// 11A
	UInt8								byte11B;					// 11B
	UInt32								unk11C;						// 11C-
	UInt32								resetTime;					// 120-
	bool								forceRun;					// 124
	bool								forceSneak;					// 125
	UInt8								byte126;					// 126-
	bool								searchingForEnemies;		// 127
	Actor* combatTarget;				// 128
	BSSimpleArray<Actor*>* combatTargets;				// 12C
	BSSimpleArray<Actor*>* combatAllies;				// 130
	UInt8								byte134;					// 134-
	UInt8								byte135;					// 135+
	UInt8								byte136;					// 136
	UInt8								byte137;					// 137
	UInt32								unk138;						// 138-
	UInt32								unk13C;						// 13C-
	UInt32								actorFlags;					// 140	0x80000000 - IsEssential
	bool								ignoreCrime;				// 144
	UInt8								byte145;					// 145	Has to do with package evaluation
	UInt8								byte146;					// 146	Has to do with package evaluation
	UInt8								byte147;					// 147
	UInt32								unk148;						// 148-
	UInt8								inWater;					// 14C
	UInt8								isSwimming;					// 14D
	UInt8								byte14E;					// 14E
	UInt8								byte14F;					// 14F
	UInt32								unk150;						// 150-
	float								flt154;						// 154
	float								flt158;						// 158
	UInt8								byte15C;					// 15C-
	UInt8								byte15D;					// 15D-
	UInt8								byte15E;					// 15E
	UInt8								byte15F;					// 15F
	NiVector3							startingPos;				// 160
	float								flt16C;						// 16C
	TESForm* startingWorldOrCell;		// 170
	UInt8								byte174;					// 174-
	UInt8								byte175;					// 175-
	UInt8								byte176;					// 176
	UInt8								byte177;					// 177
	float								flt178;						// 178
	float								flt17C;						// 17C
	float								flt180;						// 180
	float								flt184;						// 184
	float								flt188;						// 188
	UInt8								byte18C;					// 18C-
	bool								isTeammate;					// 18D
	UInt8								byte18E;					// 18E-
	UInt8								byte18F;					// 18F
	ActorMover* actorMover;				// 190
	UInt32								unk194;						// 194-
	UInt32								unk198;						// 198-
	float								flt19C;						// 19C
	UInt32								unk1A0;						// 1A0-
	UInt32								unk1A4;						// 1A4-
	UInt32								unk1A8;						// 1A8-
	UInt32								sitSleepState;				// 1AC-
	UInt8								isTurret;					// 1B0-
	bool								forceHit;					// 1B1-
	UInt8								byte1B2;					// 1B2
	UInt8								byte1B3;					// 1B3

	// OBSE: unk1 looks like quantity, usu. 1; ignored for ammo (equips entire stack). In NVSE, pretty much always forced internally to 1 
	// OBSE: itemExtraList is NULL as the container changes entry is not resolved before the call
	// NVSE: Default values are those used by the vanilla script functions.
	void EquipItem(TESForm * objType, UInt32 equipCount = 1, ExtraDataList* itemExtraList = NULL, UInt32 unk3 = 1, bool lockEquip = false, UInt32 unk5 = 1);	// unk3 apply enchantment on player differently
	void UnequipItem(TESForm* objType, UInt32 unequipCount = 1, ExtraDataList* itemExtraList = NULL, UInt32 unk3 = 1, bool lockEquip = false, UInt32 unk5 = 1);

	EquippedItemsList	GetEquippedItems();
	ExtraContainerDataArray	GetEquippedEntryDataList();
	ExtraContainerExtendDataArray GetEquippedExtendDataList();

	bool GetDead() { return (lifeState == 1) || (lifeState == 2); }
	bool GetRestrained() { return lifeState == 5; }
	TESActorBase* GetActorBase();
	bool GetLOS(Actor* target);
	char GetCurrentAIPackage();
	char GetCurrentAIProcedure();
	bool IsFleeing();
	ExtraContainerChanges::EntryData* GetWeaponInfo(); // From JIP
	ExtraContainerChanges::EntryData* GetAmmoInfo();
	TESObjectWEAP* GetEquippedWeapon();
	bool IsItemEquipped(TESForm* item);


	class MiddleHighProcess : public MiddleLowProcess
	{
	public:
		virtual void	SetAnimation(UInt32 newAnimation);
		virtual void	Unk_209();
		virtual void	Unk_20A();
		virtual void	Unk_20B();
		virtual void	Unk_20C();
		virtual void	Unk_20D();
		virtual void	Unk_20E();
		virtual void	Unk_20F();
		virtual void	Unk_210();
		virtual void	Unk_211();
		virtual void	Unk_212();
		virtual void	Unk_213();
		virtual void	Unk_214();
		virtual void	Unk_215();
		virtual void	Unk_216();
		virtual void	Unk_217();
		virtual void	Unk_218();
		virtual void	Unk_219();
		virtual void	Unk_21A();
		virtual void	Unk_21B();

		enum KnockedState
		{
			kState_None,
			kState_KnockedDown,
			kState_Ragdolled,
			kState_Unconscious,
			kState_Unknown4,
			kState_Unknown5,
			kState_GettingUp
		};

		tList<TESForm>						unk0C8;				// 0C8
		tList<UInt32>						unk0D0;				// 0D0
		UInt32								unk0D8[3];			// 0D8
		UInt8								unk0FC[12];			// 0FC	Saved as one, might be Pos/Rot given size
		UInt32								unk108;				// 108
		TESIdleForm* LastPlayedIdle;	// 10C
		UInt32								unk110;				// 110  EntryData, also handled as part of weapon code. AmmoInfo.
		ExtraContainerChanges::EntryData* weaponInfo;		// 114
		ExtraContainerChanges::EntryData* ammoInfo;			// 118
		QueuedFile* unk11C;			// 11C
		UInt8								byt120;				// 120
		UInt8								byt121;				// 121
		UInt8								byt122;				// 122
		UInt8								fil123;				// 123
		UInt8								usingOneHandGrenade;// 124
		UInt8								usingOneHandMine;	// 125
		UInt8								usingOneHandThrown;	// 126
		UInt8								byte127;			// 127
		UInt32								unk128;				// 128 Gets copied over during TESNPC.CopyFromBase
		NiNode* weaponNode;		// 12C
		NiNode* projectileNode;	// 130
		UInt8								byt134;				// 134
		bool								isWeaponOut;		// 135
		UInt8								byt136;				// 136
		UInt8								byt137;				// 137
		bhkCharacterController* charCtrl;			// 138
		UInt8								knockedState;		// 13C
		UInt8								sitSleepState;		// 13D
		UInt8								unk13E[2];			// 13E
		TESObjectREFR* usedFurniture;		// 140
		UInt8								byte144;			// 144
		UInt8								unk145[3];			// 145
		UInt32								unk148[6];			// 148
		MagicItem* magicItem160;		// 160
		UInt32								unk164[3];			// 164
		float								actorAlpha;			// 170
		UInt32								unk174;				// 174
		BSFaceGenAnimationData* unk178;			// 178
		UInt8								byte17C;			// 17C
		UInt8								byte17D;			// 17D
		UInt8								byte17E;			// 17E
		UInt8								byte17F;			// 17F
		UInt32								unk180;				// 180
		UInt32								unk184;				// 184
		UInt8								hasCaughtPCPickpocketting;	// 188
		UInt8								byte189;			// 189
		UInt8								byte18A;			// 18A
		UInt8								byte18B;			// 18B
		UInt8								byte18C;			// 18C
		UInt8								byte18D[3];			// 18D
		UInt32								unk190[10];			// 190
		void* unk1B8;			// 1B8
		MagicTarget* magicTarget1BC;	// 1BC
		AnimData* animData;			// 1C0
		BSAnimGroupSequence* animSequence[3];	// 1C4
		UInt32								unk1D0[3];			// 1D0
		NiNode* limbNodes[15];		// 1DC
		NiNode* unk218;			// 218
		NiNode* unk21C;			// 21C
		void* ptr220;			// 220
		BSBound* boundingBox;		// 224
		bool								isAiming;			// 228
		UInt8								pad229[3];			// 229
		UInt32								unk22C[2];			// 22C
		float								radsSec234;			// 234
		float								rads238;			// 238
		float								waterRadsSec;		// 23C

		UInt32								unk244;				// 244
		BSFaceGenNiNode* unk248;			// 248
		BSFaceGenNiNode* unk24C;			// 24C
		NiTriShape* unk250;			// 250
		UInt32								unk258;				// 258
	};










};

STATIC_ASSERT(offsetof(Actor, magicCaster) == 0x088);

class Character : public Actor
{
public:
	virtual void	Unk_137(void);
	virtual void	Unk_138(void);

	ValidBip01Names* validBip01Names;	// 1B4
	float			totalArmorDR;		// 1B8
	float			totalArmorDT;		// 1BC
	UInt8			isTrespassing;		// 1C0
	UInt8			byt1C1;				// 1C1
	UInt16			unk1C2;				// 1C2
	float			unk1C4;				// 1C4
};

struct CombatActors;

typedef tList<BGSQuestObjective::Target> QuestObjectiveTargets;

class ExtraAudioMarker : public BSExtraData
{
public:
	// 34
	struct Data
	{
		TESFullName		fullName;
		float			layer2TriggerPerc;
		float			layer3TriggerPerc;
		UInt32			unk14[4];
		UInt32			mediaLocCtrlID;
		UInt32			flags;
		UInt32			unk2C[2];
	};

	Data* data;		// 0C
};

struct MusicMarker
{
	TESObjectREFR* markerRef;
	ExtraAudioMarker::Data* markerData;

	MusicMarker(TESObjectREFR* _markerRef, ExtraAudioMarker::Data* data) : markerRef(_markerRef), markerData(data) {}

	bool operator<(const MusicMarker& rhs) const { return markerData->mediaLocCtrlID < rhs.markerData->mediaLocCtrlID; }
};

struct PCLevelData
{
	UInt8		byte00;
	UInt8		pad01[3];
	UInt32		xpToNextLevel;
	UInt8		byte08;
	UInt8		pad09[3];
};

struct PerkRank
{
	BGSPerk* perk;
	UInt8		rank;
	UInt8		pad05[3];
};


class TESCaravanCard : public TESBoundObject
{
public:
};

struct CasinoStats
{
	UInt32		casinoRefID;
	UInt32		earnings;
	UInt16		unk08;
	UInt8		gap0A[2];
};

// 9BC
struct ParentSpaceNode;
struct TeleportLink;
struct ItemChange;
struct MusicMarker;

class bhkRigidBody //: public bhkEntity
{
public:
	virtual void	Unk_35(void);
	virtual void	Unk_36(void);
	virtual void	Unk_37(void);
	virtual void	Unk_38(void);
	virtual void	Unk_39(void);
	virtual void	Unk_3A(void);
	virtual void	Unk_3B(void);
	virtual void	Unk_3C(void);
	virtual void	Unk_3D(void);
	virtual void	Unk_3E(void);
	virtual void	Unk_3F(void);

	UInt32			unk14[2];		// 14
};











class PlayerCharacter : public Character /////////////////////////FROM JIP
{
public:
	// used to flag controls as disabled in disabledControlFlags
	enum
	{
		kControlFlag_Movement = 1 << 0,
		kControlFlag_Look = 1 << 1,
		kControlFlag_Pipboy = 1 << 2,
		kControlFlag_Fight = 1 << 3,
		kControlFlag_POVSwitch = 1 << 4,
		kControlFlag_RolloverText = 1 << 5,
		kControlFlag_Sneak = 1 << 6,
	};

	virtual void		Unk_139(void);
	virtual void		Unk_13A(void);

	struct MapMarkerInfo
	{
		ExtraMapMarker::MarkerData* markerData;
		TESObjectREFR* markerRef;
	};

	struct CompassTarget
	{
		Actor* target;
		UInt8		isHostile;
		UInt8		isDetected;
		UInt8		pad06[2];
	};

	struct PCLevelData
	{
		UInt8		byte00;
		UInt8		pad01[3];
		UInt32		xpToNextLevel;
		UInt8		byte08;
		UInt8		pad09[3];
	};

	UInt32								unk1C8[17];				// 1C8	208 could be a DialogPackage
	void								*unk20C;				// 20C
	tList<ActiveEffect>					*activeEffects;			// 210
	UInt32								unk214[2];				// 214
	void								*unk21C;				// 21C
	UInt32								unk220[8];				// 220	224 is a package of type 1C
	bool								showQuestItems;			// 240
	UInt8								byte241;				// 241
	UInt8								byte242;				// 242
	UInt8								byte243;				// 243
	float								unk244[77];				// 244	have to be a set of ActorValue
	float								permAVMods[77];			// 378	have to be a set of ActorValue
	float								flt4AC;					// 4AC
	float								actorValues4B0[77];		// 4B0	have to be a set of ActorValue
	tList<BGSNote>						notes;					// 5E4
	ImageSpaceModifierInstanceDOF		*unk5EC;				// 5EC
	ImageSpaceModifierInstanceDOF		*unk5F0;				// 5F0
	ImageSpaceModifierInstanceDRB		*unk5F4;				// 5F4
	UInt8								byte5F8;				// 5F8
	UInt8								byte5F9;				// 5F9
	UInt8								byte5FA;				// 5FA
	UInt8								byte5FB;				// 5FB
	tList<TESObjectREFR>				teammates;				// 5FC
	TESObjectREFR						*lastExteriorDoor;		// 604
	UInt8								isPlayingAttackSound;	// 608
	UInt8								pad609[3];				// 609
	tList<void>							*actionList;			// 60C
	tList<CasinoStats>					*casinoDataList;		// 610
	tList<TESCaravanCard>				*caravanCards1;			// 614
	tList<TESCaravanCard>				*caravanCards2;			// 618
	UInt32								unk61C[6];				// 61C
	void								*ptr634;				// 634	bhkMouseSpringAction when there's a grabbed reference
	TESObjectREFR						*grabbedRef;			// 638
	UInt32								unk63C;					// 63C
	UInt32								unk640;					// 640
	float								grabbedWeight;			// 644
	UInt8								byte648;				// 648
	UInt8								byte649;				// 649
	bool								byte64A;				// 64A	= not FirstPerson
	bool								is3rdPerson;			// 64B
	bool								bThirdPerson;			// 64C
	UInt8								byte64D;				// 64D
	UInt8								byte64E;				// 64E
	bool								isUsingScope;			// 64F
	UInt8								byte650;				// 650
	bool								alwaysRun;				// 651
	bool								autoMove;				// 652
	UInt8								byte653;				// 653
	UInt32								sleepHours;				// 654
	UInt8								isResting;				// 658	Something to do with SleepDeprivation; see 0x969DCF
	UInt8								byte659;				// 659
	UInt8								byte65A;				// 65A
	UInt8								byte65B;				// 65B
	UInt32								unk65C[4];				// 65C
	UInt8								byte66C;				// 66C
	UInt8								byte66D;				// 66D
	UInt8								byte66E;				// 66E
	UInt8								byte66F;				// 66F
	float								worldFOV;				// 670
	float								firstPersonFOV;			// 674
	float								flt678;					// 678
	UInt32								unk67C;					// 67C
	UInt8								pcControlFlags;			// 680
	UInt8								byte681;				// 681
	UInt8								byte682;				// 682
	UInt8								byte683;				// 683
	UInt32								unk684[2];				// 684
	ValidBip01Names						*VB01N1stPerson;		// 68C
	AnimData							*animData1stPerson;		// 690
	NiNode								*node1stPerson;			// 694
	float								flt698;					// 698
	UInt32								unk69C[3];				// 69C
	tList<TESTopic>						topicList;				// 6A8
	UInt32								unk6B0[2];				// 6B0
	TESQuest							*activeQuest;			// 6B8
	tList<BGSQuestObjective>			questObjectiveList;		// 6BC
	tList<BGSQuestObjective::Target>	questTargetList;		// 6C4
	UInt32								unk6CC[5];				// 6CC
	float								sortActorDistanceListTimer;	// 6E0
	float								seatedRotation;			// 6E4
	UInt32								unk6E8[2];				// 6E8
	UInt32								playerSpell;			// 6F0
	TESObjectREFR						*placedMarker;			// 6F4
	BSSimpleArray<ParentSpaceNode>		parentSpaceNodes;		// 6F8
	BSSimpleArray<TeleportLink>			teleportLinks;			// 708
	UInt32								unk718[6];				// 718
	float 								timeGrenadeHeld;		// 730
	UInt32								unk734[10];				// 734
	bool								inCharGen;				// 75C
	UInt8								byte75D;				// 75D
	UInt8								byte75E;				// 75E
	UInt8								byte75F;				// 75F
	TESRegion							*currentRegion;			// 760
	TESRegionList						regionsList;			// 764///////////////////////
	UInt32								unk774[14];				// 774
	TESForm								*pcWorldOrCell;			// 7AC
	UInt32								unk7B0;					// 7B0 
	BGSMusicType						*musicType;				// 7B4
	UInt8								gameDifficulty;			// 7B8
	UInt8								byte7B9;				// 7B9
	UInt8								byte7BA;				// 7BA
	UInt8								byte7BB;				// 7BB
	bool								isHardcore;				// 7BC
	UInt8								byte7BD;				// 7BD
	UInt8								byte7BE;				// 7BE
	UInt8								byte7BF;				// 7BF
	UInt32								killCamMode;			// 7C0
	UInt8								byte7C4;				// 7C4
	UInt8								byte7C5;				// 7C5
	bool								isToddler;				// 7C6
	bool								canUsePA;				// 7C7
	tList<MapMarkerInfo>				mapMarkers;				// 7C8
	TESWorldSpace						*worldSpc7D0;			// 7D0
	tList<MusicMarker>					musicMarkers;			// 7D4
	MusicMarker							*currMusicMarker;		// 7DC
	float								flycamZRot;				// 7E0
	float								flycamXRot;				// 7E4
	float								flycamPosX;				// 7E8
	float								flycamPosY;				// 7EC
	float								flycamPosZ;				// 7F0
	UInt32								unk7F4[33];				// 7F4
	PCLevelData							*pcLevelData;			// 878
	tList<PerkRank>						perkRanksPC;			// 87C
	tList<BGSEntryPointPerkEntry>		perkEntriesPC[74];		// 884
	tList<PerkRank>						perkRanksTM;			// AD4
	tList<BGSEntryPointPerkEntry>		perkEntriesTM[74];		// ADC
	UInt32								unkD2C[4];				// D2C
	NiObject							*unkD3C;				// D3C
	UInt32								unkD40;					// D40
	Actor								*reticleActor;			// D44
	tList<CompassTarget>				*compassTargets;		// D48
	UInt32								unkD4C;					// D4C
	float								lastAmmoSwapTime;		// D50
	UInt32								unkD54;					// D54
	NiVector3							vectorD58;				// D58
	CombatActors						*combatActors;			// D64
	UInt32								teammateCount;			// D68
	UInt32								unkD6C[5];				// D6C
	NiNode								*niNodeD80;				// D80
	UInt32								unkD84[12];				// D84
	NiNode								*niNodeDB4;				// DB4
	UInt32								unkDB8[7];				// DB8
	NiVector3							vectorDD4;				// DD4
	NiVector3							cameraPos;				// DE0
	bhkRigidBody						*rigidBody;				// DEC
	bool								pcInCombat;				// DF0
	bool								pcUnseen;				// DF1
	UInt8								byteDF2;				// DF2
	UInt8								byteDF3;				// DF3
	BSSimpleArray<ItemChange>			itemChanges;			// DF4
	UInt32								unkE04;					// E04
	UInt8								byteE08;				// E08
	UInt8								padE09[3];				// E09
	UInt32								unkE0C[3];				// E0C
	float								killCamTimer;			// E18
	float								killCamCooldown;		// E1C
	UInt8								byteE20;				// E20
	UInt8								byteE21;				// E21
	UInt8								byteE22;				// E22
	UInt8								byteE23;				// E23
	float								fltE24;					// E24
	float								counterAttackTimer;		// E28
	UInt32								unkE2C;					// E2C
	UInt32								itemDetectionTimer;		// E30
	NiNode								*ironSightNode;			// E34
	bool								noHardcoreTracking;		// E38	Appears to be unused
	bool								skipHCNeedsUpdate;		// E39
	UInt8								byteE3A;				// E3A
	UInt8								byteE3B;				// E3B
	BSSimpleArray<TESAmmo*>				hotkeyedWeaponAmmos;	// E3C
	UInt32								unkE4C;					// E4C


	bool IsThirdPerson() { return bThirdPerson ? true : false; }
	UInt32 GetMovementFlags() { return actorMover->GetMovementFlags(); }	// 11: IsSwimming, 9: IsSneaking, 8: IsRunning, 7: IsWalking, 0: keep moving
	bool IsPlayerSwimming() { return (actorMover->GetMovementFlags() & 0x800) ? true : false; }

	static PlayerCharacter* GetSingleton();
	bool SetSkeletonPath(const char* newPath);
	static void UpdateHead(void);

	__forceinline bool ToggleFirstPerson(bool toggleON)
	{
		return ThisCallAlt<bool>(0x950110, this, toggleON);
	}
	char GetDetectionState();
	QuestObjectiveTargets* GetCurrentQuestObjectiveTargets();
};
STATIC_ASSERT(sizeof(PlayerCharacter) == 0xE50);

//STATIC_ASSERT(offsetof(PlayerCharacter, regionsList) == 0x764);
//STATIC_ASSERT(offsetof(PlayerCharacter, unk774[14]) == 0x774);
//STATIC_ASSERT(offsetof(PlayerCharacter, byteE3B) == 0xe3B);




extern PlayerCharacter** g_thePlayer;

STATIC_ASSERT(offsetof(PlayerCharacter, ragDollController) == 0x0AC);
STATIC_ASSERT(offsetof(PlayerCharacter, questObjectiveList) == 0x6BC);
STATIC_ASSERT(offsetof(PlayerCharacter, bThirdPerson) == 0x64C);
STATIC_ASSERT(offsetof(PlayerCharacter, actorMover) == 0x190);



