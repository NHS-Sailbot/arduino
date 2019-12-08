#include <arduino/arduino.hpp>
#include <debug/debug.hpp>

int main() {
    struct RData {
        unsigned int a;
    };
    struct TData {
        unsigned int a, b, c, d;
    };

    constexpr unsigned int baud = 115200;
    constexpr unsigned char key = 0xA5;

    auto ard = arduino::connect(sizeof(RData), sizeof(TData), baud, key);
    if (!ard.is_valid) {
        debug::log::error("Unable to connect to arduino");
        return -1;
    }

    debug::log::success("Connected to arduino");
    return 0;
}
