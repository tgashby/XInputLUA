#ifndef XINPUTLUA_H
#define XINPUTLUA_H

#include "IXInputLUA.h"
#include "love_helpers.h"

IXInputLUA* createXInputLUA();

#ifdef _WINDLL
#define LUA_EXPORT __declspec(dllexport)
#include "Windows\XInputLUA.h"
#endif


#endif