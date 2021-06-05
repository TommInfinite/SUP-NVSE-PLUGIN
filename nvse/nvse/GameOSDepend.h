#pragma once

// keeping this in a separate file so we don't need to include dinput/dsound everywhere

#define DIRECTINPUT_VERSION 0x0800
#define DIRECTSOUND_VERSION 0x0800
#include <dinput.h>
//#include <dsound.h>






struct _IDirectInput8;


class DInputInterface
{
public:
	virtual void	Unk_00(void);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(DInputInterface* intrfc, UInt32 arg2, void* arg3);
	virtual UInt32	GetStatesChanged(DInputInterface* intrfc);
	virtual void	Unk_08(void);
	virtual void	OutputStates(DInputInterface* intrfc, UInt32 bufSize, char* buffer);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(DInputInterface* intrfc, void* arg2);
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(DInputInterface* intrfc, HWND window, UInt32 type);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(void);
	virtual void	Unk_11(void);
	virtual void	Unk_12(void);
	virtual void	Unk_13(void);
	virtual void	Unk_14(void);
	virtual void	Unk_15(void);
	virtual void	Unk_16(void);
	virtual void	Unk_17(void);
	virtual void	Unk_18(void);
	virtual void	QueryStates(DInputInterface* intrfc);
};




class OSInputGlobals
{
public:
	enum
	{
		kFlag_HasJoysticks = 1 << 0,
		kFlag_HasMouse = 1 << 1,
		kFlag_HasKeyboard = 1 << 2,
		kFlag_BackgroundMouse = 1 << 3,
	};

	UInt8				controllerDisabled;		// 0000
	UInt8				pad0001[3];				// 0001
	UInt32				flags;					// 0004
	_IDirectInput8* directInput;			// 0008
	void* ptr000C[8];			// 000C
	DInputInterface* keyboardInterface;		// 002C
	DInputInterface* mouseInterface;		// 0030
	UInt32				unk0034[320];			// 0034
	UInt32				unk0534[1264];			// 0534
	UInt32				unk18F4;				// 18F4
	UInt8				currKeyStates[256];		// 18F8
	UInt8				lastKeyStates[256];		// 19F8
	UInt32				unk1AF8[11];			// 1AF8
	int					currMouseMovementX;		// 1B24
	int					currMouseMovementY;		// 1B28
	int					currMouseWheelScroll;	// 1B2C
	UInt8				currButtonStates[8];	// 1B30
	int					lastMouseMovementX;		// 1B38
	int					lastMouseMovementY;		// 1B3C
	int					lastMouseWheelScroll;	// 1B40
	UInt8				lastButtonStates[8];	// 1B44
	UInt32				ltrtButtonState;		// 1B4C
	UInt32				unk1B50[2];				// 1B50
	UInt8				buttonStates1B58[8];	// 1B58
	UInt32				unk1B60[8];				// 1B60
	UInt32* controllerVibration;	// 1B80
	UInt32				unk1B84;				// 1B84
	UInt8				controllerNotDisabled;	// 1B88
	UInt8				pad1B89[3];				// 1B89
	UInt32				unk1B8C[2];				// 1B8C
	UInt8				keyBinds[28];			// 1B94
	UInt8				mouseBinds[28];			// 1BB0
	UInt8				joystickBinds[28];		// 1BCC
	UInt8				controllerBinds[28];	// 1BE8
};
STATIC_ASSERT(sizeof(OSInputGlobals) == 0x1C04);

extern OSInputGlobals** g_OSInputGlobals;

#if 0
#include "GameTypes.h"

class TESGameSound;
class NiAVObject;

// 58
class TESGameSound
{
public:
	TESGameSound();
	~TESGameSound();

	UInt32			unk00[3];	// 00
	UInt32			hashKey;	// 0C
	UInt32			unk10[4];	// 10
	float			x;			// 20
	float			y;			// 24
	float			z;			// 28
	UInt32			unk2C[4];	// 2C
	float			unk3C;		// 3C
	UInt32			unk40[3];	// 40
	const char *	name;		// 4C
	UInt32			unk50;		// 50
	UInt32			unk54;		// 54
};

// 328
class OSSoundGlobals
{
public:
	OSSoundGlobals();
	~OSSoundGlobals();

	enum
	{
		kFlags_HasDSound =		1 << 0,
		kFlags_HasHardware3D =	1 << 2,
	};
	
	typedef NiTPointerMap <TESGameSound>	TESGameSoundMap;
	typedef NiTPointerMap <NiAVObject>		NiAVObjectMap;

	UInt32					unk000;						// 000
	UInt32					unk004;						// 004
	IDirectSound8			* dsoundInterface;			// 008
	IDirectSoundBuffer8		* primaryBufferInterface;	// 00C
	DSCAPS					soundCaps;					// 010
	UInt32					unk070;						// 070
	UInt32					unk074;						// 074
	IDirectSound3DListener	* listenerInterface;		// 078
	UInt32					unk07C[(0x0A4-0x07C) >> 2];	// 07C
	UInt8					unk0A4;						// 0A4
	UInt8					unk0A5;						// 0A5
	UInt8					unk0A6;						// 0A6
	UInt8					pad0A7;						// 0A7
	UInt32					unk0A8;						// 0A8
	UInt32					flags;						// 0AC - flags?
	UInt32					unk0B0;						// 0B0
	float					unk0B4;						// 0B4
	float					masterVolume;				// 0B8
	float					footVolume;					// 0BC
	float					voiceVolume;				// 0C0
	float					effectsVolume;				// 0C4
	UInt32					unk0C8;						// 0C8 - time
	UInt32					unk0CC;						// 0CC - time
	UInt32					unk0D0;						// 0D0 - time
	UInt32					unk0D4[(0x0DC-0x0D4) >> 2];	// 0D4
	UInt32					unk0DC;						// 0DC
	UInt32					unk0E0[(0x2F0-0x0E0) >> 2];	// 0E0
	float					musicVolume;				// 2F0
	UInt32					unk2F4;						// 2F4
	float					musicVolume2;				// 2F8
	UInt32					unk2FC;						// 2FC
	TESGameSoundMap			* gameSoundMap;				// 300
	NiAVObjectMap			* niObjectMap;				// 304
	NiTPointerList <void>	* soundMessageMap;			// 308 - AudioManager::SoundMessage *
	UInt32					unk30C[(0x320-0x30C) >> 2];	// 30C
	void					* soundMessageList;			// 320
	UInt32					unk324;						// 324
};

STATIC_ASSERT(sizeof(OSSoundGlobals) == 0x328);
#endif

class OSSoundGlobals {
};

// A4
class OSGlobals
{
public:
	OSGlobals();
	~OSGlobals();

	UInt8							oneMore;		// 00
	UInt8							quitGame;		// 01
	UInt8							exitToMainMenu;	// 02
	UInt8							byte03;			// 03
	UInt8							byte04;			// 04
	UInt8							byte05;			// 05
	UInt8							tfcState;		// 06
	bool							freezeTime;		// 07
	HWND			window;				// 08
	HINSTANCE		procInstance;		// 0C
	UInt32			mainThreadID;		// 10
	HANDLE			mainThreadHandle;	// 14
	UInt32*			unk18;				// 18 ScrapHeapManager::Buffer*
	UInt32			unk1C;				// 1C
	OSInputGlobals	* input;			// 20
	OSSoundGlobals	* sound;			// 24
	UInt32			unk28;				// 28 relates to unk18
	//...
	UInt32*			unk50;				// 50, same object as unk18
	//..
	UInt32			unk60;				// 60 relates to unk50
};

//STATIC_ASSERT(sizeof(OSGlobals) == 0x0A4);	// found in oldWinMain 0x0086AF4B

extern OSGlobals ** g_osGlobals;
