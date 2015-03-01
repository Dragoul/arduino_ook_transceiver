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
    uint16_t getWireDataSize() const;
    uint8_t getTransmissionRepeats() const;


    // NexaLightSwitchPacket interface
    void setHouse(uint8_t house);
    void setUnit(uint8_t unit);
    void setLightState(state_t state);

  private:
    void setBit(WireMessage *ook_msg,
                uint16_t ms_high,
                uint16_t ms_low);

    void appendData(WireMessage *ook_msg,
                    uint16_t *msg_index,
                    uint16_t data,
                    uint8_t data_width);

    WireMessage *ook_msg;
    uint8_t house;
    uint8_t unit;
    state_t state;
};

#endif //__NEXA_LIGHT_SWITCH_PACKET_H__