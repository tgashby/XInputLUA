#ifndef IXINPUTLUA_H
#define IXINPUTLUA_H

class IXInputLUA
{
public:
	const char * getName() const = 0;

	void update(lua_State *L) = 0;
	void setTriggerThreshold(int threshold) = 0;
	int getNumJoysticks() = 0;
	const char * getName(int index) = 0;
	int getNumAxes(int index) = 0;
	int getNumButtons(int index) = 0;
	int getNumHats(int index) = 0;
	float getAxis(int index, int axis) = 0;
	int getAxes(lua_State * L) = 0;
	bool isDown(int index, int * buttonlist) = 0;
	Hat getHat(int index, int hat) = 0;

	//the following are for Love backwards compatibility with Joystick module. 
	bool open(int index) = 0;
	bool isOpen(int index) = 0;
	int getNumBalls(int index) = 0;
	int getBall(lua_State * L) = 0;
	void close(int index) = 0;

};

#endif