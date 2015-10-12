#pragma once

namespace Keys {
	/**********************************/
	//Keys
	/**********************************/
	//Init Keys
	void InitKeys();
	void InsertKey(Key key);
	size_t GetSize();
	Key GetKey(int key);
	void UpdateKey(int key_num, Key key);
#ifndef Keys_No_Place
	struct MouseState
	{
		unsigned l_mouse : 1;
		unsigned c_mouse : 1;
		unsigned r_mouse : 1;
	} extern  mouse_state;
#endif
}

