#pragma once
#include "Globals.h"
#include <Windows.h>

// Convert to DIPs
template<typename T> float ToDIPsX(T x)
{
	return static_cast<float>(x / (dpiX / 96.f) * screenScale);
}

template<typename T> float ToDIPsY(T y)
{
	return static_cast<float>(y / (dpiY / 96.f) * screenScale);
}

template<typename T> float FromDIPsX(T x)
{
	return static_cast<float>(x * (dpiX / 96.f) / screenScale);
}

template<typename T> float FromDIPsY(T y)
{
	return static_cast<float>(y * (dpiY / 96.f) / screenScale);
}