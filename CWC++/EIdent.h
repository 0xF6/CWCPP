#pragma once

#include "Types.h"

class EIdent {
	Ident *Id;
public:
	Ident *GetIdent();
	void SetCustomId(CString cid);
};

Ident *RegisterId();

void UnRegisterId(Ident *Id);
