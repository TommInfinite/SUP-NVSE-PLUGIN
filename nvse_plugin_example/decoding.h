#pragma once


// Contents of this header file are copied from decoding.h from JiP NVSE Plugin.(https://github.com/jazzisparis/JIP-LN-NVSE)


class ImageSpaceModifierInstanceRB;
class ImageSpaceModifierInstance : public NiObject
{
public:
	virtual void	Unk_23(void);
	virtual void	Unk_24(void);
	virtual void	Unk_25(void);
	virtual void	Unk_26(UInt32 arg);

	UInt8					hidden;			// 08
	UInt8					pad09[3];		// 09
	float					percent;		// 0C
	NiObject* obj10;			// 10
	float					flt14;			// 14
	UInt32					unk18;			// 18
};


class ImageSpaceModifierInstanceForm : public ImageSpaceModifierInstance
{
public:
	TESImageSpaceModifier* imageSpace;	// 1C
	void* ptr20;			// 20
	float					flt24;			// 24
	NiObject* obj28;			// 28
	float					flt2C;			// 2C
};

struct VATSCameraData //FROM JIP 
{
	tList<void>						targetsList;	// 00
	UInt32							mode;			// 08
	UInt32							unk0C;			// 0C
	BGSCameraShot* camShot;		// 10
	float							flt14;			// 14
	float							flt18;			// 18
	UInt32							unk1C;			// 1C
	UInt32							unk20;			// 20
	UInt32							unk24;			// 24
	ImageSpaceModifierInstanceForm* isModInstForm;	// 28
	ImageSpaceModifierInstanceRB* isModInstRB;	// 2C
	UInt32							unk30;			// 30
	NiObject* object34;		// 34
	UInt8							byte38;			// 38
	UInt8							pad39[3];		// 39
	UInt32							unk3C;			// 3C
	UInt32							unk40;			// 40
	UInt32							unk44;			// 44
};
STATIC_ASSERT(sizeof(VATSCameraData) == 0x48);

struct HotKeyWheel
{
	TileRect* parent;		// 00
	TileRect* hotkeys[8];	// 04
	UInt8		byte24;			// 24
	UInt8		pad25[3];		// 25
	UInt32		unk28;			// 28
	UInt32		selectedHotkeyTrait;// 2C
	UInt32		selectedTextTrait;	// 30
};
STATIC_ASSERT(sizeof(HotKeyWheel) == 0x34);



// 278
class HUDMainMenu : public Menu			// 1004
{

public:

	struct QueuedMessage
	{
		char	msgText[0x204];			// 000
		char	iconPate[MAX_PATH];		// 204
		char	soundPath[MAX_PATH];	// 308
		float	displayTime;			// 40C

	};

	struct SubtitleData;

	struct Struct224
	{
		UInt8		byte00;		// 00
		UInt8		pad01[3];	// 01
		float		flt04;		// 04
		float		flt08;		// 08
		float		flt0C;		// 0C
		float		flt10;		// 10
		UInt32		unk14;		// 14
		UInt32		tickCount;	// 18
		UInt8		byte1C;		// 1C
		UInt8		byte1D;		// 1D
		UInt8		pad1E[2];	// 1E
	};

	struct QueuedQuestText
	{
		char		msg[0x104];
		bool		byte104;
		bool		byte105;
	};

	enum VisibilityFlags
	{
		kActionPoints = 0x1,//0
		kHitPoints = 0x2,//1
		kRadiationMeter = 0x4,//2
		kEnemyHealth = 0x8,//3
		kQuestReminder = 0x10,//4
		kRegionLocation = 0x20,//5
		kReticleCenter = 0x40,//6
		kSneakMeter = 0x80, //7
		kMessages = 0x100, //8
		kInfo = 0x200, //9
		kSubtibles = 0x400,//10
		kHotkeys = 0x800,//11
		kXpMeter = 0x1000,//12
		kBreathMeter = 0x2000,//13
		kExplosivePositioning = 0x4000,//14
		kCrippledLimbIndicator = 0x8000,//15
		kHardcoreMode = 0x10000,//16
	};

	enum HUDStates
	{
		kRECALCULATE = 0x1,
		kNormal,
		kPipBoy,
		kPause,
		kLoading,
		kDialog,
		kVATSMenu,
		kVATSPlayback,
		kContainer,
		kBeginSit,
		kSleepWait,
		kPlayerDisabledControls,
		kUnusedD,
		kLevelUpMenu,
		kHacking,
		kComputers,
		kMessage,
		kSpecialBook,
		kLoveTester,
		kVanityCam,
		kUnused15,
		kAiming,
		kAimingScope,
		kIntroMovie,
		kGambling,
	};

	UInt32							unk028;			// 028
	union
	{
		Tile* tiles[0x56];
		struct
		{
			TileImage* tile02C;		// 02C	HitPoints\meter
			TileText* tile030;		// 030	HitPoints\justify_right_text
			TileRect* tile034;		// 034	HitPoints\compass_window\compass_icon_group
			TileRect* tile038;		// 038	HitPoints\compass_window\compass_icon_group
			TileRect* tile03C;		// 03C	HitPoints\compass_window\compass_icon_group
			TileImage* tile040;		// 040	HitPoints\compass_window
			TileImage* tile044;		// 044	ActionPoints\meter
			TileText* tile048;		// 048	ActionPoints\justify_right_text
			TileText* tile04C;		// 04C	ActionPoints\justify_right_text
			TileImage* tile050;		// 050	ActionPoints\meter
			TileImage* tile054;		// 054	ActionPoints\MeterBackground
			TileText* tile058;		// 058	ActionPoints\justify_right_text
			TileRect* tile05C;		// 05C	QuestReminder\QuestStages
			TileRect* tile060;		// 060	QuestReminder\QuestAdded
			TileText* tile064;		// 064	Region_Location\justify_left_text
			TileText* tile068;		// 068	Region_Location\justify_left_text
			TileImage* tile06C;		// 06C	RadiationMeter\radiation_bracket
			TileImage* tile070;		// 070	RadiationMeter\radiation_pointer
			TileText* tile074;		// 074	RadiationMeter\radiation_text_value
			TileText* tile078;		// 078	RadiationMeter\radiation_text
			TileImage* tile07C;		// 07C	EnemyHealth\enemy_health_bracket
			TileImage* tile080;		// 080	EnemyHealth\meter
			TileText* tile084;		// 084	EnemyHealth\justify_center_text
			TileText* sneakLabel;	// 088	SneakMeter\sneak_nif
			TileImage* tile08C;		// 08C	Messages\message_icon
			TileText* tile090;		// 090	Messages\justify_left_text
			TileImage* tile094;		// 094	Messages\message_bracket
			TileText* tile098;		// 098	Subtitles\justify_center_text
			TileRect* tileTakePrompt;		// 09C	Info\justify_center_hotrect
			TileText* tile0A0;		// 0A0	Info\justify_center_hotrect\PCShortcutLabel
			TileImage* tile0A4;		// 0A4	Info\justify_center_hotrect\xbox_button
			TileText* tile0A8;		// 0A8	Info\justify_center_text
			TileText* tileLockLevelStr;	// 0AC	Info\justify_center_text
			TileText* tile0B0;		// 0B0	Info\justify_center_text
			TileText* tilePickupWeightStr;	// 0B4	Info\justify_right_text
			TileText* tilePickupWG;			// 0B8	Info\justify_left_text
			TileText* tilePickupValueStr;	// 0BC	Info\justify_right_text
			TileText* tilePickupVAL;		// 0C0	Info\justify_left_text
			TileImage* tile0C4;		// 0C4	Info\info_seperator
			TileRect* tile0C8;		// 0C8	Hokeys\hotkey_selector
			TileText* tile0CC;		// 0CC	Hokeys\justify_center_text
			TileImage* tile0D0;		// 0D0	HitPoints\left_bracket
			TileImage* tile0D4;		// 0D4	ActionPoints\right_bracket
			TileImage* tile0D8;		// 0D8	XPMeter\XPBracket
			TileText* tile0DC;		// 0DC	XPMeter\XPAmount
			TileText* tile0E0;		// 0E0	XPMeter\XPLabel
			TileImage* tile0E4;		// 0E4	XPMeter\XPPointer
			TileText* tile0E8;		// 0E8	XPMeter\XPLastLevel
			TileText* tile0EC;		// 0EC	XPMeter\XPNextLevel
			TileText* tile0F0;		// 0F0	XPMeter\XPLevelUp
			TileImage* tile0F4;		// 0F4	ReticleCenter\reticle_center
			TileImage* tile0F8;		// 0F8	crippled_limb_indicator\Face
			TileImage* tile0FC;		// 0FC	crippled_limb_indicator\Head
			TileImage* tile100;		// 100	crippled_limb_indicator\Torso
			TileImage* tile104;		// 104	crippled_limb_indicator\Left_Arm
			TileImage* tile108;		// 108	crippled_limb_indicator\Right_Arm
			TileImage* tile10C;		// 10C	crippled_limb_indicator\Left_Leg
			TileImage* tile110;		// 110	crippled_limb_indicator\Right_Leg
			TileRect* tile114;		// 114	ActionPoints
			TileRect* tile118;		// 118	HitPoints
			TileRect* tile11C;		// 11C	RadiationMeter
			TileRect* tile120;		// 120	EnemyHealth
			TileRect* tile124;		// 124	QuestReminder
			TileRect* tile128;		// 128	Region_Location
			TileRect* tile12C;		// 12C	ReticleCenter
			TileRect* tile130;		// 130	SneakMeter
			TileRect* tile134;		// 134	Messages
			TileRect* tile138;		// 138	Info
			TileRect* tile13C;		// 13C	Subtitles
			TileRect* tile140;		// 140	Hokeys
			TileRect* tile144;		// 144	XPMeter
			int* tile148;			// 148	BreathMeter
			TileRect* tile14C;		// 14C	Explosive_positioning_rect
			TileRect* crippledLimbIndicator;		// 150	crippled_limb_indicator
			TileImage* tile154;		// 154	DDTIcon
			TileImage* tile158;		// 158	DDTIconEnemy
			TileText* tile15C;		// 15C	AmmoTypeLabel
			TileRect* tile160;		// 160	HardcoreMode
			TileText* tile164;		// 164	HardcoreMode\Dehydration
			TileText* tile168;		// 168	HardcoreMode\Sleep
			TileText* tile16C;		// 16C	HardcoreMode\Hunger
			TileImage* tile170;		// 170	DDTIcon
			TileImage* tile174;		// 174	DDTIconEnemyAP
			TileText* tile178;		// 178	HardcoreMode\Rads
			TileText* tile17C;		// 17C	HardcoreMode\LMBs
			TileImage* tile180;		// 180	CNDArrows

		};
	};
	UInt32							unk184;				// 184
	float							flt188;				// 188
	tList<QueuedMessage>			queuedMessages;		// 18C
	UInt32							currMsgKey;			// 194
	BSSimpleArray<SubtitleData>		subtitlesArr;		// 198
	UInt32							unk1A8[4];			// 1A8
	TESObjectREFR* crosshairRef;		// 1B8
	UInt32							unk1BC;				// 1BC
	UInt32							visibilityOverrides;	// 1C0
	UInt32							stage;				// 1C4
	HotKeyWheel						hotKeyWheel;		// 1C8
	UInt8							isUsingScope;		// 1FC
	UInt8							byte1FD[3];			// 1FD
	NiControllerSequence* niContSeq;			// 200
	UInt8							isLevelUpQueued;	// 204
	UInt8							autoDisplayObjectives;	// 205
	UInt8							pad206[2];			// 206
	UInt32							unk208;				// 208
	UInt32							questTargets;		// 20C
	UInt32							unk210;				// 210
	UInt32							compassWidth;		// 214
	UInt32							maxCompassAngle;	// 218
	Actor* healthTarget;		// 21C
	UInt32							unk220;				// 220
	Struct224						unk224;				// 224
	UInt32							unk244;				// 244
	UInt32							unk248[4];			// 248
	tList<QueuedQuestText>			queuedQuestTextList;// 258
	UInt8							byte260;			// 260
	UInt8							byte261;			// 261
	UInt8							pad262[2];			// 262
	tList<UInt32>					xpMessages;			// 264
	tList<UInt32>					list26C;			// 26C
	float							hudShake;			// 274

	//static HUDMainMenu* GetSingleton() { return *(HUDMainMenu**)(0x11D96C0); }
};
STATIC_ASSERT(sizeof(HUDMainMenu) == 0x278);




template <typename Item> struct ListBoxItem //From JIP DECODING
{
	Tile* tile;
	Item* object;
};

template <typename Item> class ListBox : public BSSimpleList<ListBoxItem<Item>> //From JIP DECODING
{
public:
	Tile* parentTile;	// 0C
	Tile* selected;		// 10
	Tile* scrollBar;		// 14
	const char* templateName;	// 18
	UInt16			itemCount;		// 1C
	UInt16			pad1E;			// 1E
	float			flt20;			// 20
	float			listIndex;		// 24
	float			currValue;		// 28
	UInt16			word2C;			// 2C
	UInt16			pad2E;			// 2E

	Item* GetSelected()
	{
		ListNode<ListBoxItem<Item>>* iter = list.Head();
		ListBoxItem<Item>* item;
		do
		{
			item = iter->data;
			if (item && (item->tile == selected))
				return item->object;
		} 		while (iter = iter->next);
		return NULL;
	}

	void Clear()
	{
		ListNode<ListBoxItem<Item>>* iter = list.Head();
		ListBoxItem<Item>* item;
		do
		{
			item = iter->data;
			if (!item) continue;
			if (item->tile)
				item->tile->Destroy(true);
			GameHeapFree(item);
		} 		while (iter = iter->next);
		list.RemoveAll();
		selected = NULL;
		itemCount = 0;
	}
};

































class VATSMenu : public Menu			// 1056 From JIP DECODING
{
public:
	virtual void		Unk_12(void);

	enum ActionTypes
	{
		kAPCost_AttackUnarmed,
		kAPCost_AttackOneHandMelee,
		kAPCost_AttackTwoHandMelee,
		kAPCost_AttackPistol,
		kAPCost_AttackRifle,
		kAPCost_AttackHandle,
		kAPCost_AttackLauncher,
		kAPCost_AttackGrenade,
		kAPCost_AttackMine,
		kAPCost_Reload,
		kAPCost_Crouch,
		kAPCost_Stand,
		kAPCost_SwitchWeapon,
		kAPCost_ToggleWeaponDrawn,
		kAPCost_Heal,
		kAPCost_OneHandThrown = 0x13,
		kAPCost_AttackThrown,
		kAPCost_MAX = 0x16,
	};

	UInt32				unk028;			// 028
	TileImage* tile02C;		// 02C
	TileImage* tile030;		// 030
	TileImage* tile034;		// 034
	TileImage* tile038;		// 038
	TileImage* tile03C;		// 03C
	TileImage* tile040;		// 040
	TileImage* tile044;		// 044
	TileImage* tile048;		// 048
	TileImage* tile04C;		// 04C
	TileImage* tile050;		// 050
	TileText* tile054;		// 054
	TileText* tile058;		// 058
	TileText* tile05C;		// 05C
	TileText* tile060;		// 060
	TileImage* tile064;		// 064
	TileImage* tile068;		// 068
	TileImage* tile06C;		// 06C
	TileImage* tile070;		// 070
	TileText* tile074;		// 074
	TileRect* tile078;		// 078
	TileRect* tile07C;		// 07C
	TileRect* tile080;		// 080
	TileImage* tile084;		// 084
	TileRect* tile088;		// 088
	TileImage* tile08C;		// 08C
	TileImage* tile090;		// 090
	TileImage* tile094;		// 094
	TileImage* tile098;		// 098
	TileText* tile09C;		// 09C
	TileImage* tile0A0;		// 0A0
	TileImage* tile0A4;		// 0A4
	UInt32				unk0A8[2];		// 0A8
	ListBox<UInt32>		queuedActions;	// 0B0
	float				actionPoints;	// 0E0
	float				maxActionPoints;// 0E4
	float				ClipAmmoMax;			// 0E8
	float				reserveAmmoMax;			// 0EC
	float				clipAmmo;		// 0F0
	float				reserveAmmo;	// 0F4
	UInt32				unk0F8;			// 0F8
	float				flt0FC;			// 0FC
	UInt32				unk100[7];		// 100
	int					actionType;		// 11C
	UInt8				isSuccess;		// 120
	UInt8				byte121;		// 121
	UInt8				isMysteriousStrangerVisit;	// 122
	UInt8				byte123;		// 123
	UInt8				remainingShotsToFire;	// 124
	UInt8				count125;		// 125
	UInt8				pad126[2];		// 126
	TESObjectREFR* targetRef;		// 128
	UInt32				avCode;			// 12C
	//ActorHitData* hitData;		// 130
	float				unk134;			// 134
	float				unk138;			// 138
	float				apCost;			// 13C
	UInt8				isMissFortuneVisit;	// 140
	UInt8				pad141[3];		// 141
};
//STATIC_ASSERT(sizeof(VATSMenu) == 0x144);


struct TaskletInfo
{
	enum TaskletStage
	{
		kStage_EnterCS,
		kStage_Execute,
		kStage_LeaveCS
	};

	UInt32			stage;
	UInt32			unk04;	// Unused
	TESObjectREFR* execOn;
};

// 08
class BSTaskletData
{
public:
	virtual void	Destroy(bool doFree);
	virtual bool	Unk_01(void);	// Returns true
	virtual void	ExecuteQueuedTasks();
	virtual void	Unk_03(void);	// Does nothing

	UInt8		byte04;		// 04
	UInt8		pad05[3];	// 05
};

// 14
class BSWin32TaskletData : public BSTaskletData
{
public:
	UInt32		unk08;		// 08
	UInt8		byte0C;		// 0C
	UInt8		pad0D[3];	// 0D
	UInt32		unk10;		// 10
};

// 14
class BSTCommonLLMessageQueue
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);

	UInt32		unk04[4];	// 04
};

// 38
class MobileObjectTaskletData : public BSWin32TaskletData
{
public:
	virtual void	EnterCriticalSections(UInt32 unused);
	virtual void	ExecuteTask(TESObjectREFR* execOn);
	virtual void	LeaveCriticalSections(UInt32 unused);

	UInt32					unk14[2];		// 14
	BSTCommonLLMessageQueue	messageQueue;	// 1C
	UInt32					unk30[2];		// 30
};







// 40
class DetectionTaskData : public MobileObjectTaskletData
{
public:
	float			flt38;		// 38
	UInt32			unk3C;		// 3C
};

// 38
class AnimationTaskData : public MobileObjectTaskletData
{
public:
};

// 44
class PackageUpdateTaskData : public MobileObjectTaskletData
{
public:
	UInt32			unk38[3];	// 38
};

// 3C
class ActorUpdateTaskData : public MobileObjectTaskletData
{
public:
	UInt32			unk38;		// 38
};

// 40
class ActorsScriptTaskData : public MobileObjectTaskletData
{
public:
	UInt32			unk38[2];	// 38
};

// 3C
class MovementTaskData : public MobileObjectTaskletData
{
public:
	UInt32			unk38;		// 38
};

struct LipTask;


struct ProcessManager
{
	UInt32									unk000;				// 000
	NiTArray<MobileObject*>					objects;			// 004
	UInt32									beginOffsets[4];	// 014	0: High, 1: Mid-High, 2: Mid-Low, 3: Low
	UInt32									endOffsets[4];		// 024
	UInt32									offsets034[4];		// 034	Same as beginOffsets
	UInt32									unk044[5];			// 044
	tList<void>								list058;			// 058
	tList<BSTempEffect>						tempEffects;		// 060
	tList<void>								list068;			// 068
	tList<void>								list070;			// 070
	tList<void>								list078;			// 078
	tList<Actor>							highActors;			// 080
	UInt32									unk088[50];			// 088
	UInt32									unk150;				// 150
	float									flt154;				// 154
	float									flt158;				// 158
	UInt8									byte15C;			// 15C
	UInt8									byte15D;			// 15D
	UInt8									pad15E[2];			// 15E
	DetectionTaskData						detectionTasks;		// 160
	AnimationTaskData						animationTasks;		// 1A0
	PackageUpdateTaskData					packageUpdTasks;	// 1D8
	ActorUpdateTaskData						actorUpdTasks;		// 21C
	ActorsScriptTaskData					actorScriptTasks;	// 258
	MovementTaskData						movementTasks;		// 298
	UInt32									unk2D4[0x4023];		// 2D4
	//LockFreeMap<MobileObject*, LipTask*>	lipTasksMap;		// 10360
	UInt8									toggleAI;			// 103A0
	UInt8									toggleDetection;	// 103A1
	UInt8									toggleDetectionStats;	// 103A2
	UInt8									byte103A3;			// 103A3
	UInt32									detectionStats;		// 103A4
	UInt8									toggleHighProcess;	// 103A8
	UInt8									toggleLowProcess;	// 103A9
	UInt8									toggleMidHighProcess;	// 103AA
	UInt8									toggleMidLowProcess;	// 103AB
	UInt8									toggleAISchedules;	// 103AC
	UInt8									showSubtitle;		// 103AD
	UInt8									byte103AE;			// 103AE
	UInt8									byte103AF;			// 103AF
	UInt32									unk103B0;			// 103B0
	float									flt103B4;			// 103B4
	UInt32									unk103B8;			// 103B8
	float									removeDeadActorsTime;	// 103BC
	UInt32									unk103C0[3];		// 103C0

	__forceinline int GetTotalDetectionValue(Actor* actor, bool arg2 = false)
	{
		return ThisCall<int>(0x973710, this, actor, arg2);
	}
};
//STATIC_ASSERT(sizeof(ProcessManager) == 0x103CC);







struct CombatTarget
{
	Actor* target;
	UInt32		unk04[25];
};

// 14
struct CombatAlly
{
	Actor* ally;
	UInt32		unk04[4];
};


struct NiPoint2
{
	float	x;
	float	y;

	NiPoint2() {}
	NiPoint2(float _x, float _y) : x(_x), y(_y) {}
	NiPoint2(const NiPoint2& rhs) { *(double*)this = *(double*)&rhs; }

	inline NiPoint2& operator=(const NiPoint2& rhs)
	{
		*(double*)this = *(double*)&rhs;
		return *this;
	}
};


struct CombatSearchLocation;
struct CombatSearchDoor;
struct CombatGroupCluster;



// 15C
struct CombatActors
{
	struct Unk0F0
	{
		NiVector3		vector00;
		UInt32			unk0C;
	};

	UInt32									unk000[2];			// 000
	BSSimpleArray<CombatTarget>				targets;			// 008
	BSSimpleArray<CombatAlly>				allies;				// 018
	UInt32									unk028;				// 028
	UInt8									byte02C;			// 02C
	UInt8									pad02D[3];			// 02D
	NiPoint2								unk030;				// 030
	NiPoint2								unk038;				// 038
	UInt32									unk040;				// 040
	float									flt044;				// 044
	NiPoint2								unk048;				// 048
	NiPoint2								unk050;				// 050
	NiPoint2								unk058;				// 058
	NiPoint2								unk060;				// 060
	NiPoint2								unk068;				// 068
	NiPoint2								unk070;				// 070
	NiPoint2								unk078;				// 078
	NiPoint2								unk080;				// 080
	NiPoint2								unk088;				// 088
	NiPoint2								unk090;				// 090
	NiPoint2								unk098;				// 098
	NiPoint2								unk0A0;				// 0A0
	NiPoint2								unk0A8;				// 0A8
	NiPoint2								unk0B0;				// 0B0
	NiPoint2								unk0B8;				// 0B8
	float									flt0C0;				// 0C0
	float									flt0C4;				// 0C4
	float									flt0C8;				// 0C8
	float									flt0CC;				// 0CC
	UInt32									targetSearchState;	// 0D0	0 - Not searching; 1-2 - Searching
	NiRefObject* object0D4;			// 0D4
	NiPoint2								unk0D8;				// 0D8
	NiPoint2								unk0E0;				// 0E0
	float									flt0E8;				// 0E8
	UInt32									unk0EC;				// 0EC
	Unk0F0									unk0F0;				// 0F0
	NiVector3								vector100;			// 100
	float									flt10C;				// 10C
	BSSimpleArray<CombatSearchLocation>		searchLocations;	// 110
	BSSimpleArray<CombatSearchDoor>			searchDoors;		// 120
	NiRefObject* object130;			// 130
	UInt8									byte134;			// 134
	UInt8									pad135[3];			// 135
	BSSimpleArray<CombatGroupCluster>		groupClusters;		// 138
	UInt32									unk148;				// 148
	UInt32									unk14C;				// 14C
	UInt32									unk150;				// 150
	UInt8									byte154;			// 154
	UInt8									pad155[3];			// 155
	NiRefObject* object158;			// 158
};
STATIC_ASSERT(sizeof(CombatActors) == 0x15C);



enum ActorValueCode
{
	kAVCode_Aggression,
	kAVCode_Confidence,
	kAVCode_Energy,
	kAVCode_Responsibility,
	kAVCode_Mood,
	kAVCode_Strength,
	kAVCode_Perception,
	kAVCode_Endurance,
	kAVCode_Charisma,
	kAVCode_Intelligence,
	kAVCode_Agility,
	kAVCode_Luck,
	kAVCode_ActionPoints,
	kAVCode_CarryWeight,
	kAVCode_CritChance,
	kAVCode_HealRate,
	kAVCode_Health,
	kAVCode_MeleeDamage,
	kAVCode_DamageResist,
	kAVCode_PoisonResist,
	kAVCode_RadResist,
	kAVCode_SpeedMult,
	kAVCode_Fatigue,
	kAVCode_Karma,
	kAVCode_XP,
	kAVCode_PerceptionCondition,
	kAVCode_EnduranceCondition,
	kAVCode_LeftAttackCondition,
	kAVCode_RightAttackCondition,
	kAVCode_LeftMobilityCondition,
	kAVCode_RightMobilityCondition,
	kAVCode_BrainCondition,
	kAVCode_Barter,
	kAVCode_BigGuns,
	kAVCode_EnergyWeapons,
	kAVCode_Explosives,
	kAVCode_Lockpick,
	kAVCode_Medicine,
	kAVCode_MeleeWeapons,
	kAVCode_Repair,
	kAVCode_Science,
	kAVCode_Guns,
	kAVCode_Sneak,
	kAVCode_Speech,
	kAVCode_Survival,
	kAVCode_Unarmed,
	kAVCode_InventoryWeight,
	kAVCode_Paralysis,
	kAVCode_Invisibility,
	kAVCode_Chameleon,
	kAVCode_NightEye,
	kAVCode_Turbo,
	kAVCode_FireResist,
	kAVCode_WaterBreathing,
	kAVCode_RadiationRads,
	kAVCode_BloodyMess,
	kAVCode_UnarmedDamage,
	kAVCode_Assistance,
	kAVCode_ElectricResist,
	kAVCode_FrostResist,
	kAVCode_EnergyResist,
	kAVCode_EmpResist,
	kAVCode_Variable01,
	kAVCode_Variable02,
	kAVCode_Variable03,
	kAVCode_Variable04,
	kAVCode_Variable05,
	kAVCode_Variable06,
	kAVCode_Variable07,
	kAVCode_Variable08,
	kAVCode_Variable09,
	kAVCode_Variable10,
	kAVCode_IgnoreCrippledLimbs,
	kAVCode_Dehydration,
	kAVCode_Hunger,
	kAVCode_SleepDeprivation,
	kAVCode_DamageThreshold,
};



class NiProperty : public NiObjectNET
{
public:
	virtual UInt32	GetPropertyType();
	virtual void	UpdateController(float arg);

	enum
	{
		kPropertyType_Alpha = 0,
		kPropertyType_Culling = 1,
		kPropertyType_Material = 2,
		kPropertyType_Shade = 3,
		kPropertyType_TileShader = kPropertyType_Shade,
		kPropertyType_Stencil = 4,
		kPropertyType_Texturing = 5,
		kPropertyType_Dither = 8,
		kPropertyType_Specular = 9,
		kPropertyType_VertexColor = 10,
		kPropertyType_ZBuffer = 11,
		kPropertyType_Fog = 13,
	};
};



class NiTexturingProperty : public NiProperty
{
public:
	UInt32				unk18[6];	// 18
};



struct FontInfoJIP
{
	struct BufferData
	{
		float			lineHeight;		// 0000
		UInt32			unk0004[73];	// 0004
		UInt32			unk0128[458];	// 0128
		float			baseHeight;		// 0850
		float			flt0854;		// 0854
		float			flt0858;		// 0858
	};

	struct ButtonIcon;

	UInt8						isLoaded;	// 00
	UInt8						pad01[3];	// 01
	char* filePath;	// 04
	UInt8						fontID;		// 08
	UInt8						pad09[3];	// 09
	NiTexturingProperty* texProp;	// 0C
	UInt32						unk10[7];	// 10
	float						flt2C;		// 2C
	float						flt30;		// 30
	UInt32						unk34;		// 34
	BufferData* bufferData;// 38
	UInt32						unk3C[2];	// 3C
	BSSimpleArray<ButtonIcon>	arr44;		// 44
};
//STATIC_ASSERT(sizeof(FontInfoJIP) == 0x54);






class FontManagerJIP
{
public:
	FontInfoJIP* fontInfos[8];		// 00
	UInt8			byte20;				// 20
	UInt8			pad21[3];			// 21
	//FontInfo* extraFonts[80];	// 24

	//	outDims.x := width (pxl); outDims.y := height (pxl); outDims.z := numLines
	__forceinline NiVector3* GetStringDimensions(NiVector3* outDims, const char* srcString, UInt32 fontID, UInt32 maxFlt = 0x7F7FFFFF, UInt32 startIdx = 0)
	{
		return ThisCall<NiVector3*>(0xA1B020, this, outDims, srcString, fontID, maxFlt, startIdx);
	}
};



struct GradualSetFloat // From JIP
{
	float		startValue;		// 00
	float		endValue;		// 04
	UInt32		startTimeMS;	// 08
	float		durationMS;		// 0C
	UInt32		valueID;		// 10
	Tile* tile;			// 14
	UInt32		changeMode;		// 18	0-4, see 0xA081B5
	//	0		From start to end in duration
	//	1		From start to end to start, in duration, perpetually
	//	2		From start to end to start, in duration, 4 times
	//	3		From start to end to start, 7 times, in duration
	//	4		From start to end in duration/6, end for duration*2/3, from end to start in duration/6
	//	5**		From start to end, in duration, perpetually (suitable for image rotation)
};

struct SystemColorManager
{
	// 0C
	class SystemColor
	{
	public:
		virtual void	Destructor(bool doFree);
		virtual UInt32	GetColor();
		virtual void	SetColor(UInt32 newColor);
		virtual bool	IsHard();
		virtual bool	IsSoft();

		String		traitName;
	};

	// 10
	class HardSystemColor : public SystemColor
	{
	public:
		UInt32		color;		// 0C
	};

	// 10
	//class SoftSystemColor : public SystemColor
	//{
	//public:
	//	Setting* setting;	// 0C
	//};

	DList<SystemColor>	sysColors;
	UInt32				unk0C;
};


class LockPickMenu : public Menu		// 1014
{
public:
	UInt32					stage;			// 28
	TileRect* tile2C;		// 2C
	TileRect* tile30;		// 30
	TileRect* tile34;		// 34
	TileImage* tile38;		// 38
	TileText* tile3C;		// 3C
	TileImage* tile40;		// 40
	TileImage* tile44;		// 44
	TileText* tile48;		// 48
	TileImage* tile4C;		// 4C
	TileImage* tile50;		// 50
	TileImage* tile54;		// 54
	TileText* tile58;		// 58
	TileText* tile5C;		// 5C
	TileImage* tile60;		// 60
	TileImage* tile64;		// 64
	TileImage* tile68;		// 68
	TESObjectREFR* targetRef;		// 6C
	UInt32					lockLevel;		// 70
	UInt32					skillLevel;		// 74
	float					fSkillLevel;	// 78
	UInt32					unk7C;			// 7C
	UInt32					unk80;			// 80
	float					flt84;			// 84
	UInt32					sweetSpotLen;	// 88
	float					cylinderAngle;	// 8C
	float					pickAngle;		// 90
	float					pickHealth;		// 94
	UInt8					isForceRotate;	// 98
	UInt8					byte99;			// 99
	UInt8					pad9A[2];		// 99
	NiControllerSequence* ctrlSeq9C;		// 9C
	NiControllerSequence* ctrlSeqA0;		// A0
	NiControllerManager* ctrlManager;	// A4
	NiControllerSequence* ctrlSeqA8;		// A8
	NiControllerSequence* ctrlSeqAC;		// AC
	NiControllerSequence* ctrlSeqB0;		// B0
	NiQuaternion			quaternionB4;	// B4
	NiQuaternion			quaternionC4;	// C4
	NiQuaternion			quaternionD4;	// D4
};
STATIC_ASSERT(sizeof(LockPickMenu) == 0xE4);