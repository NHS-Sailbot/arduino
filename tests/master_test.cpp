#include <arduino/arduino.hpp>

int main() {
    auto ard = arduino::connect(115200, 0x5C);
    if (!ard.is_open) return -1;
    return 0;
}
