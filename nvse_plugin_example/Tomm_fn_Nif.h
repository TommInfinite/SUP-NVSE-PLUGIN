DEFINE_COMMAND_PLUGIN(GetNifBlockTranslationToVars, "", 1, 6, kParams_Tomm_GetNifBlockTranslationToVars)
DEFINE_COMMAND_PLUGIN(GetNifBlockNumVerticles, "", 1, 2, kParams_Tomm_GetNifBlockNumVerticles)
DEFINE_COMMAND_PLUGIN(CompareNifBlocks, "", 1, 2, kParams_Tomm_TwoStrings)
DEFINE_COMMAND_PLUGIN(NifBlockInterpolateScale, "", 1, 7, kParams_Tomm_NifBlockInterpolateScale)
DEFINE_COMMAND_PLUGIN(NifBlockInterpolateRotation, "", 1, 11, kParams_Tomm_NifBlockInterpolateRotation)
DEFINE_COMMAND_PLUGIN(NifBlockInterpolateTranslation, "", 1, 12, kParams_Tomm_NifBlockInterpolateTranslation)
DEFINE_COMMAND_PLUGIN(IsNifBlockInterpolated, "", 1, 3, kParams_Tomm_IsNifBlockInterpolated)
DEFINE_COMMAND_PLUGIN(NifBlockInterpolateStop, "", 1, 3, kParams_Tomm_NifBlockInterpolateStop)
DEFINE_COMMAND_PLUGIN(SetSUPOnInterpolateFinishEvent, "", 0, 2, kParams_Tomm_SetSUPOnInterpolateFinishEvent)
DEFINE_COMMAND_PLUGIN(RemoveSUPOnInterpolateFinishEvent, "", 0, 1, kParams_Tomm_OneForm)
DEFINE_COMMAND_PLUGIN(NifBlockInterpolateSetTrait, "", 1, 5, kParams_Tomm_NifBlockInterpolateSetTrait)




#define NifBlockIter1 (*block1)
#define NifBlockIter2 (*block2)


typedef NiTArray<NiAVObject*> NiAVObjectArray;
#define IS_TYPE(objPtr, className) (*(UInt32*)objPtr == kVtbl_##className)
#define IS_NODE(object) ((*(UInt32**)object)[3] == 0x6815C0)



__declspec(naked) UInt32 NiAVObject::GetIndex() 
{
	__asm
	{
		mov		eax, [ecx + 0x18]
		test	eax, eax
		jz		done
		mov		edx, [eax + 0xA0]
		movzx	eax, word ptr[eax + 0xA6]
		test	eax, eax
		jz		done
		ALIGN 16
		iterHead:
		cmp[edx + eax * 4 - 4], ecx
			jz		done
			dec		eax
			jnz		iterHead
			done :
		retn
	}
}


bool Cmd_GetNifBlockTranslationToVars_Execute(COMMAND_ARGS)
{
	*result = 0;
	char blockName[0x40];
	UInt32 getWorld = 0, pcNode = 0;
	ScriptVar* outX = NULL, * outY = NULL, * outZ = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &outX, &outY, &outZ, &blockName, &getWorld, &pcNode))
	{
		*result = -1;
		NiAVObject* niBlock = GetNifBlock(thisObj, pcNode, blockName);
		if (niBlock)
		{
			*result = 1;
			if (getWorld)
			{
				NiVector3& transltn = niBlock->m_worldTranslate;
				outX->data.num = transltn.x;
				outY->data.num = transltn.y;
				outZ->data.num = transltn.z;
			}
			else
			{
				NiVector3& transltn = niBlock->m_localTranslate;
				outX->data.num = transltn.x;
				outY->data.num = transltn.y;
				outZ->data.num = transltn.z;
			}
		}
	}
	return true;
}



bool fn_VerticlesAreDifferent(NiAVObject* niBlock1, NiAVObject* niBlock2)
{
	int iNumVerticles1 = -100;
	int iNumVerticles2 = -105;

	NiGeometry* geom1 = NULL;
	NiGeometryData* geomdata1 = NULL;
	NiGeometry* geom2 = NULL;
	NiGeometryData* geomdata2 = NULL;

	NiVector3* vertices1 = NULL;
	NiVector3* vertices2 = NULL;

	geom1 = niBlock1->GetNiGeometry();

	if (geom1)
	{
		geomdata1 = geom1->geometryData;
		if (geomdata1)
		{
			iNumVerticles1 = geomdata1->numVertices;
		}
	}

	geom2 = niBlock2->GetNiGeometry();

	if (geom2)
	{
		geomdata2 = geom2->geometryData;
		if (geomdata2)
		{
			iNumVerticles2 = geomdata2->numVertices;
		}
	}

	if (iNumVerticles1 != iNumVerticles2)
	{
		return 1;
	}
	else {
		if (iNumVerticles1 > 0 && iNumVerticles1 > 0)
		{
			vertices1 = geomdata1->vertices;
			vertices2 = geomdata2->vertices;

			if (vertices1 && vertices2 && (vertices1[0].x == vertices2[0].x && vertices1[0].y == vertices2[0].y && vertices1[0].z == vertices2[0].z))
			{
				_MESSAGE("VERTICALS SAME 1");
				return 0;

			}
			else {
				_MESSAGE("DIFFERENT VERTICLES");
				return 1;
			}
		}
	}

	_MESSAGE("VERTICALS SAME 2");
	return 0;

}




String* fn_GetNifBlockTextures(NiAVObject* niBlock)
{
	
	if (niBlock->GetTriBasedGeom())
	{
		BSShaderPPLightingProperty* shaderProp = (BSShaderPPLightingProperty*)niBlock->GetProperty(3);

		if (shaderProp && IS_TYPE(shaderProp, BSShaderPPLightingProperty) && shaderProp->textureSet)
		{
			return shaderProp->textureSet->textures;
		}
		else {
			return NULL;
		}

	}
	else {
		return NULL;
	}

}


UInt16 fn_GetNifBlockNumVerticles(NiAVObject* niBlock)
{

	NiGeometry* geom = NULL;

	geom = niBlock->GetNiGeometry();

	if (geom)
	{
		NiGeometryData* geomdata = NULL;
		geomdata = geom->geometryData;

		if (geomdata)
		{
			return geomdata->numVertices;
		}
		else {
			return -2;
		}

	}
	else {
		return -2;
	}

}

bool Cmd_GetNifBlockNumVerticles_Execute(COMMAND_ARGS)
{
	*result = 0;
	char blockName[0x40];
	int PCNode = 0;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &PCNode))
	{
		*result = -1;
		NiAVObject* niBlock = GetNifBlock(thisObj, PCNode, blockName);
		if (niBlock)
		{
			*result = fn_GetNifBlockNumVerticles(niBlock);
		}
	}
	return true;
}



vector<NiAVObject*> Ar_Blocks1;
vector<NiAVObject*> Ar_Blocks2;

vector<NiAVObject*>  __fastcall  GetChildBlocks(NiNode* node, vector<NiAVObject*> &OutAr)
{
	NiAVObjectArray ChildArray = node->m_children;
	NiAVObject** blockCurrent = NULL;
	blockCurrent = ChildArray.data;

	int iKey2 = -1;
	_MESSAGE("NUM CHILDS::%d", ChildArray.numObjs);


	while (ChildArray.numObjs > (iKey2 += 1))
	{
		if (IS_NODE(blockCurrent[iKey2]))
		{
			GetChildBlocks((NiNode*)blockCurrent[iKey2], OutAr);
		}
		else {
			OutAr.push_back(blockCurrent[iKey2]);
		}

	}
	_MESSAGE("OutAr SIZE::%d", OutAr.size());
	return OutAr;

}






bool Cmd_CompareNifBlocks_Execute(COMMAND_ARGS)
{
	*result = 0;
	char blockName1[0x40];
	char blockName2[0x40];

	
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName1, &blockName2))
	{

		NVSEArrayVar* ResArr = ArrIfc->CreateArray(NULL, 0, scriptObj);
		Ar_Blocks1.clear();
		Ar_Blocks2.clear();

		NiNode* objNode = (NiNode*)GetNifBlock(thisObj, 0, blockName1);
		if (objNode && IS_NODE(objNode))
		{
			_MESSAGE("Block1 exists");
			Ar_Blocks1 = GetChildBlocks(objNode, Ar_Blocks1);
		}


		objNode = NULL;
		objNode = (NiNode*)GetNifBlock(thisObj, 0, blockName2);
		if (objNode && IS_NODE(objNode))
		{
			_MESSAGE("Block2 exists");
			Ar_Blocks2 = GetChildBlocks(objNode, Ar_Blocks2);
		}


			_MESSAGE("Block 1 array SIZE::%d", Ar_Blocks1.size());
			_MESSAGE("Block 2 array SIZE::%d", Ar_Blocks2.size());


		std::vector<NiAVObject*>::iterator block1;
		std::vector<NiAVObject*>::iterator block2;

		int iFound = 0;
		int iAdded = 0;
		String* Text1 = NULL, * Text2 = NULL;

		for (block2 = Ar_Blocks2.begin(); block2 != Ar_Blocks2.end();)
		{

			iFound = 0;

			for (block1 = Ar_Blocks1.begin(); block1 != Ar_Blocks1.end();)
			{
				if (0 == stricmp(NifBlockIter1->GetName(), NifBlockIter2->GetName()))
				{
					iFound = 1;
					iAdded = 0;
					_MESSAGE("NAME MATCH::%s", NifBlockIter1->GetName());
					if (fn_VerticlesAreDifferent(NifBlockIter1, NifBlockIter2) == 1)
					{
						//_MESSAGE("NUM VERTICLES ARE DIFFERENT::%s", NifBlockIter1->GetName());
						ArrIfc->AppendElement(ResArr, NifBlockIter2->GetName());
						iAdded = 1;
					}

					if (iAdded == 0)
					{
						Text1 = fn_GetNifBlockTextures(NifBlockIter1);
						Text2 = fn_GetNifBlockTextures(NifBlockIter2);

						if  (Text1 && Text2 && (0 != (Text1[0].Compare(Text2[0]))))
						{
							_MESSAGE("TEXTURES ARE DIFFERENT::%s", NifBlockIter1->GetName());
							ArrIfc->AppendElement(ResArr, NifBlockIter2->GetName());
						}
					}
				}

				++block1;
			}

			if (iFound == 0)
			{
				ArrIfc->AppendElement(ResArr, NifBlockIter2->GetName());
			}

			++block2;
		}

		ArrIfc->AssignCommandResult(ResArr, result);
	}

	return true;
}








bool Cmd_NifBlockInterpolateScale_Execute(COMMAND_ARGS)
{

	*result = 0;
	char blockName[0x40];
	int mode = 0, PCNode = 0, iCalculateStartValue = 0;
	float StartValue = 0, EndValue = 0, timer = 0;
	int modIdx = scriptObj->GetOverridingModIdx();

	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &StartValue, &EndValue, &timer, &mode, &PCNode, &iCalculateStartValue)) return true;

	if (thisObj == g_ThePlayer)
	{
		PCNode += 1;
		if ((PCNode < 1) || (PCNode > 2))
		{
			PCNode = 1;
		}
	}else {
		PCNode = 0;

	}

	NiAVObject* niBlock = GetNifBlock(thisObj, PCNode, blockName);
	if (!niBlock) return true;


	if (fn_NifInterpolator_AlreadyExists(thisObj, niBlock, k_NifInterTypeScale, PCNode) == 1)
	{
		_MESSAGE("Block already exists in array::%s, type::%d", blockName, k_NifInterTypeScale);
		*result = -1;
		return true;
	}


	NifInterpolator NewEntry;
	

	sprintf(NewEntry.blockName, "%s", blockName);


	if (iCalculateStartValue)
	{
		NewEntry.StartValue = niBlock->m_localScale;
	}else {
		NewEntry.StartValue = StartValue;
	}

	NewEntry.thisObj = thisObj;
	NewEntry.niBlock = niBlock;
	NewEntry.mode = mode;
	NewEntry.CurValue = NewEntry.StartValue;
	NewEntry.EndValue = EndValue;
	NewEntry.timer = timer;
	NewEntry.delta = (EndValue - NewEntry.StartValue) / (timer * 1000);
	NewEntry.type = k_NifInterTypeScale;
	NewEntry.modIdx = modIdx;

	if (thisObj == g_ThePlayer)
	{
		NewEntry.PCNode = PCNode;
	}






	NewEntry.ObjectRefID = thisObj->refID;
	
	NewEntry.ID = g_NifInterpolatorArrayID;
	g_NifInterpolatorArrayID++;


	if (g_InterpolTimerStarted == 0)
	{
		g_InterpolTimerStarted = 1;
		g_InterpolTimer.start();
		g_InterpolTimeElapsed = g_InterpolTimer.get_elapsed_ms();
	}

	g_NifInterpolatorArrayV.push_back(NewEntry);

	if (g_NifInterpolatorArrayIterate == 0)
	{
		g_InterpolTimeElapsed = g_InterpolTimer.get_elapsed_ms();
	}

	g_NifInterpolatorArrayIterate = 1;



	_MESSAGE("Adding new NIF Interpolator with type::%d, mode::%d, ID::%d, name::%s", NewEntry.type, NewEntry.mode, g_NifInterpolatorArrayID, niBlock->GetName());
	*result = 1;
	return true;
}



bool Cmd_NifBlockInterpolateRotation_Execute(COMMAND_ARGS)
{

	*result = 0;
	char blockName[0x40];
	int mode = 0, PCNode = 0, iCalculateStartValue = 0;;
	float timer = 0, StartX = 0, StartY = 0, StartZ = 0, EndX = 0, EndY = 0, EndZ = 0;
	int modIdx = scriptObj->GetOverridingModIdx();

	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &StartX, &StartY, &StartZ, &EndX, &EndY, &EndZ, &timer, &mode, &PCNode, &iCalculateStartValue)) return true;

	if (thisObj == g_ThePlayer)
	{
		PCNode += 1;
		if ((PCNode < 1) || (PCNode > 2))
		{
			PCNode = 1;
		}
	}else {
		PCNode = 0;
	}

	NiAVObject* niBlock = GetNifBlock(thisObj, PCNode, blockName);
	if (!niBlock) return true;






	if (fn_NifInterpolator_AlreadyExists(thisObj, niBlock, k_NifInterTypeRotation, PCNode) == 1)
	{
		*result = -1;
		return true;
	}


	NifInterpolator NewEntry;

	NewEntry.thisObj = thisObj;
	NewEntry.niBlock = niBlock;
	NewEntry.mode = mode;
	NewEntry.timer = timer;
	NewEntry.timerMS = timer * 1000;

	if (thisObj == g_ThePlayer)
		NewEntry.PCNode = PCNode;

	


	if (iCalculateStartValue)
	{
		NiVector3 rot;
		NiMatrix33& rotMat = niBlock->m_localRotate;
		rotMat.ExtractAngles(&rot);

		NewEntry.StartX = rot.x * kDbl180dPI;
		NewEntry.StartY = rot.y * kDbl180dPI;
		NewEntry.StartZ = rot.z * kDbl180dPI;
	}
	else {
		NewEntry.StartX = StartX;
		NewEntry.StartY = StartY;
		NewEntry.StartZ = StartZ;
	}





	NewEntry.EndX = EndX;
	NewEntry.EndY = EndY;
	NewEntry.EndZ = EndZ;

	NewEntry.deltaX = (EndX - NewEntry.StartX)/100;
	NewEntry.deltaY = (EndY - NewEntry.StartY) / 100;
	NewEntry.deltaZ = (EndZ - NewEntry.StartZ) / 100;
	NewEntry.ObjectRefID = thisObj->refID;
	


	NewEntry.type = k_NifInterTypeRotation;
	NewEntry.modIdx = modIdx;

	NewEntry.ID = g_NifInterpolatorArrayID;
	g_NifInterpolatorArrayID++;


	if (g_InterpolTimerStarted == 0)
	{
		g_InterpolTimerStarted = 1;
		g_InterpolTimer.start();
		g_InterpolTimeElapsed = g_InterpolTimer.get_elapsed_ms();
	}

	g_NifInterpolatorArrayV.push_back(NewEntry);


	if (g_NifInterpolatorArrayIterate == 0)
	{
		g_InterpolTimeElapsed = g_InterpolTimer.get_elapsed_ms();
	}

	g_NifInterpolatorArrayIterate = 1;
	_MESSAGE("Adding new NIF Interpolator with type::%d, mode::%d, ID::%d, name::%s", NewEntry.type, NewEntry.mode, g_NifInterpolatorArrayID, niBlock->GetName());
	*result = 1;
	return true;
}

bool Cmd_NifBlockInterpolateTranslation_Execute(COMMAND_ARGS)
{

	*result = 0;
	char blockName[0x40];
	int mode = 0, world = 0, PCNode = 0, iCalculateStartValue = 0;
	float timer = 0, StartX = 0, StartY = 0, StartZ = 0, EndX = 0, EndY = 0, EndZ = 0;
	int modIdx = scriptObj->GetOverridingModIdx();

	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &StartX, &StartY, &StartZ, &EndX, &EndY, &EndZ, &timer, &mode, &world, &PCNode, &iCalculateStartValue)) return true;

	if (thisObj == g_ThePlayer)
	{
		PCNode += 1;
		if ((PCNode < 1) || (PCNode > 2))
		{
			PCNode = 1;
		}
	}else {
		PCNode = 0;
	}


	NiAVObject* niBlock = GetNifBlock(thisObj, PCNode, blockName);
	if (!niBlock) return true;


	if (fn_NifInterpolator_AlreadyExists(thisObj, niBlock, k_NifInterTypeTranslation, PCNode) == 1)
	{
		*result = -1;
		return true;
	}


	NifInterpolator NewEntry;

	if (iCalculateStartValue)
	{

		NewEntry.StartX = StartX;
		NewEntry.StartY = StartY;
		NewEntry.StartZ = StartZ;

		if (world)
		{
			NiVector3& transltn = niBlock->m_worldTranslate;
			NewEntry.StartX = transltn.x;
			NewEntry.StartY = transltn.y;
			NewEntry.StartZ = transltn.z;
		}
		else
		{
			NiVector3& transltn = niBlock->m_localTranslate;
			NewEntry.StartX = transltn.x;
			NewEntry.StartY = transltn.y;
			NewEntry.StartZ = transltn.z;
		}

	}
	else {
		NewEntry.StartX = StartX;
		NewEntry.StartY = StartY;
		NewEntry.StartZ = StartZ;
	}



	NewEntry.thisObj = thisObj;
	NewEntry.niBlock = niBlock;
	NewEntry.mode = mode;
	NewEntry.timer = timer;
	NewEntry.timerMS = timer * 1000;



	NewEntry.EndX = EndX;
	NewEntry.EndY = EndY;
	NewEntry.EndZ = EndZ;

	NewEntry.deltaX = (EndX - NewEntry.StartX) / 100;
	NewEntry.deltaY = (EndY - NewEntry.StartY) / 100;
	NewEntry.deltaZ = (EndZ - NewEntry.StartZ) / 100;
	NewEntry.ObjectRefID = thisObj->refID;
	NewEntry.world = world;
	if (thisObj == g_ThePlayer)
		NewEntry.PCNode = PCNode;
	

	NewEntry.type = k_NifInterTypeTranslation;
	NewEntry.modIdx = modIdx;


	NewEntry.ID = g_NifInterpolatorArrayID;
	g_NifInterpolatorArrayID++;


	if (g_InterpolTimerStarted == 0)
	{
		g_InterpolTimerStarted = 1;
		g_InterpolTimer.start();
		g_InterpolTimeElapsed = g_InterpolTimer.get_elapsed_ms();
	}

	g_NifInterpolatorArrayV.push_back(NewEntry);

	if (g_NifInterpolatorArrayIterate == 0)
	{
		g_InterpolTimeElapsed = g_InterpolTimer.get_elapsed_ms();
	}

	g_NifInterpolatorArrayIterate = 1;
	_MESSAGE("Adding new NIF Interpolator with type::%d, mode::%d, ID::%d, name::%s", NewEntry.type, NewEntry.mode, g_NifInterpolatorArrayID, niBlock->GetName());
	*result = 1;
	return true;
}






bool fn_IsNifBlockInterpolated_EXECUTE(TESObjectREFR* thisObj, NiAVObject* niBlock, int type, int PCNode)
{
	vector<NifInterpolator>::iterator it;

	if (type == -1)
	{
		for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
		{

			if ((VectorIter.thisObj == thisObj) && (VectorIter.niBlock == niBlock))
			{
				return true;
			}
			++it;
		}
	}
	else if (niBlock == NULL)
	{

		for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
		{

			if ((VectorIter.thisObj == thisObj))
			{
				if (thisObj == g_ThePlayer)
				{
					if (VectorIter.PCNode == PCNode)
						return true;
				}
				else {
					return true;
				}
			}
			++it;
		}

	}
	else {

		for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
		{

			if ((VectorIter.thisObj == thisObj) && (VectorIter.niBlock == niBlock) && (VectorIter.type == type))
			{
				return true;
			}
			++it;
		}

	}

	return false;
}

bool Cmd_IsNifBlockInterpolated_Execute(COMMAND_ARGS)
{

	*result = 0;
	char blockName[0x40];
	int type = 0, PCNode = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &type, &PCNode)) return true;

	if (thisObj == g_ThePlayer)
	{
		PCNode += 1;
		if ((PCNode < 1) || (PCNode > 2))
		{
			PCNode = 1;
		}
	}


	NiAVObject* niBlock = GetNifBlock(thisObj, PCNode, blockName);
	if (!niBlock) return true;



	if (NUM_ARGS >= 2)
	{
		*result = fn_IsNifBlockInterpolated_EXECUTE(thisObj, niBlock, type, PCNode);
	}
	else if (NUM_ARGS == 1)
	{

		*result = fn_IsNifBlockInterpolated_EXECUTE(thisObj, niBlock, -1, PCNode);
	}
	else if (NUM_ARGS == 0)
	{
		*result = fn_IsNifBlockInterpolated_EXECUTE(thisObj, NULL, -1, PCNode);
	}
}




bool Cmd_NifBlockInterpolateStop_Execute(COMMAND_ARGS)
{

	*result = 0;
	char blockName[0x40];
	int type = 0, PCNode = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &type, &PCNode)) return true;

	if (thisObj == g_ThePlayer)
	{
		PCNode += 1;
		if ((PCNode < 1) || (PCNode > 2))
		{
			PCNode = 1;
		}
	}

	NiAVObject* niBlock = GetNifBlock(thisObj, PCNode, blockName);
	if (!niBlock) return true;


	*result = -1;

	vector<NifInterpolator>::iterator it;
	vector<NifInterpolator>::iterator it_temp;

	if (NUM_ARGS >= 2) // only one entry
	{

		if (fn_IsNifBlockInterpolated_EXECUTE(thisObj, niBlock, type, PCNode))
		{
			if (thisObj == g_ThePlayer)
			{

				it_temp = f_NifInterpolator_GetIter(thisObj, niBlock, type, PCNode);

				if ((*it_temp).PCNode == PCNode)
				{
					*result = 1;
					fn_NifInterpolator_Destroy(it_temp,0);
				}
			}
			else {
				*result = 1;
				fn_NifInterpolator_Destroy(f_NifInterpolator_GetIter(thisObj, niBlock, type, PCNode),0);
			}



		}

	}
	else if (NUM_ARGS == 1)
	{

		if (fn_IsNifBlockInterpolated_EXECUTE(thisObj, niBlock, -1, PCNode))
		{

			for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
			{

				if ((VectorIter.thisObj == thisObj) && (VectorIter.niBlock == niBlock))
				{

					if (thisObj == g_ThePlayer)
					{
						if (VectorIter.PCNode == PCNode)
						{
							it = fn_NifInterpolator_Destroy(it,0);
							continue;
						}

					}
					else {
						it = fn_NifInterpolator_Destroy(it,0);
						continue;
					}

				}
				++it;
			}

			*result = 1;
		}

	}
	else if (NUM_ARGS == 0)
	{
		if (fn_IsNifBlockInterpolated_EXECUTE(thisObj, NULL, -1, PCNode))
		{
			for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
			{

				if (VectorIter.thisObj == thisObj)
				{
					if (thisObj == g_ThePlayer)
					{
						if (VectorIter.PCNode == PCNode)
						{
							it = fn_NifInterpolator_Destroy(it,0);
							continue;
						}

					}
					else {
						it = fn_NifInterpolator_Destroy(it,0);
						continue;
					}
				}
				++it;
			}

			*result = 1;
		}

	}

	return true;
}


bool Cmd_SetSUPOnInterpolateFinishEvent_Execute(COMMAND_ARGS)
{
	Script* FunctionToCall = NULL;
	int modIdx = scriptObj->GetOverridingModIdx();
	int SendFromLoops = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &FunctionToCall, &SendFromLoops)) return true;

		vector<NifInterpolatorEventCaller>::iterator it;

		for (it = g_NifInterpolatorEventCallerArrayV.begin(); it != g_NifInterpolatorEventCallerArrayV.end();)
		{
			if (VectorIter.modIdx == modIdx)
			{
				VectorIter.FunctionToCall = FunctionToCall;
				VectorIter.SendFromLoops = SendFromLoops;
				*result = 1;
				return true;
			}
			++it;
		}



		NifInterpolatorEventCaller NewItem;
		NewItem.FunctionToCall = FunctionToCall;
		NewItem.modIdx = modIdx;
		NewItem.SendFromLoops = SendFromLoops;

		NewItem.ID = g_NifInterpolatorEventCallerArrayID;
		g_NifInterpolatorEventCallerArrayID += 1;


		g_NifInterpolatorEventCallerArrayV.push_back(NewItem);
		_MESSAGE("Added new Interpolator event, ID::%d,Loops::%d", g_NifInterpolatorEventCallerArrayID, SendFromLoops);

		return true;
}



bool Cmd_RemoveSUPOnInterpolateFinishEvent_Execute(COMMAND_ARGS)
{
	Script* FunctionToCall = NULL;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &FunctionToCall)) return true;

	*result = 0;
	vector<NifInterpolatorEventCaller>::iterator it;

	for (it = g_NifInterpolatorEventCallerArrayV.begin(); it != g_NifInterpolatorEventCallerArrayV.end();)
	{
		if (VectorIter.modIdx == modIdx)
		{
			g_NifInterpolatorEventCallerArrayV.erase(it);
			*result = 1;
			return true;
		}
		++it;
	}

	return true;
}






//Traits
//0 - Set Start Value on ending
bool fn_NifBlockInterpolateSetTrait_EXECUTE(TESObjectREFR* thisObj, NiAVObject* niBlock, int type, int PCNode, int iTrait, float fValue)
{
	vector<NifInterpolator>::iterator it;

	if (type == -1)
	{
		for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
		{

			if ((VectorIter.thisObj == thisObj) && (VectorIter.niBlock == niBlock))
			{
				//if (iTrait == 0)
				//{
					VectorIter.iSetStartValueOnEnd = fValue;
				//}
				return true;
			}
			++it;
		}
	}
	else if (niBlock == NULL)
	{

		for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
		{

			if ((VectorIter.thisObj == thisObj))
			{
				if (thisObj == g_ThePlayer)
				{
					if (VectorIter.PCNode == PCNode)
					{
						//if (iTrait == 0)
						//{
						VectorIter.iSetStartValueOnEnd = fValue;
						//}
						return true;
					}

				}
				else {
					//if (iTrait == 0)
					//{
					VectorIter.iSetStartValueOnEnd = fValue;
					//}
					return true;
				}
			}
			++it;
		}

	}
	else {

		for (it = g_NifInterpolatorArrayV.begin(); it != g_NifInterpolatorArrayV.end();)
		{

			if ((VectorIter.thisObj == thisObj) && (VectorIter.niBlock == niBlock) && (VectorIter.type == type))
			{
				//if (iTrait == 0)
				//{
				VectorIter.iSetStartValueOnEnd = fValue;
				//}
				return true;
			}
			++it;
		}

	}

	return false;
}

bool Cmd_NifBlockInterpolateSetTrait_Execute(COMMAND_ARGS)
{

	*result = 0;
	char blockName[0x40];
	int type = 0, PCNode = 0, iTrait = 0;
	float fValue = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &iTrait, &fValue, &type, &PCNode)) return true;

	if (thisObj == g_ThePlayer)
	{
		PCNode += 1;
		if ((PCNode < 1) || (PCNode > 2))
		{
			PCNode = 1;
		}
	}


	NiAVObject* niBlock = GetNifBlock(thisObj, PCNode, blockName);
	if (!niBlock) return true;



	if (NUM_ARGS >= 2)
	{
		*result = fn_NifBlockInterpolateSetTrait_EXECUTE(thisObj, niBlock, type, PCNode, iTrait, fValue);
	}
	else if (NUM_ARGS == 1)
	{

		*result = fn_NifBlockInterpolateSetTrait_EXECUTE(thisObj, niBlock, -1, PCNode, iTrait, fValue);
	}
	else if (NUM_ARGS == 0)
	{
		*result = fn_NifBlockInterpolateSetTrait_EXECUTE(thisObj, NULL, -1, PCNode, iTrait, fValue);
	}
}











//NiVector3* vertices = geomdata1->vertices;
//NiVector3 vert;
//int iKey = -1;
//_MESSAGE("VERTICALS_OUTPUT_STARTS");
//while (iKey < geomdata1->numVertices)
//{
//	vert = vertices[iKey];
//	_MESSAGE("VertX::%f,VertY::%f,VertZ::%f", vert.x, vert.y, vert.z);
//
//	iKey++;
//}