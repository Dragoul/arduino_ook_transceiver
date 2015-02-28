/* 
* Tellstick.cpp
*
* Created: 2014-12-29 11:37:25
* Author: Erik
*/

#include "Tellstick.h"
#include "protocol/TellstickPacket.h"

// default constructor
Tellstick::Tellstick(byte pin_data)
{
    this->pin_data = pin_data;
} //Tellstick

// default destructor
Tellstick::~Tellstick()
{
} //~Tellstick

void Tellstick::init()
{
}

void Tellstick::setHighFor(unsigned int ms_duration) const
{
    digitalWrite(pin_data, HIGH);
    delayMicroseconds(ms_duration);
}
void Tellstick::setLowFor(unsigned int ms_duration) const
{
    digitalWrite(pin_data, LOW);
    delayMicroseconds(ms_duration);
}

void Tellstick::sendMessage(const TellstickPacket &packet) const {

    const WireMessage *message = packet.getWireData();
    const int message_size = packet.getWireDataSize();
    const int transmition_repeats = packet.getTransmitionRepeats();

    for (int i = 0; i < transmition_repeats; i++) {
        for(int i = 0; i < message_size; i++) {
            setHighFor(message[i].ms_delay_high);
            setLowFor(message[i].ms_delay_low);
        }
    }
}