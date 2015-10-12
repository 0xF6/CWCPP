

#include "EIdent.h"

///////////////////////////////////////////////////////////
//Class
///////////////////////////////////////////////////////////

void EIdent::SetCustomId(CString cid)
{
	this->Id->SetCustomId(cid);
}

Ident *EIdent::GetIdent()
{
	return this->Id;
}



///////////////////////////////////////////////////////////
//Registration
///////////////////////////////////////////////////////////

namespace {
	CArray<Ident> IdentList;
	CArray<Ident> EmptyList;
}

Ident *RegisterId()
{
	if (EmptyList.empty())
	{
		unsigned long id = 0;
		for (size_t i = 0; i < IdentList.size(); i++)
		{
			
			if (IdentList[i]->id > id)
				id = IdentList[i]->id + 1;
		}
		Ident Id_obj(id);
		Id_obj.registered = true;
		IdentList.push_back(Id_obj);
	}
	else
	{
		Ident id = *EmptyList[0];
		EmptyList.remove(0);
		id.registered = true;
		IdentList.push_back(id);
	}
	return IdentList[IdentList.size() - 1];
}

void UnRegisterId(Ident *Id)
{
	for (size_t i = 0; i < IdentList.size(); i++)
	{
		if (*IdentList[i] == *Id)
		{
			EmptyList.push_back(*Id);
			IdentList.remove(i);
			break;
		}
	}
}