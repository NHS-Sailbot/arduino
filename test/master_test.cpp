#include <HENRY/arduino.hpp>
#include <debug/debug.hpp>

struct RData {
    unsigned int a;
};
struct TData {
    unsigned int a, b, c, d;
};

int main() {
    HENRY::Arduino ard(sizeof(RData), sizeof(TData), 115200, 0xA5);

    if (!ard.is_valid) {
        debug::log::error("Unable to connect to arduino");
        return 0;
    }

    debug::log::success("Connected to arduino");
    return 0;
}
