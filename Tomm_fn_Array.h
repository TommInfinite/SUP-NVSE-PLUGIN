#pragma once

DEFINE_COMMAND_PLUGIN(Ar_GetRandomKey, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(Ar_GetRandomKeyMap, "", 0, 1, kParams_Tomm_OneInt)
DEFINE_COMMAND_PLUGIN(Ar_SetFloatsFrom, "", 0, 11, kParams_Tomm_Ar_SetFloatsFrom)
DEFINE_COMMAND_PLUGIN(Ar_HasInvalidRefs, "", 0, 2, kParams_Tomm_OneInt_OneInt_Optional)


bool Cmd_Ar_HasInvalidRefs_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 arrID;
	int iMDArrayNumber;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &arrID, &iMDArrayNumber)) return true;

	NVSEArrayVar* inArr = ArrIfc->LookupArrayByID(arrID);
	NVSEArrayVar* MD_Arr;
	if (!inArr) return true;
	NVSEArrayElement ElementArray;
	NVSEArrayElement* elements;

	UInt32 size;
	if (NUM_ARGS > 1)
	{
		ArrIfc->GetElement(inArr, iMDArrayNumber, ElementArray);
		//Console_Print("MDArray type is >>> %d", ElementArray.GetType());
		if (ElementArray.GetType() == 4)
		{
			MD_Arr = ElementArray.Array();
			size = ArrIfc->GetArraySize(MD_Arr);
			inArr = MD_Arr;
		}else{return true;}
	} else{size = ArrIfc->GetArraySize(inArr);}

	//Console_Print("Array size is  is >>> %d", size);

	if (size < 1)
	{return true;}

	elements = new NVSEArrayElement[size];
	ArrIfc->GetElements(inArr, elements, NULL);
	TESForm* CheckRef = NULL;

	for (int i = 0; i < size; i++) {
		if (elements[i].GetType() == 2)
		{
			CheckRef = elements[i].Form();
			if (CheckRef){}else {*result = 1;break;}
		}
	}
	delete[] elements;
	return true;
}



bool Cmd_Ar_SetFloatsFrom_Execute(COMMAND_ARGS)
{
	UInt32 arrID;

	ScriptVar* V0, * V1 = NULL, * V2 = NULL, * V3 = NULL, * V4 = NULL, * V5 = NULL, * V6 = NULL, * V7 = NULL, * V8 = NULL, * V9 = NULL;

	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &arrID, &V0, &V1, &V2, &V3, &V4, &V5, &V6, &V7, &V8, &V9)) return true;

	NVSEArrayVar* inArr = ArrIfc->LookupArrayByID(arrID);
	if (!inArr) return true;
	UInt32 size = ArrIfc->GetArraySize(inArr);
	NVSEArrayElement* elements = new NVSEArrayElement[size];
	ArrIfc->GetElements(inArr, elements, NULL);
	int iNeededVars = NUM_ARGS - 1;
	//Console_Print("%s %d", "Array size is>>>>", size);
	//Console_Print("%s %d", "iNeededVars>>>>", iNeededVars);
	UInt8	iType;

	for (int i = 0; i < size; i++) {

		if (i > iNeededVars)
		{
			//Console_Print("%s %d,value ENDING", "ArrayEntry>>", i);
			break;
		}

		iType = elements[i].GetType();

		if (iType == 1)
		{
			//Console_Print("%s %d,value is>>> %f", "ArrayEntry>>", i, elements[i].Number());
			if (i == 0) { if (V0)V0->data.num = elements[i].Number(); }
			else if (i == 1) { if (V1)V1->data.num = elements[i].Number(); }
			else if (i == 2) { if (V2)V2->data.num = elements[i].Number(); }
			else if (i == 3) { if (V3)V3->data.num = elements[i].Number(); }
			else if (i == 4) { if (V4)V4->data.num = elements[i].Number(); }
			else if (i == 5) { if (V5)V5->data.num = elements[i].Number(); }
			else if (i == 6) { if (V6)V6->data.num = elements[i].Number(); }
			else if (i == 7) { if (V7)V7->data.num = elements[i].Number(); }
			else if (i == 8) { if (V8)V8->data.num = elements[i].Number(); }
			else if (i == 9) { if (V9)V9->data.num = elements[i].Number(); }
		}
		else {
			//Console_Print("%s %d,value is not a number", "ArrayEntry>>", i);
		}
	}
	delete[] elements;
	*result = 0;
	return true;
}









bool Cmd_Ar_GetRandomKey_Execute(COMMAND_ARGS)
{
	UInt32 arrID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &arrID)) return true;
	NVSEArrayVar* inArr = ArrIfc->LookupArrayByID(arrID);
	if (!inArr) return true;
	UInt32 iSize = ArrIfc->GetArraySize(inArr);
	if (iSize > 0)
	{
		*result = GetRandomIntInRange(0, iSize);
	}
	return true;
}


bool Cmd_Ar_GetRandomKeyMap_Execute(COMMAND_ARGS)
{
	UInt32 arrID;
	int iChosenKeyNum;
	char Cresult[0x4000];
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &arrID)) return true;

	NVSEArrayVar* inArr = ArrIfc->LookupArrayByID(arrID);
	if (!inArr) return true;
	UInt32 size = ArrIfc->GetArraySize(inArr);

	iChosenKeyNum = GetRandomIntInRange(0, size);
	NVSEArrayElement* elements = new NVSEArrayElement[size];
	NVSEArrayElement* keys = new NVSEArrayElement[size];

	ArrIfc->GetElements(inArr, elements, keys);

	UInt8	iType;

	if (size > 0)
	{
		for (int i = 0; i < size; i++) {


			//Console_Print("type >>> %d", iType);

			if (i == iChosenKeyNum) 
			{
				iType = elements[i].GetType();

				if (iType == 3) //String
				{
					//Console_Print("%s %d,value is>>> %s", "ArrayEntry>>", i, keys[i].str);
					sprintf(Cresult, "%s", keys[i].str);
				}
				else if (iType == 1) //Number
				{
					//Console_Print("%s %d,value is>>> %f", "ArrayEntry>>", i, keys[i].num);
					sprintf(Cresult, "%f", keys[i].num);
				}
				//Console_Print("SRESULT IS >>> %s", Cresult);
			}
		}
	}

	delete[] keys;
	delete[] elements;
	if (Cresult)
	{
		StrIfc->Assign(PASS_COMMAND_ARGS, Cresult);
	}
	return true;
}

