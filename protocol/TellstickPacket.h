
#ifndef __TELLSTICK_PACKET_H__
#define __TELLSTICK_PACKET_H__

struct WireMessage
{
	int		ms_delay_high;
	int		ms_delay_low;
};

class TellstickPacket
{
  public:
	virtual ~TellstickPacket() {}
	virtual void preparePacket() = 0;
	virtual const WireMessage *getWireData() const = 0;
	virtual unsigned int getWireDataSize() const = 0;
	virtual unsigned int getTransmitionRepeats() const = 0;
};

#endif //__TELLSTICK_H__