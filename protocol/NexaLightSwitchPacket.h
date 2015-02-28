
#include "TellstickPacket.h"

#define NEXA_LIGHT_SWITCH_MESSAGE_SIZE 12*2+1

typedef enum state_t {
	LIGHT_ON,
	LIGHT_OFF
} state_t;

class NexaLightSwitchPacket : public TellstickPacket
{
  public:
	int house;
	int unit;
	state_t state;
	
	WireMessage *ook_msg;
	
	NexaLightSwitchPacket() {
		ook_msg = new WireMessage[NEXA_LIGHT_SWITCH_MESSAGE_SIZE];
	}
	
	~NexaLightSwitchPacket() {
		delete ook_msg;
	}
		
	void preparePacket() {
		
		String message = getPacketString();
		const int T = 400;
		int times = 3;

		unsigned int msg_index = 0;
		for (int i = 0; i < message.length(); i++) {
			if(message[i] == 'X') {
				ook_msg[msg_index].ms_delay_high = T;
				ook_msg[msg_index++].ms_delay_low = T*times;
				
				ook_msg[msg_index].ms_delay_high = T*times;
				ook_msg[msg_index++].ms_delay_low = T;
			} else if(message[i] == '1') {
				ook_msg[msg_index].ms_delay_high = T*times;
				ook_msg[msg_index++].ms_delay_low = T;
			
				ook_msg[msg_index].ms_delay_high = T*times;
				ook_msg[msg_index++].ms_delay_low = T;
			} else {
				ook_msg[msg_index].ms_delay_high = T;
				ook_msg[msg_index++].ms_delay_low = T*times;
				
				ook_msg[msg_index].ms_delay_high = T;
				ook_msg[msg_index++].ms_delay_low = T*times;
			}
		}
		ook_msg[msg_index].ms_delay_high = T;
		ook_msg[msg_index++].ms_delay_low = T*32;
	}
	
	const WireMessage *getWireData() const {
		return ook_msg;
	}
	
	unsigned int getWireDataSize() const {
		return NEXA_LIGHT_SWITCH_MESSAGE_SIZE;
	}
	
	unsigned int getTransmitionRepeats() const {
		return 3;
	}
	
  private:
  
    String getPacketString() const {
	    String houseStr = "0000";
	    String unitStr = String(unit == 1 ? "X000" : "0000");
	    String stateStr = (state == LIGHT_ON ? String("X") : String("0"));
	    return houseStr + unitStr + "0XX" + stateStr;
    }
};