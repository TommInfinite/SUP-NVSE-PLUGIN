#pragma once
DEFINE_COMMAND_PLUGIN(ReadINISectionsFromFile, "", 0, 1, kParams_Tomm_OneString)
DEFINE_COMMAND_PLUGIN(ReadINISectionKeysFromFile, "", 0, 2, kParams_Tomm_TwoStrings)
DEFINE_COMMAND_PLUGIN(ReadINIFloatFromFile, "", 0, 3, kParams_Tomm_ThreeStrings)
DEFINE_COMMAND_PLUGIN(ReadINIFloatFromFileAlt, "", 0, 3, kParams_Tomm_ThreeStrings)



bool Cmd_ReadINIFloatFromFile_Execute(COMMAND_ARGS)
{
	alignas(16)
		char s_FilePath[0x4000];
	char s_IniSection[0x4000];
	char s_IniValue[0x4000];

	float fNAN = 0;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath, &s_IniSection, s_IniValue))
	{
		CSimpleIniA ini;
		ini.SetUnicode();
		auto errVal = ini.LoadFile(s_FilePath);
		if (errVal == SI_Error::SI_FILE) {
			*result = fNAN;
			return true;
		};
		float fValue = ini.GetLongValue(s_IniSection, s_IniValue);
		*result = fValue;
	}
	return true;
}



bool Cmd_ReadINIFloatFromFileAlt_Execute(COMMAND_ARGS)
{
	alignas(16)
		char s_FilePath[0x4000];
	char s_IniSection[0x4000];
	char s_IniValue[0x4000];

	float fNAN = std::numeric_limits<float>::quiet_NaN();

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath, &s_IniSection, s_IniValue))
	{
		CSimpleIniA ini;
		ini.SetUnicode();
		auto errVal = ini.LoadFile(s_FilePath);
		if (errVal == SI_Error::SI_FILE) {
			*result = fNAN;
			return true;
		};
		float fValue = ini.GetLongValueAlt(s_IniSection, s_IniValue);
		*result = fValue;
	}
	return true;
}

bool Cmd_ReadINIStringFromFile_Execute(COMMAND_ARGS)
{
	alignas(16)
		char s_FilePath[0x4000];
	char s_IniSection[0x4000];
	char s_IniValue[0x4000];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath, &s_IniSection, s_IniValue))
	{
		CSimpleIniA ini;
		ini.SetUnicode();
		auto errVal = ini.LoadFile(s_FilePath);
		if (errVal == SI_Error::SI_FILE) { return true; };
		const char* sResult = ini.GetValue(s_IniSection, s_IniValue);
		StrIfc->Assign(PASS_COMMAND_ARGS, sResult);
	}
	return true;
}


bool Cmd_ReadINISectionsFromFile_Execute(COMMAND_ARGS)
{
	alignas(16) char s_FilePath[0x4000];
	NVSEArrayVar* Array_result = ArrIfc->CreateArray(NULL, 0, scriptObj);

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath))
	{
		CSimpleIniA ini;
		ini.SetUnicode();
		auto errVal = ini.LoadFile(s_FilePath);
		if (errVal == SI_Error::SI_FILE) { return true; };

		CSimpleIniA::TNamesDepend sections;
		ini.GetAllSections(sections);

		CSimpleIniA::TNamesDepend::const_iterator it;
		int i;
		for (i = 0, it = sections.begin(); it != sections.end(); ++i, ++it) {
			ArrIfc->AppendElement(Array_result, NVSEArrayElement(it->pItem));
		}
	}
	ArrIfc->AssignCommandResult(Array_result, result);
	return true;
}



bool Cmd_ReadINISectionKeysFromFile_Execute(COMMAND_ARGS)
{
	alignas(16)
		char s_FilePath[0x4000];
	char s_Section[0x4000];
	NVSEArrayVar* Array_result = ArrIfc->CreateArray(NULL, 0, scriptObj);

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_FilePath, &s_Section))
	{
		CSimpleIniA ini;
		ini.SetUnicode();
		auto errVal = ini.LoadFile(s_FilePath);
		if (errVal == SI_Error::SI_FILE) { return true; };

		CSimpleIniA::TNamesDepend keys;
		ini.GetAllKeys(s_Section, keys);

		CSimpleIniA::TNamesDepend::const_iterator it;
		int i;
		for (i = 0, it = keys.begin(); it != keys.end(); ++i, ++it) {
			ArrIfc->AppendElement(Array_result, NVSEArrayElement(it->pItem));
		}
	}
	ArrIfc->AssignCommandResult(Array_result, result);
	return true;
}