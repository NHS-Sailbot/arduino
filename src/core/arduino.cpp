#include <arduino/arduino.hpp>
#include <debug/debug.hpp>

namespace arduino {

    void disconnect(Device &device) {
        DEBUG_BEGIN_FUNC_PROFILE;

        if (device.rdata_buffer) delete device.rdata_buffer;
        if (device.tdata_buffer) delete device.tdata_buffer;
        serial::close(device.serial_device);
    }

    void update(Device &device) {
        const double current_time = debug::timer::now();
        if (current_time - device.tick_begin > TICK_INTERVAL) {
            DEBUG_BEGIN_FUNC_PROFILE;

            switch (device.tick) {
            case 0:
                serial::write(device.serial_device, device.tdata_buffer, device.tsize);
                device.tick = 1;
                break;
            case 1:
                serial::read(device.serial_device, device.rdata_buffer, device.rsize);
                if (device.rdata_buffer[0] == device.key && device.rdata_buffer[device.rsize - 1] == device.key) {
                    device.tick_begin = debug::timer::now();
                    device.is_valid = true;
                } else {
                    debug::log::error("failed to find the key (%x) when reading the device.", device.key);
                    device.is_valid = false;
                }
                DEBUG_BEGIN_PROFILE(User_defined_update_function);
                device.on_update(device);
                DEBUG_END_PROFILE(User_defined_update_function);
                device.tick = 0;
                break;
            }

            device.tick_begin += TICK_INTERVAL;
        }
    }
} // namespace arduino
