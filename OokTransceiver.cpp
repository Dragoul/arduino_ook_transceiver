
#include "OokTransceiver.h"
#include "protocol/OokTransceiverPacket.h"

OokTransceiver::OokTransceiver(byte tx_pin)
{
    this->tx_pin = tx_pin;
}

OokTransceiver::~OokTransceiver()
{
}

void OokTransceiver::init()
{
    pinMode(tx_pin, OUTPUT);
}

void OokTransceiver::setHighFor(uint16_t us_duration) const
{
    digitalWrite(tx_pin, HIGH);
    delayMicroseconds(us_duration);
}
void OokTransceiver::setLowFor(uint16_t us_duration) const
{
    digitalWrite(tx_pin, LOW);
    delayMicroseconds(us_duration);
}

void OokTransceiver::sendMessage(const OokTransceiverPacket &packet) const {

    const WireMessage *message = packet.getWireData();
    const uint16_t message_size = packet.getWireDataSize();
    const uint8_t transmission_repeats = packet.getTransmissionRepeats();

    for (uint8_t i = 0; i < transmission_repeats; i++) {
        for(uint16_t i = 0; i < message_size; i++) {
            setHighFor(message[i].us_delay_high);
            setLowFor(message[i].us_delay_low);
        }
    }
}