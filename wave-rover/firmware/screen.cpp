#include <Wire.h>

#include "screen.hpp"
#include "board.hpp"

namespace screen {
constexpr auto width       = 128;
constexpr auto height      = 32;
constexpr auto reset_pin   = -1;
constexpr auto screen_addr = 0x3c; // 0x3d for 128x64

Adafruit_SSD1306 display(width, height, &Wire, reset_pin);

bool init() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, screen_addr)) {
        return false;
    }
    display.clearDisplay();
    display.display();
    return true;
}
} // namespace screen
