#pragma once
#include <Adafruit_SSD1306.h>

namespace screen {
bool init();

extern Adafruit_SSD1306 display;
} // namespace screen
