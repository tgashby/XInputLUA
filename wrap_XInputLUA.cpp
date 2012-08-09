/**
* Copyright (c) 2006-2012 LOVE Development Team
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
**/

#include "XInputLUA.h"
#include "wrap_XInputLUA.h"

#include <exception>

namespace Wrapper
{
	static IXInputLUA * instance = 0;

	int w_update(lua_State * L)
	{
		instance->update(L);
		return 0;
	}

	int w_getNumJoysticks(lua_State * L)
	{
		lua_pushinteger(L, instance->getNumJoysticks());
		return 1;
	}

	int w_getName(lua_State * L)
	{
		int index = luaL_checkint(L, 1)-1;
		lua_pushstring(L, instance->getName(index));
		return 1;
	}

	int w_setTriggerThreshold(lua_State * L)
	{
		int threshold = luaL_checkint(L, 1);
		instance->setTriggerThreshold(threshold);
		return 0;
	}

	int w_open(lua_State * L)
	{
		int index = luaL_checkint(L, 1)-1;
		lua_pushboolean(L, instance->open(index) ? 1 : 0);
		return 1;
	}

	int w_isOpen(lua_State * L)
	{
		int index = luaL_checkint(L, 1)-1;
		lua_pushboolean(L, instance->isOpen(index) ? 1 : 0);
		return 1;
	}

	int w_getNumAxes(lua_State * L)
	{
		int index = luaL_checkint(L, 1)-1;
		lua_pushinteger(L, instance->getNumAxes(index));
		return 1;
	}

	int w_getNumBalls(lua_State * L)
	{
		int index = luaL_checkint(L, 1)-1;
		lua_pushinteger(L, instance->getNumBalls(index));
		return 1;
	}

	int w_getNumButtons(lua_State * L)
	{
		int index = luaL_checkint(L, 1)-1;
		lua_pushinteger(L, instance->getNumButtons(index));
		return 1;
	}

	int w_getNumHats(lua_State * L)
	{
		int index = luaL_checkint(L, 1)-1;
		lua_pushinteger(L, instance->getNumHats(index));
		return 1;
	}

	int w_getAxis(lua_State * L)
	{
		int index = luaL_checkint(L, 1)-1;
		int axis = luaL_checkint(L, 2)-1;
		lua_pushnumber(L, instance->getAxis(index, axis));
		return 1;
	}

	int w_getAxes(lua_State * L)
	{
		return instance->getAxes(L);
	}

	int w_getBall(lua_State * L)
	{
		return instance->getBall(L);
	}

	int w_isDown(lua_State * L)
	{
		int index = luaL_checkint(L, 1)-1;
		unsigned int num = lua_gettop(L);
		int * buttonlist = new int[num];
		unsigned int counter = 0;

		for (unsigned int i = 1; i < num; i++)
		{
			buttonlist[counter++] = (int) luaL_checknumber(L, i+1)-1;
		}
		buttonlist[counter] = -1;

		lua_pushboolean(L, instance->isDown(index, buttonlist) ? 1 : 0);
		delete[] buttonlist;
		return 1;
	}

	int w_getHat(lua_State * L)
	{
		int index = luaL_checkint(L, 1)-1;
		int hat = luaL_checkint(L, 2)-1;

		const char * direction = instance->getHat(index, hat);

		lua_pushstring(L, direction);

		return 1;
	}

	int w_close(lua_State * L)
	{
		int index = luaL_checkint(L, 1)-1;
		instance->close(index);
		return 0;
	}

	// List of functions to wrap.
	static const luaL_Reg functions[] = {
		{ "update", w_update },
		{ "getNumJoysticks", w_getNumJoysticks },
		{ "getName", w_getName },
		{ "open", w_open },
		{ "isOpen", w_isOpen },
		{ "getNumAxes", w_getNumAxes },
		{ "getNumBalls", w_getNumBalls },
		{ "getNumButtons", w_getNumButtons },
		{ "getNumHats", w_getNumHats },
		{ "getAxis", w_getAxis },

		{ "getAxes", w_getAxes },
		{ "getBall", w_getBall },

		{ "isDown", w_isDown },
		{ "getHat", w_getHat },
		{ "close", w_close },
		{ 0, 0 }
	};

	extern "C" int luaopen_XInputLUA(lua_State * L)
	{
		if (instance == 0)
		{
			try
			{
				instance = createXInputLUA();
			}
			catch (std::exception & e)
			{
				return luaL_error(L, e.what());
			}
		}
		//else
			//instance->retain();


		luaL_register(L, "XInputLUA", functions);
		
		return 1;
	}
} 
