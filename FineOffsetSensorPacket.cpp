#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "FineOffsetSensorPacket.h"

// Constants
namespace {
const uint8_t message_size_bytes = 6;
const uint8_t message_size_bits = 48;
const uint16_t time_one_us = 500;
const uint16_t time_delay_us = 1000;
const uint16_t time_zero_us = 1500;
const uint8_t transmission_repeats_default = 2;
}

// Constructors/Destructors
FineOffsetSensorPacket::FineOffsetSensorPacket() {
    this->transmission_repeats = transmission_repeats_default;
    this->ook_msg = new WireMessage[message_size_bits];
}

FineOffsetSensorPacket::FineOffsetSensorPacket(uint8_t device_id) {
    this->device_id = device_id;
    this->transmission_repeats = transmission_repeats_default;
    this->ook_msg = new WireMessage[message_size_bits];
}

FineOffsetSensorPacket::~FineOffsetSensorPacket() {
    delete ook_msg;
}

// TellstickPacket interface
void FineOffsetSensorPacket::preparePacket() {
    uint8_t *message = new uint8_t[message_size_bytes];
    prepareMessageData(message);
    prepareWireMessage(message);
    delete message;
}

const WireMessage *FineOffsetSensorPacket::getWireData() const {
    return ook_msg;
}

uint16_t FineOffsetSensorPacket::getWireDataSize() const {
    return message_size_bits;
}

uint8_t FineOffsetSensorPacket::getTransmissionRepeats() const {
    return transmission_repeats;
}

// Setters
void FineOffsetSensorPacket::setTransmissionRepeats(uint8_t repeats) {
    this->transmission_repeats = repeats;
}

void FineOffsetSensorPacket::setDeviceID(const uint8_t id) {
    this->device_id = id;
}

void FineOffsetSensorPacket::setHumidity(const uint8_t value) {
    this->humidity = value;
}

void FineOffsetSensorPacket::setTemperature(const int16_t value) {
    this->temperature = value;
}

// Helper functions
String FineOffsetSensorPacket::packetToString() const {
    String packet = "";
    for (uint8_t i = 0; i < message_size_bits; i++) {
        if (ook_msg[i].ms_delay_high == time_one_us) {
            packet += "1";
        } else {
            packet += "0";
        }
    }
    return packet;
}

bool FineOffsetSensorPacket::isBitOne(const uint8_t *number, const uint8_t bit)
const {
    return (*number & (1 << bit));
}

void FineOffsetSensorPacket::setOne(WireMessage *bit) {
    bit->ms_delay_high = time_one_us;
    bit->ms_delay_low = time_delay_us;
}

void FineOffsetSensorPacket::setZero(WireMessage *bit) {
    bit->ms_delay_high = time_zero_us;
    bit->ms_delay_low = time_delay_us;
}

void FineOffsetSensorPacket::prepareMessageData(uint8_t *message) const {
    /* Message consists of 6 bytes
     * 8-bit preamble (all 1:s)
     * 12-bit device id and type, (four 0:s, eight bit device id)
     * 12-bit temperature * 10 degrees, MSB is sign bit
     * 8-bit humidity
     * 8-bit CRC-8 of previous 4 bytes (data part excluding preamble)
     *
     * Byte 1: 1111 1111
     * 8-bit preamble
     *
     * Byte 2: 0000 DDDD
     * DDDD = Higher 4 bits of 8-bit device id
     *
     * Byte 3: dddd STTT
     * dddd = Lower 4 bits of 8-bit device id
     * S = 1 if temperature < 0
     * TTT = Bit 11-9 of 12-bit temperature value
     *
     * Byte 4: TTTT TTTT
     * Lower 8 bits of 12-bit temperature value
     *
     * Byte 5: HHHH HHHH
     * 8-bit humidity value
     * 
     * Byte 6: CCCC CCCC
     * 8-bit CRC-8 value for previous 4 bytes (excluding preamble)
     */

    // Preamble
    message[0] = 0xFF;

    // Device ID and type
    // Higher 4 bits of device id as lower 4 bits of byte, higher 4 bits zero
    uint8_t device_id_high = device_id >> 4;
    message[1] = device_id_high;

    // Device ID + Temperature
    // Lower 4 bits of device id
    uint8_t device_id_low = device_id << 4;
    // 12-bit temperature absolute value * 10 degrees, MSB is sign bit
    uint16_t temp_value = abs(temperature) * 10;
    // Set 12th bit of temperature to 1 for negative temperature
    if (temperature < 0) {
        temp_value += 2048;
    }
    // Bits 12-9 of 12-bit temperature as lower 4 bits of this byte
    uint8_t temp_high = (uint8_t) (temp_value >> 8);
    message[2] = device_id_low | temp_high;

    // Temperature
    // Lower 8 bits of 12-bit temperature
    uint8_t temp_low = (uint8_t) temp_value;
    message[3] = temp_low;

    // Humidity
    // 8-bit humidity value in percent
    message[4] = humidity;

    // CRC
    // 8-bit CRC of previous 4 bytes (exclude preamble)
    uint8_t crc_value = calculateCRC8(&message[1], 4);
    message[5] = crc_value;
}

void FineOffsetSensorPacket::prepareWireMessage(const uint8_t *message) {
    for (int byte = 0; byte < message_size_bytes; byte++) {
        uint8_t data = message[byte];
        for (int bit = 0; bit < 8; bit++) {
            if (isBitOne(&data, 7 - bit)) {
                setOne(&ook_msg[byte * 8 + bit]);
            } else {
                setZero(&ook_msg[byte * 8 + bit]);
            }
        }
    }
}

/* 
 * Adapted from code in the public domain:
 * http://www.barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code
 */
uint8_t FineOffsetSensorPacket::calculateCRC8(const uint8_t *msg,
                                              const uint8_t len) const {
    const uint8_t polynomial = 0x31;
    const uint8_t width = sizeof(uint8_t) * 8;
    const uint8_t topbit = (1 << (width - 1));
    uint8_t  remainder = 0;	

    // Perform modulo-2 division, a byte at a time.
    for (int byte = 0; byte < len; ++byte) {
        // Bring the next byte into the remainder.
        remainder ^= (msg[byte] << (width - 8));

        // Perform modulo-2 division, a bit at a time.
        for (uint8_t bit = 8; bit > 0; --bit) {
            // Try to divide the current data bit.
            if (remainder & topbit) {
                remainder = (remainder << 1) ^ polynomial;
            } else {
                remainder = (remainder << 1);
            }
        }
    }

    // The final remainder is the CRC result.
    return (remainder);
}
