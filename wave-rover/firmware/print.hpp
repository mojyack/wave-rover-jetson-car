#pragma once
#include <Arduino.h>

inline void serial_print() {
    Serial.print('\n');
}

template <class Arg, class... Args>
void serial_print(const Arg arg, const Args... args) {
    Serial.print(arg);
    serial_print(args...);
}
