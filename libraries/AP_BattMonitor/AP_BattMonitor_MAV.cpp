#include "AP_BattMonitor_config.h"

#if AP_BATTERY_ENABLED

#include <AP_HAL/utility/sparse-endian.h>
#include "AP_BattMonitor_MAV.h"

void AP_BattMonitor_MAV::handle_msg(const mavlink_message_t &msg){
	switch (msg.msgid) {
		case (MAVLINK_MSG_ID_BATTERY_STATUS):
			handle_battery_status_msg(msg);
			break;     
	}
}

// handle mavlink DISTANCE_SENSOR messages
void AP_BattMonitor_MAV::handle_battery_status_msg(const mavlink_message_t &msg)
{
	mavlink_battery_status_t packet;
	mavlink_msg_battery_status_decode(&msg, &packet);
	
	float v_bat = 0;
	
	// FILL THE CELLS 
	 for (int i=0; i < 10 && i < AP_BATT_MONITOR_CELLS_MAX; i++) {
		//  VALID CELLS NEED TO BE CONTINUOUS
		if (packet.voltages[i] == UINT16_MAX) {
			break;
		}
		_state.cells[i] = packet.voltages[i];
		v_bat += packet.voltages[i]
		
	}
	
	// UPDATE STATES
	_state.voltage = v_bat/1000;
	_state.healthy = true;
	_state.current_amps=packet.current_battery /100;
	_state.consumed_mah=packet.current_consumed;
	_state.temperature=packet.temperature/100;
	_state.last_time_micros = AP_HAL::micros();
	_state.temperature_time = AP_HAL::micros();
	_state.last_healthy_ms = AP_HAL::millis();
	
	
	// TO DO: WRITE A DYNAMIC RTL FROM HOME AND BATTERY FAILSAFE
	return;
}


void AP_BattMonitor_MAV::init(void)
{
}


/// read the battery_voltage and current, should be called at 10hz
void AP_BattMonitor_MAV::read(void)
{   
	// CHECK BATTERY HEALTH 
	if(_state.last_time_micros - AP_HAL::micros()>1000000){
		_state.healthy = false;
	}
}

#endif  // AP_BATTERY_ENABLED