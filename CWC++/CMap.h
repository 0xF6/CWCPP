#pragma once

#ifndef NULL
#define NULL 0
#endif

#pragma region CMap

#define PFX template<class _type1, class _type2>
#define TCNSTR (_type1 _key, _type2 _object)
#define PSXP Pair<_type1, _type2>
#define PSXM CMap<_type1, _type2>

#pragma region Declaration


#pragma region PairDeclaration

PFX struct Pair {
	_type1 key;
	_type2 object;
	//////////////
	Pair TCNSTR;
	Pair(Pair &pair)
	{
		this->key = pair.key;
		this->object = pair.object;
	}
	bool operator==(_type1 _key);
	Pair *operator=(Pair &pair)
	{
		this->key = pair.key;
		this->object = pair.object;
		return this;
	}
};

#pragma endregion

#pragma region CMapDeclaration


PFX class CMap {
	Pair<_type1, _type2> **ppObjects;
	size_t _size;
public:
	CMap();
	CMap(CMap &);
	void add(Pair<_type1, _type2> pair);
	void addl(Pair<_type1, _type2> &pair);
	void add(Pair<_type1, _type2> *pair);
	__forceinline void add TCNSTR
	{
		add(Pair<_type1, _type2>(_key, _object));
	}
	__forceinline void addp(Pair<_type1, _type2> &pair)
	{
		addp(&pair);
	}
	void addp(Pair<_type1, _type2> *pair);
	void clear();
	__forceinline size_t size() { return this->_size; }
	__forceinline size_t count() { return this->_size; }
	void remove(size_t pos);
	void removekey(_type1 &key);
	__forceinline void erase(size_t pos)
	{
		remove(pos);
	}
	__forceinline void erase(_type1 &key)
	{
		remove(key);
	}
	void erase(const Pair<_type1, _type2> *p)
	{
		for (register size_t _pos = 0; _pos < _size; _pos++)
		{
			if (p == ppObjects[_pos])
			{
				remove(_pos);
			}
		}
	}
	__forceinline PSXP *get(size_t pos)
	{
		if (pos >= this->_size)
			return NULL;
		else
		{
			return this->ppObjects[pos];
		}
	}
	PSXP *operator[](_type1 &key);
	__forceinline PSXP *find(_type1 &key)
	{
		return operator[](key);
	}
	__forceinline void insert TCNSTR
	{
		this->add(_key, _object);
	}
	__forceinline CMap *operator=(CMap &old)
	{
		clear();
		for (register size_t _pos = 0; _pos < old.count(); _pos++)
			this->add(old.ppObjects[_pos]);
		return this;
	}
	__forceinline CMap *operator+=(CMap &old)
	{
		for (register size_t _pos = 0; _pos < old.count(); _pos++)
			this->add(old.ppObjects[_pos]);
		return this;
	}
	__forceinline CMap *operator-=(_type1 &key)
	{
		remove(key);
	}
	bool exists(_type1 &key);
	__forceinline bool empty()
	{
		return _size ? false : true;
	}
};

#pragma endregion

#pragma endregion

#pragma region Code

#pragma region PairCode

PFX
PSXP::Pair TCNSTR
{
	this->key = _key;
	this->object = _object;
}

PFX
bool PSXP::operator==(_type1 _key)
{
	return (this->key == _key);
}

#pragma endregion

#pragma region CMapCode

PFX
PSXM::CMap()
: _size(0), ppObjects(0)
{

}

PFX
PSXM::CMap(CMap &obj)
: _size(0), ppObjects(0)
{
	if (obj._size)
		for (register size_t _pos = 0; _pos < obj._size; _pos++)
			add(obj.ppObjects[_pos]);
}

PFX
void PSXM::add(Pair<_type1, _type2> pair)
{
	if (exists(pair.key))
		return;
	else
	{
		Pair<_type1, _type2> *np = new Pair<_type1, _type2>(pair);
		addp(np);
	}
}

PFX
void PSXM::add(Pair<_type1, _type2> *pair)
{
	if (exists(pair->key))
		return;
	else
	{
		Pair<_type1, _type2> *np = new Pair<_type1, _type2>(*pair);
		addp(np);
	}
}

PFX
void PSXM::addl(Pair<_type1, _type2> &pair)
{
	if (exists(pair.key))
		return;
	else
	{
		Pair<_type1, _type2> *np = new Pair<_type1, _type2>(pair);
		addp(np);
	}
}

PFX
void PSXM::addp(Pair<_type1, _type2> *pair)
{
	if (exists(pair->key))
		return;
	else
	{
		Pair<_type1, _type2> **new_pointer = new Pair<_type1, _type2>*[this->_size + 1];
		for (register size_t _pos = 0; _pos < this->_size; _pos++)
		{
			new_pointer[_pos] = this->ppObjects[_pos];
		}
		delete [this->_size] ppObjects;
		new_pointer[this->_size] = pair;
		this->ppObjects = new_pointer;
		this->_size++;
	}
}

PFX
void PSXM::remove(size_t pos)
{
	if (pos >= this->_size)
		return;
	else
	{
		delete this->ppObjects[pos];
		Pair<_type1, _type2> **new_pointer = new Pair<_type1, _type2>*[this->_size - 1];
		register size_t _pos = 0;
		for (pos; _pos < pos && _pos < this->_size; _pos++)
		{
			new_pointer[_pos] = ppObjects[_pos];
		}
		for (pos; _pos < this->_size; _pos++)
		{
			new_pointer[_pos] = ppObjects[_pos + 1];
		}
		delete [this->_size] ppObjects;
		ppObjects = new_pointer;
		this->_size--;
	}
}

PFX
void PSXM::removekey(_type1 &key)
{
	for (register size_t _pos = 0; _pos < this->_size; _pos++)
	{
		if ((*this->ppObjects[_pos]) == key)
		{
			remove(_pos);
			break;
		}
	}
}

PFX
PSXP *PSXM::operator[](_type1 &key)
{
	if (!_size) return 0;
	for (register size_t _pos = 0; _pos < _size; _pos++)
	{
		if (*(this->ppObjects[_pos]) == key)
			return this->ppObjects[_pos];
	}
	return false;
}

PFX
void PSXM::clear()
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
bool PSXM::exists(_type1 &key)
{
	if (!_size) return false;
	for (register size_t _pos = 0; _pos < _size; _pos++)
	{
		if (*(this->ppObjects[_pos]) == key)
			return true;
	}
	return false;
}

#pragma endregion

#pragma endregion

#undef PFX
#undef PSXP
#undef PSXM
#undef TCNSTR


#pragma endregion