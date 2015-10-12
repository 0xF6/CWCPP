#pragma once

#ifndef NULL
#define NULL 0
#endif

#pragma region CQueue

#define PFX template<class _type>

#pragma region Declaration


PFX class CQueue {
	_type **ppObjects;
	size_t _size;
public:
	__forceinline CQueue() :
	ppObjects(NULL), _size(0)
	{

	}
	__forceinline CQueue(const CQueue &obj)
	{

	}
	void push(const _type);
	void pushl(const _type&);
	void push(const _type*);
	__forceinline void pushp(const _type &obj)
	{
		pushp(&obj);
	}
	void pushp(const _type *);
	__forceinline _type *front()
	{
		if (_size)
			return ppObjects[0];
		else
			return NULL;
	}
	void pop();
	void popp();
	__forceinline void size() const
	{
		return _size;
	}
	__forceinline void count() const 
	{
		return _size;
	}
	__forceinline bool empty()
	{
		return _size ? false : true;
	}
};

#pragma endregion

#define PSX CQueue<_type>

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
		delete ppObjects[0];
		popp();
	}
}

PFX
void PSX::popp()
{
	if (_size)
	{
		_type **new_ptr = new _type*[_size - 1];
		for (register size_t _pos = 1; _pos < _size; _pos++)
			new_ptr[_pos - 1] = ppObjects[_pos];
		delete [_size] ppObjects;
		ppObjects = new_ptr;
		_size--;
	}
}

#pragma endregion


#undef PFX
#undef PSX

#pragma endregion