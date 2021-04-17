#pragma once


DEFINE_COMMAND_PLUGIN(SetTFCPosEx, "", 0, 3, kParams_Tomm_ThreeFloats)
DEFINE_COMMAND_PLUGIN(GetTFCPosEx, "", 0, 3, kParams_Tomm_ThreeScriptVars)
DEFINE_COMMAND_PLUGIN(GetTFCRotEx, "", 0, 2, kParams_Tomm_TwoScriptVars)
DEFINE_COMMAND_PLUGIN(SetTFCRotEx, "", 0, 2, kParams_Tomm_TwoFloats)
DEFINE_COMMAND_PLUGIN(SetTFCAngleEx, "", 0, 2, kParams_Tomm_TwoFloats)
DEFINE_COMMAND_PLUGIN(GetTFCAngleEx, "", 0, 2, kParams_Tomm_TwoScriptVars)
DEFINE_COMMAND_PLUGIN(GetTFCPos, "", 0, 1, kParams_Tomm_Axis)
DEFINE_COMMAND_PLUGIN(SetTFCPos, "", 0, 1, kParams_Tomm_OneAxis_OneFloat)
DEFINE_COMMAND_PLUGIN(GetTFCAngle, "", 0, 1, kParams_Tomm_Axis)
DEFINE_COMMAND_PLUGIN(SetTFCAngle, "", 0, 1, kParams_Tomm_OneAxis_OneFloat)
DEFINE_COMMAND_PLUGIN(GetTFCRot, "", 0, 1, kParams_Tomm_Axis)
DEFINE_COMMAND_PLUGIN(SetTFCRot, "", 0, 1, kParams_Tomm_OneAxis_OneFloat)

bool Cmd_SetTFCPosEx_Execute(COMMAND_ARGS)
{
	float	X, Y, Z;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &X, &Y, &Z))
	{
		g_ThePlayer->flycamPosX = X;
		g_ThePlayer->flycamPosY = Y;
		g_ThePlayer->flycamPosZ = Z;
	}

	return true;
}




bool Cmd_GetTFCPosEx_Execute(COMMAND_ARGS)
{

	ScriptVar* X, * Y, * Z;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &X, &Y, &Z))
	{
		X->data.num = g_ThePlayer->flycamPosX;
		Y->data.num = g_ThePlayer->flycamPosY;
		Z->data.num = g_ThePlayer->flycamPosZ;
	}

	return true;
}


//X == Angle X
//Z == Angle Z
bool Cmd_GetTFCRotEx_Execute(COMMAND_ARGS)
{

	ScriptVar* X, * Z;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &X, &Z))
	{
		X->data.num = g_ThePlayer->flycamXRot;
		Z->data.num = g_ThePlayer->flycamZRot;
	}

	return true;
}

bool Cmd_SetTFCRotEx_Execute(COMMAND_ARGS)
{

	float X, Z;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &X, &Z))
	{
		g_ThePlayer->flycamXRot = X;
		g_ThePlayer->flycamZRot = Z;
	}

	return true;

}

bool Cmd_SetTFCAngleEx_Execute(COMMAND_ARGS)
{
	float fAngleX, fAngleZ;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &fAngleX, &fAngleZ))
	{
		g_ThePlayer->flycamXRot = 0.017453292519943295 * fAngleX;
		g_ThePlayer->flycamZRot = 0.017453292519943295 * fAngleZ;
	}
	return true;
}

bool Cmd_GetTFCAngleEx_Execute(COMMAND_ARGS)
{
	ScriptVar* X, * Z;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &X, &Z))
	{
		X->data.num = g_ThePlayer->flycamXRot / 0.017453292519943295;
		Z->data.num = g_ThePlayer->flycamZRot / 0.017453292519943295;
	}
	return true;
}


bool Cmd_GetTFCPos_Execute(COMMAND_ARGS) // axis template taken from JiP
{
	ScriptVar* X, * Y, * Z;
	*result = 0;
	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis))
	{
			switch (axis)
			{
			case 'X':
			{*result = g_ThePlayer->flycamPosX;break;}
			case 'Y':
			{*result = g_ThePlayer->flycamPosY;break;}
			case 'Z':
			{*result = g_ThePlayer->flycamPosZ;break;}
			}
	}
	return true;
}

bool Cmd_SetTFCPos_Execute(COMMAND_ARGS) // axis template taken from JiP
{
	float	fValue;
	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis, &fValue))
	{
		switch (axis)
		{
		case 'X':
		{g_ThePlayer->flycamPosX = fValue;break;}
		case 'Y':
		{g_ThePlayer->flycamPosY = fValue;break;}
		case 'Z':
		{g_ThePlayer->flycamPosZ = fValue;break;}
		}
	}
	return true;
}



bool Cmd_GetTFCAngle_Execute(COMMAND_ARGS)
{

	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis) && (axis != 'Y'))
	{
		switch (axis)
		{
		case 'X':
		{*result = g_ThePlayer->flycamXRot / 0.017453292519943295;break;}
		case 'Z':
		{*result = g_ThePlayer->flycamZRot / 0.017453292519943295;break;}
		}
	}
	return true;
}


bool Cmd_SetTFCAngle_Execute(COMMAND_ARGS)
{
	float fValue;
	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis,&fValue) && (axis != 'Y'))
	{
		switch (axis)
		{
		case 'X':
		{g_ThePlayer->flycamXRot = 0.017453292519943295 * fValue;break;}
		case 'Z':
		{g_ThePlayer->flycamZRot = 0.017453292519943295 * fValue;break;}
		}
	}
	return true;
}


bool Cmd_GetTFCRot_Execute(COMMAND_ARGS)
{

	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis) && (axis != 'Y'))
	{
		switch (axis)
		{
		case 'X':
		{*result = g_ThePlayer->flycamXRot;break;}
		case 'Z':
		{*result = g_ThePlayer->flycamZRot;break;}
		}
	}
	return true;

}

bool Cmd_SetTFCRot_Execute(COMMAND_ARGS)
{
	float fValue;
	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis, &fValue) && (axis != 'Y'))
	{
		switch (axis)
		{
		case 'X':
		{g_ThePlayer->flycamXRot = fValue;break;}
		case 'Z':
		{g_ThePlayer->flycamZRot = fValue;break;}
		}
	}
	return true;
}


//bool Cmd_GetActorTiltAngle_Execute(COMMAND_ARGS)
//{
//	*result = 0;
//	char axis;
//	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis) && (axis < 'Z'))
//	{
//		bhkCharacterController* charCtrl = thisObj->GetCharacterController();
//		if (charCtrl)
//			*result = ((axis == 'X') ? charCtrl->tiltAngleX : charCtrl->tiltAngleY) / kDblPId180;
//	}
//	return true;
//}
//
