#include <Arduino.h>

#include "screen.hpp"
#include "vcm.hpp"

namespace {
void updater_main(void*) {
    char buf[32];
    auto tick = 0;
loop:
    auto& display = screen::display;
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // tick
    sprintf(buf, "%d ticks", tick);
    display.setCursor(0, 0);
    display.print(F(buf));

    // vcm
    const auto v = vcm::get_battery_voltage();
    const auto i = vcm::get_current();
    sprintf(buf, "%.1fV %.1fmA %.1fmW", v, i, v * i);

    display.setCursor(0, 8);
    display.print(F(buf));

    // refresh
    display.display();

    delay(config::status_screen_refresh_delay);
    tick += 1;
    goto loop;
}
} // namespace

namespace status {
void init() {
    auto handle = TaskHandle_t();
    xTaskCreateUniversal(updater_main, "status_updater", 0x1000, nullptr, 1, &handle, APP_CPU_NUM);
}
} // namespace status
