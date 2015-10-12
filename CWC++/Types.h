#pragma once
#include "CArray.h"
#include "CString.h"

enum CDirection {
	CDR_TOP, CDR_BOTTOM,
	CDR_FRONT, CDR_BACK,
	CDR_RIGHT, CDR_LEFT
};


typedef struct Vec2
{
	Vec2(long double x, long double y);
	Vec2();
	long double X;
	long double Y;
	Vec2 *operator=(const Vec2 &vec);
	Vec2 operator+(const Vec2 &vec) const;
	Vec2 operator-(const Vec2 &vec) const;
	Vec2 operator*(const long double scale) const;
	Vec2 operator/(const long double scale) const;
	Vec2 *operator+=(const Vec2 &vec);
	Vec2 *operator-=(const Vec2 &vec);
	Vec2 *operator*=(const long double scale);
	Vec2 *operator/=(const long double scale);
	Vec2 operator-() const;
	void norm();
	long double len() const;
} Vec2;

typedef struct Vec3
{
	Vec3(long double x, long double y, long double z);
	Vec3();
	long double X;
	long double Y;
	long double Z;
	Vec3 *operator=(const Vec3 &vec);
	Vec3 operator+(const Vec3 &vec) const;
	Vec3 operator-(const Vec3 &vec) const;
	Vec3 *operator+=(const Vec3 &vec);
	Vec3 *operator-=(const Vec3 &vec);
	Vec3 operator*(const Vec3 &vec) const;
	Vec3 operator/(const Vec3 &vec) const;
	Vec3 *operator*=(const Vec3 &vec);
	Vec3 *operator/=(const Vec3 &vec);
	Vec3 operator*(const long double scale) const;
	Vec3 operator/(const long double scale) const;
	Vec3 *operator*=(const long double scale);
	Vec3 *operator/=(const long double scale);
	Vec3 operator-() const;
	void norm();
	long double len() const;
} Vec3;

typedef struct Key 
{
	Key(int key_code);
	char name;
	int code;
	bool active;
} Key;

#ifndef STDExclude

typedef struct Ident
{
private:
	unsigned long id;
	CString cid;
	bool registered;
public:
	explicit Ident(unsigned long id);
	Ident(const Ident &i);
	unsigned long GetId();
	void SetCustomId(CString cid); //You can apply custom id only one time
	CString GetCustomId();
	bool operator==(Ident Id);
	bool operator<(Ident Id);
	bool operator>(Ident Id);
	friend Ident *RegisterId();
	Ident *operator=(const Ident &obj);
} Ident;

typedef struct SArguments {
private:
	CArray<CString> args;
public:
	~SArguments();
	void add(CString arg);
	size_t count();
	CString get(size_t num);
	void clear();
	bool has(CString arg);
	CString operator[](size_t num);
	void operator+=(CString arg);
	SArguments *operator=(const SArguments &obj);
} SArguments;

typedef struct SParameter {
private:
	CString parameter_name;
	SArguments args;
public:
	bool operator==(CString name);
	bool operator!=(CString name);
	SParameter(CString name, SArguments &args);
	SParameter();
	~SParameter();
	CString GetName();
	SArguments GetArgs();
	CString operator[](size_t num);
	void operator+=(CString arg);
	SParameter *operator=(const SParameter &obj);
} SParameter;


/* Быстро создаёт 1 параметр с 1 аргументом
 * 
 * @param name - имя параметра
 * @param arg - аргумент
*/
SParameter FastParameter(CString name, CString arg);

typedef struct SParams {
private:
	CArray<SParameter> params;
public:
	~SParams();
	void add(SParameter);
	size_t count();
	SParameter get(size_t num);
	SParameter get(CString name);
	void clear();
	bool has(CString name);
	SParameter operator[](size_t num);
	SParameter operator[](CString name);
	void operator+=(SParameter param);
	SParams *operator=(const SParams &obj);
} SParams;


CString FastArgGet(SParams &params, CString param);

typedef struct Command {
private:
	CString command;
	void (*func)(SParams*);
	SParams by_default;
public:
	Command(CString command, void (*func)(SParams*), SParams def = SParams());
	Command(const Command &cm);
	bool operator==(CString);
	bool operator!=(CString);
	void Run(SParams *params);
	void Run(SParams &params);
	Command *operator=(const Command &obj);
} Command;

#endif

extern __forceinline Vec2 Vec(long double x, long double y);
extern __forceinline Vec3 Vec(long double x, long double y, long double z);


