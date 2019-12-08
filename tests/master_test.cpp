#include <arduino/arduino.hpp>

int main() {
    auto ard = arduino::connect(115200, 0xA5);
    if (!ard.is_valid) {
        //
        return -1;
    }
    return 0;
}
