#ifndef WINDOWS_XINPUTLUA_H
#define WINDOWS_XINPUTLUA_H

#include "../XInputLUA.h"
#include <Windows.h>
#include <XInput.h>

namespace Windows 
{
#define MAXCONTROLLERS 4


	class XInputLUA: public IXInputLUA
	{
	private:
		bool padValid[MAXCONTROLLERS];

		unsigned char triggerPressThreshold; 

		XINPUT_STATE currentState[MAXCONTROLLERS];
		XINPUT_STATE oldState[MAXCONTROLLERS];

		XINPUT_CAPABILITIES caps[MAXCONTROLLERS];

	public:
		XInputLUA();
		virtual ~XInputLUA();

		float clampval(float x);
		bool wasButtonDown(int index, int button);
		bool getButtonDown(int index, int button);
		void notifyRelease(lua_State *L, int index, int button);
		void notifyPress(lua_State *L, int index, int button);

		// Implements Module.
		const char * getName() const;

		void update(lua_State *L);
		void setTriggerThreshold(int threshold) { triggerPressThreshold = threshold; }
		int getNumJoysticks();
		const char * getName(int index);
		bool open(int index);
		bool isOpen(int index);
		int getNumAxes(int index);
		int getNumBalls(int index);
		int getNumButtons(int index);
		int getNumHats(int index);
		float getAxis(int index, int axis);
		int getAxes(lua_State * L);
		int getBall(lua_State * L);
		bool isDown(int index, int * buttonlist);
		const char* getHat(int index, int hat);
		void close(int index);

	private:

	}; // Joystick
}

#endif