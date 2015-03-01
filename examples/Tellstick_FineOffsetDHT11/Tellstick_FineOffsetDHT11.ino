// DHT11 lib: http://playground.arduino.cc/Main/DHT11Lib
#include <dht11.h>
#include <OokTransceiver.h>
#include <FineOffsetSensorPacket.h>

// Pins
const int dht11_pin = 22;
const int transmit_pin = 26;

// Configuration
const uint8_t device_id = 200;
const int transmit_delay_seconds = 60;

// Global variables
dht11 DHT11;
OokTransceiver ookTransceiver(transmit_pin);
FineOffsetSensorPacket sensor(device_id);

// Helper functions
void transmit_sensor_data() {
  sensor.setHumidity((uint8_t)DHT11.humidity);
  sensor.setTemperature((int16_t)DHT11.temperature);
  sensor.preparePacket();
  Serial.println("Transmitting message: " + sensor.packetToString());
  ookTransceiver.sendMessage(sensor);
}

void print_error_code(int error_code) {
  switch (error_code)
  {
    case DHTLIB_ERROR_CHECKSUM:
        Serial.println("DHT11 Error: Checksum");
        break;
    case DHTLIB_ERROR_TIMEOUT:
        Serial.println("DHT11 Error: Timeout");
        break;
    default: 
        Serial.println("DHT11 Error: Unknown");
        break;
  }
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int result = DHT11.read(dht11_pin);
  if (result == DHTLIB_OK) {
    transmit_sensor_data();
  } else {
    print_error_code(result);
  }
  
  delay(transmit_delay_seconds * 1000);
}

