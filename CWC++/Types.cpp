#include <windows.h>
#include "Types.h"
#include "CString.h"
#include "CArray.h"

Vec2::Vec2()
{
	this->X = 0;
	this->Y = 0;
}

Vec2::Vec2(long double x, long double y)
{
	this->X = x;
	this->Y = y;
}


Vec2 *Vec2::operator=(const Vec2 &vec)
{
	this->X = vec.X;
	this->Y = vec.Y;
	return this;
}


Vec2 Vec2::operator+(const Vec2 &vec) const
{
	Vec2 temp;
	temp.X = this->X + vec.X;
	temp.Y = this->Y + vec.Y;
	return temp;
}

Vec2 Vec2::operator-(const Vec2 &vec) const
{
	Vec2 temp;
	temp.X = this->X - vec.X;
	temp.Y = this->Y - vec.Y;
	return temp;
}

Vec2 Vec2::operator*(const long double Scale) const
{
	Vec2 temp;
	temp.X = this->X * Scale;
	temp.Y = this->Y * Scale;
	return temp;
}

Vec2 Vec2::operator/(const long double Scale) const
{
	Vec2 temp;
	temp.X = this->X / Scale;
	temp.Y = this->Y / Scale;
	return temp;
}

Vec2 *Vec2::operator+=(const Vec2 &vec)
{
	this->X += vec.X;
	this->Y += vec.Y;
	return this;
}

Vec2 *Vec2::operator-=(const Vec2 &vec)
{
	this->X -= vec.X;
	this->Y -= vec.Y;
	return this;
}

Vec2 *Vec2::operator*=(const long double Scale)
{
	this->X *= Scale;
	this->Y *= Scale;
	return this;
}

Vec2 *Vec2::operator/=(const long double Scale)
{
	this->X /= Scale;
	this->Y /= Scale;
	return this;
}

Vec2 Vec2::operator-() const
{
	return Vec(-this->X, -this->Y);
}


void Vec2::norm()
{
	long double lenght = this->len();
	this->X /= lenght;
	this->Y /= lenght;
}

long double Vec2::len() const
{
	return ( sqrt( (this->X * this->X) + (this->Y * this->Y)) );
}




Vec3 *Vec3::operator=(const Vec3 &vec)
{
	this->X = vec.X;
	this->Y = vec.Y;
	this->Z = vec.Z;
	return this;
}


Vec3::Vec3()
{
	this->X = 0;
	this->Y = 0;
	this->Z = 0;
}

Vec3::Vec3(long double x, long double y, long double z)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
}



Vec3 Vec3::operator+(const Vec3 &vec) const
{
	Vec3 temp;
	temp.X = this->X + vec.X;
	temp.Y = this->Y + vec.Y;
	temp.Z = this->Z + vec.Z;
	return temp;
}

Vec3 Vec3::operator-(const Vec3 &vec) const
{
	Vec3 temp;
	temp.X = this->X - vec.X;
	temp.Y = this->Y - vec.Y;
	temp.Z = this->Z - vec.Z;
	return temp;
}

Vec3 *Vec3::operator+=(const Vec3 &vec)
{
	this->X += vec.X;
	this->Y += vec.Y;
	this->Z += vec.Z;
	return this;
}



Vec3 *Vec3::operator-=(const Vec3 &vec)
{
	this->X -= vec.X;
	this->Y -= vec.Y;
	this->Z -= vec.Z;
	return this;
}

Vec3 Vec3::operator*(const Vec3 &vec) const
{
	Vec3 temp;
	temp.X = this->X * vec.X;
	temp.Y = this->Y * vec.Y;
	temp.Z = this->Z * vec.Z;
	return temp;
}

Vec3 Vec3::operator/(const Vec3 &vec) const
{
	Vec3 temp;
	temp.X = this->X / vec.X;
	temp.Y = this->Y / vec.Y;
	temp.Z = this->Z / vec.Z;
	return temp;
}

Vec3 *Vec3::operator*=(const Vec3 &vec)
{
	this->X *= vec.X;
	this->Y *= vec.Y;
	this->Z *= vec.Z;
	return this;
}

Vec3 *Vec3::operator/=(const Vec3 &vec)
{
	this->X /= vec.X;
	this->Y /= vec.Y;
	this->Z /= vec.Z;
	return this;
}

Vec3 Vec3::operator*(long double scale) const
{
	Vec3 Temp = *this;
	Temp.X *= scale;
	Temp.Y *= scale;
	Temp.Z *= scale;
	return Temp;
}

Vec3 Vec3::operator/(long double scale) const
{
	Vec3 Temp = *this;
	Temp.X /= scale;
	Temp.Y /= scale;
	Temp.Z /= scale;
	return Temp;
}

Vec3 *Vec3::operator*=(long double scale)
{
	this->X *= scale;
	this->Y *= scale;
	this->Z *= scale;
	return this;
}

Vec3 *Vec3::operator/=(long double scale)
{
	this->X /= scale;
	this->Y /= scale;
	this->Z /= scale;
	return this;
}

Vec3 Vec3::operator-() const
{
	return Vec(-this->X, -this->Y, -this->Z);
}


void Vec3::norm()
{
	long double lenght = this->len();
	this->X /= lenght;
	this->Y /= lenght;
	this->Z /= lenght;
}


long double Vec3::len() const
{
	return ( sqrt( (this->X * this->X) + (this->Y * this->Y)) + (this->Z * this->Z));
}




Key::Key(int key_code)
{
	code = key_code;
	name = key_code;
	active = false;
}


Ident::Ident(unsigned long id)
{
	this->registered = false;
	this->id = id;
}

Ident::Ident(const Ident &i)
{
	this->cid = i.cid;
	this->id = i.id;
	this->registered = i.registered;
}

Ident *Ident::operator=(const Ident &i)
{
	this->cid = i.cid;
	this->id = i.id;
	this->registered = i.registered;
	return this;
}

unsigned long Ident::GetId()
{
	return this->id;
}

bool Ident::operator==(Ident Id)
{
	return (this->id == Id.GetId());
}

bool Ident::operator<(Ident Id)
{
	return (this->id < Id.GetId());
}

bool Ident::operator>(Ident Id)
{
	return (this->id > Id.GetId());
}

void Ident::SetCustomId(CString cid)
{
	if (this->cid == "" && cid != "")
	{
		this->cid = cid;
	}
}

CString Ident::GetCustomId()
{
	return this->cid;
}


void SArguments::add(CString arg)
{
	this->args.push_back(arg);
}

size_t SArguments::count()
{
	return this->args.size();
}

void SArguments::clear()
{
	for (register size_t i = 0; i < args.size(); i++)
	{
		this->args[i]->clear();
	}
	this->args.clear();
}

CString SArguments::get(size_t num)
{
	if (this->args.empty()) return "";
	return *this->args[num];
}

CString SArguments::operator[](size_t num)
{
	if (this->args.empty()) return "";
	return *this->args[num];
}

void SArguments::operator+=(CString arg)
{
	this->args.push_back(arg);
}

SArguments::~SArguments()
{
	this->args.clear();
}

bool SArguments::has(CString name)
{
	bool result = false;
	for (register size_t _pos = 0; _pos < args.size(); _pos++)
	{
		if (*args[_pos] == name || *args[_pos]  == (name + '\0'))
		{
			result = true;
			break;
		}
	}
	return result;
}

SArguments *SArguments::operator=(const SArguments &obj)
{
	this->args = obj.args;
	return this;
}


SParameter::SParameter(CString name, SArguments &args)
{
	this->parameter_name = name;
	this->args = args;
}

SParameter::SParameter()
{

}

SParameter::~SParameter()
{
	this->args.clear();
}

bool SParameter::operator==(CString name)
{
	return (parameter_name == name);
}
bool SParameter::operator!=(CString name)
{
	return (parameter_name == name);
}

CString SParameter::GetName()
{
	return this->parameter_name;
}

SArguments SParameter::GetArgs()
{
	return this->args;
}

CString SParameter::operator[](size_t num)
{
	return this->args.get(num);
}

void SParameter::operator+=(CString arg)
{
	this->args.add(arg);
}

SParameter *SParameter::operator=(const SParameter &obj)
{
	this->args = obj.args;
	this->parameter_name = obj.parameter_name;
	return this;
}

/* Быстро создаёт 1 параметр с 1 аргументом
 * 
 * @param name - имя параметра
 * @param arg - аргумент
*/
SParameter FastParameter(CString name, CString arg)
{
	SArguments args;
	args += arg;
	SParameter param(name, args);
	return param;
}

void SParams::add(SParameter param)
{
	if (param == "") return;
	this->params.push_back(param);
}

size_t SParams::count()
{
	return this->params.size();
}

SParameter SParams::get(size_t num)
{
	return *this->params[num];
}

SParams::~SParams()
{
	this->clear();
}

SParams *SParams::operator=(const SParams &obj)
{
	this->params = obj.params;
	return this;
}

SParameter SParams::get(CString name)
{
	bool result = false;
	for (register size_t _pos = 0; _pos < params.size(); _pos++)
	{
		if (*params[_pos] == name)
			return *params[_pos];
	}
	return SParameter();
}

bool SParams::has(CString name)
{
	if (params.empty()) return false;
	bool result = false;
	for (register size_t _pos = 0; _pos < params.size(); _pos++)
	{
		if (*params[_pos] == name)
			return true;
	}
	return result;
}

void SParams::clear()
{
	this->params.clear();
}

SParameter SParams::operator[](size_t num)
{
	return *this->params[num];
}

SParameter SParams::operator[](CString name)
{
	return this->get(name);
}

void SParams::operator+=(SParameter param)
{
	if (param == "") return;
	this->params.push_back(param);
}


CString FastArgGet(SParams &params, CString param)
{
	return params.has("param") ? params["param"].GetArgs()[0] : "";
}


Command::Command(CString command, void (*func)(SParams*), SParams def)
{
	this->command = command;
	this->func = func;
	this->by_default = def;
}

Command::Command(const Command &cm)
{
	this->command = cm.command;
	this->by_default = cm.by_default;
	this->func = cm.func;
}

Command *Command::operator=(const Command &cm)
{
	this->command = cm.command;
	this->by_default = cm.by_default;
	this->func = cm.func;
	return this;
}

bool Command::operator==(CString command)
{
	return (this->command == command);
}

bool Command::operator!=(CString command)
{
	return (this->command != command);
}

void Command::Run(SParams *params)
{
	if (this->func == NULL) return;
	this->func(params ? params : &this->by_default);
	if (params) delete params; //очищаем память, выделенную заранее
}

void Command::Run(SParams &params)
{
	this->func(&params ? &params : &this->by_default);
}

Vec2 Vec(long double x, long double y)
{
	return Vec2(x, y);
}

Vec3 Vec(long double x, long double y, long double z)
{
	return Vec3(x, y, z);
}


