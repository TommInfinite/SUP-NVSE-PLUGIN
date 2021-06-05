#pragma once
#define CALL_EAX(addr) __asm mov eax, addr __asm call eax

// All from JIP

static const float kFltPId180 = 0.01745329238F;

static const double kDbl180dPI = 57.29577951308232088;


class NiCollisionObject : public NiObject
{
public:
	virtual void	Attach(NiAVObject* obj);
	virtual void	Unk_24(UInt32 arg);
	virtual void	Unk_25(void);
	virtual void	LoadBoundingVolume(UInt32 arg);
	virtual void	Unk_27(UInt32 version, UInt32 arg1);

	NiNode* linkedNode;	// 08
};



class bhkNiCollisionObject : public NiCollisionObject
{
public:
	virtual void	Unk_28(void);
	virtual void	Unk_29(void);
	virtual void	Unk_2A(void);
	virtual void	Unk_2B(void);
	virtual void	Unk_2C(UInt32 moType, UInt32 arg2, UInt32 arg3);	//	arg3 unused?
	virtual void	Unk_2D(void);
	virtual void	ProcessCollision(UInt32 arg);
	virtual void	Unk_2F(void);
	virtual void	Unk_30(void);

	UInt16			flags;			// 0C	0x40 is Update? Callbacks array @ 0x11AFE88
	UInt16			word0E;			// 0E
	//bhkWorldObject* worldObj;		// 10
};








class NiAVObject : public NiObjectNET
{
public:
	virtual void	Unk_23(UInt32 arg1);
	virtual void	Unk_24(NiMatrix33* arg1, NiVector3* arg2, bool arg3);
	virtual void	Unk_25(UInt32 arg1);
	virtual void	Unk_26(UInt32 arg1);
	virtual NiAVObject* GetObjectByName(NiString* objName);
	virtual void	Unk_28(UInt32 arg1, UInt32 arg2, UInt32 arg3);
	//virtual void	Unk_29(UpdateParams* updParams, UInt32 arg2);
	virtual void	Unk_2A(UInt32 arg1, UInt32 arg2);
	virtual void	Unk_2B(UInt32 arg1, UInt32 arg2);
	virtual void	Unk_2C(UInt32 arg1);
	virtual void	Unk_2D(UInt32 arg1);
	//virtual void	UpdateTransform(UpdateInfo* updInfo);
	virtual void	Unk_2F(void);
	//virtual void	UpdateBounds(UpdateInfo* updInfo);
	virtual void	Unk_31(UInt32 arg1, UInt32 arg2);
	virtual void	Unk_32(UInt32 arg1);
	virtual void	Unk_33(UInt32 arg1);
	virtual void	Unk_34(void);
	virtual void	Unk_35(UInt32 arg1);
	virtual void	Unk_36(UInt32 arg1);

	enum NiFlags
	{
		kNiFlag_Hidden = 0x00000001,
		kNiFlag_SelectiveUpdate = 0x00000002,
		kNiFlag_SelUpdTransforms = 0x00000004,
		kNiFlag_SelUpdController = 0x00000008,
		kNiFlag_SelUpdRigid = 0x00000010,
		kNiFlag_DisplayObject = 0x00000020,
		kNiFlag_DisableSorting = 0x00000040,
		kNiFlag_SelUpdTransformsOverride = 0x00000080,
		kNiFlag_UnkBit8 = 0x00000100,
		kNiFlag_SaveExternalGeomData = 0x00000200,
		kNiFlag_NoDecals = 0x00000400,
		kNiFlag_AlwaysDraw = 0x00000800,
		kNiFlag_MeshLOD = 0x00001000,
		kNiFlag_FixedBound = 0x00002000,
		kNiFlag_TopFadeNode = 0x00004000,
		kNiFlag_IgnoreFade = 0x00008000,
		kNiFlag_NoAnimSyncX = 0x00010000,
		kNiFlag_NoAnimSyncY = 0x00020000,
		kNiFlag_NoAnimSyncZ = 0x00040000,
		kNiFlag_NoAnimSyncS = 0x00080000,
		kNiFlag_NoDismember = 0x00100000,
		kNiFlag_NoDismemberValidity = 0x00200000,
		kNiFlag_RenderUse = 0x00400000,
		kNiFlag_MaterialsApplied = 0x00800000,
		kNiFlag_HighDetail = 0x01000000,
		kNiFlag_ForceUpdate = 0x02000000,
		kNiFlag_PreProcessedNode = 0x04000000,
		kNiFlag_UnkBit27 = 0x08000000,
		kNiFlag_UnkBit28 = 0x10000000,
		kNiFlag_IsPointLight = 0x20000000,	//	JIP only
		kNiFlag_DoneInitLights = 0x40000000,	//	JIP only
		kNiFlag_IsInserted = 0x80000000	//	JIP only
	};

	NiNode* m_parent;				// 18
	bhkNiCollisionObject* m_collisionObject;		// 1C
	NiSphere* m_kWorldBound;			// 20
	DList<NiProperty>		m_propertyList;			// 24
	UInt32					m_flags;				// 30
	NiMatrix33				m_localRotate;			// 34
	NiVector3				m_localTranslate;		// 58
	float					m_localScale;			// 64
	NiMatrix33				m_worldRotate;			// 68
	NiVector3				m_worldTranslate;		// 8C
	float					m_worldScale;			// 98

	void Update();
	UInt32 GetIndex();
	bool __fastcall ReplaceObject(NiAVObject* object);
	NiProperty* __fastcall GetProperty(UInt32 propID);

	void DumpProperties();
	void DumpParents();
};


class NiNode : public NiAVObject
{
public:
	virtual void	AddObject(NiAVObject* object, bool arg2);
	virtual void	AddObjectAt(UInt32 index, NiAVObject* object);
	virtual void	RemoveObject2(NiAVObject* toRemove, NiAVObject** arg2);
	virtual void	RemoveObject(NiAVObject* toRemove);	//	Calls RemoveObject2 with arg2 as ptr to NULL
	virtual void	RemoveNthObject2(UInt32 index, NiAVObject** arg2);
	virtual void	RemoveNthObject(UInt32 index);			//	Calls RemoveNthObject2 with arg2 as ptr to NULL
	virtual void	Unk_3D(UInt32 arg1, UInt32 arg2, UInt32 arg3);
	virtual void	Unk_3E(UInt32 arg1, UInt32 arg2);
	virtual void	Unk_3F(void);

	NiTArray<NiAVObject*>	m_children;		// 9C

	static NiNode* __stdcall Create(const char* nodeName);
	NiNode* CreateCopy();
	NiAVObject* __fastcall GetBlockByName(const char* objName);
	NiAVObject* __fastcall GetBlock(const char* blockName);
	NiNode* __fastcall GetNode(const char* nodeName);
	bool IsMovable();
	void RemoveCollision();
	void BulkSetMaterialPropertyTraitValue(UInt32 traitID, float value);
	//void GetContactObjects(ContactObjects* contactObjs);
	bool HasPhantom();
	void GetBodyMass(float* totalMass);
	void ApplyForce(NiVector4* forceVector);
	void Dump();
};
STATIC_ASSERT(sizeof(NiNode) == 0xAC);


__declspec(naked) NiAVObject* __fastcall NiNode::GetBlockByName(const char* objName)
{
	__asm
	{
		cmp[ecx + 8], edx
		jz		found
		movzx	eax, word ptr[ecx + 0xA6]
		test	eax, eax
		jz		done
		push	esi
		push	edi
		mov		esi, [ecx + 0xA0]
		mov		edi, eax
		ALIGN 16
		iterHead:
		dec		edi
			js		iterEnd
			mov		ecx, [esi]
			add		esi, 4
			test	ecx, ecx
			jz		iterHead
			mov		eax, [ecx]
			cmp		dword ptr[eax + 0xC], 0x6815C0
			jnz		notNode
			call	NiNode::GetBlockByName
			test	eax, eax
			jz		iterHead
			pop		edi
			pop		esi
			retn
			ALIGN 16
			notNode:
		cmp[ecx + 8], edx
			jnz		iterHead
			pop		edi
			pop		esi
			found :
		mov		eax, ecx
			retn
			ALIGN 16
			iterEnd :
			xor eax, eax
			pop		edi
			pop		esi
			done :
		retn
	}
}



__declspec(naked) NiAVObject* __fastcall NiNode::GetBlock(const char* blockName)
{
	__asm
	{
		push	ecx
		push	edx
		CALL_EAX(0xA5B690)
		pop		ecx
		pop		ecx
		test	eax, eax
		jz		done
		push	eax
		mov		edx, eax
		call	NiNode::GetBlockByName
		pop		ecx
		lock dec dword ptr[ecx - 8]
		done:
		retn
	}
}




__declspec(naked) NiAVObject* __fastcall GetNifBlock(TESObjectREFR* thisObj, UInt32 pcNode, char* blockName)
{
	__asm
	{
		test	dl, dl
		jz		notPlayer
		cmp		dword ptr[ecx + 0xC], 0x14
		jnz		notPlayer
		test	dl, 1
		jz		get1stP
		mov		eax, [ecx + 0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax + 0x14]
		jmp		gotRoot
		get1stP :
		mov		eax, [ecx + 0x694]
			jmp		gotRoot
			notPlayer :
		mov		eax, [ecx]
			call	dword ptr[eax + 0x1D0]
			gotRoot :
			test	eax, eax
			jz		done
			mov		edx, [esp + 4]
			cmp[edx], 0
			jz		done
			mov		ecx, eax
			call	NiNode::GetBlock
			done :
		retn	4
	}
}



class NiAlphaProperty : public NiProperty
{
public:
	enum AlphaFlag
	{
		kFlag_EnableBlending = 1 << 0,
		kFlag_SourceInverse = 1 << 1,
		kFlag_SourceSrcColor = 1 << 2,
		kFlag_SourceDestColor = 1 << 3,
		kFlag_SourceSrcAlpha = kFlag_SourceSrcColor | kFlag_SourceDestColor,
		kFlag_SourceDestAlpha = 1 << 4,
		kFlag_SourceAlphaSaturate = kFlag_SourceSrcColor | kFlag_SourceDestAlpha,
		kFlag_DestinInverse = 1 << 5,
		kFlag_DestinSrcColor = 1 << 6,
		kFlag_DestinDestColor = 1 << 7,
		kFlag_DestinSrcAlpha = kFlag_DestinSrcColor | kFlag_DestinDestColor,
		kFlag_DestinDestAlpha = 1 << 8,
		kFlag_DestinAlphaSaturate = kFlag_DestinSrcColor | kFlag_DestinDestAlpha,
		kFlag_EnableTesting = 1 << 9,
		kFlag_TestFuncLess = 1 << 10,
		kFlag_TestFuncEqual = 1 << 11,
		kFlag_TestFuncLessOrEq = kFlag_TestFuncLess | kFlag_TestFuncEqual,
		kFlag_TestFuncGreater = 1 << 12,
		kFlag_TestFuncNotEqual = kFlag_TestFuncLess | kFlag_TestFuncGreater,
		kFlag_TestFuncGrtOrEq = kFlag_TestFuncEqual | kFlag_TestFuncGreater,
		kFlag_TestFuncNever = kFlag_TestFuncLess | kFlag_TestFuncEqual | kFlag_TestFuncGreater,
		kFlag_NoSorter = 1 << 13,
	};

	UInt16				flags;		// 18
	UInt8				threshold;	// 1A
	UInt8				byte1B;		// 1B
};

class NiCullingProperty : public NiProperty
{
public:
	UInt32				unk18;		// 18
};

class NiMaterialProperty : public NiProperty
{
public:
	UInt32				unk18;			// 18
	NiColor				specularRGB;	// 1C
	NiColor				emissiveRGB;	// 28
	UInt32				isExternalEmit;	// 34
	float				glossiness;		// 38
	float				alpha;			// 3C
	float				emitMult;		// 40
	UInt32				unk44[2];		// 44

	void SetTraitValue(UInt32 traitID, float value);
};


class BSShaderProperty : public NiProperty
{
public:
	virtual void	Unk_25(void);
	virtual void	Unk_26(UInt32 arg1);
	virtual void	Unk_27(UInt32 arg1);
	virtual void	Unk_28(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5, UInt32 arg6);
	virtual void	Unk_29(UInt32 arg1);
	virtual void	Unk_2A(void);
	virtual void	Unk_2B(UInt32 arg1);
	virtual void	Unk_2C(UInt32 arg1, UInt32 arg2, UInt32 arg3);
	virtual void	Unk_2D(void);
	virtual void	Unk_2E(UInt32 arg1);
	virtual void	Unk_2F(UInt32 arg1, UInt32 arg2);
	virtual void	Unk_30(void);

	UInt16			unk18;				// 18
	UInt16			unk1A;				// 1A
	UInt32			unk1C;				// 1C
	UInt32			flags20;			// 20
	UInt32			flags24;			// 24
	float			alpha;				// 28
	float			fadeAlpha;			// 2C
	float			envMapScale;		// 30
	float			flt34;				// 34
	UInt32			lastRenderPassState;// 38
	void* renderPassList;	// 3C	Seen 010B8480
	UInt32			unk40;				// 40
	UInt32			unk44;				// 44
	UInt32			unk48;				// 48
	UInt32			unk4C;				// 4C
	UInt32			unk50;				// 50
	UInt32			unk54;				// 54
	UInt32			unk58;				// 58
	float			flt5C;				// 5C
};
STATIC_ASSERT(sizeof(BSShaderProperty) == 0x60);


class NiStencilProperty : public NiProperty
{
public:
	UInt16				flags;		// 18
	UInt16				word1A;		// 1A
	UInt32				unk1C;		// 1C
	UInt32				mask;		// 20
};
STATIC_ASSERT(sizeof(NiStencilProperty) == 0x24);


class NiTexturingPropertyJIP : public NiProperty
{
public:
	class Map
	{
	public:
		virtual void	Destroy(bool doFree);
		virtual void	Unk_01(void);
		virtual void	Unk_02(void);
		virtual void	Unk_03(void);

		Map* baseMap;		// 04
		NiSourceTexture* srcTexture;	// 08
		//TextureTransform* transform;		// 0C
	};

	class ShaderMap : public Map
	{
	public:
		UInt32				mapID;		// 10
	};

	UInt16					flags;				// 18
	UInt8					pad1A[2];			// 1A
	NiTArray<Map*>			textures;			// 1C
	NiTArray<ShaderMap*>* shaderTextures;	// 2C
};
//STATIC_ASSERT(sizeof(NiTexturingProperty) == 0x30);


class NiGeometryData : public NiObject
{
public:
	virtual void	Unk_23(UInt32 arg);
	virtual void	Unk_24(void);
	virtual void	Unk_25(void);
	virtual void	Unk_26(void);
	virtual bool	Unk_27(UInt32 arg);
	virtual void	Unk_28(void);

	UInt16					numVertices;	// 08
	UInt16					word0A;			// 0A
	UInt16					word0C;			// 0C
	UInt16					word0E;			// 0E
	NiSphere				bounds;			// 10
	NiVector3* vertices;		// 20
	//NiVector3* normals;		// 24
	//NiColorAlpha* vertexColors;	// 28
	//UVCoord* uvCoords;		// 2C
	//UInt32					unk30;			// 30
	//NiGeometryBufferData* bufferData;	// 34
	//UInt8					byte38;			// 38
	//UInt8					byte39;			// 39
	//UInt8					byte3A;			// 3A
	//UInt8					byte3B;			// 3B
	//UInt8					byte3C;			// 3C
	//UInt8					pad3D[3];		// 3D
};
//STATIC_ASSERT(sizeof(NiGeometryData) == 0x40);




class NiGeometry : public NiAVObject
{
public:
	virtual void	Unk_37(UInt32 arg1);
	virtual void	Unk_38(UInt32 arg1);
	virtual void	Unk_39(void);
	virtual void	Unk_3A(void);
	virtual void	Unk_3B(UInt32 arg1);

	NiAlphaProperty* alphaProp;		// 9C
	NiCullingProperty* cullingProp;	// A0
	NiMaterialProperty* materialProp;	// A4
	BSShaderProperty* shaderProp;	// A8
	NiStencilProperty* stencilProp;	// AC
	NiTexturingPropertyJIP* texturingProp;	// B0
	UInt32					unkB4;			// B4
	NiGeometryData* geometryData;	// B8
	//BSDismemberSkinInstance* skinInstance;	// BC
	//NiShader* shader;		// C0
};
//STATIC_ASSERT(sizeof(NiGeometry) == 0xC4);











class BSShaderTextureSet : public BSTextureSet
{
public:
	String		textures[6];	// 08
};



class BSShaderLightingProperty : public BSShaderProperty
{
public:
	virtual void	Unk_31(void);
	virtual void	Unk_32(void);

	UInt32			unk60[3];		// 60
	float			flt6C;			// 6C
	UInt32			unk70;			// 70
	UInt8			byte74;			// 74
	UInt8			pad75[3];		// 75
	UInt32			unk78;			// 78
};
//STATIC_ASSERT(sizeof(BSShaderLightingProperty) == 0x7C);




class BSShaderPPLightingProperty : public BSShaderLightingProperty
{
public:
	virtual void	Unk_33(void);
	virtual void	Unk_34(void);
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
	virtual void	Unk_40(void);
	virtual void	Unk_41(void);
	virtual void	Unk_42(void);
	virtual void	Unk_43(void);
	virtual void	Unk_44(void);
	virtual void	Unk_45(void);
	virtual void	Unk_46(void);
	virtual void	Unk_47(void);

	UInt32				unk07C;					// 07C
	UInt32				unk080;					// 080
	float				flt084[8];				// 084
	BSShaderTextureSet* textureSet;			// 0A4
	//UInt16				word0A8;				// 0A8
	//UInt16				word0AA;				// 0AA
	//UInt32				unk0AC[6];				// 0AC
	//void* ptr0C4;				// 0C4
	//UInt32				unk0C8;					// 0C8
	//void* ptr0CC;				// 0CC
	//UInt32				unk0D0[4];				// 0D0
	//float				refractionStrength;		// 0E0
	//SInt32				refractionFirePeriod;	// 0E4
	//float				parallaxMaxPasses;		// 0E8
	//float				parallaxScale;			// 0EC
	//UInt32				unk0F0[5];				// 0F0
};
//STATIC_ASSERT(sizeof(BSShaderPPLightingProperty) == 0x104);



__declspec(naked) NiProperty* __fastcall NiAVObject::GetProperty(UInt32 propID)
{
	__asm
	{
		push	esi
		mov		esi, [ecx + 0x24]
		iterHead:
		test	esi, esi
			jz		retnNULL
			mov		ecx, [esi + 8]
			mov		esi, [esi]
			test	ecx, ecx
			jz		iterHead
			mov		eax, [ecx]
			call	dword ptr[eax + 0x8C]
			cmp		eax, edx
			jnz		iterHead
			mov		eax, ecx
			pop		esi
			retn
			retnNULL :
		xor eax, eax
			pop		esi
			retn
	}
}



__declspec(naked) void NiAVObject::Update()
{
	__asm
	{
		push	ecx
		push	0
		push	0
		push	0
		mov		edx, esp
		push	0
		push	edx
		mov		eax, [ecx]
		call	dword ptr[eax + 0xA4]
		add		esp, 0xC
		pop		ecx
		mov		ecx, [ecx + 0x18]
		test	ecx, ecx
		jz		done
		mov		eax, [ecx]
		call	dword ptr[eax + 0xFC]
		done:
		retn
	}
}



__declspec(naked) NiMatrix33* __fastcall NiMatrix33::RotationMatrix(NiVector3* rot)
{
	__asm
	{
		push	ecx
		fld		dword ptr[edx]
		fsincos
		fstp	dword ptr[esp]
		movss	xmm1, [esp]
		fstp	dword ptr[esp]
		movss	xmm0, [esp]
		fld		dword ptr[edx + 4]
		fsincos
		fstp	dword ptr[esp]
		movss	xmm3, [esp]
		fstp	dword ptr[esp]
		movss	xmm2, [esp]
		fld		dword ptr[edx + 8]
		fsincos
		fstp	dword ptr[esp]
		movss	xmm5, [esp]
		fstp	dword ptr[esp]
		movss	xmm4, [esp]
		movss	xmm6, xmm3
		mulss	xmm6, xmm5
		movss[ecx], xmm6
		movss	xmm6, xmm3
		mulss	xmm6, xmm4
		movss[ecx + 4], xmm6
		pxor	xmm6, xmm6
		subss	xmm6, xmm2
		movss[ecx + 8], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm2
		mulss	xmm6, xmm5
		movss	xmm7, xmm1
		mulss	xmm7, xmm4
		subss	xmm6, xmm7
		movss[ecx + 0xC], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm5
		movss	xmm7, xmm0
		mulss	xmm7, xmm2
		mulss	xmm7, xmm4
		addss	xmm6, xmm7
		movss[ecx + 0x10], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm3
		movss[ecx + 0x14], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm4
		movss	xmm7, xmm1
		mulss	xmm7, xmm2
		mulss	xmm7, xmm5
		addss	xmm6, xmm7
		movss[ecx + 0x18], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm2
		mulss	xmm6, xmm4
		movss	xmm7, xmm0
		mulss	xmm7, xmm5
		subss	xmm6, xmm7
		movss[ecx + 0x1C], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm3
		movss[ecx + 0x20], xmm6
		mov		eax, ecx
		pop		ecx
		retn
	}
}



void __fastcall NiMatrix33::ExtractAngles(NiVector3* outAngles)
{
	double rotY = cr[0][2];
	if (abs(rotY) < 1)
	{
		outAngles->x = -atan2(-cr[1][2], cr[2][2]);
		outAngles->y = -asin(rotY);
		outAngles->z = -atan2(-cr[0][1], cr[0][0]);
	}
	else
	{
		float rotX = atan2(cr[1][0], cr[1][1]);
		if (rotY > 0)
		{
			outAngles->x = rotX;
			outAngles->y = -1.5707963F;
		}
		else
		{
			outAngles->x = -rotX;
			outAngles->y = 1.5707963F;
		}
		outAngles->z = 0;
	}
}














