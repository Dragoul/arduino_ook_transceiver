
#ifndef __OOK_TRANSCEIVER_PACKET_H__
#define __OOK_TRANSCEIVER_PACKET_H__

#include <stdint.h>

struct WireMessage
{
    uint16_t us_delay_high;
    uint16_t us_delay_low;
};

class OokTransceiverPacket
{
  public:
    virtual ~OokTransceiverPacket() {}
    virtual void preparePacket() = 0;
    virtual const WireMessage *getWireData() const = 0;
    virtual uint16_t getWireDataSize() const = 0;
    virtual uint8_t getTransmissionRepeats() const = 0;
};

#endif //__OOK_TRANSCEIVER_PACKET_H__