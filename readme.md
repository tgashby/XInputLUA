XinputLUA
=================

This is a drop in module replacement for Love's joystick module, which uses Xinput instead of SDL, in order to facilitate proper Xbox 360 controller support on Windows.

Usage
------------

###XinputLUA in your Love2d Project

For XInputLUA to be usable, you need to put XInputLUA.dll next to love.exe, in Love's folder.

Then, the easiest way to use XInputLUA is to disable Love2d's Joystick module in conf.lua, and then at the top of your main project do:

`love.joystick = require('XInputLUA.dll')`

After that, just use love.joystick as you did before.

###How To Build
You need the [Direct X SDK (June 2010)](http://www.microsoft.com/en-us/download/details.aspx?id=6812) installed with paths correctly set in order to build XinputLUA. It should go without saying that XinputLUA is Windows specific.

If that is done, everything should build straight out of the repo. If it doesn't, please drop me a line and let me know so I can investigate it.

###Lua Reference

XinputLUA supports all existing Love2d Joystick Module functions found [here](https://love2d.org/wiki/love.joystick).

#### Button and Axis IDs

XinputLUA stores button and axis data at the same indexes regardless of connected device. Future versions may support device specific id labels and interfaces provided by an external lua library. 

##### Buttons
1. A
2. B
3. X
4. Y
5. Left Trigger
6. Right Trigger
7. LB
8. RB
9. LS
10. RS
11. Back
12. Start

##### Axes
1. Left X
2. Left Y
3. Right X
4. Right Y
5. Left Trigger
6. Right Trigger

#### New Functions
`love.joystick.update()`

This function must be called at the beginning of `love.update()`. Because XInput is built around a polling system rather than an event system, this gets a new controller state, and fires off all state changes to love.joystickpressed() and love.joystickreleased()

`love.joystick.setTriggerThreshold(threshold)`

threshold: 1-255

Because the triggers are axes but can be used as buttons, Xinput for Love2d reports them as both types of inputs. This threshold is the axis value at which it will consider the trigger as pressed or released.

`love.joystick.setRumble(index, value)`

value: 0.0-1.0

This sets the left motor speed (the low frequency motor). 

`love.joystick.setVibrate(index, value)`

value: 0.0-1.0

This sets the right motor speed (the high frequency motor). 


Notes
-----

I've tested this with a GH2 Guitar and a normal wired Xbox 360 Controller. Both work great. You can also test with a testing app I've thrown together [here](https://github.com/mrcharles/love2d-x360-test)

For now, any 360 device connected reports 6 axes and 12 buttons. Not all of them will be functional, depending on your device. You can get an idea of what *will* work [here](http://msdn.microsoft.com/en-us/library/windows/desktop/hh405050(v=vs.85).aspx).

Xinput devices do not need to be 'opened', a paradigm which is being removed in Love 0.8.1. The open(), close(), and isOpen() functions exist but are mostly useless.

Because of reporting difference between SDL and Xinput, any existing controller code you have will be messed up. Axes specifically will be way wrong. I considered changing XInputLUA to match the SDL implementation, but decided it was best to leave the input as close to raw as possible so that people used to Xbox360 development or people who've used Xinput before will see values consistent with what they are used to.

I've started a thread on [Love2d.org](https://love2d.org/forums/viewtopic.php?f=4&t=10356) where I talk about the development of this project. Feel free to join in.

Coming Soon
-----------

* DONE! Vibration Support 
* Deadzone support
* Lua library for higher level controller management
* D-Pad smoothing to combat terrible xbox 360 dpad issues.
* Stick transformation libraries. 