#pragma once
#include <Arduino.h>

enum class MotorMode : uint8_t {
    Forward,
    Reverse,
    Brake,
    Coast,
};

auto mode_to_string(MotorMode mode) -> const char*;

struct Motor {
    // config
    uint8_t pwm;
    uint8_t pwm_channel;
    uint8_t in1;
    uint8_t in2;

    // state
    MotorMode mode;
    float     speed;

    void set_mode(MotorMode mode);
    void set_speed(float speed);
    void init();

    Motor(const uint8_t pwm, const uint8_t pwm_channel, const uint8_t in1, const uint8_t in2)
        : pwm(pwm),
          pwm_channel(pwm_channel),
          in1(in1),
          in2(in2),
          mode(MotorMode::Brake),
          speed(0.0) {}
};
