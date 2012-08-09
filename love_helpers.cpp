#include "love_helpers.h"

namespace love
{
	int luax_assert_argc(lua_State * L, int min, int max)
	{
		int argc = lua_gettop(L);
		if ( argc < min || argc > max)
			return luaL_error(L, "Incorrect number of arguments. Got [%d], expected [%d-%d]", argc, min, max);
		return 0;
	}

}
