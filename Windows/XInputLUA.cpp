
#include <Windows.h>
#include "XInputLUA.h"
//#include "love_helpers.h"

extern "C" {
	#include "lua.h"
	#include "lauxlib.h"
}

// STD
#include <cmath>

namespace Windows
{
	XInputLUA::XInputLUA()
	{
		for(int i = 0; i < MAXCONTROLLERS; i++)
		{
			padValid[i] = false;
		}
		triggerPressThreshold = 30;

		memset(oldState, 0, sizeof(XINPUT_STATE) * MAXCONTROLLERS);
		memset(currentState, 0, sizeof(XINPUT_STATE) * MAXCONTROLLERS);
		memset(caps, 0, sizeof(XINPUT_CAPABILITIES) * MAXCONTROLLERS);
	}

	XInputLUA::~XInputLUA()
	{
	}

	void XInputLUA::notifyRelease(lua_State *L, int index, int button)
	{
		lua_getglobal(L, "love"); // s + 1
		if( lua_istable(L, -1) )
		{
			lua_pushstring(L, "handlers"); // s + 1
			lua_gettable(L, -2);
			if( lua_istable(L, -1) )
			{
				lua_pushstring(L, "joystickreleased"); //s + 1
				lua_gettable(L, -2);

				if( lua_isfunction(L, -1) )
				{
					lua_pushinteger( L, index+1 ); //s + 1
 					lua_pushinteger( L, button+1  ); //s + 1

					lua_call(L, 2, 0); // s - 3
					lua_pop(L, 2);
				}
				else
					lua_pop(L, 3);

			}
			else
				lua_pop(L, 2);
		}
		else
			lua_pop(L, 1);
	}

	void XInputLUA::notifyPress(lua_State *L, int index, int button)
	{
		lua_getglobal(L, "love"); // s + 1
		if( lua_istable(L, -1) )
		{
			lua_pushstring(L, "handlers"); // s + 1
			lua_gettable(L, -2);
			if( lua_istable(L, -1) )
			{
				lua_pushstring(L, "joystickpressed"); //s + 1
				lua_gettable(L, -2);

				if( lua_isfunction(L, -1) )
				{
					lua_pushinteger( L, index+1 ); //s + 1
 					lua_pushinteger( L, button+1 ); //s + 1

					lua_call(L, 2, 0); // s - 3
					lua_pop(L, 2);
				}
				else
					lua_pop(L, 3);

			}
			else
				lua_pop(L, 2);
		}
		else
			lua_pop(L, 1);
	}

	void XInputLUA::update(lua_State *L)
	{
		//copy old state
		memcpy(oldState, currentState, sizeof(XINPUT_STATE) * MAXCONTROLLERS);

		//update all pads
		for( int i = 0; i < MAXCONTROLLERS; i++ )
		{
			DWORD ret = XInputGetState(i, currentState + i );

			if( ret == ERROR_SUCCESS ) //pad connected
			{
				if(!padValid[i]) 
				{
					//pad is just connected, fill out caps
					XInputGetCapabilities(i, 0, caps + i);
				}
				padValid[i] = true;
			}
			else
			{
				if(padValid[i]) //send release notifications on all held buttons
				{
					for( int b = 0; b < getNumButtons(i); b++ )
					{
						if( getButtonDown(i, b) )
						{
							notifyRelease(L, i, b);
						}
					}
					memset(oldState + i, 0, sizeof( XINPUT_STATE ) );
					memset(currentState + i, 0, sizeof( XINPUT_STATE ) );
					memset(caps + i, 0, sizeof( XINPUT_CAPABILITIES ) );
				}
				padValid[i] = false;
			}

		}

		//send press/release notifications
		for( int i = 0; i < MAXCONTROLLERS; i++ )
		{
			if( padValid[i] )
			{
				for( int b = 0; b < getNumButtons(i); b++ )
				{
					if(getButtonDown(i, b) && !wasButtonDown(i, b))
						notifyPress(L, i, b);
					else if( wasButtonDown(i, b) && !getButtonDown(i, b)) 
						notifyRelease(L, i, b);
				}
			}

		}

	}

	const char * XInputLUA::getName() const
	{
		return "love.XInputLUA.xinput";
	}

	int XInputLUA::getNumJoysticks()
	{
		int valid = 0;

	
		for(int i = 0; i < MAXCONTROLLERS; i++)
		{
			if(padValid[i])
				valid++;
		}

		return valid;
	}

	const char * XInputLUA::getName(int index)
	{
		if(padValid[index])
		{
			switch( caps[index].SubType )
			{
			case XINPUT_DEVSUBTYPE_GAMEPAD:
				return "xbox gamepad";
			case XINPUT_DEVSUBTYPE_WHEEL:
				return "xbox wheel";
			case XINPUT_DEVSUBTYPE_ARCADE_STICK:
				return "xbox arcade stick";
			case XINPUT_DEVSUBTYPE_FLIGHT_SICK: // (sic) lol
				return "xbox flight stick";
			case XINPUT_DEVSUBTYPE_DANCE_PAD:
				return "xbox dance pad";
			case XINPUT_DEVSUBTYPE_GUITAR:
			case 0x07:
				return "xbox guitar";
			case XINPUT_DEVSUBTYPE_DRUM_KIT:
				return "xbox drumkit";
			default: 
				return "unknown device";
			}
		}
		return "no device";
	}

	bool XInputLUA::open(int index)
	{
		return padValid[index];
	}

	bool XInputLUA::isOpen(int index)
	{
		return padValid[index];
	}

	int XInputLUA::getNumAxes(int index)
	{
		if(padValid[index])
		{
			//switch( caps[index].SubType )
			//{
			//case XINPUT_DEVSUBTYPE_GAMEPAD:
			//	return 6;
			//case XINPUT_DEVSUBTYPE_WHEEL:
			//	return 3;
			//case XINPUT_DEVSUBTYPE_ARCADE_STICK:
			//	return 0;
			//case XINPUT_DEVSUBTYPE_FLIGHT_SICK: // (sic) lol
			//	return 6;
			//case XINPUT_DEVSUBTYPE_DANCE_PAD:
			//	return 0;
			//case XINPUT_DEVSUBTYPE_GUITAR:
			//	return 4;
			//case XINPUT_DEVSUBTYPE_DRUM_KIT:
			//	return 0;
			//}
			return 6;
		}
		return 0;
	}

	int XInputLUA::getNumBalls(int index)
	{
		return 0;
	}

	int XInputLUA::getNumButtons(int index)
	{
		if(padValid[index])
		{
			//switch( caps[index].SubType )
			//{
			//case XINPUT_DEVSUBTYPE_GAMEPAD:
			//	return 10;
			//case XINPUT_DEVSUBTYPE_WHEEL:
			//	return 10;
			//case XINPUT_DEVSUBTYPE_ARCADE_STICK:
			//	return 12;
			//case XINPUT_DEVSUBTYPE_FLIGHT_SICK: // (sic) lol
			//	return 10;
			//case XINPUT_DEVSUBTYPE_DANCE_PAD:
			//	return 6;
			//case XINPUT_DEVSUBTYPE_GUITAR:
			//	return 10;
			//case XINPUT_DEVSUBTYPE_DRUM_KIT:
			//	return 10;
			//}
			return 12;
		}
		return 0;
	}

	int XInputLUA::getNumHats(int index)
	{
		if(padValid[index])
			return 1;
		return 0;
	}

	float XInputLUA::clampval(float x)
	{
		if (fabs((double)x) < 0.01) return 0.0f;
		if (x < -0.99f) return -1.0f;
		if (x > 0.99f) return 1.0f;
		return x;
	}

	float XInputLUA::getAxis(int index, int axis)
	{
		if( padValid[index] )
		{
			float val = 0.f;
			float max = 1.f;	
			switch(axis)
			{
			case 0: // LX
				val = currentState[index].Gamepad.sThumbLX;
				max = 32768.0f;
				break;
			case 1: // LY
				val = currentState[index].Gamepad.sThumbLY;
				max = 32768.0f;
				break;
			case 2: // RX
				val = currentState[index].Gamepad.sThumbRX;
				max = 32768.0f;
				break;
			case 3: // RY
				val = currentState[index].Gamepad.sThumbRY;
				max = 32768.0f;
				break;
			case 4: // LT
				val = currentState[index].Gamepad.bLeftTrigger;
				max = 255.f;
				break;
			case 5: // RT
				val = currentState[index].Gamepad.bRightTrigger;
				max = 255.f;
				break;
			default: 
				break;
			}

			return clampval(val/max);
		}
		return 0.f;
	}

	int XInputLUA::getAxes(lua_State * L)
	{
		love::luax_assert_argc(L, 1, 1);
		int index = (int)lua_tointeger(L, 1) - 1;

		int num = getNumAxes(index);

		for (int i = 0; i<num; i++)
			lua_pushnumber(L, getAxis(index, i));
		return num;
	}

	int XInputLUA::getBall(lua_State * L)
	{
		return 0;
	}

	bool XInputLUA::wasButtonDown(int index, int button)
	{
		if(padValid[index])
		{
			switch(button)
			{
			case 0:
				return (oldState[index].Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0; 
			case 1:
				return (oldState[index].Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0; 
			case 2:
				return (oldState[index].Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0; 
			case 3:
				return (oldState[index].Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0; 
			case 4:
				return (oldState[index].Gamepad.bLeftTrigger > triggerPressThreshold); 
			case 5:
				return (oldState[index].Gamepad.bRightTrigger > triggerPressThreshold); 
			case 6:
				return (oldState[index].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0; 
			case 7:
				return (oldState[index].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0; 
			case 8:
				return (oldState[index].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0; 
			case 9:
				return (oldState[index].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0; 
			case 10:
				return (oldState[index].Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0; 
			case 11:
				return (oldState[index].Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0; 
			}
		
		}
		return false;
	}

	bool XInputLUA::getButtonDown(int index, int button)
	{
		if(padValid[index])
		{
			switch(button)
			{
			case 0:
				return (currentState[index].Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0; 
			case 1:
				return (currentState[index].Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0; 
			case 2:
				return (currentState[index].Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0; 
			case 3:
				return (currentState[index].Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0; 
			case 4:
				return (currentState[index].Gamepad.bLeftTrigger > triggerPressThreshold); 
			case 5:
				return (currentState[index].Gamepad.bRightTrigger > triggerPressThreshold); 
			case 6:
				return (currentState[index].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0; 
			case 7:
				return (currentState[index].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0; 
			case 8:
				return (currentState[index].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0; 
			case 9:
				return (currentState[index].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0; 
			case 10:
				return (currentState[index].Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0; 
			case 11:
				return (currentState[index].Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0; 
			}
		
		}
		return false;
	}


	bool XInputLUA::isDown(int index, int * buttonlist)
	{	
		int num = getNumButtons(index);

		for (int button = *buttonlist; button != -1; button = *(++buttonlist))
		{
			if (button >= 0 && button < num && getButtonDown(index, button))
				return true;
		}

		return false;
	}

	//this is a temporary and extremely naive implementation of the d-pad on xbox which needs to be significantly
	//more complex to handle how terrible the d-pad is. 
	const char* XInputLUA::getHat(int index, int hat)
	{
		if(padValid[index])
		{
			if (hat >= getNumHats(index))
				return "c";
		
			bool up = (currentState[index].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
			bool down = (currentState[index].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
			bool left = (currentState[index].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
			bool right = (currentState[index].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
			
			if( up && right )
				return "ru";
			else if( up && left )
				return "lu";
			else if( down && right) 
				return "rd";
			else if( down && left )
				return "ld";
			else if( up ) 
				return "u";
			else if( down )
				return "d";
			else if( right )
				return "r";
			else if( left )
				return "l";

		}
		return "c";
	}

	void XInputLUA::close(int index)
	{
	}

}

IXInputLUA* createXInputLUA()
{
	return new Windows::XInputLUA;
}

