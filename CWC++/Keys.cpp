#pragma once
#define StreamExclude
#define DXExclude
#define NetExclude
#include "Types.h"
#define Keys_No_Place
#include "Keys.h"
#undef Keys_No_Place


namespace Keys {
	bool init_done = false;
	CArray<Key> Keys;
	struct MouseState
	{
		unsigned l_mouse : 1;
		unsigned c_mouse : 1;
		unsigned r_mouse : 1;
	} mouse_state;
}

void Keys::InitKeys()
{
	if (init_done) return;
	for (register UINT code = 0; code < 200; code++)
	{
		InsertKey(code);
	}
	init_done = true;
}

void Keys::InsertKey(Key key)
{
	Keys::Keys.push_back(key);
}

size_t Keys::GetSize()
{
	return Keys::Keys.size();
}

Key Keys::GetKey(int key_num)
{
	return *Keys::Keys[key_num];
}

void Keys::UpdateKey(int key_num, Key key)
{
	*Keys::Keys[key_num] = key;
}

void UpdateMouseState(unsigned short state, bool value)
{
	switch (state)
	{
	case 0:
		Keys::mouse_state.l_mouse = value;
		break;
	case 1:
		Keys::mouse_state.c_mouse = value;
		break;
	case 2:
		Keys::mouse_state.r_mouse = value;
		break;
	}
}