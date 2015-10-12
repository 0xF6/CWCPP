#include <windows.h>
#include "Types.h"
#include "MathLib.h"

#pragma region Rectangles and Sizes

RECT MakeRect(Vec2 Pos, Vec2 Size)
{
	RECT rect;
	rect.left = (LONG) Pos.X;
	rect.top = (LONG) Pos.Y;
	rect.right = (LONG) (Pos.X + Size.X);
	rect.bottom = (LONG) (Pos.Y + Size.Y);
	return rect;
}

void MakeVectors(RECT rect, Vec2 &Pos, Vec2 &Size)
{
	Pos.X =  (double) rect.left;
	Pos.Y =  (double) rect.top;
	Size.X = (double) (rect.right - rect.left);
	Size.Y = (double) (rect.bottom - rect.top);
}

bool IsInRect(RECT rect, Vec2 Pos)
{
	return ((rect.left < Pos.X && Pos.X < rect.right) && (rect.top < Pos.Y && Pos.Y < rect.bottom));
}

//Возвращает размер, приближенный к заданному
Vec2 ToSizeWithRatio(Vec2 SizeIn, //Входящий размер
			Vec2 SizeNeed, //Размер, который примерно нужен
			bool Maximum //Будет ли исходящий размер превышать нужный
			)
{
	bool XGreater = (SizeNeed.X > SizeNeed.Y); //Если значение X больше Y
	
	double Scale = 1; //Скаляр

	if (Maximum)
	{
		if (XGreater)
		{
			Scale =  SizeNeed.Y / SizeIn.Y;
		}
		else
		{
			Scale =  SizeNeed.X / SizeIn.X;
		}
	}
	else
	{
		if (!XGreater)
		{
			Scale =  SizeNeed.Y / SizeIn.Y;
		}
		else
		{
			Scale =  SizeNeed.X / SizeIn.X;
		}
	}
	return SizeIn * Scale; //Возвращает размер, умноженый на скаляр
}

void ClientWindowToScreen(RECT &rect, HWND hWnd)
{
	RECT temp_rect;
	GetWindowRect(hWnd, &temp_rect);
	LONG x = temp_rect.left;
	LONG y = temp_rect.top;
	rect.left += x;
	rect.top += y;
	rect.right += x;
	rect.bottom += y;
}

void ScreenToClientWindow(RECT &rect, HWND hWnd)
{
	RECT temp_rect;
	GetWindowRect(hWnd, &temp_rect);
	LONG x = temp_rect.left;
	LONG y = temp_rect.top;
	rect.left -= x;
	rect.top -= y;
	rect.right -= x;
	rect.bottom -= y;
}

#pragma endregion

#pragma region Angles and Rotations

long double AngleByPos(const Vec2 &Pos)
{
	long double start_angle = acos(Pos.X);
	if (Pos.Y >= 0)
		return start_angle;
	else
		return (( ((long double) Pi) * 2) - start_angle);
}


Vec2 RotateVector(const Vec2 &BV, const long double &angle)
{
	long double st_angle = AngleByPos(BV);
	return Vec(cos(st_angle + angle), sin(st_angle + angle));
}


Vec3 RotateVector(const Vec3 &BV, const Vec3 &RV)
{
	Vec3 result = BV;
	if (RV.X == 0 && RV.Y == 0 && RV.Z == 0)
		return result;
	{//Вычисление
		Vec2 RotZ = RotateVector(Vec(BV.X, BV.Y), RV.Z);
		result = Vec(RotZ.X, RotZ.Y, BV.Z);
		Vec2 RotX = RotateVector(Vec(result.Y, result.Z), RV.X);
		result = Vec(result.X, RotX.X, RotX.Y);
		Vec2 RotY = RotateVector(Vec(result.X, result.Z), RV.Y);
		result = Vec(RotY.X, result.Y, RotY.Y);
	}
	return result;
}

Vec3 RotateVector(const Vec3 &BV, const Vec3 &Hub, const long double angle)
{
	Vec3 NormalHub;
	//Step 0 - Calculating hub
	{
		//Если расстояние между осью, повёрнутой назад и вектором меньше, чем расстояние
		//Обычной оси и вектором, то мы будем использовать обёрнутую назад ось
		if ((Hub - BV).len() > (-Hub - BV).len() )
		{
			NormalHub = -Hub;
		}
	}
	//Step 1 - Normalisizing hub
	{
		NormalHub.norm();
		NormalHub *= BV.len();
	}
	//Step 2 - Getting Rotation Vector
	Vec3 RotationVector;
	{
		RotationVector = Hub - BV;
	}
	//Step 2.5 - "Projection"
	{
		//Calculating Nearest hub
		Vec3 NearestHub = NormalHub;
		NearestHub.norm();
		{
			long double lx, ly, lz;
			bool bx = false, by = false, bz = false;
			lx = (NearestHub - Vec(1, 0, 0)).len();
			ly = (NearestHub - Vec(0, 1, 0)).len();
			lz = (NearestHub - Vec(0, 0, 1)).len();
			if (lx <= ly && lx <= lz)
			{
				bx = true;
			}
			if (ly <= lx && ly <= lz)
			{
				by = true;
			}
			if (lz <= lx && lz <= ly)
			{
				bz = true;
			}
			if (bx)
				NearestHub = Vec(1, 0, 0);
			else if (by)
				NearestHub = Vec(0, 1, 0);
			else if (bz)
				NearestHub = Vec(0, 0, 1);
		}
		//Getting angle from the nearest hub and gived hub
		{

		}
	}
	return Vec3();
}

CoordSystem RotateLocale(const CoordSystem &CS, const Vec3 Rotation, bool local)
{
	if (local)
	{

	}
	else
	{

	}
	return CoordSystem();
}

#pragma endregion