#include <INA219_WE.h>
#include <Wire.h>

#include "board.hpp"

namespace vcm {
constexpr auto ina219_addr = 0x42;

auto ina219 = INA219_WE(ina219_addr);

bool init() {
    if(!ina219.init()) {
        return false;
    }
    ina219.setADCMode(BIT_MODE_9);
    ina219.setPGain(PG_320);
    ina219.setBusRange(BRNG_16);
    ina219.setShuntSizeInOhms(0.01);
    return true;
}

// in V
float get_battery_voltage() {
    const auto v_in    = ina219.getBusVoltage_V();
    const auto v_shunt = ina219.getShuntVoltage_mV() / 1000;
    return v_in + v_shunt;
}

// in mA
float get_current() {
    const auto i = ina219.getCurrent_mA();
    return i;
}
} // namespace vcm
