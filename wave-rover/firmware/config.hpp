#pragma once

namespace config {
// motors
constexpr auto freq           = 100000;
constexpr auto a_pwm_channel  = 0;
constexpr auto b_pwm_channel  = 1;
constexpr auto pwm_resolution = 8;

// status screen
constexpr auto enable_status_screen        = true;
constexpr auto status_screen_refresh_delay = 1000; // ms

// debug
constexpr auto debug = true;
} // namespace config
