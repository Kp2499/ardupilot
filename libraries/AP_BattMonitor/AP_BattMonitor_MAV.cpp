#include "AP_BattMonitor_config.h"
#include <AP_HAL/utility/sparse-endian.h>

#include "AP_BattMonitor_MAV.h"

void AP_BattMonitor_MAV::handle_msg(const mavlink_message_t &msg)
{   
    switch (msg.msgid) {
        case (147):
            // HANDLE BATTERY MONITOR MSG
            handle_battery_status_msg(msg)

            break;     
    }
}

// handle mavlink DISTANCE_SENSOR messages
void AP_BattMonitor_MAV::handle_battery_status_msg(const mavlink_message_t &msg)
{
    mavlink_battery_status_t packet;
    mavlink_msg_battery_status_decode(&msg, &packet);
    // PARSE PACKETS
    _state.voltage = 16.8;
    _state.last_time_micros = AP_HAL::micros();
    _state.healthy = true;
    _state.current_amps=packet.current_battery *100;
    _state.consumed_mah=packet.current_consumed;
    _state.temperature=packet.temperature*100;
    return;
}


void AP_BattMonitor_MAV::init(void)
{
}


/// read the battery_voltage and current, should be called at 10hz
void AP_BattMonitor_MAV::read(void)
{    GCS:
    // handle_msg(&msg)
    // _state.voltage = 16.8;
    // _state.last_time_micros = AP_HAL::micros();
    // _state.healthy = true;
    // _state.current_amps=1;
    // _state.consumed_mah=1040;
}
