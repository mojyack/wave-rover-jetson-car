#include <Arduino.h>
#include <Wire.h>

#include "board.hpp"
#include "config.hpp"
#include "motor.hpp"
#include "print.hpp"
#include "screen.hpp"
#include "status.hpp"
#include "string.hpp"
#include "util.hpp"
#include "vcm.hpp"

Motor motors[] = {
    {board::a_pwm, config::a_pwm_channel, board::a_in1, board::a_in2},
    {board::b_pwm, config::b_pwm_channel, board::b_in1, board::b_in2},
};

void setup() {
    Wire.begin(board::s_sda, board::s_scl);
    Serial.begin(115200);
    for(auto i = 0; i < array_size(motors); i += 1) {
        motors[i].init();
    }
    vcm::init();
    if(config::enable_status_screen) {
        screen::init();
        status::init();
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
    const auto v = vcm::get_battery_voltage();
    const auto i = vcm::get_current();
    serial_print(v, "V ", i, "mA ", v * i, "mW");
}

void handle_command(char* cmd) {
    static int v1 = 1;
    static int v2 = 1;
    static int v3 = 1;

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
    case '1':
        v1 = atoi(cmd + 1);
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
