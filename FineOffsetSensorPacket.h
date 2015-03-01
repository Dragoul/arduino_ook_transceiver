/*
 * Fine Offset Temperature/Humidity sensor protocol
 *
 * Author: Mattias S (https://github.com/phooey)
 *
 * Put together with help from:
 * Erik (https://github.com/Dragoul)
 * lucsmall (http://lucsmall.com/2012/04/27/weather-station-hacking-part-1/)
 * olof_n (http://forum.arduino.cc/index.php?topic=211371.0)
 * Michael Barr
 * (http://www.barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code)
 *
 * Tested working with Tellstick Duo running Firmware version 12
 */

#ifndef __FINE_OFFSET_SENSOR_PACKET_H__
#define __FINE_OFFSET_SENSOR_PACKET_H__

#include "protocol/TellstickPacket.h"

class FineOffsetSensorPacket : public TellstickPacket
{
  public:
    FineOffsetSensorPacket();
    FineOffsetSensorPacket(uint8_t device_id);
    ~FineOffsetSensorPacket();

    // TellstickPacket interface
    void preparePacket();
    const WireMessage *getWireData() const;
    uint16_t getWireDataSize() const;
    uint8_t getTransmissionRepeats() const;

    // FineOffsetPacket interface
    void setTransmissionRepeats(uint8_t repeats);
    void setDeviceID(const uint8_t id);
    void setHumidity(const uint8_t value);
    void setTemperature(const int16_t value);
    String packetToString() const;

  private:
    WireMessage *ook_msg;

    uint8_t transmission_repeats;
    uint8_t device_id;
    uint8_t humidity;
    int16_t temperature;
    
    bool isBitOne(const uint8_t *number, const uint8_t bit) const;
    void setOne(WireMessage *bit);
    void setZero(WireMessage *bit);

    void prepareMessageData(uint8_t *message);
    void prepareWireMessage(const uint8_t *message);

    uint8_t calculateCRC8(const uint8_t *msg, const uint8_t len) const;
};
#endif // __FINE_OFFSET_SENSOR_PACKET_H__