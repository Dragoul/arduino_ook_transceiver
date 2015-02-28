/* 
* Tellstick.h
*
* Created: 2014-12-29 11:37:25
* Author: Erik Hägglund
*/


#ifndef __TELLSTICK_H__
#define __TELLSTICK_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class TellstickPacket;

class Tellstick
{
//variables
public:
protected:
private:

//functions
public:
	Tellstick(byte pin_data);
	~Tellstick();
	
	void init();
	
	void sendMessage(const TellstickPacket &packet) const;
	
protected:
private:
	Tellstick( const Tellstick &c );
	Tellstick& operator=( const Tellstick &c);
	
	void setHighFor(uint16_t ms_duration) const;
	void setLowFor(uint16_t ms_duration) const;
	
	byte pin_data;
}; //Tellstick

#endif //__TELLSTICK_H__
