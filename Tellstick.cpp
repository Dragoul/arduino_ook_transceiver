
#include "Tellstick.h"
#include "protocol/TellstickPacket.h"

Tellstick::Tellstick(byte pin_data)
{
    this->pin_data = pin_data;
}

Tellstick::~Tellstick()
{
}

void Tellstick::init()
{
}

void Tellstick::setHighFor(uint16_t ms_duration) const
{
    digitalWrite(pin_data, HIGH);
    delayMicroseconds(ms_duration);
}
void Tellstick::setLowFor(uint16_t ms_duration) const
{
    digitalWrite(pin_data, LOW);
    delayMicroseconds(ms_duration);
}

void Tellstick::sendMessage(const TellstickPacket &packet) const {

    const WireMessage *message = packet.getWireData();
    const uint16_t message_size = packet.getWireDataSize();
    const uint8_t transmition_repeats = packet.getTransmitionRepeats();

    for (uint8_t i = 0; i < transmition_repeats; i++) {
        for(uint16_t i = 0; i < message_size; i++) {
            setHighFor(message[i].ms_delay_high);
            setLowFor(message[i].ms_delay_low);
        }
    }
}