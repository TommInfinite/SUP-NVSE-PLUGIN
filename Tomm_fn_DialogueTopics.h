#pragma once

DEFINE_COMMAND_PLUGIN(TopicInfoSetSaidOnce, "", 0, 2, kParams_Tomm_TopicInfoSetSaidOnce)
DEFINE_COMMAND_PLUGIN(TopicInfoAddChoice, "", 0, 3, kParams_Tomm_DialogTopicInfoAddChoice)
DEFINE_COMMAND_PLUGIN(TopicInfoHasChoice, "", 0, 2, kParams_Tomm_DialogTopicInfoHasChoice)
DEFINE_COMMAND_PLUGIN(TopicInfoSetPrompt, "", 0, 2, kParams_Tomm_TopicInfoSetPrompt)
DEFINE_COMMAND_PLUGIN(TopicInfoGetChoiceCount, "", 0, 1, kParams_Tomm_TopicInfoGetChoiceCount)
DEFINE_COMMAND_PLUGIN(TopicInfoGetAllChoices, "", 0, 1, kParams_Tomm_TopicInfoGetAllChoices)
DEFINE_COMMAND_PLUGIN(TopicSetPriority, "", 0, 2, kParams_Tomm_TopicSetPriority)
DEFINE_COMMAND_PLUGIN(TopicSetFlags, "", 0, 2, kParams_Tomm_TopicSetFlags)
DEFINE_COMMAND_PLUGIN(TopicGetType, "", 0, 1, kParams_Tomm_OneForm)
DEFINE_COMMAND_PLUGIN(TopicGetAllTopicInfos, "", 0, 2, kParams_Tomm_TopicGetAllTopicInfos)






bool Cmd_TopicInfoSetSaidOnce_Execute(COMMAND_ARGS)
{

	TESTopicInfo* topicInfo = NULL;
	int iValue = 0;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topicInfo, &iValue) && IS_ID(topicInfo, TESTopicInfo))
	{
		if (NUM_ARGS > 1)
		{
			topicInfo->saidOnce = iValue;
		}
		else {

			*result = topicInfo->saidOnce;

		}
	}

	return true;
}


bool Cmd_TopicInfoAddChoice_Execute(COMMAND_ARGS)
{

	TESTopicInfo* topicInfo = NULL;
	TESTopic* topicToAdd = NULL;
	int iKey = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topicInfo, &topicToAdd, &iKey) && IS_ID(topicInfo, TESTopicInfo) && IS_ID(topicToAdd, TESTopic))
	{

		TESTopicInfo::RelatedTopics* relatedTopics = topicInfo->relatedTopics;
		tList<TESTopic>	choices = relatedTopics->choices;

		if (iKey == -1)
		{
			choices.Append(topicToAdd);
		}
		else {
			choices.AddAt(topicToAdd, iKey);
		}
	}

	return true;
}



bool Cmd_TopicInfoHasChoice_Execute(COMMAND_ARGS)
{

	TESTopicInfo* topicInfo = NULL;
	TESTopic* topicToCheck = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topicInfo, &topicToCheck) && IS_ID(topicInfo, TESTopicInfo) && IS_ID(topicToCheck, TESTopic))
	{

		TESTopicInfo::RelatedTopics* relatedTopics = topicInfo->relatedTopics;
		tList<TESTopic>	choices = relatedTopics->choices;

		*result = choices.IsInList(topicToCheck);

	}

	return true;
}



bool Cmd_TopicInfoSetPrompt_Execute(COMMAND_ARGS)
{

	TESTopicInfo* topicInfo = NULL;
	char sPrompt[0x4000]{};

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topicInfo, &sPrompt) && IS_ID(topicInfo, TESTopicInfo))
	{
		if (NUM_ARGS > 1)
		{

			String	promptRes;
			promptRes.Set(sPrompt);
			topicInfo->prompt = promptRes;
		}
		else {
			//Console_Print("Prompt from plugin is %s", topicInfo->prompt.CStr());
			StrIfc->Assign(PASS_COMMAND_ARGS, topicInfo->prompt.CStr());
		}
	}

	return true;
}



bool Cmd_TopicInfoGetChoiceCount_Execute(COMMAND_ARGS)
{

	TESTopicInfo* topicInfo = NULL;


	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topicInfo) && IS_ID(topicInfo, TESTopicInfo))
	{
		TESTopicInfo::RelatedTopics* relatedTopics = topicInfo->relatedTopics;
		tList<TESTopic>	choices = relatedTopics->choices;

		*result = choices.Count();
	}

	return true;
}


bool Cmd_TopicInfoGetAllChoices_Execute(COMMAND_ARGS)
{

	TESTopicInfo* topicInfo = NULL;
	NVSEArrayVar* ResArr = ArrIfc->CreateArray(NULL, 0, scriptObj);



	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topicInfo) && IS_ID(topicInfo, TESTopicInfo))
	{
		TESTopicInfo::RelatedTopics* relatedTopics = topicInfo->relatedTopics;
		tList<TESTopic>	choices = relatedTopics->choices;

		auto iter = choices.Head();
		int iCount = 0;
		do
		{
			ArrIfc->AppendElement(ResArr, iter->data);
			iCount += 1;

		} while (iter = iter->next);

	}
	ArrIfc->AssignCommandResult(ResArr, result);
	return true;
}







bool Cmd_TopicSetPriority_Execute(COMMAND_ARGS)
{

	TESTopic* topic = NULL;
	int iValue= 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topic, &iValue) && IS_ID(topic, TESTopic))
	{
		if (NUM_ARGS > 1)
		{
			topic->priority = iValue;
		}
		else {
			*result = topic->priority;
		}
	}

	return true;
}



bool Cmd_TopicSetFlags_Execute(COMMAND_ARGS)
{

	TESTopic* topic = NULL;
	int iValue = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topic, &iValue) && IS_ID(topic, TESTopic))
	{
		if (NUM_ARGS > 1)
		{
			topic->topicFlags = iValue;
		}
		else {
			*result = topic->topicFlags;

		}

	}

	return true;
}

bool Cmd_TopicGetType_Execute(COMMAND_ARGS)
{

	TESTopic* topic = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topic) && IS_ID(topic, TESTopic))
	{
		*result = topic->topicType;
	}

	return true;
}


bool Cmd_TopicGetAllTopicInfos_Execute(COMMAND_ARGS) // Damn these arrays are messy.
{
	NVSEArrayVar* ResArr = ArrIfc->CreateArray(NULL, 0, scriptObj);
	TESTopic* topic = NULL;
	TopicInfoArray	infoArray;
	TESTopicInfo** CurrInfoTopic;
	int  iKey2 = 0, iQuestCheck = 0;

	TESQuest* ChosenQuest = NULL;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topic, &ChosenQuest) && IS_ID(topic, TESTopic))
	{
		//_MESSAGE("Topic Name is %s", topic->fullName.name.CStr());

		if (NUM_ARGS > 1)
		{
			if (IS_ID(ChosenQuest, TESQuest))
			{
				iQuestCheck = 1;
			}
		}



		tList<TESTopic::Info>	infos = topic->infos;
	
		auto iter = infos.Head();
		TESTopic::Info* TopicInfo;
		do
		{
			//_MESSAGE("Quest name is %s", iter->data->quest->fullName.name.CStr());

			if (iQuestCheck)
			{
				if (iter->data->quest = ChosenQuest)
				{
					infoArray = iter->data->infoArray;
					CurrInfoTopic = infoArray.data;
					iKey2 = -1;
					while (infoArray.numObjs > (iKey2 += 1))
					{
						ArrIfc->AppendElement(ResArr, CurrInfoTopic[iKey2]);
					}
					break;
				}

			}
			else {
				infoArray = iter->data->infoArray;
				CurrInfoTopic = infoArray.data;
				iKey2 = -1;
				while (infoArray.numObjs > (iKey2 += 1))
				{
					ArrIfc->AppendElement(ResArr, CurrInfoTopic[iKey2]);
				}
			}

		} while (iter = iter->next);

	}

	ArrIfc->AssignCommandResult(ResArr, result);

	return true;
}

//bool Cmd_TopicInfoDeleteChoiceNth_Execute(COMMAND_ARGS)
//{
//
//	TESTopicInfo* topicInfo;
//	int iKey = 0;
//	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topicInfo, &iKey) && IS_ID(topicInfo, TESTopicInfo))
//	{
//
//		TESTopicInfo::RelatedTopics* relatedTopics = topicInfo->relatedTopics;
//		tList<TESTopic>	choices = relatedTopics->choices;
//		//choices.DeleteAll();
//
//	}
//
//	return true;
//}
//bool Cmd_TopicInfoDeleteAllChoices_Execute(COMMAND_ARGS)
//{
//
//	TESTopicInfo* topicInfo;
//	int iKey = 0;
//	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topicInfo, &iKey) && IS_ID(topicInfo, TESTopicInfo))
//	{
//		TESTopicInfo::RelatedTopics* relatedTopics = topicInfo->relatedTopics;
//		tList<TESTopic>	choices = relatedTopics->choices;
//
//		auto iter = choices.Head();
//		do
//		{
//			choices.Remove(iter->data);
//		} while (iter = iter->next);
//
//		//		;
//
//
//	}
//
//	return true;
//}



//DEFINE_COMMAND_PLUGIN(TopicInfoDeleteAllChoices, "", 0, 1, kParams_Tomm_TopicInfoDeleteAllChoices)
//DEFINE_COMMAND_PLUGIN(TopicInfoDeleteChoiceNth, "", 0, 2, kParams_Tomm_TopicInfoDeleteChoiceNth)

//bool Cmd_TopicInfoSetSaidOnce_Execute(COMMAND_ARGS)
//{
//
//	TESTopicInfo* topicInfo;
//	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topicInfo))
//	{
//
//		TESTopicInfo::RelatedTopics* relatedTopics = topicInfo->relatedTopics;
//
//		tList<TESTopic>	choices = relatedTopics->choices;
//
//		auto iter = choices.Head();
//		int iCount = 0;
//		do
//		{
//			Console_Print("Count is %d ", iCount);
//			iCount += 1;
//
//		} while (iter = iter->next);
//
//		//*result = topicInfo->saidOnce;
//		//topicInfo->saidOnce = 1;
//
//	}
//
//	return true;
//}

//bool Cmd_DialogTopicSetSaidOnce_Execute(COMMAND_ARGS)
//{
//
//	TESTopic* topic;
//	if (ExtractArgsEx(EXTRACT_ARGS_EX, &topic))
//	{
//
//		//*result = topic->priority;
//		Console_Print("Name is %s", topic->fullName.name.CStr());
//
//		tList<TESTopic::Info>	infos = topic->infos;
//
//		auto iter = infos.Head();
//		TESTopic::Info* TopicInfo;
//		do
//		{
//			TopicInfoArray	infoArray;
//			infoArray = TopicInfo->infoArray;
//
//			TESTopicInfo** cells = infoArray.data;
//			for (UInt32 count = infoArray.Length(); count; count--, cells++)
//			{
//				//*cells->saidOnce;
//				Console_Print("Count is %d", count);
//			}
//			//Console_Print("NUM obj is %d", infoArray.numObjs);
//			//auto iterTopicInfo = infoArray.numObjs;
//		} while (iter = iter->next);
//		//bool eval;
//		//TESTopicInfo* topicInfo = GetTopicInfo(topic, &eval, g_ThePlayer, g_ThePlayer, 1, NULL, NULL);
//		//if (topicInfo)
//		//{
//		//	*result = topicInfo->saidOnce;
//		//}
//		//else {
//		//	Console_Print("Topic info doesnt exist");
//
//		//}
//
//	}
//
//	return true;
//}