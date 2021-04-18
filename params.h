#pragma once

static ParamInfo kParams_Tomm_OneInt[1] =
{
	{	"int", kParamType_Integer, 0 },
};

static ParamInfo kParams_Tomm_OneIntOptional[1] =
{
	{	"int", kParamType_Integer, 1 },
};


static ParamInfo kParams_Tomm_OneInt_OneFloat[2] =
{
	{	"int", kParamType_Integer, 0 },
	{ "float", kParamType_Float, 0 },
};


static ParamInfo kParams_Tomm_TwoInt[2] =
{
	{	"int", kParamType_Integer, 0 },
	{	"int", kParamType_Integer, 0 },
};

ParamInfo kParams_Tomm_OneFloatOptional[1] =
{

	{ "float", kParamType_Float, 1 },

};

ParamInfo kParams_Tomm_OneFloat[1] =
{

	{ "float", kParamType_Float, 0 },

};


static ParamInfo kParams_Tomm_OneForm[1] =
{
	{	"form",	kParamType_AnyForm,	0	},
};


ParamInfo kParams_Tomm_OneFloat_OneOptionalInt[2] =
{

	{ "float", kParamType_Float, 0 },
	{ "int", kParamType_Integer, 1 },

};

ParamInfo kParams_Tomm_OneFloat_TwoOptionalInt[3] =
{

	{ "float", kParamType_Float, 0 },
	{ "int", kParamType_Integer, 1 },
	{ "int", kParamType_Integer, 1 },
};


ParamInfo kParams_Tomm_OneDouble_OneOptionalInt[2] =
{

	{ "double", kParamType_Float, 0 },
	{ "int", kParamType_Integer, 1 },

};


ParamInfo kParams_Tomm_TwoFloats[2] =
{

	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },

};

ParamInfo kParams_Tomm_ThreeFloats_OneObjectRef[4] =
{

	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{"Object Ref", kParamType_ObjectRef, 0}

};

ParamInfo kParams_Tomm_FiveFloats[5] =
{

	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },


};

ParamInfo kParams_Tomm_SixFloats[6] =
{

	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },

};

ParamInfo kParams_Tomm_SevenFloats[7] =
{

	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },

};


ParamInfo kParams_Tomm_SevenFloats_ThreeScriptVars[10] =
{

	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0},

};



ParamInfo kParams_Tomm_ThreeFloats[3] =
{

	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },

};


ParamInfo kParams_Tomm_TwoFloats_TwoScriptVars[4] =
{
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0},
};

ParamInfo kParams_Tomm_TwoFloats_TwoScriptVars_TwoFloats[6] =
{
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
};




ParamInfo kParams_Tomm_TwoScriptVarsOptional[2] =
{
	{"Variable", kParamType_ScriptVariable, 1},
	{"Variable", kParamType_ScriptVariable, 1},
};


ParamInfo kParams_Tomm_ThreeScriptVarsOptional[3] =
{
	{"Variable", kParamType_ScriptVariable, 1},
	{"Variable", kParamType_ScriptVariable, 1},
	{"Variable", kParamType_ScriptVariable, 1},
};



ParamInfo kParams_Tomm_OneStringOptional[1] =
{
	{"String", kParamType_String, 1},
};

ParamInfo kParams_Tomm_OneString[1] =
{
	{"String", kParamType_String, 0},
};

ParamInfo kParams_Tomm_TwoStrings[2] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
};

ParamInfo kParams_Tomm_ThreeStrings[3] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
};


ParamInfo kParams_Tomm_OneIntOptional_OneStringOptional[2] =
{
	{"int", kParamType_Integer, 1 },
	{"String", kParamType_String, 1},

};


static ParamInfo kParams_Tomm_Axis[1] =
{
	{	"axis",	kParamType_Axis,	0	},
};

static ParamInfo kParams_Tomm_OneAxis_OneFloat[2] =
{
	{	"axis",	kParamType_Axis,	0	},
	{"Float", kParamType_Float, 0},
};


static ParamInfo kParams_Tomm_OneAnyForm[1] =
{
	{	"form",	kParamType_AnyForm,	0	},

};

ParamInfo kParams_Tomm_OneDouble[1] =
{
	{"Double", kParamType_Double, 0},
};





ParamInfo kParams_Tomm_OneScriptVar[1] =
{

	{"Variable", kParamType_ScriptVariable, 0},
};

ParamInfo kParams_Tomm_TwoScriptVars[2] =
{

	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0},
};

ParamInfo kParams_Tomm_ThreeScriptVars[3] =
{

	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0},
};


ParamInfo kParams_Tomm_FourScriptVars[4] =
{

	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 0},
};


ParamInfo kParams_Tomm_OneInt_ThreeOptFloats_OneOptInt[5] = // Copied From JIP
{
	{"Integer", kParamType_Integer, 0},
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1},
	{"Float", kParamType_Float, 1},
	{"Integer", kParamType_Integer, 1},
};



ParamInfo kParams_Tomm_CaptureScreenshotAlt[9] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Float", kParamType_Float, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
	{"Integer", kParamType_Integer, 0},
};

ParamInfo kParams_Tomm_DeleteScreenshot[2] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
};


ParamInfo kParams_Tomm_CaptureScreenshot[7] =
{
	{"String", kParamType_String, 1},
	{"Integer", kParamType_Integer, 1},
};

ParamInfo kParams_Tomm_SetFloatsFromArray[11] =
{
	{ "Integer", kParamType_Integer, 0 },
	{"Variable", kParamType_ScriptVariable, 0},
	{"Variable", kParamType_ScriptVariable, 1},
	{"Variable", kParamType_ScriptVariable, 1},
	{"Variable", kParamType_ScriptVariable, 1},
	{"Variable", kParamType_ScriptVariable, 1},
	{"Variable", kParamType_ScriptVariable, 1},
	{"Variable", kParamType_ScriptVariable, 1},
	{"Variable", kParamType_ScriptVariable, 1},
	{"Variable", kParamType_ScriptVariable, 1},
	{"Variable", kParamType_ScriptVariable, 1},
};

ParamInfo kParams_Tomm_ObjectRef[1] =
{
	{"Object Ref", kParamType_ObjectRef, 0}
};


static ParamInfo kParams_Tomm_OneActorRef[1] = // from jip
{
	{	"actor reference",	kParamType_Actor,	0	},
};