#ifndef IXINPUTLUA_H
#define IXINPUTLUA_H

extern "C"
{
	#include "lua.h"
}

class IXInputLUA
{
public:
	virtual const char * getName() const = 0;

	virtual void update(lua_State *L) = 0;
	virtual void setTriggerThreshold(int threshold) = 0;
	virtual int getNumJoysticks() = 0;
	virtual const char * getName(int index) = 0;
	virtual int getNumAxes(int index) = 0;
	virtual int getNumButtons(int index) = 0;
	virtual int getNumHats(int index) = 0;
	virtual float getAxis(int index, int axis) = 0;
	virtual int getAxes(lua_State * L) = 0;
	virtual bool isDown(int index, int * buttonlist) = 0;
	virtual const char* getHat(int index, int hat) = 0;
	virtual void setVibrate(int index, lua_Number pct) = 0; 
	virtual void setRumble(int index, lua_Number pct) = 0; 

	//the following are for Love backwards compatibility with Joystick module. 
	virtual bool open(int index) = 0;
	virtual bool isOpen(int index) = 0;
	virtual int getNumBalls(int index) = 0;
	virtual int getBall(lua_State * L) = 0;
	virtual void close(int index) = 0;

};

#endif