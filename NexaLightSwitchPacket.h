/*
 *
 * Works according to the NEXA/PROOVE Remote Protocol
 *
 * URL: http://svn.telldus.com/svn/rf_ctrl/nexa_2_6_driver/trunk/NexaProtocol.txt
 *
 */

#ifndef __NEXA_LIGHT_SWITCH_PACKET_H__
#define __NEXA_LIGHT_SWITCH_PACKET_H__

#include "protocol/TellstickPacket.h"

typedef enum {
    LIGHT_ON,
    LIGHT_OFF
} state_t;

class NexaLightSwitchPacket : public TellstickPacket
{
  public:

    NexaLightSwitchPacket();
    ~NexaLightSwitchPacket();

    // TellstickPacket interface
    void preparePacket();
    const WireMessage *getWireData() const;
    unsigned int getWireDataSize() const;
    unsigned int getTransmitionRepeats() const;

    // NexaLightSwitchPacket interface
    void setHouse(unsigned int house);
    void setUnit(unsigned int unit);
    void setLightState(state_t state);

  private:
    void setBit(WireMessage *ook_msg,
                unsigned int ms_high,
                unsigned int ms_low);

    void appendData(WireMessage *ook_msg,
                    unsigned int *msg_index,
                    unsigned int data,
                    unsigned int data_width);

    WireMessage *ook_msg;
    unsigned int house;
    unsigned int unit;
    state_t state;
};

#endif //__NEXA_LIGHT_SWITCH_PACKET_H__