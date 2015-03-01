
#ifndef __TELLSTICK_PACKET_H__
#define __TELLSTICK_PACKET_H__

#include <stdint.h>

struct WireMessage
{
    uint16_t ms_delay_high;
    uint16_t ms_delay_low;
};

class TellstickPacket
{
  public:
    virtual ~TellstickPacket() {}
    virtual void preparePacket() = 0;
    virtual const WireMessage *getWireData() const = 0;
    virtual uint16_t getWireDataSize() const = 0;
    virtual uint8_t getTransmissionRepeats() const = 0;
};

#endif //__TELLSTICK_H__