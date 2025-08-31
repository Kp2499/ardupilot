#include "AP_BattMonitor_config.h"

#if AP_BATTERY_ENABLED

#include <AP_HAL/utility/sparse-endian.h>
#include "AP_BattMonitor_MAV.h"

uint8_t battery_percentage = 0;

void AP_BattMonitor_MAV::handle_msg(const mavlink_message_t &msg) {
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
    uint32_t t_now_us=AP_HAL::micros();

    // FILL THE CELLS CURRENTLY ONLY FOR FIRST 10 CELLS
    for (int i=0; i < 10 && i < AP_BATT_MONITOR_CELLS_MAX; i++) {
        //  VALID CELLS NEED TO BE CONTINUOUS
        if (packet.voltages[i] == UINT16_MAX) {
            break;
        }
        _state.cell_voltages.cells[i] = packet.voltages[i];
        v_bat += packet.voltages[i];

    }

    // UPDATE STATES
    _state.voltage = v_bat/1000;
    _state.healthy = true;
    _state.current_amps = packet.current_battery /100;
    _state.consumed_mah = packet.current_consumed;
    _state.temperature = packet.temperature/100;
    _state.last_time_micros = t_now_us;
    _state.temperature_time = t_now_us;
    _state.last_healthy_ms = AP_HAL::millis();
    battery_percentage= packet.battery_remaining;

    // TO DO: WRITE A DYNAMIC RTL FROM HOME AND BATTERY FAILSAFE
    return;
}


void AP_BattMonitor_MAV::init(void)
{
}


/// read the battery_voltage and current, should be called at 10hz
void AP_BattMonitor_MAV::read(void)
{
    WITH_SEMAPHORE(sem);
    // CHECK BATTERY HEALTH
    if(AP_HAL::micros()-_state.last_time_micros >2000000) {
        // GCS_SEND_TEXT(MAV_SEVERITY_WARNING, "BAD BATTERY");
        _state.healthy = false;
    }
}

// MAVLINK BASED BATTERY MONITOR PROVIDES PERCENTAGE 
bool AP_BattMonitor_MAV::capacity_remaining_pct(uint8_t &percentage) const
{
    percentage = battery_percentage;
    return true;
}

#endif  // AP_BATTERY_ENABLED