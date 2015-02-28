
#include "NexaLightSwitchPacket.h"

#define UNKOWN_DATA_VALUE 6
#define SYNC_MULTIPLIER   32
#define LONG_MULTIPLIER   3
#define T_SHORT           400
#define T_LONG            T_SHORT * LONG_MULTIPLIER
#define T_SYNC_LOW        T_SHORT * SYNC_MULTIPLIER

#define HOUSE_BIT_WIDTH   4
#define UNIT_BIT_WIDTH    4
#define UKNOWN_BIT_WIDTH  3
#define STATE_BIT_WIDTH   1

#define NEXA_LIGHT_SWITCH_MESSAGE_SIZE (HOUSE_BIT_WIDTH +      \
                                        UNIT_BIT_WIDTH +       \
                                        UKNOWN_BIT_WIDTH +     \
                                        STATE_BIT_WIDTH) * 2 + \
                                        1 // For sync

NexaLightSwitchPacket::NexaLightSwitchPacket()
{
	ook_msg = new WireMessage[NEXA_LIGHT_SWITCH_MESSAGE_SIZE];
    house = 0;
    unit = 0;
    state = LIGHT_OFF;
}

NexaLightSwitchPacket::~NexaLightSwitchPacket()
{
	delete ook_msg;
}

void NexaLightSwitchPacket::setBit(WireMessage *ook_msg, unsigned int ms_high, unsigned int ms_low)
{
    ook_msg->ms_delay_high = ms_high;
    ook_msg->ms_delay_low = ms_low;
}

void NexaLightSwitchPacket::appendData(WireMessage *ook_msg, unsigned int *msg_index, unsigned int data, unsigned int data_width)
{
    for (int i = 0; i < data_width; i++) {
        setBit(&ook_msg[(*msg_index)++], T_SHORT, T_LONG);
        if(data & 0x01) {
            setBit(&ook_msg[(*msg_index)++], T_LONG, T_SHORT);
        } else {
            setBit(&ook_msg[(*msg_index)++], T_SHORT, T_LONG);
        }
        data = data >> 1;
    }
}

void NexaLightSwitchPacket::preparePacket()
{
	unsigned int msg_index = 0;

    appendData(ook_msg, &msg_index, house, HOUSE_BIT_WIDTH);
    appendData(ook_msg, &msg_index, unit, UNIT_BIT_WIDTH);
    appendData(ook_msg, &msg_index, UNKOWN_DATA_VALUE, UKNOWN_BIT_WIDTH);
    appendData(ook_msg, &msg_index, state == LIGHT_ON, STATE_BIT_WIDTH);

    setBit(&ook_msg[msg_index++], T_SHORT, T_SYNC_LOW);
}

//////////////////////////////////////////////////////////////////////////
// Getters

const WireMessage *NexaLightSwitchPacket::getWireData() const
{
	return ook_msg;
}

unsigned int NexaLightSwitchPacket::getWireDataSize() const
{
	return NEXA_LIGHT_SWITCH_MESSAGE_SIZE;
}

unsigned int NexaLightSwitchPacket::getTransmitionRepeats() const
{
	return 3;
}

//////////////////////////////////////////////////////////////////////////
// Setters

void NexaLightSwitchPacket::setHouse(unsigned int house)
{
    this->house = house;
}

void NexaLightSwitchPacket::setUnit(unsigned int unit)
{
    this->unit = unit;
}

void NexaLightSwitchPacket::setLightState(state_t state)
{
    this->state = state;
}