#pragma once

namespace config {
constexpr auto a_pwm = 25;
constexpr auto a_in2 = 17;
constexpr auto a_in1 = 21;
constexpr auto b_in1 = 22;
constexpr auto b_in2 = 23;
constexpr auto b_pwm = 26;

constexpr auto freq           = 100000;
constexpr auto a_pwm_channel  = 0;
constexpr auto b_pwm_channel  = 1;
constexpr auto pwm_resolution = 8;

constexpr auto debug = true;
} // namespace config
