#include <Arduino.h>

#include "config.hpp"
#include "motor.hpp"
#include "print.hpp"
#include "string.hpp"
#include "util.hpp"

Motor motors[] = {
    {config::a_pwm, config::a_pwm_channel, config::a_in1, config::a_in2},
    {config::b_pwm, config::b_pwm_channel, config::b_in1, config::b_in2},
};

void setup() {
    Serial.begin(115200);
    for(auto i = 0; i < array_size(motors); i += 1) {
        motors[i].init();
    }
}

void handle_motor_command(Motor& motor, const char* const cmd) {
    switch(cmd[0]) {
    // speed
    case 's': {
        const auto speed = atof(&cmd[1]);
        motor.set_speed(speed);
    } break;
    // forward
    case 'f':
        motor.set_mode(MotorMode::Forward);
        break;
    // reverse
    case 'r':
        motor.set_mode(MotorMode::Reverse);
        break;
    // brake
    case 'b':
        motor.set_mode(MotorMode::Brake);
        break;
    // coast
    case 'c':
        motor.set_mode(MotorMode::Coast);
        break;
    default:
        Serial.println("unknown motor command");
        break;
    }
}

void print_state() {
    for(auto i = 0; i < array_size(motors); i += 1) {
        serial_print("motor ", i, ": state=", mode_to_string(motors[i].mode), " speed=", motors[i].speed);
    }
}

void handle_command(char* cmd) {
    switch(cmd[0]) {
    // left
    case 'l':
        handle_motor_command(motors[0], cmd + 1);
        break;
    // right
    case 'r':
        handle_motor_command(motors[1], cmd + 1);
        break;
    // query
    case 'q':
        print_state();
        break;
    default:
        serial_print("unknown command '", cmd, "'");
        break;
    }
}

void loop() {
    char buf[8];
    read_str(buf, 8);
    handle_command(buf);
    return;
    if(Serial.available() > 0) {
    }
}
