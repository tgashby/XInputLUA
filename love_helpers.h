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

/**
*
*   File assembled by Charles Randall
*
*	These functions are pulled from love2d's source, for easy intercompatibility 
*   as well as the fact that they are just awesome. 
*
**/

#ifndef LOVE_HELPERS_H
#define LOVE_HELPERS_H

extern "C" 
{
	#include "lua.h"
	#include "lauxlib.h"
}

namespace love
{
/**
	* Require at least 'min', but more than 'max' items on the stack.
	* @param L The Lua state.
	* @param min The minimum number of items on the stack.
	* @param max The maximum number of items on the stack.
	* @return Zero if conditions are met, otherwise a Lua error (longjmp).
	**/
	int luax_assert_argc(lua_State * L, int min, int max);

}

#endif
