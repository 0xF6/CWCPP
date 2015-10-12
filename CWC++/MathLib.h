#pragma once
#include "Types.h"

#define Pi 3.141592653589793238462643383279 

struct CoordSystem {
	Vec3 Front; //Y
	Vec3 Up; //Z
	Vec3 Right; //X
	__forceinline void norm()
	{
		Front.norm();
		Up.norm();
		Right.norm();
	}
};

RECT MakeRect(Vec2 Pos, Vec2 Size);

void MakeVectors(RECT rect, Vec2 &Pos, Vec2 &Size);
void ClientWindowToScreen(RECT &rect, HWND hWnd);
void ScreenToClientWindow(RECT &rect, HWND hWnd);

bool IsInRect(RECT rect, Vec2 Pos);

Vec2 ToSizeWithRatio(Vec2 SizeIn, Vec2 SizeNeed, bool Maximum);

extern __forceinline long double AngleByPos(const Vec2 &Pos);

Vec2 RotateVector(const Vec2 &BV, const long double &angle);

Vec3 RotateVector(const Vec3 &BaseVector, const Vec3 &RotationVector);

Vec3 RotateVector(const Vec3 &BV, const Vec3 &Hub, const long double angle);

CoordSystem RotateLocale(const CoordSystem &CS, const Vec3 Rotation, bool local = true);