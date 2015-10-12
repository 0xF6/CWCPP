#pragma once

#ifndef NULL
#define NULL 0
#endif

#pragma region CStack

#define PFX template<class _type>

#pragma region Declaration

PFX class CStack {
	_type **ppObjects;
	size_t _size;
public:
	__forceinline CStack() :
	ppObjects(NULL), _size(0)
	{

	}
	__forceinline CStack(const CStack &obj): 
	ppObjects(NULL), _size(0)
	{
		for (register size_t _pos = 0; _pos < obj._size; _pos++)
			push(obj.ppObjects[_pos]);
	}
	void push(const _type *Object);
	void push(const _type Object);
	void pushl(const _type &Object);
	__forceinline void pushp(const _type &Object)
	{
		pushp(&Object);
	}
	void pushp(const _type *Object);
	__forceinline _type *top()
	{
		if (_size)
			return ppObjects[_size - 1];
		else
			return NULL;
	}
	void pop();
	void popp();
	__forceinline size_t size() const
	{
		return this->_size;
	}
	__forceinline size_t count() const
	{
		return this->_size;
	}
	__forceinline bool empty()
	{
		return _size ? false : true;
	}
};

#pragma endregion

#define PSX CStack<_type>

#pragma region Code

PFX
void PSX::push(const _type Object)
{
	_type *new_obj = new _type(Object);
	pushp(new_obj);
}

PFX
void PSX::pushl(const _type &Object)
{
	_type *new_obj = new _type(Object);
	pushp(new_obj);
}

PFX
void PSX::push(const _type *Object)
{
	_type *new_obj = new _type(*Object);
	pushp(new_obj);
}

PFX
void PSX::pushp(const _type *Object)
{
	_type **new_ptr = new _type*[_size + 1];
	for (register size_t _pos = 0; _pos < _size; _pos++)
		new_ptr[_pos] = ppObjects[_pos];
	new_ptr[_size] = (_type *) Object;
	delete [_size] ppObjects;
	ppObjects = new_ptr;
	_size++;
}

PFX
void PSX::pop()
{
	if (_size)
	{
		delete ppObjects[_size - 1];
		popp();
	}
}

PFX
void PSX::popp()
{
	if (_size)
	{
		_type **new_ptr = new _type*[_size - 1];
		for (register size_t _pos = 0; _pos < _size - 1; _pos++)
			new_ptr[_pos] = ppObjects[_pos];
		delete [_size] ppObjects;
		ppObjects = new_ptr;
		_size--;
	}
}

#pragma endregion


#undef PFX
#undef PSX

#pragma endregion