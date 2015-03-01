
#ifndef __OOK_TRANSCEIVER_H__
#define __OOK_TRANSCEIVER_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class OokTransceiverPacket;

class OokTransceiver
{
//variables
public:
protected:
private:
	byte tx_pin;

//functions
public:
	OokTransceiver(byte tx_pin);
	~OokTransceiver();
	
	void init();
	
	void sendMessage(const OokTransceiverPacket &packet) const;
	
protected:
private:
	OokTransceiver( const OokTransceiver &c );
	OokTransceiver& operator=( const OokTransceiver &c);
	
	void setHighFor(uint16_t us_duration) const;
	void setLowFor(uint16_t us_duration) const;
};

#endif //__OOK_TRANSCEIVER_H__
