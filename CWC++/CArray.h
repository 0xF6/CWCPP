#pragma once

#ifndef NULL
#define NULL 0
#endif

#pragma region CArray

#define PFX template<class _type>

#pragma region Declaration

PFX class CArray {
protected:
	_type **ppObjects;
	size_t _size;
public:
	CArray();
	__forceinline CArray(const CArray &old) : ppObjects(0), _size(0)
	{
		for (register size_t _pos = 0; _pos < old.count(); _pos++)
			this->add(old[_pos]);
	}
	~CArray();
	void add(_type object);
	__forceinline void push_back(_type object) { this->add(object); }
	void addl(_type &object);
	void add(_type *object);
	__forceinline void addp(_type &object)
	{
		addp(&object);
	}
	void addp(_type *object);
	_type *operator[](size_t pos) const;
	void remove(size_t pos);
	__forceinline void erase(size_t _pos)
	{
		remove(_pos);
	}
	__forceinline size_t size() const { return this->_size; }
	__forceinline size_t count() const { return this->_size; }
	void clear();
	__forceinline bool empty() const {return (_size ? false : true); }
	__forceinline CArray *operator=(const CArray &old)
	{
		clear();
		for (register size_t _pos = 0; _pos < old.count(); _pos++)
			this->add(old[_pos]);
		return this;
	}
	__forceinline CArray *operator+=(const CArray &copy)
	{
		for (register size_t _pos = 0; _pos < copy.count(); _pos++)
			this->add(copy[_pos]);
		return this;
	}
	__forceinline _type &back()
	{
		return *ppObjects[this->_size -1];
	}
	__forceinline void pop_back()
	{
		this->remove(this->_size - 1);
	}
};


#pragma endregion

#define PSX CArray<_type>


#pragma region Code

PFX
PSX::CArray() :
_size(0), ppObjects(0)
{
	
}


PFX
PSX::~CArray(void)
{
	clear();
}

PFX
void PSX::clear(void)
{
	if (!_size || !ppObjects) return;
	register size_t _size = this->_size;
	for (register size_t pos = 0; pos < _size; pos++)
	{
		delete this->ppObjects[pos];
	}
	delete [_size] this->ppObjects;
	this->_size = 0;
	this->ppObjects = 0;
}

PFX
void PSX::add(_type object)
{
	_type *Object = new _type(object);
	addp(Object);
}

PFX
void PSX::addl(_type &object)
{
	_type *Object = new _type(object);
	addp(Object);
}

PFX
void PSX::add(_type *object)
{
	_type *Object = new _type(*object);
	addp(Object);
}


PFX
void PSX::addp(_type *object)
{
	_type **new_array = new _type*[this->_size+1];
	register size_t _size = this->_size + 1;
	if (!empty())
	{
		for (register size_t pos = 0; pos < this->_size; pos++)
		{
			new_array[pos] = this->ppObjects[pos];
		}
		delete ppObjects;
	}
	new_array[_size - 1] = object;
	this->ppObjects = new_array;
	this->_size++;
}

PFX
void PSX::remove(size_t pos)
{
	if (pos > _size - 1 || _size == 0)
	{
		return;
	}
	else
	{
		_type *_pointer = this->ppObjects[pos];
		delete (_pointer); //удаление объекта в попу
		_type **new_array = new _type*[this->_size-1];
		register size_t _size = this->_size;
		register size_t _pos = 0;
		for (_pos; _pos < pos; _pos++)
		{
			new_array[_pos] = this->ppObjects[_pos];
		}
		_pos++;
		for (_pos; _pos < _size; _pos++)
		{
			new_array[_pos - 1] = this->ppObjects[_pos];
		}
		delete ppObjects;
		if (_size - 1)
			this->ppObjects = new_array;
		else
			this->ppObjects = NULL;
		this->_size--;
	}
}

PFX
_type *PSX::operator[](size_t pos) const
{
	if (pos > _size - 1 || _size == 0)
	{
		throw "MORE_THAN_SIZE";
		return NULL;
	}
	else
		return this->ppObjects[pos];
}

#pragma endregion

#undef PFX
#undef PSX

#pragma endregion