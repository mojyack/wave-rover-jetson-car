#include "motor.hpp"
#include "config.hpp"
#include "print.hpp"

auto mode_to_string(const MotorMode mode) -> const char* {
    switch(mode) {
    case MotorMode::Forward:
        return "forward";
    case MotorMode::Reverse:
        return "reverse";
    case MotorMode::Brake:
        return "brake";
    case MotorMode::Coast:
        return "coast";
    }
}

void Motor::set_mode(const MotorMode mode) {
    if(config::debug) {
        serial_print("set_mode:", pwm_channel, " ", int(mode));
    }
    switch(mode) {
    case MotorMode::Forward:
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        break;
    case MotorMode::Reverse:
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        break;
    case MotorMode::Brake:
        digitalWrite(in1, HIGH);
        digitalWrite(in2, HIGH);
        break;
    case MotorMode::Coast:
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        break;
    }
    this->mode = mode;
}

void Motor::set_speed(const float speed) {
    if(config::debug) {
        serial_print("set_speed:", pwm_channel, " ", speed);
    }
    const auto duty = speed * (1 << config::pwm_resolution); // TODO: is this correct?
    ledcWrite(pwm_channel, duty);
    this->speed = speed;
}

void Motor::init() {
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(pwm, OUTPUT);
    ledcSetup(pwm_channel, config::freq, config::pwm_resolution);
    ledcAttachPin(pwm, pwm_channel);

    set_mode(mode);
    set_speed(speed);
}
