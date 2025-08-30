#pragma once

#include <AP_Common/AP_Common.h>
#include "AP_BattMonitor_Backend.h"
#include <utility>

class AP_BattMonitor_MAV : public AP_BattMonitor_Backend
{
public:
    // Inherit constructor
    using AP_BattMonitor_Backend::AP_BattMonitor_Backend;

    bool has_current() const override {
        return true;
    }

    void init(void) override;
    void read() override;
    bool has_consumed_energy(void) const override {
        return true;
    }
    // handle mavlink messages
    void handle_msg(const mavlink_message_t &msg) override;

private:
    HAL_Semaphore sem;
    void handle_battery_status_msg(const mavlink_message_t &msg);
};


