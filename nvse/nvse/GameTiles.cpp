#include "nvse/GameTiles.h"
#include "nvse/Utilities.h"
#include "nvse/GameAPI.h"
#include <string>

typedef NiTMapBase <const char *, int>	TraitNameMap;
TraitNameMap	* g_traitNameMap = (TraitNameMap *)0x011F32F4;

const _TraitNameToID TraitNameToID = (_TraitNameToID)0x00A01860;

UInt32 Tile::TraitNameToID(const char * traitName)
{
	return ::TraitNameToID(traitName);
}

__declspec(naked) Tile::Value *Tile::GetValue(UInt32 typeID)
{
	__asm
	{
		push	ebx
		push	esi
		push	edi
		mov		ebx, [ecx+0x14]
		xor		esi, esi
		mov		edi, [ecx+0x18]
		mov		edx, [esp+0x10]
	iterHead:
		cmp		esi, edi
		jz		iterEnd
		lea		ecx, [esi+edi]
		shr		ecx, 1
		mov		eax, [ebx+ecx*4]
		cmp		[eax], edx
		jz		done
		jb		isLT
		mov		edi, ecx
		jmp		iterHead
	isLT:
		lea		esi, [ecx+1]
		jmp		iterHead
	iterEnd:
		xor		eax, eax
	done:
		pop		edi
		pop		esi
		pop		ebx
		retn	4
	}
}

Tile::Value * Tile::GetValueName(const char * valueName)
{
	return GetValue(TraitNameToID(valueName));
}

Tile * Tile::GetChild(const char * childName)
{
	int childIndex = 0;
	char *colon = strchr(const_cast<char*>(childName), ':');
	if (colon)
	{
		if (colon == childName) return NULL;
		*colon = 0;
		childIndex = atoi(colon + 1);
	}
	Tile *result = NULL;
	for(tList<ChildNode>::Iterator iter = childList.Begin(); !iter.End(); ++iter)
	{
		if (*iter && iter->child && ((*childName == '*') || !StrCompare(iter->child->name.m_data, childName)) && !childIndex--)
		{
			result = iter->child;
			break;
		}
	}
	if (colon) *colon = ':';
	return result;
}

// Find a tile or tile value by component path.
// Returns NULL if component path not found.
// Returns Tile* and clears "trait" if component was a tile.
// Returns Tile* and sets "trait" if component was a tile value.
Tile * Tile::GetComponent(const char * componentPath, const char **trait)
{
	Tile *parentTile = this;
	char *slashPos;
	while (slashPos = SlashPos(componentPath))
	{
		*slashPos = 0;
		parentTile = parentTile->GetChild(componentPath);
		if (!parentTile) return NULL;
		componentPath = slashPos + 1;
	}
	if (*componentPath)
	{
		Tile *result = parentTile->GetChild(componentPath);
		if (result) return result;
		*trait = componentPath;
	}
	return parentTile;
}

Tile::Value * Tile::GetComponentValue(const char * componentPath)
{
	const char *trait = NULL;
	Tile *tile = GetComponent(componentPath, &trait);
	return (tile && trait) ? tile->GetValueName(trait) : NULL;
}

Tile * Tile::GetComponentTile(const char * componentPath)
{
	const char *trait = NULL;
	Tile *tile = GetComponent(componentPath, &trait);
	return (tile && !trait) ? tile : NULL;
}

char *Tile::GetComponentFullName(char *resStr)
{
	if (*(UInt32*)this == 0x106ED44)
		return (char*)memcpy(resStr, name.m_data, name.m_dataLen) + name.m_dataLen;
	char *fullName = parent->GetComponentFullName(resStr);
	*fullName++ = '/';
	fullName = (char*)memcpy(fullName, name.m_data, name.m_dataLen) + name.m_dataLen;
	DListNode<Tile> *node = ((DList<Tile>*)&parent->childList)->Tail();
	while (node->data != this)
		node = node->prev;
	int index = 0;
	while ((node = node->prev) && !strcmp(name.m_data, node->data->name.m_data))
		index++;
	if (index)
	{
		*fullName++ = ':';
		fullName = IntToStr(fullName, index);
	}
	return fullName;
}

void Tile::Dump(void)
{
	int iCount = 0;
	Console_Print("<<%s>>", name.m_data);
	char s_Result[0x4000]{};
	//gLog.Indent();

	Console_Print("values:");

	//gLog.Indent();
	
	for(UInt32 i = 0; i < values.size; i++)
	{
		Value		* val = values[i];
		const char	* traitName = TraitIDToName(val->id);
		char		traitNameIDBuf[16];

		if(!traitName)
		{
			sprintf_s(traitNameIDBuf, "%08X", val->id);
			traitName = traitNameIDBuf;
		}


		if(val->str)
			Console_Print("%s: %s", traitName, val->str);
		//else if(val->action)
			//Console_Print("%s: action %08X", traitName, val->action);
		else
			Console_Print("%s: %f", traitName, val->num);
	}

	//gLog.Outdent();

	for(tList <ChildNode>::Iterator iter = childList.Begin(); !iter.End(); ++iter)
	{
		ChildNode	* node = iter.Get();
		if(node)
		{
			Tile	* child = node->child;
			if(child)
			{

				iCount = 0;
				size_t size = sizeof(s_Result) / sizeof(s_Result[0]);
				while (iCount < size)
				{
					s_Result[iCount] = 0;
					iCount += 1;
				}

				child->GetComponentFullName(s_Result);
				Console_Print("FULLPATH::<<%s>>", s_Result);
				child->Dump();
			}
		}
	}

	//gLog.Outdent();
}


void Tile::DumpAlt(void)
{
	char s_Result[0x4000]{};
	int iCount = 0;
	_MESSAGE("<<%s>>", name.m_data);


	//gLog.Indent();

	_MESSAGE("values:");

	//gLog.Indent();

	for (UInt32 i = 0; i < values.size; i++)
	{
		Value* val = values[i];
		const char* traitName = TraitIDToName(val->id);
		char		traitNameIDBuf[16];

		if (!traitName)
		{
			sprintf_s(traitNameIDBuf, "%08X", val->id);
			traitName = traitNameIDBuf;
		}

		if (val->str)
			_MESSAGE("%s: %s", traitName, val->str);
		//else if (val->action)
			//_MESSAGE("%s: action %08X", traitName, val->action);
		else
			_MESSAGE("%s: %f", traitName, val->num);
	}

	//gLog.Outdent();

	for (tList <ChildNode>::Iterator iter = childList.Begin(); !iter.End(); ++iter)
	{
		ChildNode* node = iter.Get();
		if (node)
		{
			Tile* child = node->child;
			if (child)
			{
				iCount = 0;
				size_t size = sizeof(s_Result) / sizeof(s_Result[0]);


				while (iCount < size)
				{
					s_Result[iCount] = 0;
					iCount += 1;
				}
	


				child->GetComponentFullName(s_Result);
				_MESSAGE("FULLPATH::<<%s>>", s_Result);
				child->DumpAlt();
			}
		}
	}

	//gLog.Outdent();
}



void Tile::SetUIFloatMass(int iTraitID, float fValue, int iTypeFilter)
{
	char s_Result[0x4000]{};
	int iCount = 0;
	Tile::Value* tileVal = NULL;

	if (iTypeFilter)
	{
		if (iTypeFilter == GetType())
		{
			tileVal = GetValue(iTraitID);
			if (tileVal)
			{
				tileVal->SetFloat(fValue, 0);
			}
		}

	}
	else {
		tileVal = GetValue(iTraitID);
		if (tileVal)
		{
			tileVal->SetFloat(fValue, 0);
		}

	}




	for (tList <ChildNode>::Iterator iter = childList.Begin(); !iter.End(); ++iter)
	{
		ChildNode* node = iter.Get();
		if (node)
		{
			Tile* child = node->child;
			if (child)
			{
				child->SetUIFloatMass(iTraitID, fValue, iTypeFilter);
			}
		}
	}

}








void Debug_DumpTraits(void)
{
	for(UInt32 i = 0; i < g_traitNameMap->numBuckets; i++)
	{
		for(TraitNameMap::Entry * bucket = g_traitNameMap->buckets[i]; bucket; bucket = bucket->next)
		{
			_MESSAGE("%s,%08X,%d", bucket->key, bucket->data, bucket->data);
		}
	}
}

// not a one-way mapping, so we just return the first
// also this is slow and sucks
const char * TraitIDToName(int id)
{
	for(UInt32 i = 0; i < g_traitNameMap->numBuckets; i++)
		for(TraitNameMap::Entry * bucket = g_traitNameMap->buckets[i]; bucket; bucket = bucket->next)
			if(bucket->data == id)
				return bucket->key;

	return NULL;
}

void Debug_DumpTileImages(void) {};

