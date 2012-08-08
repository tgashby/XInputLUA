#ifndef XINPUTLUA_H
#define XINPUTLUA_H

#include "IXInputLUA.h"

IXInputLUA* createXInputLUA();

#ifdef _WINDLL
#include "Windows\XInputLUA.h"
#endif


#endif