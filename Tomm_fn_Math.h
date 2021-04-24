#pragma once
DEFINE_COMMAND_PLUGIN(GetCalculatedPos, "Gets current PosXYZ", 1, 4, kParams_Tomm_TwoFloats_TwoScriptVars)
DEFINE_COMMAND_PLUGIN(GetHeadingAngleAlt, "", 0, 4, kParams_Tomm_ThreeFloats_OneObjectRef)
DEFINE_COMMAND_PLUGIN(GetHeadingAngleBetweenPoints, "", 0, 5, kParams_Tomm_FiveFloats)
DEFINE_COMMAND_PLUGIN(GetDistanceBetweenPoints, "", 0, 6, kParams_Tomm_SixFloats)
DEFINE_COMMAND_PLUGIN(GetCalculatedPosFrame, "", 0, 10, kParams_Tomm_SevenFloats_ThreeScriptVars)
DEFINE_COMMAND_PLUGIN(GetCalculatedPosAlt, "Gets current PosXYZ", 0, 6, kParams_Tomm_TwoFloats_TwoScriptVars_TwoFloats)
DEFINE_COMMAND_PLUGIN(RoundAlt, "", 0, 3, kParams_Tomm_OneFloat_TwoOptionalInt)
DEFINE_COMMAND_PLUGIN(Round, "", 0, 1, kParams_Tomm_OneFloat)
DEFINE_COMMAND_PLUGIN(IsNumberNAN, "", 0, 1, kParams_Tomm_OneFloatOptional)


bool Cmd_GetDistanceBetweenPoints_Execute(COMMAND_ARGS)
{
	float x1, x2, y1, y2, z1, z2;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &x1, &y1, &z1, &x2, &y2, &z2))
	{
		float d = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2) * 1.0);
		*result = d;
	}
	return true;
}



bool Cmd_IsNumberNAN_Execute(COMMAND_ARGS) //thanks Kormakur
{
	float fValue;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &fValue))
	{
		if (NUM_ARGS > 0)
		{
			if (isnan(fValue))
			{
				*result = 1;
			}
			else { *result = 0; }
		}
		else
		{
			*result = std::numeric_limits<float>::quiet_NaN();
		}
	}
	return true;
}



bool Cmd_GetHeadingAngleAlt_Execute(COMMAND_ARGS) //https://stackoverflow.com/questions/48227928/calculating-heading-angle-from-2-points
{

	float CallX, CallY, dir, TargetX, TargetY;
	TESObjectREFR* TargetRef = NULL;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &CallX, &CallY, &dir, &TargetRef))
	{
		//dir = g_ThePlayer->flycamZRot / 0.017453292519943295;

		TargetX = TargetRef->posX;
		TargetY = TargetRef->posY;

		double heading = std::atan2(TargetX - CallX, TargetY - CallY) * 180 / 3.1415926535897 - dir;
		if (heading < -180) heading += 360;
		if (heading > 180) heading -= 360;

		*result = heading;
	}

	return true;
}


bool Cmd_GetHeadingAngleBetweenPoints_Execute(COMMAND_ARGS) //https://stackoverflow.com/questions/48227928/calculating-heading-angle-from-2-points
{

	float CallX, CallY, dir, TargetX, TargetY;


	if (ExtractArgsEx(EXTRACT_ARGS_EX, &CallX, &CallY, &dir, &TargetX, &TargetY))
	{
		//dir = g_ThePlayer->flycamZRot / 0.017453292519943295;

		double heading = std::atan2(TargetX - CallX, TargetY - CallY) * 180 / 3.1415926535897 - dir;
		if (heading < -180) heading += 360;
		if (heading > 180) heading -= 360;

		*result = heading;
	}

	return true;
}


bool Cmd_RoundAlt_Execute(COMMAND_ARGS) //made by me for a change
{
	float fValue, fAdd = 0;
	UInt32 iRound = 0, iPrecision = 0, iMult;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &fValue, &iPrecision, &iRound))
	{
		if (iPrecision == 0) { iMult = 1; }
		else { iMult = pow(10, iPrecision); }

		if (iRound == 1) { fAdd = 0.5; };

		float value = (int)((fValue * iMult) + fAdd);
		*result = (float)value / iMult;
	}
	return true;
}

bool Cmd_Round_Execute(COMMAND_ARGS) //math taken from MATH
{
	float fValue;


	if (ExtractArgsEx(EXTRACT_ARGS_EX, &fValue))
	{
		*result = round(fValue);
	}
	return true;
}


bool Cmd_GetCalculatedPos_Execute(COMMAND_ARGS) //cos sin values taken from JiP

{

	ScriptVar* outX, * outY;
	float fAngle, fDistance;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &fAngle, &fDistance, &outX, &outY))
	{
		outX->data.num = ((thisObj->posX) + fDistance * (sin(fAngle * 0.017453292519943295)));
		outY->data.num = ((thisObj->posY) + fDistance * (cos(fAngle * 0.017453292519943295)));
	}

	return true;

}

bool Cmd_GetCalculatedPosAlt_Execute(COMMAND_ARGS) //cos sin values taken from JiP

{
	ScriptVar* outX, * outY;
	float fAngle, fDistance;
	float fXIn, fYIn;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &fAngle, &fDistance, &outX, &outY, &fXIn, &fYIn))
	{
		outX->data.num = (fXIn + fDistance * (sin(fAngle * 0.017453292519943295)));
		outY->data.num = (fYIn + fDistance * (cos(fAngle * 0.017453292519943295)));
	}

	return true;

}


bool Cmd_GetCalculatedPosFrame_Execute(COMMAND_ARGS) //Fallout2AM
{

	float PosX, PosY, PosZ, fEndPosX, fEndPosY, fEndPosZ, iFrame;

	float ChangeX, ChangeY, ChangeZ;

	ScriptVar* outX, * outY, * outZ;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &PosX, &PosY, &PosZ, &fEndPosX, &fEndPosY, &fEndPosZ, &iFrame, &outX, &outY, &outZ))
	{

		if (PosX > 0)
		{
			if (fEndPosX < 0)
			{
				ChangeX = -(PosX - fEndPosX) / iFrame;
			}
			else
				if (PosX > fEndPosX)
				{
					ChangeX = -(PosX - fEndPosX) / iFrame;
				}
				else { ChangeX = (fEndPosX - PosX) / iFrame; }
		}
		else
		{
			if (fEndPosX > 0)
				ChangeX = (fEndPosX - PosX) / iFrame;
			else {
				if (PosX > fEndPosX)
					ChangeX = (fEndPosX - PosX) / iFrame;
				else { ChangeX = -(PosX - fEndPosX) / iFrame; }
			}

		}


		if (PosY > 0)
		{
			if (fEndPosY < 0)
			{
				ChangeY = -(PosY - fEndPosY) / iFrame;
			}
			else
				if (PosY > fEndPosY)
				{
					ChangeY = -(PosY - fEndPosY) / iFrame;
				}
				else { ChangeY = (fEndPosY - PosY) / iFrame; }
		}
		else
		{
			if (fEndPosY > 0)
				ChangeY = (fEndPosY - PosY) / iFrame;
			else {
				if (PosY > fEndPosY)
					ChangeY = (fEndPosY - PosY) / iFrame;
				else { ChangeY = -(PosY - fEndPosY) / iFrame; }
			}

		}

		if (PosZ > 0)
		{
			if (fEndPosZ < 0)
			{
				ChangeZ = -(PosZ - fEndPosZ) / iFrame;
			}
			else
				if (PosZ > fEndPosZ)
				{
					ChangeZ = -(PosZ - fEndPosZ) / iFrame;
				}
				else { ChangeZ = (fEndPosZ - PosZ) / iFrame; }
		}
		else
		{
			if (fEndPosZ > 0)
				ChangeZ = (fEndPosZ - PosZ) / iFrame;
			else {
				if (PosZ > fEndPosZ)
					ChangeZ = (fEndPosZ - PosZ) / iFrame;
				else { ChangeZ = -(PosZ - fEndPosZ) / iFrame; }
			}

		}

		outX->data.num = PosX + ChangeX;
		outY->data.num = PosY + ChangeY;
		outZ->data.num = PosZ + ChangeZ;


	}

	return true;
}