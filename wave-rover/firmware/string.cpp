#include "string.hpp"

uint8_t read_byte() {
    while(!Serial.available()) {
        yield();
    }
    return Serial.read();
}

void read_str(char* const buf, const size_t buflen) {
    auto cur = 0;
loop:
    if(cur + 1 >= buflen) {
        // buffer full
        buf[cur] = '\0';
        return;
    }
    buf[cur] = read_byte();
    if(buf[cur] == '\0') {
        // complete
        return;
    }
    cur += 1;
    goto loop;
}

float read_float(char* const buf, const size_t buflen) {
    read_str(buf, buflen);
    return atof(buf);
}
